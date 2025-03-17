#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 256

void receive_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("File creation failed");
        return;
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);  
        ssize_t bytes_received = recv(sock, buffer, BUFFER_SIZE, 0); 
        if (bytes_received <= 0 || strcmp(buffer, "EOF") == 0) break;
        fwrite(buffer, 1, bytes_received, file);
    }
    fclose(file);
    printf("File %s downloaded successfully\n", filename);
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        
        memset(buffer, 0, BUFFER_SIZE);  
        if (recv(sock, buffer, BUFFER_SIZE, 0) <= 0) {
            perror("Server closed the connection");
            break;
        }
        printf("%s", buffer);  

        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; 

        if (send(sock, buffer, strlen(buffer) + 1, 0) <= 0) {
            perror("Failed to send message to server");
            break;
        }
        if (strcmp(buffer, "\\quit") == 0) break;

        if (strcmp(buffer, "1") == 0) { 
            memset(buffer, 0, BUFFER_SIZE);  
            if (recv(sock, buffer, BUFFER_SIZE, 0) > 0) {
                printf("%s\n", buffer);  
            }
        }
        else if (strcmp(buffer, "2") == 0) {  
            memset(buffer, 0, BUFFER_SIZE);  
            if (recv(sock, buffer, BUFFER_SIZE, 0) > 0) {
                printf("%s", buffer); 

                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strcspn(buffer, "\n")] = 0;  

                if (send(sock, buffer, strlen(buffer) + 1, 0) > 0) {
                    if (strcmp(buffer, "1") == 0) {
                        receive_file(sock, "Book.txt");
                    } else if (strcmp(buffer, "2") == 0) {
                        receive_file(sock, "Linux.png");
                    }
                }
            }
        }
        else if (strcmp(buffer, "3") == 0) {  
            while (1) {
                printf("Enter message to echo (\\quit to exit): ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strcspn(buffer, "\n")] = 0; 

                if (send(sock, buffer, strlen(buffer) + 1, 0) <= 0) break;
                if (strcmp(buffer, "\\quit") == 0) break;

                memset(buffer, 0, BUFFER_SIZE);  
                if (recv(sock, buffer, BUFFER_SIZE, 0) > 0) {
                    printf("[You] %s\n", buffer);
                }
            }
        }
    }

    close(sock);
    printf("Client terminated.\n");
    return 0;
}

