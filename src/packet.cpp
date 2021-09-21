#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include "include/packet.hpp"

using namespace std;

char* packet::toString() {
    string myString(this->_payload);
    string returnString = "type= " + std::to_string(this->type) + " seqn= " + std::to_string(this->seqn)
                          + " length= " + std::to_string(this->length) + " timestamp= " + std::to_string(this->timestamp)
                          + " string= " + myString;
    int n = returnString.length();
    char char_return[n + 1];
    strcpy(char_return, returnString.c_str());
    return char_return;
}

packet::packet(char str[2048]) {
    string strUnmarshal(str);
    string charArrayMarshal;
    stringstream ss;

    cout << strUnmarshal << std::flush;
    ss << strUnmarshal;
    ss >> this->type >> this->seqn >> this->length >> this->timestamp >> charArrayMarshal;
    this->_payload = charArrayMarshal;
}
