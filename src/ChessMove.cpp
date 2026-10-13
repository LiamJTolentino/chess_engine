/**
 * @file Implementation of all the ChessMove stuff in Chessfunctions.h
 */
#include "ChessFunctions.h"

// All the stuff here involves bitwise operations which makes things really fast but also annoying to read

ChessMove::ChessMove(bool promotion, 
                int piece, 
                int start_row, int start_col, 
                int dest_row, int dest_col)
{
    mvinfo = 0;
    // Setting stuff using bitwise operations
    mvinfo += (promotion&1)<<15;
    mvinfo += (piece&7)<<12;
    mvinfo += (start_row&7)<<9;
    mvinfo += (start_col&7)<<6;
    mvinfo += (dest_row&7)<<3;
    mvinfo += dest_col&7;
}

bool ChessMove::isPawnMove()
{
    return mvinfo>>15;
}

int ChessMove::getPieceVal()
{
    return (mvinfo>>12)&7;
}

int ChessMove::getStartRow()
{
    return (mvinfo>>9)&7;
}

int ChessMove::getStartCol()
{
    return (mvinfo>>6)&7;
}

int ChessMove::getDestRow()
{
    return (mvinfo>>3)&7;
}

int ChessMove::getDestCol()
{
    return mvinfo&7;
}

// Okay I just realised I could just directly AND the values into the correct bits without setting them to zero, but whatevs


void ChessMove::setStartPos(std::uint8_t start_row, std::uint8_t start_col)
{
    mvinfo = mvinfo&(~(63<<6));
    mvinfo += (start_row&7)<<9;
    mvinfo += (start_col&7)<<6;
}

void ChessMove::setDestPos(std::uint8_t dest_row, std::uint8_t dest_col)
{
    mvinfo = mvinfo&(~63); // Clear the last 6 bits
    mvinfo += (dest_row&7)<<3;
    mvinfo += dest_col&7;
}

void ChessMove::setPieceVal(std::uint8_t piece_val)
{
    mvinfo = mvinfo&(~(7<<12));
    mvinfo += (piece_val&7)<<12;
}

void ChessMove::setPawnMove(bool isPromotion)
{
    mvinfo = mvinfo&(isPromotion<<15);
}