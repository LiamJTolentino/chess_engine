#include "ChessFunctions.h"
#include "Analyzer.h"
#include <iostream>

int main(){
    std::string input;
    int i = 0;

    Analyzer myAnalyzer;

    std::cout << "Program Started" << std::endl;
    float mat = myAnalyzer.getMaterialDifference();

    std::cout << "Current material: " << mat << std::endl;

    while (i < 4 && std::getline(std::cin, input)){
        std::cout << testInputFunction(input) << std::endl;
        i++;
    }
}