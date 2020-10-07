// udp client driver program 
#include <iostream>
#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<chrono>
#include<iomanip>
#include<sstream>
#include<vector>
#include<string>
#include<time.h>
#include <stdlib.h>

#define PORT 5000 
#define MAXLINE 1000 

using namespace std;

class ATM
{
public:

    // int act_tok;
    int balance;
    bool tr;

	ATM();
    bool gettruth() const;
    void settruth(bool);
    void settoken(int);
    int gettoken();
    void setbalance();
	int getbalance()const;
	bool withdraw(int amount);
	string insuffientFund()const;
	void deposite(int amount);
	string transaction(char type,int amount)const;
	void print();
	
	~ATM();

private:
    int tok;
	int amount;
	int initialBalance;
	static stringstream st;
};

stringstream ATM::st;


//Sets Balance and Amount
ATM::ATM():amount(0.0f), initialBalance(500.0f){}

void ATM::settoken(int t) {
    tok=t;
}

int ATM::gettoken() {
    return tok;
}

void ATM::settruth(bool t) {
    tr=t;
}

bool ATM::gettruth() const{
    return tr;
}
//Server will generate a number from 0 to 50000 that represent current amount in the customer account.
void ATM::setbalance(){

  srand (time(NULL));

  balance = rand() % 50000 + 1;

}
//Gets Balance
int ATM::getbalance() const
{
    return balance;
}

//Gets Withdraw
bool ATM::withdraw(int amount)
{
	char type = 'w';
	if (balance >= amount && amount > 25000) {
		balance -= amount;
		transaction(type,amount);   // write this transaction to a string buffer
		return true;
	}
		
	return false;
		
}

string ATM::insuffientFund() const
{
	return string("Insufficient fund\nYou have: " + to_string(getbalance()) + " left");
}

void ATM::deposite(int amount)
{
	char type = 'd';
	balance += amount;
	transaction(type,amount);      // write this transaction to a string buffer
}


// 1,USER-1
// 2,USER-1,123
// 3,24000,392810


string ATM::transaction(char type, int amount) const
{
	
	if (type == 'd')
	{
		st << "Deposit" << right << setw(20) << amount << '\n';
	}

	if (type == 'w') {
		st << "Withdraw" << right << setw(19) << amount << '\n';
	}
	return st.str();
}

void ATM::print() 
{
    int t;
	// stringstream dtbuffer;
	cout << "------------------------------------\n";
	cout << right << setw(20) << "IBANK\n";
	cout << "   Your number one banking Solutions\n";
	cout << "------------------------------------\n";
	cout << '\n';
    if (gettruth()==true){
        // cout << "ATM ID: " << 101985 << '\n';
        // cout << "Acc No: "<< "*******"<< 606 << '\n';

        cout << "Transaction Type" << right << setw(15)<<"Amount" << '\n'<<'\n';
        
        
        cout << st.str();

        cout << "\nBalance: "<< getbalance() << '\n';
        
        cout << "------------------------------------\n";
        cout << "Thank you for using IBank\n";
        cout << "------------------------------------\n";	
    }
    else{
        cout<<"Wrong Token NUMBER"<<endl;
    }
	// string info = dtbuffer.str();
	// return string(info);

}

ATM::~ATM()
{
}


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

vector <string> tokenize1(string buffer)
{
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
        return v;
}


void pin_veri(string &full_pin,string &card)
{	
	string pin;
	getline(cin,pin,'\n');
	
	full_pin="2,"+card + ","+ pin;
	cout<<full_pin<<endl;
		
}


// Driver code 
int main() 
{ 
    ATM a;
    char buffer[100], responseMessage[100]; 
	string message = "3,24000, 1234";
	int sockfd, n; 
	struct sockaddr_in servaddr; 
	string full_pin;
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
	
	}
    while( verifyResponse(buffer) != true);// verifying response// verifying response

	cout<<"User Found"<<endl;

	//getline(cin, message, '\n');
	
	///////// Enter PIN///////////////
	
		pin_veri(full_pin,card);
		
		sendto(sockfd, full_pin.c_str(), MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	
		n = recvfrom(sockfd, responseMessage, sizeof(responseMessage), 0, (struct sockaddr *)&servaddr, NULL); 
		responseMessage[n] = '\0';
		 

        auto vectorresponse = tokenize1(responseMessage);
	

        if (vectorresponse[0] == "OK"){
            cout<<"Pin Verified"<<endl;
            a.settruth(true);
            a.settoken(stoi(vectorresponse[1]));
        }
        else {
            throw logic_error("Pin was wrong");
        }
	

	getline(cin, message, '\n');

    int t,wi;
    // a.transaction('w',wi);
    // a.withdraw(wi);
    a.print();

    cout<<"Please enter an amount to withdraw: ";
    int amountToWithdraw;
    cin>>amountToWithdraw;


    string request = "3," + to_string(amountToWithdraw) + "," + to_string(a.gettoken());
    // char w2=char(wi);
    // string str(wi);
    sendto(sockfd, request.c_str(), MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 


	// request to send datagram 
    n = recvfrom(sockfd, responseMessage, sizeof(responseMessage), 0, (struct sockaddr *)&servaddr, NULL); 
    responseMessage[n] = '\0';
        

    auto vectorresponse1 = tokenize1(responseMessage);


    if (vectorresponse1[0] == "OK"){
        cout<<"Amount has been withdrawn successfully!"<<endl;
        cout<<"Your remaining balance is = "<<vectorresponse1[1]<<endl;
    }
    else {
        throw logic_error("Couldn't withdraw!");
    }
	



	// close the descriptor 
	close(sockfd); 
} 
