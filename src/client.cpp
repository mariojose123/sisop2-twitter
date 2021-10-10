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
        isLogout=true;
    }

	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	
    if(input.find(" ") != -1)
    {
        boost::split(arguments, input, boost::is_any_of(" "), boost::token_compress_on);
    }
	
	return arguments;
}

string get_message(int type, string input){
    string message;
    message=to_string(type) + " 0 " + to_string(input.size()) + " 0 " + input + " ";
    return message;
}

void signalHandler(int signal){
    
    if(signal==2){
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
    string login_message = get_message(1, profile); //tipo 1 = login
    communication_manager.sendMessage(login_message);
    //cout << login_message;
    
    while (true) {
        cout << "Enter command: " << std::flush;
        vector<string> input = getInput();
        string command = boost::to_upper_copy(input[0]);
        string message = input[1];
        string formated_message;

        if(command == "FOLLOW") {
            formated_message = get_message(3,message);
            communication_manager.sendMessage(formated_message);
        }
        else if(command == "SEND") {
            formated_message = get_message(4,message);
            communication_manager.sendMessage(formated_message);
        }
        else if(command == "LOGOUT" || isLogout) {
            formated_message = get_message(5,"LOGOUT");
            communication_manager.sendMessage(formated_message);
            communication_manager.closeConnection();
            cout << "\nSaiu com sucesso" << endl;
            exit(EXIT_SUCCESS);
            break;
        }
    }
    return 0;
}