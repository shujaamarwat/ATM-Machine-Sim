// server program for udp connection 
#include <iostream>
#include <stdio.h> 
#include <string.h> 
#include <cstring> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <vector>
#include <sstream>
using namespace std;


#define PORT 5000 
#define MAXLINE 1000 





struct USER_VERIFICATION {
	int USER_IP;
	std::string CARDID;
	std::string PINCODE;
	int token;
	int amount;
	
	USER_VERIFICATION (std::string cardid, std::string password) {
		CARDID = cardid;
		PINCODE = password;
		token = -1;
		amount = 10000;		
	}
};

int number_of_users = 3;
USER_VERIFICATION users [3] = {
{"card-1", "123"},
{"card-2", "1234"},
{"card-3", "12345"}};


int findUser (string userID){
	for (int i=0; i<number_of_users; i++){
		if (userID == users[i].CARDID)
			return i;
	}
	return -1;
}


int findUserByToken (int token){
	for (int i=0; i<number_of_users; i++){
		if (token == users[i].token)
			return i;
	}
	return -1;
}

vector<string> tokenizeRequest (buffer){
		stringstream ss(buffer); 
	    vector<string> v; 
	  
		while (ss.good()) { 
		    string substr; 
		    getline(ss, substr, ','); 
		    v.push_back(substr); 
		} 
		
		return v;		
}

using namespace std;
// Driver code 
int main() 
{ 

		
	cout<<"--------- THE DATABASE ---------"<<endl;
	for (int i=0; i<number_of_users; i++){
		cout<<users[i].CARDID<<", "<<users[i].PINCODE<<endl;
	}	
	cout<<"------------------------------"<<endl;
		
	char buffer[1000]; 
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

	do{

		//receive the datagram, save in buffer 
		len = sizeof(cliaddr); 
		int n = recvfrom(listenfd, buffer, sizeof(buffer), 
				0, (struct sockaddr*)&cliaddr,&len); //receive message from client 
		buffer[n] = '\0';

		// tokenize the request from the client
		auto v = tokenizeRequest (buffer);

		// print the client's request
		for (int i=0; i<v.size(); i++){
			cout<<v[i]<<endl;
		}



		// CARD READ ENDPOINT
		if (v[0] == "1"){
			cout << "Recognized function 1!"<<endl;

			// Check if username is in the "database" of users
			string statusCode;
			int index = findUser(v[1]);
			if (index < 0){
				statusCode = "BAD";
			}
			else {
				statusCode = "OK";
			}

			string response = statusCode;
			sendto(listenfd, response.c_str(), MAXLINE, 0, 
			(struct sockaddr*)&cliaddr, sizeof(cliaddr));

		}



		// VERIFY PIN ENDPOINT
		else if (v[0] == "2"){
			cout << "Recognized function 2!"<<endl;

			// Check if username is in the "database" of users
			string statusCode;
			int token = -1;

			int index = findUser(v[2]);
			if (index < 0){
				statusCode = "BAD";
			}
			else {
				if (users[index].PINCODE != v[2]){
					statusCode = "BAD";
					break;
				} 

				// ELIF
				// Now create a unique token for the user
				srand (time(NULL));
				do {
					token = rand()%10000;

					for (int i=0; i<number_of_users; i++){
						if (token == users[i].token)
							continue;	//if token already exists (chances are low tho), loop again
					}
					break;
				} while (true);

				// Update the Token of the user in the database!
				users[index].token = token;
			
				statusCode = "OK";
			}
			string response = statusCode + ",";
			response += to_string(token);
			sendto(listenfd, response.c_str(), MAXLINE, 0, 
(struct sockaddr*)&cliaddr, sizeof(cliaddr));

		}
		
				
		// WITHDRAW ENDPOINT
		else if (v[0] == "3"){
			cout << "Recognized function 3!"<<endl;

			// Check if username is in the "database" of users
			string statusCode;
			int amount = -1;

			int index = findUserByToken(stoi(v[2]));

			if (index < 0){
				statusCode = "BAD";
			}
			else {
				if (users[index].amount - stoi(v[1]) < 0){
					cout<<"Invalid transaction!"<<endl;
					statusCode = "BAD";
				}
				else {
					statusCode = "OK";
					users[index].amount -= stoi(v[1]);
					amount = users[index].amount;
					statusCode = "OK";
				}
				
			}

			string response = statusCode;
			response += amount;
			sendto(listenfd, response.c_str(), MAXLINE, 0, 
			(struct sockaddr*)&cliaddr, sizeof(cliaddr));

		}
		
		
		// ERASE USER TOKEN ENDPOINT
		else if (v[0] == "0"){
			cout << "Recognized function 0!"<<endl;

			// Check if username is in the "database" of users			
			string statusCode;
			string responseMessage = "Could not erase token!";
			int index = findUserByToken(stoi(v[2]));

			if (index < 0){
				string responseMessage = "Could not find user!";
				statusCode = "BAD";
			}
			else {
				users[index].token = -1;
				statusCode = "OK";
			}

			string response = statusCode + ",";
			response += responseMessage;
			sendto(listenfd, response.c_str(), MAXLINE, 0, 
			(struct sockaddr*)&cliaddr, sizeof(cliaddr));
			
		}
		printf("SERVER >> ");
		printf("Message: ");
		puts(buffer);

		if (strcmp(buffer, "exit") == 0){
			exit = 1;
			break;
		}

	} while(!exit);

	printf("Exiting.\n");

}
