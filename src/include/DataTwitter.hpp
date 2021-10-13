#pragma once

#include "Profile.hpp"
#include <map>
#include <string.h>
#include "packet.hpp"

using namespace std;

typedef struct __notification {
    uint32_t id;         //Identificador da notificação (sugere-se um identificador único) 
    uint32_t timestamp;     //Timestamp da notificação 
    uint16_t length;       //Tamanho da mensagem 
    uint16_t pending;      //Quantidade de leitores pendentes 
    const char* _string;     //Mensagem 
} notification;

class DataTwitter {
  public:
    int usercount;
    map<string,Profile> Database;
    map<string,int> IDmap;
    DataTwitter();
    void    add_Message(string user,string message,long int timestamp);
    bool    isProfileInDatabase(string name);
    bool    addFollower(string followed, string follower);
    int     FindProfileNumber(string name);
    string    SaveDataBase();
    Profile StreamtoProfile(ifstream &ss);
    void     LoadDataBase(ifstream &ss);
    void    AddProfile(string name);
    void    Message(string message,notification Notification);
    void    Follow(string name,string followName);
    void    Notification();
};
