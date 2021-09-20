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

struct Login {
    int sockdf;
};

vector<string> getInput(){
	string input;
	cout << "> ";
	getline(cin, input);
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	vector<string> arguments;
	boost::split(arguments, input, [](char c){return c == ' ';});
	return arguments;
}

int get_connection(string PORTSTRING, const char* ipstring) {
    unsigned int PORT = stoi(PORTSTRING);
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        cout << "ERROR opening socket\n" << std::flush;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    bzero(&(serv_addr.sin_zero), 8);

    if (inet_pton(AF_INET, ipstring, &serv_addr.sin_addr) <= 0) {
        cout << "\nInvalid address/ Address not supported \n" << std::flush;
        return 1;
    }
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        cout << "ERROR connecting\n" << std::flush;

    return sockfd;
}


string get_login_message(string profile) {
    string message = "1 0 "+ to_string(profile.size())+ " 0 "+profile+" ";
    return message;
}

string get_follow_message(string profile, string followed) {
    string message =  profile + " FOLLOW "+ "followed";
    return message;
}


void send_message(int sockfd, string message) {
	/* write in the socket */
    char buffer[2048];
    strcpy(buffer, message.c_str());
	send(sockfd , buffer , strlen(buffer) , 0 );
    char bufferread[256];
    bzero(buffer,256);
	
	/* read from the socket */
    int n = read(sockfd, buffer, 256);
    if (n < 0) 
		cout<<"ERROR reading from socket\n"<<std::flush;

    cout<<buffer<<std::flush;
    
	close(sockfd); // NO FINAL DA MENSAGEM ELA FECHA A CONEXAO
}
int main(int argc,char *argv[])
{
    bool hasSessionEnded = false;
    if(argc==3){
        cout<<"incorrect input\n"<<endl;
    }
    string profile(argv[1]);
    const char* IP(argv[2]);
    string localIP(argv[3]);
    int sockfd = get_connection(localIP,IP);
    if (sockfd == -1) {
        cout<<"Server offline" << endl;
        return 1;
    }
    else {
        string initial_message = get_login_message(profile);
        send_message(sockfd, initial_message);
        cout<<"Enter the message: "<<std::flush;
        while (true) {
            vector<string> input = getInput();
            string command = input[0];
            string message = input[1];
            if (command == "FOLLOW") {

            } else if (command == "Message") {

            }

        }
    }
}