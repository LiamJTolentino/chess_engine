#include "ChessFunctions.h"
#include <iostream>

int main(){
    std::string input;
    int i = 0;

    while (i < 4 && std::getline(std::cin, input)){
        std::cout << testInputFunction(input) << std::endl;
        i++;
    }
}