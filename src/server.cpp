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
        
        int server_socket, client_socket;

        void follow() {

        }

        //não funciona:
        void message(std::string message) {
            cout << "Chegou no message: " + message;
        }

        void saveDataBase() {

        }

        int login(packet readpacket) {
            int bytesWritten; // se bytesWritten<0 -> erro

            mtx.lock();
            bool isOldUser = UserLoginCounter.find(readpacket.getPayload()) != UserLoginCounter.end();
            mtx.unlock();

            if (isOldUser) {

                mtx.lock();
                bool isFull = UserLoginCounter[readpacket.getPayload()]==2;
                mtx.unlock();

                if(isFull) {
                    bytesWritten = write(client_socket,"Login failed\n", 18);
                    if (bytesWritten < 0) {
                        //cout << bytesWritten << endl;
                        cout <<"ERROR writing to socket\n"<< std::flush;
                    }
                } else {

                    mtx.lock();
                    UserLoginCounter[readpacket.getPayload()]++;
                    mtx.unlock();

                    bytesWritten = write(client_socket,"Login successful\n", 18);
                    if (bytesWritten < 0) 
                        cout <<"Login successful\n"<< std::flush;
                }
            } else {

                mtx.lock();                
                map<string,int>::iterator it = UserLoginCounter.begin();
                database.AddProfile(readpacket.getPayload());
                UserLoginCounter.insert(it, pair<string,int>(readpacket.getPayload(),1));
                mtx.unlock();

                bytesWritten = write(client_socket,"Login successful\n", 18);
                    if (bytesWritten < 0)
                        cout <<"Login successful\n"<< std::flush;
            }
            return true;
        }

        void logout(string username) {
            mtx.lock();
            UserLoginCounter[username]--;
            mtx.unlock();
        }

        int updateData(){

        }

        void runTCP() {
            int bindSuccess = -1;
            socklen_t clilen;
            char buffer[256];
            struct sockaddr_in serv_addr, cli_addr;

            //cria socket do servidor
            server_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (server_socket == -1) 
                cout << "ERROR opening socket\n" << std::flush;
            
            //define endereço do servidor
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(PORT);
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            bzero(&(serv_addr.sin_zero), 8);

            //bind o socket ao IP especificado
            bindSuccess = ::bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
            if (bindSuccess < 0)
                cout <<"ERROR on binding\n"<<std::flush;

            listen(server_socket, 5);
            cout<<"Server online\n"<<std::flush;
            clilen = sizeof(struct sockaddr_in);

            //loop que aceita conexoes novas
            while(true) {
                //cout << "Aguardando conexões" << endl;
                client_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &clilen);
                //cout << "Nova conexão" << endl;
                if (client_socket == -1) 
                    cout <<"ERROR on accept\n"<< std::flush;
                else
                    threadsTCP.insert(threadsTCP.begin(), thread(&Server::TCPloop, this));
            }      
            
            close(client_socket);
            close(server_socket);  
        }

    private:
        map<string, int> UserLoginCounter;

        void TCPloop() {
            string username;
            char buffer[2048];
            packet readpacket = packet(buffer);
            
            while(true) {
                if (recv(client_socket, &buffer, sizeof(buffer), 0) == -1)
                    cout <<"Server: ERROR reading from socket\n"<< flush;

                readpacket = packet(buffer);

                printf("Pacote recebido:\nTipo: %d\nSequência: %d\nTimestamp: %d\n", 
                readpacket.type, readpacket.seqn, readpacket.timestamp);

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
                        cout << "Mensagem de " << username << ":\n";
                        cout << readpacket.getPayload() << endl << flush;
                        break;
                    case LOGOUTPKT:
                        logout(username);
                        cout << username << " deslogou." << endl << flush;
                        close(client_socket);   //não sei o que to fazendo
                        return;                 //não sei o que to fazendo
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
