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
 *
 *
 *
 *
 */

static int sock;
static int pc1[2];
static int pc2[2];
static pid_t pid;
static pid_t pid2;

 /*
  * Handler for feilmeldingen
  * ctrl + c
  * Dette er for a avslutte programmet riktig
  *
  *
  */

void myHandler(){
  printf("ctrl + c sensed\n");
  printf("Terminating program\n");
  send(sock, "E", 1, 0);
  close(sock);
  close(pc1[1]);
  close(pc1[0]);
  close(pc2[1]);
  close(pc2[0]);
  kill(pid, SIGTERM);
  kill(pid2, SIGTERM);
  exit(EXIT_FAILURE);
}

/*
 * Funksjon create_socket
 * Lager socket og oppretter kontakt med server
 *
 *
 *
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

  //Bruker inet_pton istedet for inet_aton grunnet den sjekker ipen opp mot adresse familien
  int ine = inet_pton(AF_INET, ip, &serveraddr.sin_addr.s_addr);
  //Sjekker om inet_pton returnerer noe annet enn 1
  if(ine == -1){
    perror("inet_pton()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Connecting to ip: %s:%d\n", ip, portNb );

  // Prover a koble til server
  if(connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr))){
    //returner 1 hvis ikke klarer a connecte
    fprintf(stderr, "\nCouldent connect to server: %s on port:%s\n", ip, port);
    perror("connect()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Connected to server: %s\n", ip);

  return sock;
}

/*
 * meny for inputs fra bruker
 *
 *
 *
 *
 *
 */

int meny(){
  for(;;){
    printf("Velg et valg fra 1-4\n" );

    printf("1: Hent en jobb fra serveren\n");
    printf("2: Hent X antall jobber fra serveren\n");
    printf("3: Hent alle jobber fra serveren\n");
    printf("4: Avslutte programmet\n");

    int alle=0;
    int antall =1;
    char valg[4];
    fgets(valg, 4, stdin);
    int in = atoi(valg);

    char msg;

    if(in == 1){
      printf("Du valgte valg 1\n");
      msg = 'G';

    }else if(in == 2){
      printf("Velg antall jobber du vil hente: \n");
      char job[256];
      fgets(job, 256, stdin);
      printf("Antall jobber hentet: %s\n", job );
      antall=atoi(job);
      msg = 'G';

    }else if(in == 3){
      printf("Du valgte valg 3\n");
      msg = 'G';
      alle=1;

    }else if(in == 4){
      msg = 'T';
    }else{
      printf("Feil valg\n" );
    }
    if(msg=='G' || msg== 'T'){
      for(int i=0; i<antall || alle; i++){
        printf("Message sent: %c\n", msg);
        ssize_t ret = send(sock, &msg, 1, 0);
        if(ret == -1){
          perror("send()");
          close(sock);
          return EXIT_FAILURE;
        }
        if(msg=='T'){
          return EXIT_SUCCESS;
        }
        char mbuf[256] = { 0 };
        ssize_t et = recv(sock, mbuf, strlen(mbuf) - 1, 0);
        if(et == 0){
          printf("SERVER disconnected: \n");
          /* Sjekker om client disconnecter */
          return EXIT_SUCCESS;
        }
        char type=mbuf[0];
        printf("Job Type: %c\n", type );
        if(type=='Q'){
          printf("Ingen flere Jobber\n");
          send(sock, "T", 1, 0);
          return EXIT_SUCCESS;
        }else if(type=='O'){
          int i = write(pc1[1], mbuf, strlen(mbuf));
          if(i==-1){
            perror("write()");
            fprintf(stderr, "Pipe pc1 dosent work\n");
          }
          // printf("MESSAGE FROM SERVER: %s\n", mbuf+2, strlen(mbuf)-1);
        }else if(type=='E'){
          int k = write(pc2[1], mbuf, strlen(mbuf));
          if(k==-1){
            perror("write()");
            fprintf(stderr, "Pipe pc2 dosent work\n");
          }
          // fprintf(stderr, "Message from server: %s\n", mbuf+2, strlen(mbuf)-1);
        }else{
          fprintf(stderr, "Error with transfer\n");
          send(sock, "T", 1, 0);
          return EXIT_SUCCESS;
        }
        // sleep(1);
        // printf("Length: %d\n", mbuf[1]);

      }
      // TODO: sleepe i 50-200 ms
      // msleep(50)
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
 * Her venter barna p beskjeder som sendes til et felles minne omrade
 *
 *
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

   // child 1 eller O
   if(pid == 0){
     for(;;){
       char msg[258] = { 0 };
       read(pc1[0], msg, 258);
       if(msg[0]=='Q'){
         printf("EXITING child 2\n");
         exit(EXIT_SUCCESS);
       }
       printf("Child 1\n" );
       printf("MESSAGE FROM SERVER: %s\n", msg+2, strlen(msg)-1);
      //  printf("Child1: %s\n", msg+2, strlen(msg)-1);

      //  sleeper 1 sekund for a spare cpu
        // sleep(1);
     }
   }

   pid2 = fork();
   if(pid == -1){
     perror("pid()");
     return EXIT_FAILURE;
   }

   // child 2 eller E
   if(pid2 == 0){
     for(;;){
       char msg[258] = { 0 };
       read(pc2[0], msg, 258);
       if(msg[0]=='Q'){
         printf("EXITING child 2\n");
         exit(EXIT_SUCCESS);
       }
       fprintf(stderr, "Child 2\n");
       fprintf(stderr, "Message from server: %s\n", msg+2, strlen(msg)-1);
      //  printf("Child1: %s\n", msg+2, strlen(msg)-1);
      //  sleeper 1 sekund for a spare cpu
        // sleep(1);
     }
   } else{    /* parrent*/

   }

   return EXIT_SUCCESS;
 }

/*
 * Funksjon main
 * Starter klient programmet. Tar inn en ip og en port. Ipen kan være på
 * Integer form eller et domene. Deretter vil den lage en SIGINT signal behandler.
 *
 *
 *
 */

int main(int argc, char *argv[]) {

  if(argc != 3){
    fprintf(stderr, "Usage: %s <ip> <port> \n", argv[0] );
    exit(EXIT_FAILURE);
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
  sig.sa_handler = myHandler;
  sigaction(SIGINT, &sig, NULL);


  /* oppretter barne prosesser */
  makeChildren();


  /* lager socket */
  int tmp = create_socket(ip, port);
  if(tmp == -1){
    exit(EXIT_FAILURE);
  } else if(tmp == 1){
    fprintf(stderr, "Couldent connect to server, exiting\n" );
    exit(EXIT_FAILURE);
  }
  // printf("socket value: %d\n", sock);
  //lage barn og pipes

  /* Skriver ut meny til bruker og kommuniserer med server programmet */
  meny();


  printf("Terminating program\n");
  close(pc1[1]);
  close(pc1[0]);
  close(pc2[1]);
  close(pc2[0]);
  kill(pid, SIGTERM);
  kill(pid2, SIGTERM);
  close(sock);
  return 0;
}
