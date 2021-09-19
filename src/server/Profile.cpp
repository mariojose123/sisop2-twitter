#include "../include/Profile.hpp"
#include <string>
#include <vector>
void Profile::profilestring(string s){
  this->profile = s;
  vector<string> followersvector;
  vector<string> receivedMessages;
  vector<packet> packetVector;
  this->followers = followersvector;
  this->receivedMessage = receivedMessages;
  this->pendingNotification = packetVector;        
}