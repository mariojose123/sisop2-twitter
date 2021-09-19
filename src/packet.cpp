#include "../include/packet.hpp"
#include <string>
using namespace std;
char* packet::toString(){
        string myString(this->_payload);
        string returnString = "type= "+std::to_string(this->type)+" seqn= "+std::to_string(this->seqn)+
        " length= "+std::to_string(this->length)+" timestamp= "+std::to_string(this->timestamp)
        +" string= "+ myString;
        int n = returnString.length();
        char char_return[n + 1];
        strcpy(char_return, returnString.c_str());
        return char_return;
    }
packet::packet(char* str){
    char strUnmarshal[2000];
    strcpy(str,strUnmarshal);
    char charArrayMarshal[256];
    sscanf(strUnmarshal,"type= $i seqn= $i length= $i timestamp $i string= $s",this->type,this->seqn,this->length,this->timestamp,&charArrayMarshal);
    string strmarshal(charArrayMarshal);
    this->_payload = charArrayMarshal;
    }

