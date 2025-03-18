#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 1024
#define MAX_CLIENT 3 // 최대 클라이언트 수

void error_handling(char *message);

int main() {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    
    fd_set reads, cpy_reads;
    int fd_max, fd_num, i;
    char buf[BUF_SIZE];
    int client_socks[MAX_CLIENT]; 
    int connected_clients = 0;   

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(12345);
    
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if (listen(serv_sock, MAX_CLIENT) == -1)
        error_handling("listen() error");
    
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    

    while (connected_clients < MAX_CLIENT) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            perror("accept() error");
            continue;
        }
        
        client_socks[connected_clients++] = clnt_sock; 
        FD_SET(clnt_sock, &reads);                    
        if (clnt_sock > fd_max) fd_max = clnt_sock;   
        printf("Client #%d connected.\n", connected_clients);
    }


    while (1) {
        cpy_reads = reads;
        printf("waiting at select...\n");
        fd_num = select(fd_max + 1, &cpy_reads, NULL, NULL, NULL);
        printf("select returned: %d\n", fd_num);
        if (fd_num == -1) break;

        for (i = 0; i < MAX_CLIENT; i++) { 
            int sock = client_socks[i];
            if (FD_ISSET(sock, &cpy_reads)) { 
                int str_len = read(sock, buf, BUF_SIZE - 1);
                if (str_len == 0) { 
                    FD_CLR(sock, &reads);
                    close(sock);
                    printf("Client %d disconnected.\n", i + 1);
                } else {
                    buf[str_len] = '\0';
                    printf("MSG from Client %d: %s\n", i + 1, buf);
                }
            }
        }
    }

    for (i = 0; i < MAX_CLIENT; i++) {
        close(client_socks[i]);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    perror(message);
    exit(1);
}

