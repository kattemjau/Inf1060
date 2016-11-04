#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//#include <netdb.h> get host by name
// #include <unistd.h> pipe it up

/*
* client
*
*
*
*
*
*/

void myHandler(int s){
  printf("\nctrl + c sensed\n");
  printf("Terminating program\n");
  //TODO: terminate program
  // close socet
  exit(EXIT_FAILURE);
}

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
  //hva som skal skje etter klient er koblet til server:

  //signal handler for ctrl + c from signal.h
  // lage en handler for signal, som kjorer en selvlaget exception

  return sock;
}


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

  // lager ny socket
  sock = create_socket(ip, port);
  if(sock == -1){
    exit(EXIT_FAILURE);
  }
  //lage barn og pipes

  printf("Terminating program\n");
  close(sock);
  return 0;
}
