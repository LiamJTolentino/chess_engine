#include "ChessFunctions.h"
#include "Analyzer.h"
#include "ChessBot.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>


int main(){
    std::string input;
    int i = 0;

    Analyzer myAnalyzer;
    // BoardSquare mySquare(SQstate::BLACK_QUEEN);

    std::cout << "Program Started ♔" << std::endl;
    float mat = myAnalyzer.getMaterialDifference();

    std::cout << "Current material: " << mat << std::endl;

    BoardCoord bc;

    std::string currentFEN;
    std::ifstream fTestFile("TEST_FEN.txt");
    std::vector<Attacker> trace;
    std::string legality;

    std::vector<ChessMove> possible_moves;
    TESTbot dockfish(Difficulty::EASY);

    std::string bestmove;
    Eval chosenMove;

    int lineNum = 1;

    while (std::getline(fTestFile,currentFEN))
    {
        if (currentFEN.rfind('#',0)==0){ lineNum++;continue; }
        std::cout << "=========================================" << std::endl;
        std::cout << "\nLine: " << lineNum << std::endl;
        std::cout << "FEN: " << currentFEN << std::endl;
        std::cout << "=========================================" << std::endl;
        myAnalyzer.loadFromFen(currentFEN);
        Position currentPos(&myAnalyzer);
        mat = myAnalyzer.getMaterialDifference();
        trace = myAnalyzer.traceFrom(7,0,0,1,8,false,true);
        // trace = myAnalyzer.traceFrom(7,0,0,1,8);
        std::cout << "Current material: " << mat << std::endl;
        std::cout<< "Heuristic: " << std::to_string(myAnalyzer.getHeuristic()) << std::endl;
        std::cout<< "Hanging Score: " << std::to_string(myAnalyzer.getHangingScore()) << std::endl;
        std::cout << myAnalyzer.boardAsString() << std::endl;
        // std::cout << mySquare.getFEN() << std::endl;
        // std::cout << "Showing trace result" << std::endl;
        // for (const Attacker& atk : trace)
        // {
        //     std::cout << atk.piece.getUnicode() << " at " << std::to_string(atk.row) << "," << std::to_string(atk.col) << std::endl;
        // }
        // std::cout<< myAnalyzer.TESTprintMoves() <<std::endl;
        // std::cout << std::flush;
        std::cout << "Checking legality" << std::endl;
        legality = (myAnalyzer.isPositionLegal() ? "Legal" : "Illegal");
        std::cout<< "Legal position:\t\t" << legality << std::endl;

        
        
        // std::cout<< "=========================================" <<std::endl;
        // std::cout<< "==========  Moves and results  ==========" <<std::endl;
        // std::cout<< "=========================================" <<std::endl;

        // possible_moves = myAnalyzer.getAllMoves();
        // std::cout<< "Moves: " << possible_moves.size() << std::endl;
        // for(ChessMove move : possible_moves)
        // {
        //     // std::cout<< "~~~~~~~~~~~~~~~   " << move.DBstring() << "   ~~~~~~~~~~~~~~~" <<std::endl;
        //     std::cout<< "~~~~~~~~~~~~~~~   " << move.AsString(true) << "   ~~~~~~~~~~~~~~~" <<std::endl;
        //     // Position result = currentPos.transpose(move);
        //     // myAnalyzer.loadFromPosition(&result);
        //     // std::cout<< "~~~~~~~~~~~~~~~   Resulting Position   ~~~~~~~~~~~~~~~" <<std::endl;
        //     // std::cout << myAnalyzer.boardAsString() << std::endl;
        //     // std::cout<< (myAnalyzer.isPositionLegal() ? "Legal" : "Illegal") <<std::endl;
        // }
        if(myAnalyzer.isPositionLegal())
        {
            std::cout<< "Calculating best move..." <<std::endl;
            auto a = std::chrono::high_resolution_clock::now();
            chosenMove = dockfish.MiniMax(currentPos,5,Eval(-1),Eval(1));
            auto b = std::chrono::high_resolution_clock::now();
            std::cout<< "Calculated in "
            << std::chrono::duration_cast<std::chrono::seconds>(b-a).count()
            << " seconds" <<std::endl;
            Position newPos = currentPos.transpose(chosenMove.m_Move);
            myAnalyzer.loadFromPosition(&newPos);
            std::cout<< "Best move to play: "
            << chosenMove.m_Move.AsString(currentPos.isWhiteToMove()) <<std::endl;
            std::cout << myAnalyzer.boardAsString() << std::endl;
            std::cout<< chosenMove.AsString() <<std::endl;
            // bestmove = dockfish.GetNextMove(currentFEN);
            // std::cout<< "Best move to play: "
            // << bestmove <<std::endl;
        }
        std::cout<< "_________________________________________" <<std::endl;
        
        lineNum++;
    }

    // std::cout << "DONE" << std::endl;

    // std::uint32_t backrank = 3401444268;
    // std::cout << backrank << std::endl;
    // backrank &= ~(15<<28);
    // std::cout << backrank << std::endl;
    // backrank += 14<<28;
    // std::cout << backrank << std::endl;
    //  std::cout << ((backrank>>16)&15) << std::endl;

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
