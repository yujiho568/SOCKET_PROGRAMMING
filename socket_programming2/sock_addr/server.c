#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>

#define SOCK_PATH "./sock_addr"
#define BUFFER_SIZE 256

int main(){
	int server_sock, client_sock;
	struct sockaddr_un server_addr, client_addr;
	char buffer[BUFFER_SIZE];
	socklen_t client_len = sizeof(client_addr);

	server_sock = socket(AF_UNIX, SOCK_STREAM,0);
	if(server_sock < 0){
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCK_PATH);

	unlink(SOCK_PATH);
	if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_sock, 5) < 0){
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on %s\n", SOCK_PATH);

	client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
	if(client_sock < 0){
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	while(1){
		memset(buffer, 0, BUFFER_SIZE);
		int bytes_read = read(client_sock, buffer, BUFFER_SIZE);
		if(bytes_read <= 0) break;

		printf("Received: %s\n", buffer);

		if(strcmp(buffer, "\\quit")==0) break;
	}

	close(client_sock);
	close(server_sock);
	unlink(SOCK_PATH);
	printf("Server terminated. \n");
	return 0;
}
