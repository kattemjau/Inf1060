#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

//#include <netdb.h> get host by name
// #include <unistd.h> pipe it up

/*
 * Client program
 *
 *
 *
 *
 *
 */



 /*
  * Handler for feilmeldingen
  * ctrl + c
  * Dette er for a avslutte programmet riktig
  *
  *
  */

void myHandler(){
  printf("\nctrl + c sensed\n");
  printf("Terminating program\n");
  //TODO: terminate program
  // close socet
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
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
  int ine = inet_pton(AF_INET, ip, &serveraddr.sin_addr);

  //Sjekker om inet_pton returnerer noe annet enn 1
  if(ine == 0){
    fprintf(stderr, "Not a valid ip adress: %s\n", ip );

  }else if(ine == -1){
    perror("inet_pton()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Connecting to ip: %s:%d\n", ip, portNb );

  // Prover a koble til server
  if(connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr))){
    //returner 1 hvis ikke klarer a connecte
    fprintf(stderr, "Couldent connect to server: %s on port:%s\n", ip, port);
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

void meny(int sock){

  int running =1;
  while(running){
    printf("Velg et valg fra 1-4\n" );

    printf("1: Hent en jobb fra serveren\n");
    printf("2: Hent X antall jobber fra serveren\n");
    printf("3: Hent alle jobber fra serveren\n");
    printf("4: Avslutte programmet\n");



    char valg[4];
    fgets(valg, 4, stdin);
    int in = atoi(valg);

    char msg[256] = { 0 };

    if(in == 1){
      printf("Du valgte valg 1\n");
      msg[0] = 'G';

    }else if(in == 2){
      printf("Velg antall jobber du vil hente: \n");
      char job[512];
      fgets(job, 512, stdin);
      printf("Antall jobber hentet: %s\n", job );

    }else if(in == 3){
      printf("Du valgte valg 3\n");


    }else if(in == 4){
      msg[0] = 'T';
      running=0;
    }else{
      printf("Feil valg\n" );
    }
    printf("Message sent: %s\n", msg);

    ssize_t ret = send(sock, msg, strlen(msg), 0);
    if(ret == -1){
      perror("send()");
      close(sock);
      return EXIT_FAILURE;
    }



  }
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

   int fds[2];

   if(pipe(fds) == -1){
     perror("pipe()");
     return EXIT_FAILURE;
   }
   //opretter pipe



   pid_t pid = fork();
   if(pid == -1){
     perror("pid()");
     return EXIT_FAILURE;
   }


   char *msg = "dicks out for Harambe";
   // child 1
   if(pid == 0){
     /*for(;;){

       sleep(1);
     }*/
     close(fds[0]);
     write(fds[1], msg, strlen(msg));
     exit(EXIT_SUCCESS);

   }

   pid_t pid2 = fork();
   if(pid == -1){
     perror("pid()");
     return EXIT_FAILURE;
   }

   // child 2
   if(pid2 == 0){
    /* for(;;){

       sleep(1);
     }*/
    printf("Dis is child 2\n" );
    exit(EXIT_SUCCESS);

   } else{    /* parrent*/
     close(fds[1]);
     read(fds[0], msg, strlen(msg) - 1);
     printf("%s\n", msg);
   }

   return EXIT_SUCCESS;

 }


/*
 * Funksjon main
 *
 *
 *
 *
 *
 */

int main(int argc, char const *argv[]) {

  if(argc != 3){
    fprintf(stderr, "Usage: %s <ip> <port> \n", argv[0] );
    exit(EXIT_FAILURE);
  }

  int sock;
  char *ip=argv[1];
  char *port=argv[2];

  //lager en ctrl + c signal behandler
  struct sigaction sig;
  sig.sa_handler = myHandler;

  sigaction(SIGINT, &sig, NULL);

  makeChildren();

/*
  // lager ny socket
   sock = create_socket(ip, port);
  if(sock == -1){
    exit(EXIT_FAILURE);
  } else if(sock == 1){
    fprintf(stderr, "Couldent connect to server, exiting\n" );
    exit(EXIT_FAILURE);
  }
  printf("socket value: %d\n", sock);
  //lage barn og pipes

  meny(sock);
*/

  printf("Terminating program\n");
  close(sock);
  return 0;
}
