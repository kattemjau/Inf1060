#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>


/* define macros commonly used */
#define ERROR 'E'
#define GET 'G'
#define ALL 'A'
#define XJOBS 'X'
#define QUIT 'Q'
#define CORRUPT 'C'
#define TERMINATE 'T'
#define OUT 'O'
#define WRITE 0
#define READ 1

struct sockaddr_in server_addr;
static int sock;
static char msg[2];
static char recieve[258];
int fds2[2];
pid_t pid2;
int fds[2];
pid_t pid;

int exit_flag = 0;

// REMOVE:
int count = 0;

//methods:
int die();
int createSock(char *ip, char *port);
int sendMsg();
int getMsg();
int getMany();
char interperateMsg();
int createChildren();
int handle();
int meny();
int userInteraction();

/* Method which terminates the program:
 *
 * closes all pipes
 * kills child processes
 * closes socket
 * terminates itself
 */
int die()
{
// REMOVE:
    printf("I HAVE DONE %d JOBS\n", count);

    close(fds[0]);
    close(fds[1]);
    close(fds2[0]);
    close(fds2[1]);
    kill(pid, SIGINT);
    kill(pid2, SIGINT);
    close(sock);
    printf("client dying\n");
    exit(EXIT_SUCCESS);

}

/* Method which handles ctrl + c
 *
 * kills child-processes
 * sends msg to server to terminate
 * calls die() function
 */
int handler()
{
    kill(pid, SIGINT);
    kill(pid2, SIGINT);
    printf("\nCTRL+C was pressed, terminating server and client\n");
    msg[0] = ERROR;
    msg[1] = '\0';
    ssize_t ret = send(sock, msg, sizeof(msg), 0);
    die();
    exit_flag = 1;

}


/* function to create sock
 *
 * param: ip
 * param: port
 */
int createSock (char *ip, char *port)
{
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// REMOVE:
    printf("MAKING A SOCKET YWAG\n");

    if (sock == -1) {
        perror("socket()");
        return EXIT_FAILURE;
    }

    int portno = atoi(port);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);

    int ip_r = inet_pton(AF_INET, ip, &server_addr.sin_addr.s_addr);

// TODO: fix so it can handle names
/*if (ip_r == 0)
    {
        fprintf(stderr, "Not valid IP address: %s\n", ip);
        close(sock);
        return EXIT_FAILURE;

    } */
    if (ip_r == -1) {
        perror("inet_pton()");
        close(sock);
        return EXIT_FAILURE;
    }

    printf("Connecting to %s:%d\n", ip, portno);
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Couldnt connect\n");
        perror("connect()");
	close(sock);
	return EXIT_FAILURE;
    }

    printf("CONNECTED \n");

    return sock;
}

int sendMsg()
{
    ssize_t ret = send(sock, msg, sizeof(msg), 0);
    if (ret == -1) {
        perror("sendMsg()");
    }
}


int getMsg()
{
    //memset(recieve, 0, sizeof(recieve));
    char rec [258] = { 0 };
    ssize_t ret = recv(sock, rec, sizeof(rec)-1, 0);

        if (ret == 0) {
            printf("Server disconnected\n");
                //ferdig
        } else if (ret == -1) {
            perror("recv()");
            close(sock);
            return EXIT_FAILURE;
       } else {
            printf("Message from server: %s\n", rec);
            interperateMsg(rec);
// REMOVE: remove this:
            //printing message in binary:
            // for(size_t i = 0; i < strlen(recieve); i++) printf("%02x ", recieve[i]);
            // printf("\n");
       }
}

int getMany()
{
    for (; ;) {
        memset(recieve, 0, sizeof(recieve));
        ssize_t ret = recv(sock, recieve, 2, 0);
        char temp[256];
        memset(temp, 0, sizeof(temp));
        char length = recieve[1];
        ret = recv(sock, temp, length, 0);
        memcpy(recieve+2, temp, length);
// REMOVE:
        printf("HELLOOOO Got here, recieve is now: %s\n", recieve);
        printf("LENGTH OF RECIEVE char length: %d strlen: %d\n", length, strlen(recieve));
        interperateMsg();
// REMOVE:
        printf("AIGHT HDSFGSG\n");
    }
}


char interperateMsg()
{
	char rec[256] = "niggers gonna nigg niggngiggiggig";


      write(fds[WRITE], rec, strlen(rec));
      write(fds2[WRITE], rec, sizeof(rec)-1);
}


int createChildren()
{

	if (pipe(fds) == -1) {
		perror("pipe()");
		exit(EXIT_FAILURE);
	}

        if (pipe(fds2) == -1) {
		perror("pipe()");
                return EXIT_FAILURE;

        }

        /* pipe child 1 */
	pid = fork();
	if (pid == -1) {
		perror("fork()");
		exit(EXIT_FAILURE);
	}
        //parent lukke lesesiden

        /* check if child 1 */
	if (pid == 0) {
            printf("BABY 1 alive\n");
            close(fds[WRITE]);
            for (; ;) {
                char bufOut [258] = { 0 };
                read(fds[READ], bufOut, sizeof(bufOut));
                printf("fds 1 %s\n", bufOut);
                char bufOut1 [258] = { 0 };
                read(fds2[READ], bufOut1, sizeof(bufOut1));
                printf("fds 2> %s\n", bufOut1);

                sleep(1);
            }
	} else {
            //parent stenger skrivesiden
            printf("CLOSED 1\n");
            //close(fds[READ]);
        }

        /* child 2 */
	pid2 = fork();
	if (pid == -1) {
		perror("fork()");
                return EXIT_FAILURE;
	}


        /* check if child 2 */
        if (pid2 == 0) {
            printf("BABY 2 alive\n");
            close(fds2[WRITE]);
            for (; ; ){
                char bufErr [258] = {0};
                read(fds2[0], bufErr, sizeof(bufErr)-1);
                if (bufErr[0] == QUIT) {

                }
                if (bufErr[0] == ERROR) {
                    fprintf(stderr, "Error: %s\n", bufErr+2, sizeof(bufErr)-1);
                }
                memset(bufErr, 0, sizeof(bufErr));

            }
	} else {
            printf("CLOSED 2\n");
            //close(fds2[READ]);
        }
        return EXIT_SUCCESS;
}

int meny()
{

    printf("\n--Menu--\n");
    printf("1 - Get next job\n");
    printf("2 - Get x numbers of jobs\n");
    printf("3 - Get all remaining jobs\n");
    printf("0 - Terminate\n");

    return EXIT_SUCCESS;
}


int userInteraction()
{

    for (;;) {
        meny();
    	int in = 0;
        scanf("%d", &in);
        int no = 1;

        // memset(msg, 0, sizeof(msg));

        if(in == 1){
            //get next job
            msg[0] = GET;
            msg[1] = '\0';
            sendMsg();
            getMsg();
            interperateMsg();

        } else if (in == 2){
            printf("How many jobs do you want to get? (max 255)\n");
            msg[0] = XJOBS;
            msg[1] = '\0';
            //read the number
            scanf("%d", &no);


//TODO: fix other flag to ask for several jobs
            //sendMsg();

            return EXIT_SUCCESS;
// FIXME: perhaps : i being the int
            //c = i +'0';
            //
            return EXIT_SUCCESS;

        } else if (in == 3){
            printf("Getting all jobs:\n");
            msg[0] = 'A';
            msg[1]= '\0';
            printf("Msg is: %c\n", msg[0]);
            sendMsg();
            getMany();


            return EXIT_SUCCESS;
        } else if (in == 0){
            printf("You have ended the program\n");
            msg[0] = TERMINATE;
            msg[1] = '\0';


        }  else {
            printf("Invalid input: %d\n", in);
        }

    }

    return EXIT_SUCCESS;
}



int main (int argc, char* argv [])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP/Hostname> <port> \n", argv[0]);
        return EXIT_FAILURE;

    }

    char *ip;

    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = handler;

    ip = argv[1];
    printf("Making socket\n");
    int temp = createSock(ip, argv[2]);
    if (temp == EXIT_FAILURE) {
        fprintf(stderr, "Couldn't connect to server\n");
        exit(EXIT_FAILURE);
    }

    //MAKE DEM BABIES
    createChildren();

    if(sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction()");
    }

    userInteraction();

    kill(pid, SIGTERM);
    kill(pid2, SIGTERM);
    close(sock);

    return EXIT_SUCCESS;

}
