#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string input = "one two three";
    std::stringstream ss(input);

    std::string word;
    int i = 0;
    while (ss >> word) {
        std::cout << word << std::endl; // Outputs each word on a new line
        i++;
    }
    std::cout<< "+i+" << i << std::endl;
    return 0;
}
