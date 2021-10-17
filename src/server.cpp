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
#include <stdexcept>
#include <csignal>
#include <iostream>
#include <iostream>
#include <fstream>
#include <algorithm>    

#define PORT 5061

using namespace std;

class Server {
public:
    mutex mtx;
    DataTwitter database;
    vector <thread> threadsTCP;

    //follower seguiu followed
    void follow(string follower, string followed) {
        mtx.lock();
        //if this->database.is
        if (this->database.isProfileInDatabase(followed)) {
            this->database.addFollower(followed, follower);
        }
        else {
            cerr << "User: " << follower << ": " "Profile "<< followed << " dont exist!" << endl << flush;
        }
        mtx.unlock();
    }

    void storeNotification(packet notification, Profile follower){
        follower.add_message(notification.getPayload());
        follower.add_pendingNotification(notification);
    }

    set<string> checkFollowers(string username){
        Profile profile;
        set<string> followers;
        set<string>::iterator itrFollowers;
        
        //se o perfil estiver presente no database, retorna sua lista(set) de followers
        mtx.lock();
        if(this->database.isProfileInDatabase(username))
        {            
            cout<<"Perfil encontrado"<<endl;
            profile=this->database.Database.at(username);
            followers=profile.get_followers();
            cout<<"Seguidores do perfil:"<<endl;
            for (itrFollowers = followers.begin(); itrFollowers != followers.end(); itrFollowers++)
            {                         
                cout << *itrFollowers<<" ";
            }
            cout<<endl;
        }else{
            cout<<"Perfil nao encontrado"<<endl;
        }
        mtx.unlock();
        return followers;
    }

    //não funciona:
    void message(std::string message) {
        cout << "Chegou no message: " + message;
    }

    void saveDataBase() {

    }

    int login(packet readpacket,int client_socket) {
        int bytesWritten; // se bytesWritten<0 -> erro

        mtx.lock();
        bool isOldUser = UserLoginCounter.find(readpacket.getPayload()) != UserLoginCounter.end();
        mtx.unlock();

        if (isOldUser) {

            mtx.lock();
            bool isFull = UserLoginCounter[readpacket.getPayload()] == 2;
            mtx.unlock();

            if (isFull) {
                bytesWritten = write(client_socket, "Login failed\n", 18);
                if (bytesWritten < 0) {
                    //cout << bytesWritten << endl;
                    cout << "ERROR writing to socket\n" << std::flush;
                }
            } else {

                mtx.lock();
                UserLoginCounter[readpacket.getPayload()]++;
                mtx.unlock();

                bytesWritten = write(client_socket, "Login successful\n", 18);
                if (bytesWritten < 0)
                    cout << "Login successful\n" << std::flush;
            }
        } else {

            mtx.lock();
            //cout << "login" << readpacket.getPayload() << endl << flush;
            map<string, int>::iterator it = UserLoginCounter.begin();
            database.AddProfile(readpacket.getPayload());
            UserLoginCounter.insert(it, pair<string, int>(readpacket.getPayload(), 1));
            mtx.unlock();

            bytesWritten = write(client_socket, "Login successful\n", 18);
            if (bytesWritten < 0)
                cout << "Login successful\n" << std::flush;
        }
        return true;
    }

    void logout(string username) {
        mtx.lock();
        UserLoginCounter[username]--;
        mtx.unlock();
    }

    
    void runTCP() {
        int server_socket, client_socket;
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
            cout << "ERROR on binding\n" << std::flush;

        listen(server_socket, 5);
        cout << "Server online\n" << std::flush;
        clilen = sizeof(struct sockaddr_in);

        //loop que aceita conexoes novas
        while (true) {
            //cout << "Aguardando conexões" << endl;
            client_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &clilen);
            //cout << "Nova conexão" << endl;
            if (client_socket == -1)
                cout << "ERROR on accept\n" << std::flush;
            else
                threadsTCP.insert(threadsTCP.begin(), thread(&Server::TCPloop, this,server_socket,client_socket));
        }

        close(client_socket);
        close(server_socket);
    }
    void load(){
        ifstream myReadFile;
        myReadFile.open("server.txt");

        this->database.LoadDataBase(myReadFile);
        SaveDatabase();
    }
    void notification(){
        mtx.lock();
        this->database.Notification();
        mtx.unlock();
    }


private:
    struct myclass {
        bool operator() (packet i,packet j) { return (i.timestamp<j.timestamp);}
    } myfunction;
    void SaveDatabase(){
            mtx.lock();
            ofstream myfile ("server.txt");
            DataTwitter data = this->database;
            myfile << data.SaveDataBase();
            myfile.close();
            mtx.unlock();
    }
    map<string, int> UserLoginCounter;
    void sendNotification(string username,int client_socket){
        while(true){
        this_thread::sleep_for (std::chrono::seconds(3));
        char *buffer;
        mtx.lock();
        vector<packet> notifications = this->database.NotificationVector(username);    
        mtx.unlock();
        if(notifications.size()>0){
        sort(notifications.begin(), notifications.end(), myfunction);
        int bytesWritten;
        char buffer[2048];
        bzero(buffer,2048);
        mtx.lock();
        string message = "@"+notifications[0].user + " " + notifications[0].getPayload();
        const char* c =message.c_str();
        bytesWritten = send(client_socket, c, 256,0);
                if (bytesWritten < 0) {
                    //cout << bytesWritten << endl;
                    cout << "ERROR writing to socket\n" << std::flush;
            }
        this->database.deleteNotification(username,notifications[0].getPayload());
        mtx.unlock();
        }
    }
    }
    void NotificationLoop(string username){
        
    }

    void TCPloop(int server_socket,int client_socket) {
        string username;
        set<string> followers;
        set<string>::iterator itrFollowers;
        char buffer[2048];
        packet readpacket = packet(buffer);
        while (true) {
            notification();
            if (recv(client_socket, &buffer, sizeof(buffer), 0) == -1)
                cout << "Server: ERROR reading from socket\n" << flush;

            readpacket = packet(buffer);

            //printf("Pacote recebido:\nTipo: %d\nSequência: %d\nTimestamp: %d\n",
//                   readpacket.type, readpacket.seqn, readpacket.timestamp);

            //cout << "\nPayload: " + readpacket.getPayload() << endl;
            switch (readpacket.type) {
                case LOGINPKT:
                    username = readpacket.getPayload();
                    cout << "Login username: " << username << endl;
                    login(readpacket,client_socket);
                    SaveDatabase();
                    break;
                case FOLLOWPKT:
                    cout<<username<<flush;
                    follow(username, readpacket.getPayload());
                    SaveDatabase();
                    break;
                case MESSAGEPKT:
                    //message(packet.getPayload())
                    cout << "Mensagem de " << username << ":\n";
                    cout << readpacket.getPayload() << endl << flush;
                    cout << " "<<to_string(readpacket.timestamp) << endl <<flush;
                    mtx.lock();
                    this->database.add_Message(username,readpacket.getPayload(),readpacket.timestamp);
                    mtx.unlock();
                    SaveDatabase();
                    break;
                case LOGOUTPKT:
                    logout(username);
                    cout << username << " deslogou." << endl << flush;
                    mtx.lock();
                    close(client_socket);   //não sei o que to fazendo
                    mtx.unlock();
                    return;                 //não sei o que to fazendo
                case 6:  //notification
                    sendNotification(readpacket.getPayload(),client_socket);
                default:
                    cout << "Tipo de pacote desconhecido!" << endl << flush;
            }
        }
        // Notification()
        // saveServer();
    }
};
Server server;
int main() {
    server.load();
    server.saveDataBase();
    server.runTCP();
    return 0;
}