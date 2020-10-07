// udp client driver program 
#include <iostream>
#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<stdlib.h> 

#define PORT 5000 
#define MAXLINE 1000 

using namespace std;

// Driver code 
int main() 
{ 
	char buffer[100]; 
	string message = "Hello Server"; 
	int sockfd, n; 
	struct sockaddr_in servaddr; 
	
	// clear servaddr 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_addr.s_addr = inet_addr("172.16.54.75"); 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_family = AF_INET; 
	
	// create datagram socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	// connect to server 
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	{ 
		printf("\n Error : Connect Failed \n"); 
		exit(0); 
	} 


	getline(cin, message, '\n');


	// request to send datagram 
	// no need to specify server address in sendto 
	// connect stores the peers IP and port 
	sendto(sockfd, message.c_str(), MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	
	// waiting for response 
	n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, NULL); 
	buffer[n] = '\0'; 
	printf("CLIENT >> ");
	printf("Message: ");
	puts(buffer); 

	/* IMPORTANT: If you comment the following line on my system, the client and server print properly */
	// printf("%d", (int)ntohs(servaddr.sin_port));


	// close the descriptor 
	close(sockfd); 
} 
