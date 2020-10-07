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
#include <string> 

#define PORT 5000 
#define MAXLINE 1000 

using namespace std;

void initiateCardReading(string& Message){

	string cardno;
	getline(cin, cardno, '\n');

	Message = "1," + cardno;
	
		
}

bool verifyResponse(char* response){

	if (strcmp(response,"OK") == 0){
		return true;
	}
	else
		return false;
		
}
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

	
	///////////////////// CARD READIND FUNCTIONALITY ////////////////
	
	do{
	// Final Card message variable
	string Message;
	
	// Inititaing Card reading
	initiateCardReading(Message);
	
	// Send card details to server
	sendto(sockfd, Message.c_str(), MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	
	// waiting for response 
	n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, NULL);
	
	buffer[n] = '\0'; 
	
	cout<<buffer<<endl;
	}while( verifyResponse(buffer) != true);// verifying response
	
	//////////////////////////////////////////////////////////////////
	
	cout<<"Successful"<<endl;
		
	buffer[n] = '\0'; 
	printf("CLIENT >> ");
	printf("Message: ");
	puts(buffer); 

	/* IMPORTANT: If you comment the following line on my system, the client and server print properly */
	// printf("%d", (int)ntohs(servaddr.sin_port));


	// close the descriptor 
	close(sockfd); 
} 
