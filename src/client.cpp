#include <sys/types.h>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "include/ClientCommunicationManager.hpp"
#include <signal.h>

using namespace std;

bool isLogout = false;

vector<string> getInput(){
	string input;
    vector<string> arguments;
    arguments.push_back("error");
    arguments.push_back("error");

	cout << "> ";
	getline(cin, input);
    
    if(cin.eof())
    {
        cout << endl << "Ctrl+D detectado" << endl;
        isLogout=true;
    }

	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	
    if(input.find(" ") != -1)
    {
        boost::split(arguments, input, boost::is_any_of(" "), boost::token_compress_on);
    }
	
	return arguments;
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


void signalHandler(int signal){
    printf("\nCtrl+C detectado! (signal %d)\n",signal);
    if(signal==2){
        //logout();
        isLogout = true;
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
    
    ClientCommunicationManager communication_manager = ClientCommunicationManager(ip, port); // #TODO tratamento servidor offline

    communication_manager.openConnection();
    string login_message = get_login_message(profile);
    communication_manager.sendMessage(login_message);
    //cout << login_message;
    
    while (true) {
        cout << "Enter command: " << std::flush;
        vector<string> input = getInput();
        string message = input[1];
        string command = boost::to_upper_copy(input[0]);
        string formated_message;

        if(command == "FOLLOW") {
            formated_message = get_follow_message(message);; // #TODO
            communication_manager.sendMessage(formated_message);
        }
        else if(command == "SEND") {
            formated_message = get_tweet_message(message);
            communication_manager.sendMessage(formated_message);
        }

        else if(command == "LOGOUT" || isLogout) {
            formated_message = get_logout_message();
            communication_manager.sendMessage(formated_message);
            communication_manager.closeConnection();
            cout << "Saiu com sucesso" << endl;
            break;
        }
    }
    return 0;
}