#include <vector>
#include <string.h>
#include <set>
#include "packet.hpp"
#include <iostream>
#include <iostream>
#include <fstream>
using namespace std;
class Profile{
  public:
    set<string> followers;
    vector<string> messages;
    vector<packet> pendingNotification;
    string name;
    string toString();
    void setup_profile(string name);
    void add_follower(string follower);
    set<string> get_followers();
    void add_message(string message);
    vector<string> get_messages();
    void add_pendingNotification(packet notification);
};
 