#pragma once
#include <string>
#include <array>
#ifndef CHESS_FUNCTIONS_H
#define CHESS_FUNCTIONS_H

// This is the header file for out chess bot
std::string testInputFunction(const std::string& input);

// == Analyzer Class == //
class Analyzer
{
private:
    std::array<std::array<int, 8>, 8> board; 
public:
    void loadFromFen(string FEN);
    bool isWhiteToMove();
    bool isPositionLegal();
    void transpose(string move);
    float getHeuristic();
}

#endif