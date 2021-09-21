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

using namespace std;

class Server {
    public:
        mutex mtx;
        DataTwitter database;
        vector<thread> threadsTCP;

        //server_socket, client_socket
        int sockfd, newsockfd;

        void follow() {

        }

        //não funciona:
        void message(std::string message) {
            cout << "Chegou no message: " + message;
        }

        void saveDataBase() {

        }

        int login(packet readpacket) {
            int n;
            mtx.lock();
            bool isOldUser=NumberofUsers.find(readpacket._payload) != NumberofUsers.end();
            mtx.unlock();
            if (isOldUser) {
                mtx.lock();
                bool isFull = NumberofUsers[readpacket._payload]==2;
                mtx.unlock();
                if(isFull) {
                    n = write(newsockfd,"Login failed\n", 18);
                    if (n < 0) {
                        cout << n << endl;
                        cout <<"ERROR writing to socket\n"<< std::flush;
                    }
                } else {
                    mtx.lock();
                    NumberofUsers[readpacket._payload]++;
                    mtx.unlock();
                    n = write(newsockfd,"Login successful\n", 18);
                    if (n < 0) 
                        cout <<"Login successful\n"<< std::flush;
                }
            } else {
                mtx.lock();
                map<string,int>::iterator it = NumberofUsers.begin();
                database.AddProfile(readpacket._payload);
                NumberofUsers.insert(it, pair<string,int>(readpacket._payload,1));
                mtx.unlock();
                n = write(newsockfd,"Login successful\n", 18);
                    if (n < 0) 
                        cout <<"Login successful\n"<< std::flush;
            }
            return true;
        }

        void logout(char buffer[]) {
            mtx.lock();
            NumberofUsers[buffer]--;
            mtx.unlock();
        }

        int updateData(){

        }

        void runTCP() {
            int opt = 1;
            int n;
            socklen_t clilen;
            char buffer[256];
            struct sockaddr_in serv_addr, cli_addr;

            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
                cout << "ERROR opening socket\n" << std::flush;
            
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(PORT);
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            bzero(&(serv_addr.sin_zero), 8);
            if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
                cout <<"ERROR on binding\n"<<std::flush;
            listen(sockfd, 5);
            cout<<"Server online\n"<<std::flush;
            clilen = sizeof(struct sockaddr_in);

            //loop que aceita conexoes novas
            while(true) {
                if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
                    cout <<"ERROR on accept\n"<< std::flush;
                else
                    threadsTCP.insert(threadsTCP.begin(), thread(&Server::TCPloop, this));
            }      
            
            close(newsockfd);
            close(sockfd);  
        }

    private:
        map<string, int> NumberofUsers;

        void TCPloop() {
            string username;
            char buffer[2048];
            packet readpacket = packet(buffer);
            
            while(true) {
                if (recv(newsockfd, &buffer, sizeof(buffer), 0) == -1)
                    cout <<"Server: ERROR reading from socket\n"<< flush;

                readpacket = packet(buffer);

                printf("\n\nPacote recebido:\nTipo: %d\nSequência: %d\nTimestamp: %d\n", 
                readpacket.type, readpacket.seqn, readpacket.timestamp);
                // cout << "\n\nPacote recebido:\n";
                // cout << "\nTipo: " + readpacket.type;
                // cout << "\nSequência: " + readpacket.seqn;
                // cout << "\nTimestamp: " + readpacket.timestamp;
                cout << "\nPayload: " + readpacket.getPayload() << endl;

                switch(readpacket.type) {
                    case LOGINPKT:
                        username = readpacket.getPayload();
                        cout << "Username: " << username << endl;
                        login(readpacket);
                        break;
                    case FOLLOWPKT:
                        break;
                    case NOTIFICATIONPKT:
                        break;
                    case MESSAGEPKT:
                        //message(packet.getPayload())
                        cout << "Mensagem de " << username << " ";
                        cout << "Chegou um tweet: " << endl << flush;
                        cout << readpacket.getPayload() << endl;
                        break;
                    default:
                        cout << "Tipo de pacote desconhecido!" << endl << flush;
                }
            }
            // Notification()
            // saveServer();
        }
};

int main() {
    Server server;
    server.runTCP();
    return 0;
}
