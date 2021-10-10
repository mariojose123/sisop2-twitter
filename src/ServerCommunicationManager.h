#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <cstring>
#include "include/DataTwitter.hpp"
#include <cstdio>

#define PORT 4924


class ServerCommunicationManager {
private:
    int server_socket_number, client_socket_number;
public:
    ServerCommunicationManager();
    void closeConnection();
    void sendMessage(string message);
};


#endif //SISOP2_TWITTER_SERVERCOMMUNICATIONMANAGER_H
