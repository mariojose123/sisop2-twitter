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
    if (this->Database.count(followed)) {
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
    else
        return false;
}

void DataTwitter::AddProfile(string name) {
    //cout << "\n" << "adicionando perfil " << name << endl << flush;
    Profile profile;
    profile.setup_profile(name);
    this->Database[name] = profile;
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
  return p;
}