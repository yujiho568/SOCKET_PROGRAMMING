#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>

#define SOCK_PATH "./sock_addr"
#define BUFFER_SIZE 256

int main(){
	int sock;
	struct sockaddr_un server_addr;
	char buffer[BUFFER_SIZE];

	sock = socket(AF_UNIX, SOCK_STREAM,0);
	if(sock < 0){
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCK_PATH);

	if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	while(1){
		printf("Enter message: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		buffer[strcspn(buffer,"\n")] = 0;

		write(sock, buffer, strlen(buffer)+1);

		if(strcmp(buffer, "\\quit")==0) break;
	}

	close(sock);
	printf("Client terminated.\n");
	return 0;
}
