#include <netinet/in.h>
#include <sys/socket.h>
//#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/*
 * Server program
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

void myHandler(int s){
  printf("\nctrl + c sensed\n");
  exit(EXIT_SUCCESS);

}

/*
 * Funksjon create_socket
 * Lager socket og binder port
 *
 *
 *
 */

int create_socket(char *port){

  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock == -1){
    perror("socket()");
    return EXIT_FAILURE;
  }

  int portNb = atoi(port);

  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(struct sockaddr_in));
  // server_addr.sin_family = AF_INET;
  serveraddr.sin_port=htons(portNb);

  // har ikke server sin ip
  // TODO: legge til &server_addr.sin_addr.s_addr ??
  /*
  int pton = inet_pton(AF_INET, ip, &server_addr.sin_addr);

  if(pton == -1){
    perror("inet_pton()");
    close(sock);
    return EXIT_FAILURE;
  } else if(pton == 0){
      fprintf(stderr, "inet_pton family errror\n");
    close(sock);
    return EXIT_FAILURE;
  } */

  int is=1;
  if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &is, sizeof(int))){
    perror("setsockopt()");
    close(sock);
    return EXIT_FAILURE;
  }


  if(bind(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr))){
    fprintf(stderr, "PORT%s\n", port);
    perror("bind()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Sucessfully binded to port: %s\n", port);

  if(listen(sock, SOMAXCONN)){
    perror("listen()");
    close(sock);
    return EXIT_FAILURE;
  }


  return sock;


}

/*
 *  Funksjon accept_connections
 *  handler innkommende klienter
 *  lar kun 1 koble seg til om gangen
 *
 *
 *
 */

int accept_connections(int sock){
  struct sockaddr_in caddr;
  memset(&caddr, 0, sizeof(caddr));
  socklen_t addrlen = sizeof(caddr);

  for (;;) {
    printf("Waiting for clients to connect: \n" );
    int csock = accept(sock, (struct sockaddr *)&caddr, &addrlen);

    if(csock == -1){
      perror("accept()");
      close(sock);
      return EXIT_FAILURE;
    }
    printf("Client connected!\n" );

    char buf[256] = { 0 };
    ssize_t ret = recv(csock, buf, strlen(buf) - 1, 0);
    if(ret == 0){
      printf("Client disconnected: \n");
    }else if(ret == -1){
      perror("recv()");
      close(csock);
      return EXIT_FAILURE;

    }else{
      printf("Message from client: %s\n", buf);

    }

    close(csock);
  }


  //char *clientIp = inet_ntoa(caddr.sin_addr);
  //printf("Ip/port: %s:%x\n",clientIp, caddr.sin_addr.s_addr );
  return 0;

}

/*
 *  Funksjon lesjobs
 *  leser fra f\il
 *
 *
 *
 *
 */

void lesJobs(){

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
    fprintf(stderr, "Usage: %s <job> <port> \n", argv[0] );
    exit(EXIT_FAILURE);
  }

  //int sock;
  char *job=argv[1];
  char *port=argv[2];

  //ctrl + c handling
  struct sigaction sig;
  // memset(&sig, 0, sizeof(sig));
  sig.sa_handler = myHandler;
  sigaction(SIGINT, &sig, NULL);

  int sock = create_socket(port);

  if(sock == -1){
    exit(EXIT_FAILURE);
  }
  accept_connections(sock);


  close(sock);
  return 0;
}
