#pragma once

#include <string>

#define LOGINPKT 1
#define NOTIFICATIONPKT 2
#define FOLLOWPKT 3
#define MESSAGEPKT 4

class packet {
  public:
    char* toString();
    packet(char* stri);
    int type;        //Tipo do pacote (p.ex. DATA | CMD) 
    int seqn;        //Número de sequência 
    int length;       //Comprimento do payload  
    int timestamp;     // Timestamp do dado 
    std::string _payload;    //Dados da mensagem 
};
