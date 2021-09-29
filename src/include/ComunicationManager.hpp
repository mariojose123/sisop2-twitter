#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
using namespace std;

class ComunicationManager {
private:
    int socket_number;
    const char* ip;
    string port;
public:
    ComunicationManager(const char* ip, string port);
    void openConnection();
    void closeConnection();
    void sendMessage(string message);
//    void setIp(const char* ip);
//    void setPort(string port);
//    void setSocket(int socket);
//    const char* getIp();
//    string getPort(){;
//    int getSockerNumber();
};
