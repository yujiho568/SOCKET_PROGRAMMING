#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    fd_set reads, cpy_reads;
    char buf[BUF_SIZE];
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(12345);
    
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    
    FD_ZERO(&reads);
    FD_SET(0, &reads); 
    FD_SET(sock, &reads); 
    
    
    
    while (1) {
        cpy_reads = reads;
        
        if (select(sock + 1, &cpy_reads, NULL, NULL, NULL) == -1)
            break;
        
        if (FD_ISSET(0, &cpy_reads)) { 
            printf("Enter: ");
            fgets(buf, BUF_SIZE, stdin);
            if (!strcmp(buf, "quit\n")) break;
            write(sock, buf, strlen(buf));
        }
        
        
    }

    close(sock);
    return 0;
}

void error_handling(char *message) {
    perror(message);
    exit(1);
}

