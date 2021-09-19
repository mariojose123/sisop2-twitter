#pragma once
#include "Profile.hpp"
#include <map>
#include <string.h>
using namespace std;
 typedef struct __notification{ 
    uint32_t id;         //Identificador da notificação (sugere-se um identificador único) 
    uint32_t timestamp;     //Timestamp da notificação 
    uint16_t length;       //Tamanho da mensagem 
    uint16_t pending;      //Quantidade de leitores pendentes 
    const char* _string;     //Mensagem 
  } notification; 
class DataTwitter{
      public:
        int usercount;
        map<string,Profile> Database;
        map<string,int> IDmap;
        DataTwitter();
        Profile FindProfile(string name);
        int     FindProfileNumber(string name);
        void    SaveDataBase();
        void    AddProfile(string name);
        void    Message(string message,notification Notification);
        void    Follow(string name,string followName);
};