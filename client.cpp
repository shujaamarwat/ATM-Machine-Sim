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

string initiateCardReading(string& Message){

	string cardno;
	getline(cin, cardno, '\n');

	Message = "1," + cardno;
	
	return cardno;	
}

bool verifyResponse(char* response){

	if (strcmp(response,"OK") == 0){
		return true;
	}
	else
		return false;
		
}



bool makeToken(char * response){

	int count=0;
	char *token = strtok(response, ",");
	char *tt;
	char *ttt;
	while (token != NULL) 
    	{ 
    		if (count==0)
    		{
			strcpy(tt, token);
	  		if (verifyResponse(tt))
			{
				cout<<"Successfull"<<endl;
			}	
			token = strtok(NULL, ","); 
			count++;
		}
		else
		{
			strcpy(ttt, token);	
		}
    	}
	
	
}


void pin_veri(string &full_pin,string &card)
{	
	string pin;
	getline(cin,pin,'\n');
	
	full_pin="2,"+card + ","+ pin;
	cout<<full_pin<<endl;
		
}

int main() 
{ 
	char buffer[100], buffertoken[100];
	string message = "Hello Server"; 
	string full_pin;
	int sockfd, n; 
	struct sockaddr_in servaddr; 
	string card;
	
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
	
	do{
	// Final Card message variable
	string Message;
	
	// Inititaing Card reading
	card = initiateCardReading(Message);
	
	// Send card details to server
	sendto(sockfd, Message.c_str(), MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	
	// waiting for response 
	n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, NULL);
	
	buffer[n] = '\0'; 
	
	}while( verifyResponse(buffer) != true);// verifying response// verifying response

	cout<<"User Found"<<endl;

	//getline(cin, message, '\n');
	
	///////// Enter PIN///////////////
	
		pin_veri(full_pin,card);
		
		sendto(sockfd, full_pin.c_str(), MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	
		n = recvfrom(sockfd, buffertoken, sizeof(buffertoken), 0, (struct sockaddr *)&servaddr, NULL); 
		buffertoken[n] = '\0';
		 
		makeToken(buffertoken) ;
	
	cout<<"Pin Verified"<<endl;
	
	
	buffer[n] = '\0'; 
	printf("CLIENT >> ");
	printf("Message: ");
	puts(buffer); 

	/* IMPORTANT: If you comment the following line on my system, the client and server print properly */
	// printf("%d", (int)ntohs(servaddr.sin_port));


	// close the descriptor 
	close(sockfd); 
} 
