#include "Analyzer.h"
#include "ChessFunctions.h"
#include <cctype>

Analyzer::Analyzer()
    : board {
        {-5,-3,-3.1,-9,-20,-3.1,-3,-5},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {5,3,3.1,9,20,3.1,3,5}

    },
    whiteToPlay(true),
    diff(Difficulty::EASY)
{
}

bool Analyzer::isPositionLegal()
{
    // First we check if both kings are on the board


    // Then we check if the current player can capture the opponent king with a piece


    // Then we see if there are any unpromoted pawns in their final rank

    return true;
}

/**
 * @brief Used for converting FEN
 */
std::map<char,float> FENmap = {
    { 'p', -1},     // Black pawn
    { 'n', -3},     // Black knight
    { 'b', -3.1},   // Black bishop
    { 'r', -5},     // Black rook
    { 'q', -9},     // Black queen
    { 'k', -20},    // Black king
    { 'P', 1},      // White pawn
    { 'N', 3},      // White knight
    { 'B', 3.1},    // White bishop
    { 'R', 5},      // White rook
    { 'Q', 9},      // White queen
    { 'K', 20},     // White king
};

void Analyzer::loadFromFen(std::string& FEN)
{
    // First set everything to 0
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            board[row][col] = 0.0;
        }
    }
    // Iterate over each character
    int charIndex = 0;
    int FENlength = FEN.length();
    int row = 0;
    int col = 0;

    // First the board
    while (row < 8 && charIndex < FENlength)
    {
        char currentChar = FEN[charIndex];

        // Slash means next rank and space means we're done with the board
        if (currentChar == '/' || currentChar == ' ')
        {
            row++;
            col = 0;
            charIndex++;
            continue;
        }

        // Number means empty squares so we skip that many
        if (currentChar >= '0' && currentChar <= '9')
        {
            col += currentChar - '0';
            charIndex++;
            continue;
        }
        
        // Pieces
        board[row][col] = FENmap[currentChar];

        col++;
        charIndex++;
    }

    // Now the other stuff
    while (charIndex < FENlength)
    {
        char currentChar = FEN[charIndex];
        switch (currentChar)
        {
        case 'w':
            whiteToPlay = true;
            break;
        case 'b':
            whiteToPlay = false;
            break;
        // TODO: Implement castle and en passant and ply
        default:
            break;
        }
        charIndex++;
    }
}

bool Analyzer::isWhiteToMove()
{
    return whiteToPlay;
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