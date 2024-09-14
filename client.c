#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 8080            
#define BUF_SIZE 4096

int main(int argc, char **argv) {
    int c, s, bytes;
    char buf[BUF_SIZE];             
    struct hostent *h;
    struct sockaddr_in channel;

    if (argc != 3) {
        printf("To start, enter: client server_name file_name\n");
        exit(-1);
    }

    h = gethostbyname(argv[1]);
    if (!h) {
        printf("gethostbyname could not be found %s\n", argv[1]);
        exit(-1);
    }

    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0) {
        printf("socket call failed\n");
        exit(-1);
    }

    memset(&channel, 0, sizeof(channel));
    channel.sin_family = AF_INET;
    memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
    channel.sin_port = htons(SERVER_PORT);

    c = connect(s, (struct sockaddr *) &channel, sizeof(channel));
    if (c < 0) {
        printf("connection failure\n");
        exit(-1);
    }


    write(s, argv[2], strlen(argv[2]) + 1);


    while (1) {
        bytes = read(s, buf, BUF_SIZE); 
        if (bytes <= 0) exit(0);         
        write(1, buf, bytes);            
    }
}
