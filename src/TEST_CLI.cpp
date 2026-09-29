#include "ChessFunctions.h"
#include <iostream>

int main(){
    std::string input;

    while (std::getline(std::cin, input)){
        std::cout << testInputFunction(input) << std::endl;
    }
}