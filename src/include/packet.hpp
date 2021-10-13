#pragma once

#include <string>

#define LOGINPKT 1
#define NOTIFICATIONPKT 2
#define FOLLOWPKT 3
#define MESSAGEPKT 4
#define LOGOUTPKT 5

class packet {
  public:
    char* toString();   //Traduz os dados de string para vetor de caracteres
    packet(char* stri); //Organiza os atributos a partir dos dados crus recebidos
    int type;           //Tipo do pacote (p.ex. DATA | CMD) 
    int seqn;           //Número de sequência 
    int length;         //Comprimento do payload  
    long int timestamp;      //Timestamp do dado 
    packet(long int timestamp,string message);
    
    std::string getPayload(){
      return payload;
    }

    void setPayload(std::string newPayload){
      payload = newPayload;
    }
    
  private:
    std::string payload;    //Dados da mensagem 
};
