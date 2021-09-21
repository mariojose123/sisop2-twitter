#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include "include/DataTwitter.hpp"


#define PORT 4924
#define LOGINCODE 1;
#define NOTIFICATIONCODE 2;
#define FOLLOWCODE 3;
#define MESSAGECODE 4;
using namespace std;
class Server{
    public:
        mutex mtx;
        DataTwitter database;
        vector<thread> threadsTCP;
        int sockfd, newsockfd;

        void follow(){

        }
        void message(){

        }
        void saveDataBase(){

        }
        int login(packet  readpacket )
        {
            int n;
            mtx.lock();
            bool IsntNewUser=NumberofUsers.find(readpacket._payload) != NumberofUsers.end();
            mtx.unlock();
            if (IsntNewUser) {
                mtx.lock();
                bool isFull = NumberofUsers[readpacket._payload]==2;
                mtx.unlock();
                if(isFull){
                    n = write(newsockfd,"Login failed\n", 18);
                    if (n < 0) 
                        cout <<"ERROR writing to socket\n"<< std::flush;
                }
                else{
                    mtx.lock();
                    NumberofUsers[readpacket._payload]++;
                    mtx.unlock();
                    n = write(newsockfd,"login successful", 18);
                    if (n < 0) 
                        cout <<"Login successful\n"<< std::flush;
                }
            }
            else {
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
        void logout(char buffer[]){
            mtx.lock();
            NumberofUsers[buffer]--;
            mtx.unlock();
        }
        int updateData(){

        }

    void runTCP(){
        int opt = 1;
        int  n;
	    socklen_t clilen;
	    char buffer[256];
	    struct sockaddr_in serv_addr, cli_addr;
	    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
            cout <<"ERROR opening socket\n"<< std::flush;
        
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(PORT);
	    serv_addr.sin_addr.s_addr = INADDR_ANY;
	    bzero(&(serv_addr.sin_zero), 8);
	    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		    printf("ERROR on binding\n");
	    listen(sockfd, 5);
        cout<<"Server online\n"<<std::flush;
	    clilen = sizeof(struct sockaddr_in);
        while(true){
	    if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
		    cout <<"ERROR on accept\n"<< std::flush;
        else{
            threadsTCP.insert(threadsTCP.begin(),thread(&Server::TCPloop,this));
        }
        }
        close(newsockfd);
        close(sockfd);  
    }
        void runUDP(){
            
        }
    private:
        map<string, int> NumberofUsers;
        void TCPloop(){
            char buffer[2048];
            /* read from the socket */
            int n = read(newsockfd, buffer, 2048);
            //cout<<buffer<<std::flush;
            packet readpacket = packet(buffer);
//            cout<<"Message Recivied\n"<<std::flush;
//            cout<<readpacket.type<<std::flush;
//            cout<<readpacket.seqn<<std::flush;
            switch(readpacket.type){
                case 1: login(readpacket);
                break;
                //case 3: follow(packet);
                //break;
               //case 4: message(packet);
               // break;
            }
            // Notification()
            // saveServer();
            if (n < 0) 
                cout <<"ERROR reading from socket\n"<< std::flush;

        }
};

int main(){
    Server serv;
    serv.runTCP();
}