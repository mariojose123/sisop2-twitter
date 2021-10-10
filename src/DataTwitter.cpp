#include "include/DataTwitter.hpp"
#include "include/Profile.hpp"
#include <string>

using namespace std;

DataTwitter::DataTwitter(){
    map<string,Profile> profileMap;
    map<string,int> nameID;
    this->IDmap = nameID;
    this->Database = profileMap;
}

Profile DataTwitter::FindProfile(string name)
{
    return this->Database[name];
}

void DataTwitter::AddProfile(string name) {
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

void DataTwitter::SaveDataBase(){

}

