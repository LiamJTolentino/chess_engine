#include "ChessFunctions.h"
#include "Analyzer.h"
#include <iostream>

int main(){
    std::string input;
    int i = 0;

    Analyzer myAnalyzer;

    std::cout << "Program Started ♔" << std::endl;
    float mat = myAnalyzer.getMaterialDifference();

    std::cout << "Current material: " << mat << std::endl;

    BoardCoord bc;

    while (i < 4 && std::getline(std::cin, input)){
        std::cout << i << std::endl;
        std::cout << testInputFunction(input) << std::endl;
        // bc = coordToIndices(input);
        // std::cout << bc.row << "," << bc.col << std::endl;
        myAnalyzer.loadFromFen(input);
        mat = myAnalyzer.getMaterialDifference();
        std::cout << "Current material: " << mat << std::endl;
        i++;
    }
}