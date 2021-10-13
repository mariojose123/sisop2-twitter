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
    vector<long int> timestamp;
    vector<packet> pendingNotification;
    vector<bool> ismessageSended;
    string name;
    string toString();
    long int MinTimeNotSended();
    string MessageAt(long int timestamp);
    vector<long int> get_timestamp();
    void add_timestamp(long int timestamp);
    void setup_profile(string name);
    void add_follower(string follower);
    set<string> get_followers();
    void add_message(string message);
    vector<string> get_messages();
    void add_pendingNotification(packet notification);
    void  setToTrue(int position);
    bool IsSended(int position);
};
 