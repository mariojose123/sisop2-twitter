#include "include/Profile.hpp"
#include "iostream"

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

void save_profile() {
}