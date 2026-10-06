#include "ChessFunctions.h"
#include "Analyzer.h"
#include <iostream>
#include <vector>
#include <fstream>

int main(){
    std::string input;
    int i = 0;

    Analyzer myAnalyzer;
    BoardSquare mySquare(SQstate::BLACK_QUEEN);

    std::cout << "Program Started ♔" << std::endl;
    float mat = myAnalyzer.getMaterialDifference();

    std::cout << "Current material: " << mat << std::endl;

    BoardCoord bc;

    std::string currentFEN;
    std::ifstream fTestFile("TEST_FEN.txt");
    std::vector<Attacker> trace;
    std::string legality;

    while (std::getline(fTestFile,currentFEN))
    {
        std::cout << "FEN: " << currentFEN << std::endl;
        myAnalyzer.loadFromFen(currentFEN);
        mat = myAnalyzer.getMaterialDifference();
        trace = myAnalyzer.traceFrom(7,0,0,1,8,false,true);
        // trace = myAnalyzer.traceFrom(7,0,0,1,8);
        std::cout << "Current material: " << mat << std::endl;
        std::cout << myAnalyzer.boardAsString() << std::endl;
        std::cout << mySquare.getFEN() << std::endl;
        std::cout << "Showing trace result" << std::endl;
        for (const Attacker& atk : trace)
        {
            std::cout << atk.piece->getUnicode() << " at " << std::to_string(atk.row) << "," << std::to_string(atk.col) << std::endl;
        }
        // std::cout << std::flush;
        std::cout << "Checking legality" << std::endl;
        legality = (myAnalyzer.isPositionLegal() ? "Legal" : "Illegal");
        std::cout << "Legal position: " << legality << std::endl;
    }

    std::cout << "DONE" << std::endl;

    fTestFile.close();

    // while (i < 4 && std::getline(std::cin, input)){
    //     std::cout << i << std::endl;
    //     std::cout << testInputFunction(input) << std::endl;
    //     // bc = coordToIndices(input);
    //     // std::cout << bc.row << "," << bc.col << std::endl;
    //     myAnalyzer.loadFromFen(input);
    //     mat = myAnalyzer.getMaterialDifference();
    //     std::cout << "Current material: " << mat << std::endl;
    //     std::cout << myAnalyzer.boardAsString() << std::endl;
    //     std::cout << mySquare.getFEN() << std::endl;
    //     i++;
    // }
}

