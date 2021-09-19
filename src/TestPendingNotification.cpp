//
// Created by rafa on 19/09/2021.
//
#include <string>
#include <iostream>

#include "PendingNotification.cpp"

using namespace std;

void print_vector(const std::vector<int>& vec) {
    for (const auto& i : vec) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}


int main() {
    int src[] = { 1, 2, 3, 4};
    int n = sizeof(src) / sizeof(src[0]);

    vector<int> dest(src, src + n);
    PendingNotification notification(12,dest, "dfsdfsd");
    vector<int> a = notification.getPending();
    print_vector(a);
    notification.removePending(3);
    vector<int> b = notification.getPending();
    print_vector(b);
    return 0;
}
