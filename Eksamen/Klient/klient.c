#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <arpa/inet.h>


/*
 * Client program
 *
 * Tar inn 2 argumenter
 *  IP pa server, som tall, eller som et domene navn.
 *  Port pa server.
 *
 */

/* Globale variabler */
static int sock;
static int pc1[2];
static int pc2[2];
static pid_t pid;
static pid_t pid2;


/* Liste med funksjoner */
void myHandler();
int create_socket();
int meny();
int makeChildren();
void closePipe();


/*
 * Funksjon main
 * Starter klient programmet. Tar inn en ip og en port. Ipen kan være på
 * Integer form eller et domene. Deretter vil den lage en SIGINT signal behandler.
 *
 * Kaller pa alle funksjoner som skal kjores, og closer alle globale variabler ved
 * program avslutning.
 *
 * RETURN int:
 *  Returnerer EXIT_FAILURE ved feil, ellers returneres EXIT_SUCCESS
 *
 *
 */

int main(int argc, char *argv[]) {

  if(argc != 3){
    fprintf(stderr, "Usage: %s <ip> <port> \n", argv[0] );
    return EXIT_FAILURE;
  }

  /* Lager domene om til ip */
  struct hostent *server;
  server = gethostbyname(argv[1]);
  char *ip;
  if(server == NULL){
    ip=argv[1];
  }else{
    ip = inet_ntoa(*((struct in_addr *) server->h_addr_list[0]));
    printf("iP:%s\n",ip );
  }

  char *port=argv[2];

  /* lager en SIGINT signal behandler */
  struct sigaction sig;
  memset(&sig, 0, sizeof(sig));
  sig.sa_handler = myHandler;
  sigaction(SIGINT, &sig, NULL);


  /* oppretter barne prosesser, lukker alt av pipes og dreper barneprosesser i tilfelle noen ble oprettet */
  if(makeChildren() == EXIT_FAILURE){
    fprintf(stderr, "Kunne ikke oprette pipes/forke\n" );
    closePipe();
    return EXIT_FAILURE;
  }


  /* lager socket */
  int tmp = create_socket(ip, port);
  if(tmp == -1){
    closePipe();
    return EXIT_FAILURE;
  } else if(tmp == 1){
    fprintf(stderr, "Couldent connect to server, exiting\n" );
    closePipe();
    return EXIT_FAILURE;
  }
  // printf("socket value: %d\n", sock);
  //lage barn og pipes

  /* Skriver ut meny til bruker og kommuniserer med server programmet */
  meny();

  printf("Terminating program\n");
  closePipe();
  close(sock);
  return 0;
}

/* Lukker pipes og dreper barnaprosesser */

void closePipe(){
  write(pc2[1], "Q", 1);
  write(pc1[1], "Q", 1);
  usleep(50000);
  close(pc1[1]);
  close(pc1[0]);
  close(pc2[1]);
  close(pc2[0]);
  kill(pid, SIGKILL);
  kill(pid2, SIGKILL);

}



 /*
  * Handler for feilmeldingen
  * ctrl + c
  * Dette er for a avslutte programmet riktig
  *
  * Den sender en error melding til Server programmet.
  * Og lukker alle globale variabler og dreper barne prosesser.
  * Deretter avlsutter den programmet.
  *
  */

void myHandler(){
  printf("ctrl + c sensed\n");
  printf("Terminating program\n");
  send(sock, "E", 1, 0);
  close(sock);
  closePipe();
  exit(EXIT_FAILURE);
}

/*
 * Funksjon create_socket
 * Lager socket og oppretter kontakt med server
 *
 * INPUT:
 *  Tar inn ip og port som input.
 *  Disse brukes til a koble til server pogrammet.
 *
 *
 * RETURN int:
 *  Returnerer sock.
 */

int create_socket(char *ip, char *port){
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(sock == -1){
    perror("socket()");
    return EXIT_FAILURE;
  }

  int portNb = atoi(port);

  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(struct sockaddr_in));
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_port=htons(portNb);

  // Bruker inet_pton istedet for inet_aton grunnet den sjekker ipen opp mot adresse familien
  int ine = inet_pton(AF_INET, ip, &serveraddr.sin_addr.s_addr);

  /* Sjekker om Funksjonen returnerer en feilmelding.
  Dette blir ogsa gjort videre i programmet */
  if(ine == -1){
    perror("inet_pton()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Connecting to ip: %s:%d\n", ip, portNb );

  /* Kobler seg til server programmet */
  if(connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr))){
    fprintf(stderr, "\nCouldent connect to server: %s on port:%s\n", ip, port);
    perror("connect()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Connected to server!\n");

  return sock;
}

/*
 * Meny for inputs fra bruker
 * Leser en char fra terminalen som representerer et valg.
 * Lager en melding som sendes til server.
 * Kjorer en for lokke som kjorer ihendhold til hvor mange ganger programmet forventer a fa sendt meldinger.
 * Her leser Klient programmet jobber fra serveren, som igjen sender beskjeder til bestemte barne prosesser.
 *
 *
 *
 * SEND:
 *  Klienten sender G for hente job meldinger, med et antall som medfølger
 *  Sender antall 0 for 1 job, dette tolker server programmet som 1 job.
 *  Sender A for alle
 *
 *
 * RETURN int:
 *  returnerer EXIT_FAILURE ved feil, og EXIT_SUCCESS ved riktig avslutning.
 *  I main vil programmet avslutte rett etter avslutning av meny, sa return verdien
 *  blir ikke brukt. Dette kan enkelt brukes hvis programmet skal utvides.
 *
 */

int meny(){
  for(;;){
    printf("\nVelg et valg fra 1-4\n" );
    printf("1: Hent en jobb fra serveren\n");
    printf("2: Hent X antall jobber fra serveren\n");
    printf("3: Hent alle jobber fra serveren\n");
    printf("4: Avslutte programmet\n");

    int antall =1;
    char kin[2];
    scanf("%s", kin);

    int in = atoi(kin);
    printf("VALG: %d\n", in);

    char msg[3] = { 0 };

    if(in == 1){
      msg[0] = 'G';

    }else if(in == 2){
      printf("Velg antall jobber du vil hente (MAX 255): \n");
      int job;
      scanf("%d", &job);
      if(job>255){
        printf("Kan ikke hente %d jobber!\n", job );
        printf("Henter 255 jobber: \n" );
        job=255;

      }
      // printf("Antall jobber hentet: %d\n", antall );
        msg[0] = 'G';
        msg[1] = (unsigned char) job;
        antall = (unsigned char) job;

    }else if(in == 3){
      msg[0] = 'A';

    }else if(in == 4){
      msg[0] = 'T';
    }else{
      printf("Feil valg: %d\n", in);

    }
    if(in <= 4 && in>=1){
        printf("Message sent: %c\n\n", msg[0]);
        ssize_t ret = send(sock, &msg, sizeof(msg), 0);
        if(ret == -1){
          perror("send()");
          close(sock);
          return EXIT_FAILURE;
        }
        if(msg[0]=='T'){
          return EXIT_SUCCESS;
        }


      for(int i=0; i<antall || msg[0] == 'A'; i++){
        char mbuf[3] = { 0 };
        ssize_t et = recv(sock, mbuf, 2, 0);
        if(et == 0){
          /* Sjekker om server disconnecter */
          printf("SERVER disconnected: \n");
          return EXIT_SUCCESS;
        }else if (et == -1){
          perror("recv()");
          return EXIT_FAILURE;
        }

        char type=mbuf[0];
        printf("Job Type: %c\n", type );
        if(type=='Q'){
          printf("Ingen flere Jobber\n\n");
          send(sock, "T", 1, 0);
          return EXIT_SUCCESS;
        }

        // printf("TYPE: %c\n", mbuf[0]);
        unsigned char length = mbuf[1];
        // printf("LENGTH: %d\n", length);
        char minne[length];
        memset(minne, 0, length);
        ssize_t mes = recv(sock, minne, length, 0);
        if(mes == 0){
          /* Sjekker om server disconnecter */
          printf("SERVER disconnected: \n");
          return EXIT_SUCCESS;
        }else if (et == -1){
          perror("recv()");
          return EXIT_FAILURE;
        }
        minne[length] = '\0';
        char buf[length +2];
        memset(buf, 0, sizeof(buf));
        strcat(buf, mbuf);
        strcat(buf, minne);

        if(type=='O'){
          int i = write(pc1[1], buf, strlen(buf));
          if(i==-1){
            perror("write()");
            fprintf(stderr, "Pipe pc1 dosent work\n");
            return EXIT_FAILURE;
          }

        }
        else if(type=='E'){
          int k = write(pc2[1], buf, strlen(buf));
          if(k==-1){
            perror("write()");
            fprintf(stderr, "Pipe pc2 dosent work\n");
            return EXIT_FAILURE;
          }
        }
        else{
          fprintf(stderr, "Error with transfer\n");
          send(sock, "T", 1, 0);
          return EXIT_SUCCESS;
        }
        // Timing av barneprosesser og parrent pa 1 ms
        usleep(1000);

      }
    }
  }
  return EXIT_SUCCESS;
}

/*
 * Funksjon makeChildren
 *
 * Opretter alle barn
 * og oppretter pipes mellom dem
 *
 * Her venter barna pa beskjeder som printes ut til stderr, eller stdout
 *
 * Hvis barna motar en medling med Q, terminerer de.
 *
 * RETURN int:
 *  Returnerer EXIT_SUCCESS ved success, eller  EXIT_FAILURE ved feil.
 *  Retur verdien blir sjekket i main.
 */

 int makeChildren(){
   //opretter pipes
   /* pipe child 1 */
   if(pipe(pc1) == -1){
     perror("pipe()");
     return EXIT_FAILURE;
   }
   /* pipe child 2 */
   if(pipe(pc2) == -1){
     perror("pipe()");
     return EXIT_FAILURE;
   }

   /* oppretter childs */
   pid = fork();
   if(pid == -1){
     perror("pid()");
     return EXIT_FAILURE;
   }

   // child 1
   if(pid == 0){
     for(;;){
       char msg[258] = { 0 };
       read(pc1[0], msg, 258);
       if(msg[0]=='Q'){
         printf("Terminating child 1\n");
         exit(EXIT_SUCCESS);
       }
       fprintf(stdout, "Child 1\n" );
       fprintf(stdout, "%s\n" , msg+2);
     }
   }

   pid2 = fork();
   if(pid == -1){
     perror("pid()");
     return EXIT_FAILURE;
   }

   // child 2
   if(pid2 == 0){
     for(;;){
       char msg[258] = { 0 };
       read(pc2[0], msg, 258);
       if(msg[0]=='Q'){
         printf("Terminating child 2\n");
         exit(EXIT_SUCCESS);
       }
       fprintf(stderr, "Child 2\n");
       fprintf(stderr, "%s\n", msg+2);
     }
   }

   return EXIT_SUCCESS;
 }
