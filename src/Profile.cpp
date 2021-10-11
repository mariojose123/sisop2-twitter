#include "include/Profile.hpp"
#include "iostream"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <iterator>
#include <numeric>

using namespace std;

void Profile::setup_profile(string name) {
    set<string> followers;
    vector<string> messages;
    vector<packet> packets;
    this->name = name;
  this->followers = followers;
  this->messages = messages;
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
}

vector<string> Profile::get_messages() {
    return this->messages;
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


