#include <netinet/in.h>
#include <sys/socket.h>
//#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void myHandler(int s){
  printf("ctrl + c sensed\n");
  exit(EXIT_FAILURE);

}

int create_socket(char *port){

  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock == -1){
    perror("socket()");
    return EXIT_FAILURE;
  }

  int portNb = atoi(port);
  
  struct sockaddr_in serveraddr;
  memset(&sockaddr_in, 0, sizeof(struct sockaddr_in));
  // server_addr.sin_family = AF_INET;
  server_addr.sin_port=htons(portNb);

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


  if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))){
    perror("setsockopt()");
    close(sock);
    return EXIT_FAILURE;
  }

  if(bind(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr))){
    perror("bind()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Sucessfully binded to port: %s\n", port);

  return sock;


}

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
  sa.sa_handler = myHandler;
  sigaction(SIGINT, &sa, NULL);

  int sock = create_socket(port);

  if(sock ){

  }


  close(sock);
  return 0;
}
