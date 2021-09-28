#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <signal.h>

using namespace std;

int sockfd;

struct Login {
    int sockdf;
};

vector<string> getInput(){
	string input;
    vector<string> arguments;
    arguments.push_back("error");
    arguments.push_back("error");

	cout << "> ";
	getline(cin, input);
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	
    if(input.find(" ") != -1)
    {
        boost::split(arguments, input, boost::is_any_of(" "), boost::token_compress_on);
    }
	
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

string get_follow_message(string followed) {
    string message = "3 0 " + to_string(followed.size()) + " 0 " + followed + " ";
    return message;
}

string get_tweet_message(string tweet) {
    string message = "4 0 " + to_string(tweet.size()) + " 0 " + tweet + " ";
    return message;
}

string get_logout_message() {
    string message = "5 0 6 0 LOGOUT ";
    return message;
}

void send_login(int sockfd, string message) {
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

void send_message(int sockfd, string message) {
	/* write in the socket */
    char buffer[2048];
    strcpy(buffer, message.c_str());
	send(sockfd, buffer, strlen(buffer), 0);
}

void send_logout(int sockfd) {
	/* write in the socket */
    string message = get_logout_message();
    char buffer[2048];
    strcpy(buffer, message.c_str());
	send(sockfd, buffer, strlen(buffer), 0);
}

void signalHandler(int signal){
    printf("\nCtrl+C detectado! (signal %d)\n",signal);
    
    if(signal==2){
        send_logout(sockfd);
        close(sockfd);
        cout << "Saiu com sucesso" << endl;
        exit(EXIT_SUCCESS);
    }
}

int main(int argc,char *argv[]) {
    if(argc !=4) {
        cout << "Usage: ./client <user> 127.0.0.1 4924" << endl;
        return 1;
    }

    //inicialização para detectar ctrl+c
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    //inicialização para detectar ctrl+c

    string profile(argv[1]);
    string port(argv[3]);
    const char* ip(argv[2]);

    sockfd = get_connection(port, ip);

    if (sockfd == -1) {
        cout<<"Server offline" << endl;
        return 2;
    }
    
    string initial_message = get_login_message(profile);
    send_login(sockfd, initial_message);
    vector<string> input;
    string command;
    string message;

    while (true) {
        cout << "Enter command: " << std::flush;

        input = getInput();
        command = boost::to_upper_copy(input[0]);        
        message = input[1];

        if (command == "FOLLOW") {
            message=get_follow_message(message);
            send_message(sockfd, message);
            continue;
        } else if (command == "SEND") {            
            message=get_tweet_message(message);
            send_message(sockfd, message);
        } else {
            cout << "Usage: SEND <message> / FOLLOW <user>" << endl << flush;
        }
    }

    return 0;
}