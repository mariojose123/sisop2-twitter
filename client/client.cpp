#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <boost/algorithm/string.hpp>


using namespace std;


vector<string> getInput(){
	string input;
	cout << "> ";
	getline(cin, input);
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	vector<string> arguments;
	boost::split(arguments, input, [](char c){return c == ' ';});
	return arguments;
}

void Login(char profile[],string PORTSTRING,const char* ipstring){
    unsigned int PORT = stoi(PORTSTRING);
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    strcpy(buffer, profile);
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        cout<<"ERROR opening socket\n"<<std::flush;
    
	serv_addr.sin_family = AF_INET;     
	serv_addr.sin_port = htons(PORT);    
	bzero(&(serv_addr.sin_zero), 8);     
	
    if(inet_pton(AF_INET, ipstring, &serv_addr.sin_addr)<=0) 
    {
        cout<<"\nInvalid address/ Address not supported \n"<<std::flush;
        return;
    }
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        cout<<"ERROR connecting\n"<<std::flush;

    cout<<"Enter the message: "<<std::flush;
    
	/* write in the socket */
	send(sockfd , buffer , strlen(buffer) , 0 );
    bzero(buffer,256);
	
	/* read from the socket */
    n = read(sockfd, buffer, 256);
    if (n < 0) 
		cout<<"ERROR reading from socket\n"<<std::flush;

    cout<<buffer<<std::flush;
    
	close(sockfd);
}
int main(int argc,char *argv[])
{
    cout<<"bEGIN "<<std::flush;
    bool hasSessionEnded = false;
    if(argc==3){
        cout<<"incorrect input\n"<<endl;
    }
    char* profile(argv[1]);
    const char* IP(argv[2]);
    string localIP(argv[3]);
    Login(profile,localIP,IP);
    while(true){
        vector<string> input = getInput();
        string command = input[0];
        string message = input[1];
        if(command =="FOLLOW"){
            
        }
        else if(command=="Message"){

        }

    }


}