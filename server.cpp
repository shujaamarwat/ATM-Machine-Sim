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




int findUser (string userID, USER_VERIFICATION * users, int number_of_users){
	for (int i=0; i<number_of_users; i++){
		if (userID == users[i].CARDID)
			return i;
	}
	return -1;
}

using namespace std;
// Driver code 
int main() 
{ 
	int number_of_users = 3;
	USER_VERIFICATION users [3] = {
	{"card-1", "123"},
	{"card-2", "1234"},
	{"card-3", "12345"}};
		
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
	//receive the datagram 
	do{
		len = sizeof(cliaddr); 
		int n = recvfrom(listenfd, buffer, sizeof(buffer), 
				0, (struct sockaddr*)&cliaddr,&len); //receive message from client 
			
		buffer[n] = '\0';

		stringstream ss(buffer); 
	    vector<string> v; 
	  
		while (ss.good()) { 
		    string substr; 
		    getline(ss, substr, ','); 
		    v.push_back(substr); 
		} 
		
		for (int i=0; i<v.size(); i++){
			cout<<v[i]<<endl;
		}
		
		// CARD READ ENDPOINT
		if (v[0] == "1"){
			cout << "Recognized function 1!"<<endl;
		
			// Check if username is in the "database" of users
			string statusCode;
			for (int i=0; i<number_of_users; i++){
				cout<<"v[1] == "<<v[1]<<", user[i].CARDID == "<<users[i].CARDID<<endl; 
				if (v[1] == users[i].CARDID){
				cout<<"User found!"<<endl;
					// User found! 
					statusCode = "OK";		
					break;			
				}
				else statusCode = "BAD"; //user couldn't be found!
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
			for (int i=0; i<number_of_users; i++){
				cout<<"v[1] == "<<v[1]<<", user[i].CARDID == "<<users[i].CARDID<<endl; 
				if (v[1] == users[i].CARDID){
					cout<<"User found!"<<endl;
					// User found! 

					// Check PIN
					// IF not equal to pincode, exit the loop!
					if (users[i].PINCODE != v[2]){
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
					users[i].token = token;

					statusCode = "OK";
					break;
				}
				else statusCode = "BAD"; //user couldn't be found!
			}

			string response = statusCode + ",";
			response += to_string(token);
			sendto(listenfd, response.c_str(), MAXLINE, 0, 
			(struct sockaddr*)&cliaddr, sizeof(cliaddr));
			
		}
		// WITHDRAW AMOUNT ENDPOINT
		else if (v[0] == "3"){
			cout << "Recognized function 3!"<<endl;

			// Check if username is in the "database" of users			
			string statusCode;
			int amount = -1;
			for (int i=0; i<number_of_users; i++){
				cout<<"v[2] == "<<v[2]<<", user[i].token == "<<users[i].token<<endl; 
				if (v[2] == to_string(users[i].token)){
					cout<<"User token found!"<<endl;
					// User Token found! 

					// Subtract money from user amount
					if ( ( users[i].amount - stoi(v[1]) )< 0)
						statusCode = "BAD";
					else {
						users[i].amount -= stoi(v[1]);
					}
					amount = users[i].amount;
					statusCode = "OK";
					break;
				}
				else statusCode = "BAD"; //user couldn't be found!
			}

			string response = statusCode + ",";
			response += to_string(amount);
			sendto(listenfd, response.c_str(), MAXLINE, 0, 
			(struct sockaddr*)&cliaddr, sizeof(cliaddr));
			
		}
		// ERASE USER TOKEN ENDPOINT
		else if (v[0] == "0"){
			cout << "Recognized function 0!"<<endl;

			// Check if username is in the "database" of users			
			string statusCode;
			string responseMessage = "Could not erase token!";
			for (int i=0; i<number_of_users; i++){
				cout<<"v[1] == "<<v[1]<<", user[i].token == "<<users[i].token<<endl; 
				if (v[1] == to_string(users[i].token)){
					cout<<"User token found!"<<endl;
					// User Token found! 

					// Erase the user token
					users[i].token = -1;
					
					
					responseMessage = "Successfully logged out";
					statusCode = "OK";
					break;

				}
				else statusCode = "BAD"; //user couldn't be found!
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
