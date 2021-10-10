#include "include/DataTwitter.hpp"
#include "include/Profile.hpp"
#include "iostream"
#include <string>
#include <stdexcept>
#include "utils.cpp"

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

void DataTwitter::SaveDataBase(){

}

