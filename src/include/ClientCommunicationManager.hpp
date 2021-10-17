#pragma once
#include <string>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <thread>
#include <boost/algorithm/string.hpp>

using namespace std;

class ClientCommunicationManager {
private:
    int socket_number;
    const char* ip;
    string port;
    thread t1;
public:
    ClientCommunicationManager(const char* ip, string port);
    void JoinThread();
    void openConnection();
    void closeConnection();
    void sendMessage(string message);
    void receiveMessage();
    void readMessageNotification();
    int GetSocketNumber();
    void ThreadNotification();
//    void setIp(const char* ip);
//    void setPort(string port);
//    void setSocket(int socket);
//    const char* getIp();
//    string getPort(){;
//    int getSockerNumber();
};
