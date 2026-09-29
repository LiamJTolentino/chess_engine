#pragma once
#include <string>
#include <array>
// == Analyzer Class == //
class Analyzer
{
private:
    // std::array<std::array<int, 8>, 8> board; 
    int board[8][8];
public:
    Analyzer();
    void loadFromFen(std::string FEN);
    bool isWhiteToMove();
    bool isPositionLegal();
    void transpose(std::string move);
    float getHeuristic();
};
