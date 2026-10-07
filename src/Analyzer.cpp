#include "Analyzer.h"
#include "ChessFunctions.h"
#include <cctype>
#include <vector>
#include <utility>
#include <cmath>
// #include <fstream>
#include <iostream>
#include <cstdint>

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
    int numkings = 0;

    BoardCoord bkloc;
    BoardCoord wkloc;
    std::cout << "Counting Kings" << std::endl;
    // == First we check if both kings are on the board and there are only 2
    for (int row=0; row<8; row++)
    {
        for (int col=0; col<8; col++)
        {
            // std::cout << board[row][col].getUnicode() <<std::endl;
            if(board[row][col].getState() == SQstate::BLACK_KING) { bking = true; bkloc.row = row; bkloc.col = col; numkings++;}
            if(board[row][col].getState() == SQstate::WHITE_KING) { wking = true; wkloc.row = row; wkloc.col = col; numkings++;}
        }
    }
    if (!(bking && wking) || numkings != 2) { std::cout<< "Illegal because numkings " << numkings <<std::endl; return false; }

    // == Then we check if the current player can capture the opponent king with a piece
    // First make sure the kings aren't touching
    std::cout << "Are kings touching?" << std::endl;
    std::cout << "Vert dist: " << std::abs(bkloc.row - wkloc.row) << std::endl;
    std::cout << "Horiz dist: " << std::abs(bkloc.col - wkloc.col) << std::endl;
    if(std::abs(bkloc.row - wkloc.row) <= 1 && std::abs(bkloc.col - wkloc.col) <= 1)
    { std::cout<< "Illegal because Kings touch" <<std::endl; return false; }
    BoardCoord& ekloc = (whiteToPlay ? bkloc : wkloc); // Which one is the enemy king?

    // Looking for pawn attacks
    std::cout << "Can pawn take?" << std::endl;
    BoardSquare* squarePtr;
    
    squarePtr = getSquareAt(ekloc.row + (whiteToPlay ? 1 : -1),ekloc.col + 1);
    
    if(squarePtr && squarePtr->isWhitePiece() == whiteToPlay && squarePtr->isPiece(SQtype::TYPE_PAWN)) { std::cout<< "Illegal because Pawn can take king on " << ekloc.row << ", " << ekloc.col <<std::endl; return false; }
    
    squarePtr = getSquareAt(ekloc.row + (whiteToPlay ? 1 : -1),ekloc.col - 1);
    if(squarePtr && squarePtr->isWhitePiece() == whiteToPlay && squarePtr->isPiece(SQtype::TYPE_PAWN)) { std::cout<< "Illegal because Pawn can take king on " << ekloc.row << ", " << ekloc.col <<std::endl; return false; }

    std::vector<Attacker> attackers;
    std::cout << "Can Knight take?" << std::endl;
    
    // Looking for knights attacking
    for (const auto& [dr, dc] : std::vector<std::pair<int, int>>{
        {1, 2},
        {2, 1},
        {-1, 2},
        {-2,1},
        {1, -2},
        {2, -1},
        {-1, -2},
        {-2, -1}
    }) {
        attackers.clear();
        attackers = traceFrom(ekloc.row, ekloc.col, dr, dc, 1, false,true);
        if(!attackers.empty() 
        && attackers.at(0).piece->isWhitePiece() == whiteToPlay 
        && attackers.at(0).piece->isPiece(SQtype::TYPE_KNIGHT)) 
        { std::cout<< "Illegal because " << attackers.at(0).piece->getUnicode() << " at " << static_cast<int>(attackers.at(0).row) << ", " << static_cast<int>(attackers.at(0).col) << " can take king on " << ekloc.row << ", " << ekloc.col <<std::endl; return false; }
    }
    std::cout << "Can bishop take?" << std::endl;
    // Looking for bishops and queens
    for (const auto& [dr, dc] : std::vector<std::pair<int, int>>{
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1}
    }) {
        attackers.clear();
        attackers = traceFrom(ekloc.row, ekloc.col, dr, dc, 8, false,true);
        if(!attackers.empty() 
        && attackers.at(0).piece->isWhitePiece() == whiteToPlay 
        && ((attackers.at(0).piece->isPiece(SQtype::TYPE_BISHOP)
        || attackers.at(0).piece->isPiece(SQtype::TYPE_QUEEN)))) 
        { std::cout<< "Illegal because " << attackers.at(0).piece->getUnicode() << " at " << static_cast<int>(attackers.at(0).row) << ", " << static_cast<int>(attackers.at(0).col) << " can take king on " << ekloc.row << ", " << ekloc.col <<std::endl; return false;}
    }
    std::cout << "Can rook take?" << std::endl;
    // Looking for rooks and queens
    for (const auto& [dr, dc] : std::vector<std::pair<int, int>>{
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    }) {
        attackers.clear();
        attackers = traceFrom(ekloc.row, ekloc.col, dr, dc, 8, false,true);
        if(!attackers.empty() 
        && attackers.at(0).piece->isWhitePiece() == whiteToPlay 
        && ((attackers.at(0).piece->isPiece(SQtype::TYPE_ROOK)
        || attackers.at(0).piece->isPiece(SQtype::TYPE_QUEEN)))) 
        { std::cout<< "Illegal because " << attackers.at(0).piece->getUnicode() << " at " << static_cast<int>(attackers.at(0).row) << ", " << static_cast<int>(attackers.at(0).col) << " can take king on " << ekloc.row << ", " << ekloc.col <<std::endl; return false;}
    }
    std::cout << "Unpromoted pawns?" << std::endl;
    // Then we see if there are any unpromoted pawns in their final rank
    for (int col=0; col<8; col++){
        if(board[0][col].getState() == SQstate::WHITE_PAWN || board[7][col].getState() == SQstate::BLACK_PAWN)
        {
            std::cout<< "Illegal because Pawn in column " << col << " is not promoted." <<std::endl;
            return false;
        }
    }

    return true;
}

std::vector<Attacker> Analyzer::traceFrom(int from_row, int from_col, int d_row, int d_col, int range, bool includeEmpty,bool stopAtPiece)
{
    std::vector<Attacker> output;
    int i = 0;
    std::uint8_t current_row = from_row + d_row;
    std::uint8_t current_col = from_col + d_col;
    BoardSquare* pSquare = getSquareAt(current_row,current_col);
    while(pSquare && i < range)
    {
        if(includeEmpty || !pSquare->isEmptySquare())
        {
            std::cout << "Current row " << static_cast<int>(current_row) << std::endl;
            output.push_back(Attacker(pSquare,current_row,current_col));
            std::cout << output.back().row << std::endl;
            if(stopAtPiece && !pSquare->isEmptySquare()){ break; }
        }
        i++;
        current_row += d_row;
        current_col += d_col;
        pSquare = getSquareAt(current_row,current_col);
    }

    return output;
}

// std::vector<Attacker> Analyzer::getMovesFromSquare(int sourcerow, int sourcecol, bool include_defense)
// {
//     return;
// }

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
    : piece(ptrPiece),
    row(rownum),
    col(colnum)
{
    // piece = ptrPiece;
    // row = rownum;
    // col = colnum;
}