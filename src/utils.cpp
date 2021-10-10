#include <iostream>
#include <vector>
#include <set>

using namespace std;

void print_followers(set<string> const &followers){
    for (string follower : followers)
    {
        cout << follower << ' ' << flush;
    }
    cout << endl << flush;
}