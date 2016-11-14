#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

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

static FILE *infile;
static int sock;
static int csock;

void myHandler(){
  printf("\nctrl + c sensed\n");
  close(sock);
  close(csock);
  fclose(infile);
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

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock == -1){
    perror("socket()");
    return EXIT_FAILURE;
  }

  int portNb = atoi(port);

  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(struct sockaddr_in));
  // server_addr.sin_family = AF_INET;
  serveraddr.sin_port=htons(portNb);


  /* fora kunne starten en ny instance av en server etter avslutning */
  int is=1;
  if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &is, sizeof(int))){
    perror("setsockopt()");
    close(sock);
    return EXIT_FAILURE;
  }

  // bind til en spesifikk port
  if(bind(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr))){
    fprintf(stderr, "PORT%s\n", port);
    perror("bind()");
    close(sock);
    return EXIT_FAILURE;
  }

  printf("Sucessfully binded to port: %s\n", port);

  //sjekke etter trafikk pa porten
  if(listen(sock, SOMAXCONN)){
    perror("listen()");
    close(sock);
    return EXIT_FAILURE;
  }


  return sock;


}

static char msg[256];

void getJob(){
  unsigned char type;
  unsigned char length;
  printf("Starter innlesning\n");
  //leser jobb type
  if(!fread(&type, sizeof(char), 1, infile)){
    printf("END OF JOBS\n");
    memset(msg, 0, sizeof(char) + sizeof(int));
    msg[0]='Q';
    msg[1]=0;
    return;
  }

  printf("Operation: %c\n", type);
  //leser lengen
  fread(&length, sizeof(char), 1, infile);

  printf("length: %d\n", length);

  char mbuf[length];
  //manually initialize array
  memset(mbuf, 0, length*sizeof(char)+2);

  fread(mbuf, sizeof(char), length, infile);
  // legge til \0 i buf
  mbuf[length]='\0';
  printf("MSG: %s\n", mbuf);

  memset(msg, 0, strlen(mbuf)+sizeof(char)*2);
  msg[0]=type;
  msg[1]=length;
  strcat(msg, mbuf);

}

/*
 *  Funksjon accept_connections
 *  handler innkommende klienter
 *  lar kun 1 koble seg til om gangen
 *
 *
 *
 */

int accept_connections(){
  struct sockaddr_in caddr;
  memset(&caddr, 0, sizeof(caddr));
  socklen_t addrlen = sizeof(caddr);

    printf("Waiting for clients to connect: \n" );
    csock = accept(sock, (struct sockaddr *)&caddr, &addrlen);
    if(csock == -1){
      perror("accept()");
      close(sock);
      return EXIT_FAILURE;
    }

    printf("Client connected!\n" );

    //apner fil for innlesning
    /* bare 1 client kan connecte */
    for (;;) {

    char buf[256] = { 0 };
    ssize_t ret = recv(csock, buf, sizeof(buf) - 1, 0);
    if(ret == 0){
      printf("Client disconnected: \n");
      /* Sjekker om client disconnecter */
      return EXIT_SUCCESS;
    }
    else if(ret == -1){
      close(csock);
      perror("recv()");
      return EXIT_FAILURE;

    }else{
      printf("Message from client: %s\n", buf);
      char temp = buf[0];
      if(temp == 'G'){
        printf("Henter job\n");
        getJob();
        // printf("\nMSG: %s\n", msg);
        ssize_t te = send(csock, &msg, strlen(msg), 0);
        if(te == -1){
          perror("send()");
          close(sock);
          return EXIT_FAILURE;
        }
      }else if(temp == 'T' || temp == 'E'){
        if(temp == 'E'){
          fprintf(stderr, "Client exitet by error\n");
        }
        close(csock);
        fclose(infile);
        close(sock);
        printf("Close correctly\n");
        exit(EXIT_SUCCESS);
      }else{
        printf("Wrong msg recieved from client\n");
      }
    }

  }
  fclose(infile);
  close(csock);
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

int main(int argc, char *argv[]) {

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

  infile = fopen(job, "r");
  if(infile == NULL){
    perror("fopen()");
    exit(EXIT_FAILURE);
  }


  create_socket(port);

  if(sock == -1){
    exit(EXIT_FAILURE);
  }
  accept_connections();


  close(sock);
  return 0;
}
