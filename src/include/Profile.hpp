 #pragma once
 #include <vector>
 #include <string.h>
 #include "packet.hpp"
using namespace std;
class Profile{
  public:
    void profilestring(string profile);
  private:
    string profile;
    vector<string> followers;
    vector<string> receivedMessage;
    vector<packet> pendingNotification;
};
 