//
// Created by rafa on 19/09/2021.
//

#ifndef SISOP2_TWITTER_NOTIFICATION_H
#define SISOP2_TWITTER_NOTIFICATION_H

#include <string>
#include <iostream>
#include <bits/stdc++.h>


using namespace std;

class PendingNotification {
private:
    int id;
    int timestamp;
    int length;
    vector<int> pending;
    string message;

public:
    PendingNotification(int id, vector<int> &pending, string message) {
        this->id = id;
        this->pending = pending;
        this->timestamp = 1;//chrono::seconds(std::time(NULL));
        this->length = message.length();
        this->message = message;
    }

    string getMessage() {
        return message;

    }

    int getLenghtMessage() {
        return this->length;
    }

    int getTimestamp() {
        //int a = timestamp.count();
        return 1;
    }

    vector<int> getPending() {
        return this->pending;
    }

    void removePending(int id_pending) {
        pending.erase(remove(this->pending.begin(),
                             this->pending.end(), id_pending),
                      this->pending.end());
    }
};




#endif //SISOP2_TWITTER_NOTIFICATION_H

