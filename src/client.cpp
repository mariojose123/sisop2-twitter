#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
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
	//boost::split(arguments, input, [](char c){return c == ' ';});
    boost::split( arguments, input , boost::is_any_of(" "), boost::token_compress_on);
	return arguments;
}

int get_connection(string portstr, const char* ipstring) {
    unsigned int port = stoi(portstr);
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        cout << "ERROR opening socket\n" << std::flush;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bzero(&(serv_addr.sin_zero), 8);

    if (inet_pton(AF_INET, ipstring, &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/Address not supported\n" << std::flush;
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout << "ERROR connecting\n" << std::flush;
        return 2;
    }

    return sockfd;
}

string get_login_message(string profile) {
    string message = "1 0 " + to_string(profile.size()) + " 0 " + profile +" ";
    return message;
}

string get_follow_message(string profile, string followed) {
    string message = "3 0 " + to_string(profile.size()) + " 0 " + followed + " ";
    return message;
}

string get_tweet_message(string tweet) {
    string message = "4 0 " + to_string(tweet.size()) + " 0 " + tweet + " ";
    return message;
}

void send_message(int sockfd, string message) {
	/* write in the socket */
    char buffer[2048];
    strcpy(buffer, message.c_str());
	send(sockfd, buffer, strlen(buffer), 0);
    char bufferread[256];
    bzero(buffer, 256);
	
	/* read from the socket */
    int n = read(sockfd, buffer, 256);
    if (n == -1)
		cout << "ERROR reading from socket\n" << std::flush;

    cout << buffer << std::flush;
	//close(sockfd); // NO FINAL DA MENSAGEM ELA FECHA A CONEXAO
}

void send_tweet(int sockfd, string message) {
	/* write in the socket */
    char buffer[2048];
    strcpy(buffer, message.c_str());
	send(sockfd, buffer, strlen(buffer), 0);
}

bool isLogout(){
    return false;
}


int main(int argc,char *argv[]) {
    if(argc !=4) {
        cout << "Usage: ./client <user> 127.0.0.1 4924" << endl;
        return 1;
    }

    string profile(argv[1]);
    string port(argv[3]);
    const char* ip(argv[2]);

    int sockfd = get_connection(port, ip);
    if (sockfd == -1) {
        cout<<"Server offline" << endl;
        return 2;
    }
    
    string initial_message = get_login_message(profile);
    send_message(sockfd, initial_message);
    
    while (true && !isLogout()) {
        cout << "Enter command: " << std::flush;
        vector<string> input = getInput();
        string command = boost::to_upper_copy(input[0]);
        string message = input[1];
        if (command == "FOLLOW") {
            continue;
        } else if (command == "SEND") {
            string tweet;
            tweet=get_tweet_message(message);
            send_tweet(sockfd, tweet);
        } else {
            cout << "Usage: SEND <message> / FOLLOW <user>" << endl << flush;
        }
    }

    close(sockfd);
    return 0;
}