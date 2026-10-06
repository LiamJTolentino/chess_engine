#include "Analyzer.h"
#include "ChessFunctions.h"
#include <cctype>
#include <vector>

Analyzer::Analyzer()
    : board {
        {BoardSquare(4),BoardSquare(2),BoardSquare(3),BoardSquare(5),BoardSquare(6),BoardSquare(3),BoardSquare(2),BoardSquare(4)},
        {BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9)},
        {BoardSquare(12),BoardSquare(10),BoardSquare(11),BoardSquare(13),BoardSquare(14),BoardSquare(11),BoardSquare(10),BoardSquare(12)}

    },
    whiteToPlay(true),
    diff(Difficulty::EASY)
{
}

BoardSquare* Analyzer::getSquareAt(int row, int col)
{
    if (row > 7 || row < 0 || col > 7 || col < 0) { return nullptr;} // Out of range

    return &board[row][col];
}

bool Analyzer::isPositionLegal()
{
    bool bking = false;
    bool wking = false;

    BoardCoord bkloc;
    BoardCoord wkloc;
    // First we check if both kings are on the board
    for (int row=0; row<8; row++)
    {
        for (int col=8; row<8; row++)
        {
            if(board[row][col].getState() == SQstate::BLACK_KING) { bking = true; bkloc.row = row; bkloc.col = col;}
            if(board[row][col].getState() == SQstate::WHITE_KING) { wking = true; bkloc.row = row; bkloc.col = col;}
        }
    }
    if (!(bking && wking)) { return false; }

    // Then we check if the current player can capture the opponent king with a piece
    BoardSquare* squarePtr;
    BoardCoord& ekloc = (whiteToPlay ? bkloc : wkloc); // Which one is the enemy king?


    // Then we see if there are any unpromoted pawns in their final rank
    for (int col=0; col<8; col++){
        if(board[0][col].getState() == SQstate::WHITE_PAWN || board[7][col].getState() == SQstate::BLACK_PAWN)
        {
            return false;
        }
    }

    return true;
}

std::vector<Attacker> Analyzer::traceFrom(int from_row, int from_col, int d_row, int d_col, int range, bool includeEmpty)
{
    std::vector<Attacker> output;
    int i = 0;
    int current_row = from_row + d_row;
    int current_col = from_col + d_row;
    BoardSquare* pSquare = getSquareAt(current_row,current_col);
    while(pSquare && i < range)
    {
        if(includeEmpty || !pSquare->isEmptySquare())
        output.push_back(Attacker(pSquare,current_row,current_col));
        i++;
        current_row += d_row;
        current_col += d_col;
        pSquare = getSquareAt(current_row,current_col);
    }

    return output;
}

void Analyzer::loadFromFen(std::string& FEN)
{
    // First set everything to 0
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            // board[row][col] = 0.0;
            board[row][col].clearSquare();
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
        // board[row][col] = FENmap[currentChar];
        board[row][col].setFromFEN(currentChar);

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

std::string Analyzer::boardAsString()
{
    std::string output = "================\n";
    for (int row=0; row< 8; row++)
    {
        output += "|";
        for (int col=0; col<8; col++)
        {
            // output += unicodemap[board[row][col]];
            output += board[row][col].getUnicode();
            output += "|";
        }
        output += "\n================\n";
    }
    output += (whiteToPlay ? "White" : "Black");
    output += " to move";
    return output;
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
            // total += board[row][col];
            total += board[row][col].getMaterialValue();
        }
    }
    return total;
}



Attacker::Attacker()
    : piece(nullptr),
    row(0),
    col(0)
{}

Attacker::Attacker(BoardSquare* ptrPiece, std::uint8_t rownum, std::uint8_t colnum)
{
    piece = ptrPiece;
    row = rownum;
    col = colnum;
}