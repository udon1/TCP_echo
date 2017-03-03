#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sysexits.h>

#define BUF 512

struct echo_msg {
    unsigned short seq;
    unsigned short reserve;
    char msg[32];
};

int main (int argc, char *argv[]) {
    in_port_t myport;
    int soc, soc1;
    socklen_t opt_len;
    struct sockaddr_in myskt;
    socklen_t fromlen;
    struct sockaddr_in skt;
    struct echo_msg msg;
    int procid = 0;
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./udpserver [port]\n");		
        exit(1);
    } 
    myport = atoi(argv[1]);
    if((soc = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return (-1);
    } 

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = PF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    opt_len = sizeof(myskt);

    /*bind*/
    if(bind(soc, (struct sockaddr *)&myskt, opt_len) == -1) {
        perror("bind");
        return (-1);
    } 

    fromlen = sizeof(skt);
    /*listen*/
    if (listen(soc, 5) == -1) {
        perror("listen");
        return (-1);
    }
    while(1) {
        /*accept*/
        if ((pid = fork()) == -1) {
            perror("fork");
            return (-1);
        } else if ((pid = fork()) == 0) {

        } else {
            procid++;
        }
        if ((soc1 = (accept(soc, (struct sockaddr *)&skt, &fromlen))) == -1) {
            perror("accept");
            return (-1);
        }

        for(;;) {
            /*仕様に合うように実装*/
            if ((recv(soc1, &msg, sizeof(msg), 0)) == -1) {
                perror("recv");
                exit(-1);
            } 

            //printf("clisent%d  ", getpid());
            printf("clisent%d  ", procid);
            printf("seq: %d, msg: %s\n", msg.seq, msg.msg);
            msg.seq++;

            if ((send(soc1, &msg, sizeof(msg), 0)) == -1) {
                perror("send");
                exit(-1);
            } 
            if (strcmp(msg.msg, "FIN\n") == 0 || msg.seq >= 10) {
                break;
            }
        }
        fprintf(stderr, "close\n");
        close(soc1);
    }
    close(soc);
    fprintf(stderr, "exit main\n");
    return 0;
}
