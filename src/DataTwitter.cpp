#include "../include/DataTwitter.hpp"
#include <string>
using namespace std;
Profile DataTwitter::FindProfile(string name)
{
    return this->Database[name];
}



void DataTwitter::AddProfile(string name) {
    Profile newprofilex;
    newprofilex.profilestring(name);
    this->Database[name]=  newprofilex;
    usercount++;
    this->IDmap[name]=usercount;
}

DataTwitter::DataTwitter(){
    map<string,Profile> profileMap;
    map<string,int> nameID;
    this->IDmap = nameID;
    this->Database = profileMap;
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

