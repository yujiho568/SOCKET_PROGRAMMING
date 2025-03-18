#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define BUF_SIZE 1024
#define BACKLOG 5

int client_cnt = 0;

void error_handling(char *message) {
    perror(message);
    exit(1);
}

void handle_client(int client_sock) {
    char buffer[BUF_SIZE];
    while (1) {
        int str_len = read(client_sock, buffer, sizeof(buffer) - 1);
        if (str_len == 0) break; 
        buffer[str_len] = '\0';
        printf("Recv from Client: %s\n", buffer);
        write(client_sock, buffer, str_len); 
        if (strcmp(buffer, "quit\n") == 0) break;
    }
    close(client_sock);
}

int main() {
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_size;
    pid_t pid;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(12345);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, BACKLOG) == -1)
        error_handling("listen() error");

    while (1) {
        client_addr_size = sizeof(client_addr);
        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);
        if (client_sock == -1) continue;

        client_cnt++;
        printf("New Client!\n");
        printf("Number of service client: %d\n", client_cnt);

        pid = fork();
        if (pid == -1) {
            close(client_sock);
            continue;
        }
        if (pid == 0) { 
            close(serv_sock);
            handle_client(client_sock);
            exit(0);
        } else {
            close(client_sock);
        }
    }
    close(serv_sock);
    return 0;
}

