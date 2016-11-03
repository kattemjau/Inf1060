#include <netinet/in.h>
#include <sys/socket.h>
//#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  if(argc != 3){
    fprintf(stderr, "Usage: %s <ip> <port> \n", argv[0] );
    exit(EXIT_FAILURE);
  }

  int sock;
  char *ip=argv[1];
  char *port=argv[2];

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
    perror("connect()");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Connecting to ip: %s:%s\n", ip, port );

  if(connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr))){
    //returner 1 hvis ikke klarer a connecte


  }




  close(sock);
  return 0;
}
