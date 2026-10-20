/**
 * @file Position.cpp
 * @brief Implementation of the Position class declared in CompactPosition.h
 */

#include "ChessFunctions.h"
#include "CompactPosition.h"
#include "Analyzer.h"

Position::Position()
    : board {
        1110795044,     // 0100 0010 0011 0101 0110 0011 0010 0100
        286331153,      // 0001 0001 0001 0001 0001 0001 0001 0001
        0,              // 0000 0000 0000 0000 0000 0000 0000 0000
        0,              // 0000 0000 0000 0000 0000 0000 0000 0000
        0,              // 0000 0000 0000 0000 0000 0000 0000 0000
        0,              // 0000 0000 0000 0000 0000 0000 0000 0000
        2576980377,     // 1001 1001 1001 1001 1001 1001 1001 1001
        3401444268      // 1100 1010 1011 1101 1110 1011 1010 1100
    }
{
}

Position::Position(const Analyzer* const pAnalyzer)
{
    BoardSquare currentPiece;
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            currentPiece = pAnalyzer->SquareAt(row,col);

        }
    }
    WhiteToMove(pAnalyzer->isWhiteToMove());
    WhiteShortCastle(pAnalyzer->CanWhiteShortCastle());
    WhiteLongCastle(pAnalyzer->CanWhiteLongCastle());
    BlackShortCastle(pAnalyzer->CanBlackShortCastle());
    BlackLongCastle(pAnalyzer->CanBlackLongCastle());
}

void Position::SetSquare(int row, int col, BoardSquare square)
{
    if (row > 7 || row < 0 || col > 7 || col < 0)
    { return; }
    int shift_amount = (7-col)*4;
    std::uint32_t piece_val = square.getState() << shift_amount;
    board[row] &= ~(15<<shift_amount); // Clear the 4 bits at that square
    board[row] += piece_val;            // Add the new value
}

BoardSquare Position::GetSquareAt(int row, int col) const
{
    if (row > 7 || row < 0 || col > 7 || col < 0)
    { return BoardSquare(SQstate::INVALID_SQUARE); }
    int shift_amount = (7-col)*4;
    std::uint8_t piece_val = ((board[row]>>shift_amount)&15);
    return BoardSquare(piece_val);
}


// == extra_info stuff == //


void Position::WhiteToMove(bool whitemove)
{
    extra_info &= 15;   //extra_info & 01111
    extra_info += (whitemove&1)<<4;
}

void Position::WhiteShortCastle(bool castle)
{
    extra_info &= 23;   //extra_info & 10111
    extra_info += (castle&1)<<3;
}

void Position::WhiteLongCastle(bool castle)
{
    extra_info &= 27;   //extra_info & 11011
    extra_info += (castle&1)<<2;
}

void Position::BlackShortCastle(bool castle)
{
    extra_info &= 29;   //extra_info & 11101
    extra_info += (castle&1)<<1;
}

void Position::BlackLongCastle(bool castle)
{
    extra_info &= 30;   //extra_info & 11110
    extra_info += (castle&1);
}


bool Position::isWhiteToMove() const
{
    return (extra_info>>4)&1;
}

bool Position::CanWhiteShortCastle() const
{
    return (extra_info>>3)&1;
}

bool Position::CanWhiteLongCastle() const
{
    return (extra_info>>2)&1;
}

bool Position::CanBlackShortCastle() const
{
    return (extra_info>>1)&1;
}

bool Position::CanBlackLongCastle() const
{
    return extra_info&1;
}



bool Position::isEnPassantAvailable() const
{
    BoardSquare currentPiece;
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            currentPiece = GetSquareAt(row,col);
            if(currentPiece.isPiece(SQtype::TYPE_EN_PASSANT))
            {return true;}
        }
    }
    return false;
}