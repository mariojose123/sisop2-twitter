#include "include/Profile.hpp"
#include "iostream"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <iterator>
#include <numeric>
#include<bits/stdc++.h>

using namespace std;

void Profile::setup_profile(string name) {
    set<string> followers;
    vector<string> messages;
    vector<long int>    timestamp;
    vector<packet> packets;
    this->name = name;
    this->followers = followers;
    this->messages = messages;
    this->timestamp = timestamp;
    this->pendingNotification = packets;
}

void Profile::add_follower(string name) {
    this->followers.insert(name);
}

set<string> Profile::get_followers() {
    return this->followers;
}

void Profile::add_message(string message) {
    this->messages.push_back(message);
    this->ismessageSended.push_back(false);
}

void Profile::add_timestamp(long int   timestamp) {
    this->timestamp.push_back(timestamp);
}


long int Profile::MinTimeNotSended(){
    long int min = 2147483647;
    int position;
    for(int i=0;i<this->timestamp.size();i++){
        if(!(this->ismessageSended[i])){
            if(min>this->timestamp[i]){
                min = this->timestamp[i];
            }
        }
    }
    return min;
}

string Profile::MessageAt(long int timestamp){
    vector<long int >::iterator it;
    int position;
    it = find((this->timestamp).begin(), (this->timestamp).end(), timestamp);
    if (it != this->timestamp.end())
    {
        position = it - this->timestamp.begin();
    }
    return this->messages[position];
}

vector<string> Profile::get_messages() {
    return this->messages;
}

vector<long int> Profile::get_timestamp() {
    return this->timestamp;
}

void   Profile::setToTrue(int position){
    this->ismessageSended[position]=true;
}

bool Profile::IsSended(int position){
    this->ismessageSended[position];
}

void Profile::add_pendingNotification(packet notification){
    this->pendingNotification.push_back(notification);
}

void save_profile() {
}

string Profile::toString(){
    string s;
    s= s+ this->name;
    set<string> followers = this->followers;
    cout<< followers.size();
    s = s +" "+ to_string(followers.size())+ " ";
    std::ostringstream stream;
    std::copy(followers.begin(), followers.end(), std::ostream_iterator<std::string>(stream, " "));
    s = s +  stream.str();
    vector<string> messages = this->messages;
    s = s +" "+ to_string(messages.size())+" ";
    for(string i : messages){
        s = s + " " + i;
    }
    return s;
}


