#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256

int main(){
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	char buffer[BUFFER_SIZE];
	socklen_t client_len = sizeof(client_addr);

	server_sock = socket(AF_INET, SOCK_STREAM,0);
	if(server_sock < 0){
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_sock, 5) < 0){
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);

	client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
	if(client_sock < 0){
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	while(1){
		memset(buffer, 0, BUFFER_SIZE);
		if(recv(client_sock, buffer, BUFFER_SIZE,0)<=0) break;
		printf("[You] %s\n", buffer);

		if(strcmp(buffer, "\\quit")==0) break;

		printf("Enter message: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		buffer[strcspn(buffer, "\n")] = 0;

		send(client_sock, buffer, strlen(buffer) + 1, 0);
		if(strcmp(buffer, "\\quit")==0) break;
	}

	close(client_sock);
	close(server_sock);
	printf("Server terminated. \n");
	return 0;
}
