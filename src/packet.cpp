#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include "include/packet.hpp"

using namespace std;

char* packet::toString() {
    int length;
    string myString(this->getPayload());
    string returnString = "type= " + std::to_string(this->type) + " seqn= " + std::to_string(this->seqn)
                          + " length= " + std::to_string(this->length) + " timestamp= " + std::to_string(this->timestamp)
                          + " string= " + myString;

    int length = returnString.length();
    char char_return[length + 1];
    strcpy(char_return, returnString.c_str());
    
    //warning: ao retornar dessa maneira, esse endereço de memória é desalocado. 
    //funciona só porque o conteúdo da memória continua igual
    return char_return;
}

//Traduz os dados crus do formato type+seqn+length+timestamp+payload para atributos
packet::packet(char str[2048]) {
    string untranslatedString(str);
    string translatedCharArray;
    stringstream ss;

    //cout << untranslatedString << std::flush;
    ss << untranslatedString;
    ss >> this->type >> this->seqn >> this->length >> this->timestamp >> translatedCharArray;
    this->setPayload(translatedCharArray);
}
