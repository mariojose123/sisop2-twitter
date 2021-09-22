#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "include/ComunicationManager.hpp"

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

string get_login_message(string profile) {
    string message = "1 0 " + to_string(profile.size()) + " 0 " + profile +" ";
    return message;
}

string get_follow_message(string profile) {
    string message = "3 0 " + to_string(profile.size()) + " 0 " + profile + " ";
    return message;
}

string get_tweet_message(string tweet) {
    string message = "4 0 " + to_string(tweet.size()) + " 0 " + tweet + " ";
    return message;
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
    
    ComunicationManager comunication_manager = ComunicationManager(ip, port); // #TODO tratamento servidor offline

    comunication_manager.openConnection();
    string login_message = get_login_message(profile);
    comunication_manager.sendMessage(login_message);
    //cout << login_message;
    
    while (true && !isLogout()) {
        cout << "Enter command: " << std::flush;
        vector<string> input = getInput();
        string command = boost::to_upper_copy(input[0]);
        string message = input[1];
        string formated_message;
        if(command == "FOLLOW") {
            formated_message = get_follow_message(message);; // #TODO
        }
        else if(command == "SEND") {
            formated_message = get_tweet_message(message);
        }

        else if(command == "LOGOUT") {
            formated_message = ""; // #TODO
        }
        comunication_manager.sendMessage(formated_message);
    }
    
    return 0;
}