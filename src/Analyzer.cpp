#include "Analyzer.h"

Analyzer::Analyzer()
    : board {
        {-5,-3,-3,-9,-20,-3,-3,-5},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {5,3,3,9,20,3,3,5}

    },
    whiteToPlay(true)
{
}

float Analyzer::getMaterialDifference()
{
    float total = 0.0;
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            total += board[row][col];
        }
    }
    return total;
}