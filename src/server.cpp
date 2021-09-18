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
#include <thread>
#include <mutex>
#define PORT 5000
using namespace std;

class Server{
public:
    mutex mtx;
    vector<thread> threadsTCP;
    int sockfd, newsockfd;

    void follow(){

    }
    int login(char  buffer[] ){
        int n;
        if (NumberofUsers.find(buffer) != NumberofUsers.end()) {
            if(NumberofUsers[buffer]==2){
                n = write(newsockfd,"login failed", 18);
                if (n < 0)
                    cout <<"ERROR writing to socket"<< std::flush;
            }
            else{
                mtx.lock();
                NumberofUsers[buffer]++;
                mtx.unlock();
                n = write(newsockfd,"login successful", 18);
                if (n < 0)
                    cout <<"login successful"<< std::flush;
            }
        }
        else {
            mtx.lock();
            map<string,int>::iterator it = NumberofUsers.begin();
            NumberofUsers.insert(it, pair<string,int>(buffer,1));
            mtx.unlock();
            n = write(newsockfd,"login successful", 18);
            if (n < 0)
                cout <<"login successful"<< std::flush;
        }
        return true;
    }
    int logout(char buffer[]){
        mtx.lock();
        NumberofUsers[buffer]--;
        mtx.unlock();
    }

    void runTCP(){
        int opt = 1;
        cout <<"SERVER BEGUN"<< std::flush;
        int  n;
        socklen_t clilen;
        char buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
        cout <<"SERVER BEGUN"<< std::flush;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            cout <<"ERROR opening socket"<< std::flush;

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(serv_addr.sin_zero), 8);
        cout <<"SERVER BEGUN"<< std::flush;
        if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            printf("ERROR on binding");
        cout <<"SERVER BEGUN"<< std::flush;
        listen(sockfd, 5);

        clilen = sizeof(struct sockaddr_in);
        cout <<"SERVER BEGUN"<< std::flush;
        while(true){
            if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
                cout <<"ERROR on accept"<< std::flush;
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
        char buffer[256];
        bzero(buffer, 256);

        /* read from the socket */
        int n = read(newsockfd, buffer, 256);
        if (n < 0)
            cout <<"ERROR reading from socket"<< std::flush;
        int i = login(buffer);
    }
};

int main(){
    cout<<"Hi"<< std::flush;
    Server serv;
    serv.runTCP();
}