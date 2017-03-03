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
    int soc;
    socklen_t opt_len;
    struct sockaddr_in myskt;
    socklen_t fromlen;
    struct sockaddr_in skt;
    struct echo_msg msg;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./udpserver [port]\n");		
        exit(1);
    } else {
        printf("argc: 2\n");
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
        if ((soc = (accept(soc, (struct sockaddr *)&skt, &fromlen))) == -1) {
            perror("");
            return (-1);
            }

        for(;;) {
            /*仕様に合うように実装*/
            if ((recv(soc, &msg, sizeof(msg), 0)) == -1) {
                perror("recvfrom");
                exit(-1);
            } 

            printf("seq: %d, msg: %s\n", msg.seq, msg.msg);
            msg.seq++;

            if ((send(soc, &msg, sizeof(msg), 0)) == -1) {
                perror("sendto");
                exit(-1);
            } 
        }
        close(soc);
    }
    return 0;
}
