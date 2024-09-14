#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 8080              
#define BUF_SIZE 4096                
#define QUEUE_SIZE 10

int main(int argc, char *argv[]) {
    int s, b, l, fd, sa, bytes, on = 1;
    char buf[BUF_SIZE];          
    struct sockaddr_in channel;   


    memset(&channel, 0, sizeof(channel)); 
    channel.sin_family = AF_INET;
    channel.sin_addr.s_addr = htonl(INADDR_ANY);
    channel.sin_port = htons(SERVER_PORT);


    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    
    if (s < 0) { printf("socket call failed"); exit(-1); }

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

    b = bind(s, (struct sockaddr *) &channel, sizeof(channel));  
    if (b < 0) { printf("bind call failed"); exit(-1); }

    l = listen(s, QUEUE_SIZE);   
    if (l < 0) { printf("waiting failure"); exit(-1); }

    while (1) {
        sa = accept(s, 0, 0);   
        if (sa < 0) { printf("access failure"); exit(-1); }

        read(sa, buf, BUF_SIZE);   


        fd = open(buf, O_RDONLY); 
        if (fd < 0) { printf("file open failure"); exit(-1); }

        while (1) {
            bytes = read(fd, buf, BUF_SIZE);   
            if (bytes <= 0) break;             
            write(sa, buf, bytes);             
        }

        close(fd);  
        close(sa);  
    }

    close(s);
}
