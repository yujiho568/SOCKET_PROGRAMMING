// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 256


void send_file(int client_sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        char error_msg[] = "File not found\n";
        send(client_sock, error_msg, strlen(error_msg), 0);
        return;
    }

    char buffer[BUFFER_SIZE];
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
        send(client_sock, buffer, bytes_read, 0);
    }
    fclose(file);
    char end_msg[] = "EOF";
    send(client_sock, end_msg, sizeof(end_msg), 0);
}

void send_service_menu(int client_sock) {
    char menu[] = "[Service List]\n1. Get Current Time\n2. Download File\n3. Echo Server\nEnter: ";
    send(client_sock, menu, strlen(menu), 0);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    send_service_menu(client_sock);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_sock, buffer, BUFFER_SIZE, 0);

        if (strcmp(buffer, "\\quit") == 0) break;

        if (strcmp(buffer, "1") == 0) {
            
            time_t now = time(NULL);
            char time_str[BUFFER_SIZE];
            snprintf(time_str, BUFFER_SIZE, "Current Time: %s", ctime(&now));
            
            send(client_sock, time_str, strlen(time_str), 0);
            
            
        }
        else if (strcmp(buffer, "2") == 0) {
            
            char file_menu[] = "[Available File List]\n1. Book.txt\n2. Linux.png\n3. Go back\nEnter: ";
            send(client_sock, file_menu, strlen(file_menu), 0);

            memset(buffer, 0, BUFFER_SIZE);
            recv(client_sock, buffer, BUFFER_SIZE, 0);

            if (strcmp(buffer, "1") == 0) {
                send_file(client_sock, "Book.txt");
            } else if (strcmp(buffer, "2") == 0) {
                send_file(client_sock, "Linux.png");
            } else {
                send_service_menu(client_sock);
            }
        }
        else if (strcmp(buffer, "3") == 0) {
            
            while (1) {
                memset(buffer, 0, BUFFER_SIZE);
                recv(client_sock, buffer, BUFFER_SIZE, 0);
                if (strcmp(buffer, "\\quit") == 0) break;

                send(client_sock, buffer, strlen(buffer), 0);
            }
        }
        //1
        send_service_menu(client_sock);
    }

    close(client_sock);
    close(server_sock);
    printf("Server terminated.\n");
    return 0;
}

