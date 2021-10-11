//
// Created by rafa on 22/09/2021.
//

#include "include/ClientCommunicationManager.hpp"
using namespace std;

ClientCommunicationManager::ClientCommunicationManager(const char* ip, string port) {
    this->ip = ip;
    this->port = port;
}

void ClientCommunicationManager::openConnection() {
    unsigned int port = stoi(this->port);
    struct sockaddr_in serv_addr;

    int a = socket(AF_INET, SOCK_STREAM, 0);
    this->socket_number = a;
    if (this->socket_number== -1)
        cout << "ERROR opening socket\n" << std::flush;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bzero(&(serv_addr.sin_zero), 8);

    if (inet_pton(AF_INET, this->ip, &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/Address not supported\n" << std::flush;
    }

    if (connect(this->socket_number, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout << "ERROR connecting\n" << std::flush;
    }

    if (this->socket_number == -1) {
        cout<<"Server offline" << endl;
    }
    return;
}

void ClientCommunicationManager::closeConnection() {
    close(this->socket_number);
}

void ClientCommunicationManager::sendMessage(string message) {
    char buffer[2048];
    strcpy(buffer, message.c_str());
    send(this->socket_number, buffer, strlen(buffer), 0);
    // char bufferread[256];
    // bzero(buffer, 256);

//    /* read from the socket */ // #TODO ARRUMAR
//    int n = read(this->socket_number, buffer, 256);
//    if (n == -1)
//        cout << "ERROR reading from socket\n" << std::flush;
//
//    cout << buffer << std::flush;
}

// void ClientCommunicationManager::send_logout(int sockfd) {
// 	/* write in the socket */
//     string message = get_logout_message();
//     char buffer[2048];
//     strcpy(buffer, message.c_str());
// 	send(sockfd, buffer, strlen(buffer), 0);
// }

//void ComunicationMannger::setIp(const char* ip) {
//    this->ip=ip;
//}
//
//void ComunicationMannger::setPort(string port){
//    this->port=port;
//}
//
//void ComunicationMannger::setSocket(int socket) {
//    this->socket_number = socket;
//}
//
//const char* ComunicationMannger::getIp(){
//    return this->ip;
//}
//
//string ComunicationMannger::getPort(){
//    return this->port;
//}
//
//int ComunicationMannger::getSockerNumber(){
//    return this->socket_number;
//}
