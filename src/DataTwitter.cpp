
#include "include/DataTwitter.hpp"
#include "iostream"
#include <string>
#include <stdexcept>
#include "utils.cpp"
#include <vector>

using namespace std;

DataTwitter::DataTwitter(){
    map<string, Profile> profileMap;
    map<string,int> nameID;
    this->IDmap = nameID;
    this->Database = profileMap;
}


bool DataTwitter::isProfileInDatabase(string profile_name) {
    return this->Database.count(profile_name) > 0;
}

bool DataTwitter::addFollower(string followed, string follower) {
        //Profile profile = this->Database[followed];
        this->Database[followed].add_follower(follower);
        cout << follower << " seguiu " << followed << endl << flush;
        cout << "Seguidores de " << followed << ": " << flush;
        print_followers(this->Database[followed].get_followers());
        cout << endl << flush;

        //cout << "Endereco de " << followed << ": " << addressof(this->Database[followed]) << endl << flush;
        //Profile profile = this->Database[name];
        //Profile profile2 = this->Database.at(name);
        //cout << "Endereco de " << name << ": " << addressof(this->Database[name]) << endl << flush;
        return true;
}

void DataTwitter::add_Message(string user,string message,long int timestamp){
    this->Database[user].add_message(message);
    this->Database[user].add_timestamp(timestamp);
}

void DataTwitter::Notification(){
    std::map<string, Profile>::iterator it;
    vector<long int> minProfileTimestamp;
    vector<string> Profile;
    vector<int> profilePosition;
    vector<string>   message;
    for (it = this->Database.begin(); it != this->Database.end(); it++)
       {
          vector<string> messages = this->Database[it->first].get_messages();
          vector<bool> isSended = this->Database[it->first].get_isSended();
          vector<long int> timestamp = this->Database[it->first].get_timestamp();
          set<string> followers = this->Database[it->first].get_followers();
          for(int i=0;i<messages.size();i++){
              if(!(isSended[i])){
                for(auto follower : followers){
                  packet newpacket1;
                  newpacket1.newpacket(messages[i],timestamp[i],it->first);
                  this->Database[follower].add_pendingNotification(newpacket1);
                }
                this->Database[it->first].setToTrue(i);
              }
          }
          }
}

void DataTwitter::AddProfile(string name) {
    //cout << "\n" << "adicionando perfil " << name << endl << flush;
    Profile profile;
    profile.setup_profile(name);
    this->Database.insert ( std::pair<string,Profile>(name,profile) );
    usercount++;
    this->IDmap[name]=usercount;
}


int DataTwitter::FindProfileNumber(string name) {
    return this->IDmap[name];
}
void DataTwitter::Message(string message, notification Notification)
{

}
void DataTwitter::Follow(string name,string followName)
{
    
}

string DataTwitter::SaveDataBase(){
     map<string, Profile>::iterator it;
    map<string,Profile> Database =  this->Database;
    string s;
    s = s +" "+ to_string(Database.size())+ " ";
    string v;
    cout<<s;
    for (it =Database.begin(); it !=Database.end(); it++)
    {
         v = v+ " "+ it->first+" "+it->second.toString();  
    }
    s = s+v;
    return s;
}

void DataTwitter::LoadDataBase(ifstream &ss){
    map<string, Profile> profileMap;
    int lengthprofiles;
    ss>>lengthprofiles;
    string name;
    for( int i =0;i<lengthprofiles;i++){
        ss>>name;
        this->Database[name]=StreamtoProfile(ss);
        this->IDmap[name]= i;
    }
}

Profile  DataTwitter::StreamtoProfile(ifstream &ss){
  Profile p;
  ss>>p.name;
  int lengthfollowers;
  ss>>lengthfollowers;
  set<string> followers;
  for(int i=0;i<lengthfollowers;i++){
    string s;
    ss>>s;
    followers.insert(s);
  }
  p.followers = followers;
  int lengthReceivedMessages;
  ss>>lengthReceivedMessages;
  vector<string> receivedMessages;
  for(int i=0;i<lengthReceivedMessages;i++){
    string s;
    ss>>s;
    receivedMessages.push_back(s);
  }
  p.messages = receivedMessages;
  int lengthtimeStamp;
  ss>>lengthtimeStamp;
  vector<long int> receivedTimestamp;
  for(int i=0;i<lengthtimeStamp;i++){
    long int s;
    ss>>s;
    receivedTimestamp.push_back(s);
  }
  p.timestamp = receivedTimestamp;

  int lengthtimeSent;
  ss>>lengthtimeSent;
  vector<bool> receivedSent;
  for(int i=0;i<lengthtimeSent;i++){
    string s;
    ss>>s;
    bool b;
    if(s=="0"){
      b=false;
    }
    else{
      b=true;
    }
    receivedSent.push_back(b);
  }
  p.ismessageSended = receivedSent;

  return p;
}

vector<packet>   DataTwitter::NotificationVector(string username){
  return this->Database[username].pendingNotification;
}

void DataTwitter::deleteNotification(string username,string packetmessage){
    this->Database[username].deletePending(packetmessage);
}