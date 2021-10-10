#include <iostream>
#include <vector>

void print_vector(std::vector<string> const &vector){
    for (int i = 0; i < vector.size(); i++) {
    std::cout << vector.at(i) << " | ";
    }
    printf("\n");
}