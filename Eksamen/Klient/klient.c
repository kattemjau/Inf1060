#include <netinet/in.h>
#include <sys/socket.h>
//#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void myHandler(int s){
  printf("ctrl + c sensed\n");
  exit(1);
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
  dig.sa_handler = myHandler;

  sigaction(SIGINT, &sig, NULL);

  // lager ny socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(sock == -1){
    fprintf(stderr, "An socket error has accured\n" );
    perror("socket()");
    exit(EXIT_FAILURE);
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
    exit(EXIT_FAILURE);
  }

  printf("Connecting to ip: %s:%s\n", ip, port );

  // Prover a koble til server
  if(connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr))){
    //returner 1 hvis ikke klarer a connecte
  	fprintf(stderr, "Couldent connect to server: %son:%s\n", ip, port);
    perror("connect()");
  	close(sock);
  	exit(EXIT_FAILURE);
  }

  printf("Connected to server: %s\n", ip);
  //hva som skal skje etter klient er koblet til server:

  //signal handler for ctrl + c from signal.h
  // lage en handler for signal, som kjorer en selvlaget exception

  printf("Terminating program\n");
  close(sock);
  return 0;
}
