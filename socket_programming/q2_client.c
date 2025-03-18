#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(char *message) {
    perror(message);
    exit(1);
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(12345);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);
        write(sock, buffer, strlen(buffer));
        if (strcmp(buffer, "quit\n") == 0) break;

        int str_len = read(sock, buffer, BUF_SIZE - 1);
        buffer[str_len] = '\0';
        printf("Recv from Server:\n%s\n", buffer);
    }

    close(sock);
    return 0;
}

