#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 256

int main(){
	int sock;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0){
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <=0){
		perror("Invalid address or address not supported");
		exit(EXIT_FAILURE);
	}

	if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	while(1){
		printf("Enter message: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		buffer[strcspn(buffer,"\n")] = 0;

		send(sock, buffer, strlen(buffer)+1,0);

		if(strcmp(buffer, "\\quit")==0) break;

		memset(buffer, 0, BUFFER_SIZE);
		if(recv(sock, buffer, BUFFER_SIZE, 0) <=0 ) break;
		printf("[You] %s\n", buffer);
	}

	close(sock);
	printf("Client terminated.\n");
	return 0;
}
