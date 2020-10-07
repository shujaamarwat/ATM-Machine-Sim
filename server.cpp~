// server program for udp connection 
#include <iostream>
#include <stdio.h> 
#include <string.h> 
#include <cstring> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#define PORT 5000 
#define MAXLINE 1000 

char* reverse (char * message){
	int length = strlen(message);
	char* reverseMessage = new char [length];

	for (int i=0, j=length-1; i<length; i++, j--){
		reverseMessage[i] = message[j];
	}

	return reverseMessage;
}


using namespace std;
// Driver code 
int main() 
{ 
	char buffer[100]; 
	char *message = "Hello Client"; 
	int listenfd;
	unsigned int len; 
	struct sockaddr_in servaddr, cliaddr; 
	bzero(&servaddr, sizeof(servaddr)); 

	// Create a UDP Socket 
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);		 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_family = AF_INET; 

	// bind server address to socket descriptor 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	
	int exit = 0;
	//receive the datagram 
	do{
		len = sizeof(cliaddr); 
		int n = recvfrom(listenfd, buffer, sizeof(buffer), 
				0, (struct sockaddr*)&cliaddr,&len); //receive message from server 
		buffer[n] = '\0'; 
		printf("SERVER >> ");
		printf("Message: ");
		puts(buffer);

		if (strcmp(buffer, "exit") == 0){
			exit = 1;
			break;
		}

			//reverse the received string
		auto reversed = reverse(buffer);
		printf("Reversed the message to '%s'...\n", reversed);

			// send the response 
		printf("Sending response\n");
		sendto(listenfd, reversed, MAXLINE, 0, 
			(struct sockaddr*)&cliaddr, sizeof(cliaddr));
	} while(!exit);

	printf("Exiting.\n");

}
