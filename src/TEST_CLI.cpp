#include "ChessFunctions.h"
#include "Analyzer.h"
#include <iostream>
#include <vector>
#include <fstream>


int range(int a, int b)
{
    static long long int i;
    static int state = 0;
    switch (state) {
    case 0: /* start of function */
        state = 1;
        for (i = a; i < b; i++) {
            return i;

        /* Returns control */
        case 1:
            std::cout << "control at range"
                 << std::endl; /* resume control straight
                           after the return */
        }
    }
    state = 0;
    return 0;
}

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

    int lineNum = 1;

    while (std::getline(fTestFile,currentFEN))
    {
        std::cout << "\nLine: " << lineNum << std::endl;
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
        lineNum++;
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

    std::vector<int> myNumbers;

    int j;
    for (; j = range(1, 5);){
        std::cout << "control at main: j = " << j << std::endl;
        myNumbers.push_back(j);
        std::cout << "myNumbers has " << myNumbers.size() << std::endl;
    }
}
