#pragma once
#include <vector>
#include <string.h>
#include "packet.hpp"

using namespace std;
class Profile{
  private:
    string name;
    vector<string> followers;
    vector<string> messages;
    vector<packet> pendingNotification;
  public:
    void setup_profile(string name);
    void add_follower(string follower);
    vector<string> get_followers();
    void add_message(string message);
    vector<string> get_messages();
};
 