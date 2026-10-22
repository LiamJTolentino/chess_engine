/**
 * @file Implementation of all the ChessMove stuff in Chessfunctions.h
 */
#include "ChessFunctions.h"
#include "Tracer.h"

// All the stuff here involves bitwise operations which makes things really fast but also annoying to read

ChessMove::ChessMove(std::uint16_t mvinfo)
    : mvinfo(mvinfo)
{}

ChessMove::ChessMove(bool promotion, 
                MVpiece piece, 
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

ChessMove::ChessMove(bool promotion, 
            Attacker attacker,
            int target_row,
            int target_col)
{
    mvinfo = 0;
    mvinfo += (promotion&1)<<15;
    mvinfo += (attacker.piece.getMVpiece()&7)<<12;
    mvinfo += (attacker.row&7)<<9;
    mvinfo += (attacker.col&7)<<6;
    mvinfo += (target_row&7)<<3;
    mvinfo += target_col&7;
}

bool ChessMove::isPawnMove() const
{
    return (mvinfo>>15) || (isPieceType(MVpiece::MV_PAWN));
}

MVpiece ChessMove::getPieceVal() const
{
    return static_cast<MVpiece>((mvinfo>>12)&7);
}

SQtype ChessMove::getPieceType() const
{
    return static_cast<SQtype>(((mvinfo>>12)&7) + 1);
}

bool ChessMove::isPieceType(MVpiece piece) const
{
    return static_cast<MVpiece>((mvinfo>>12)&7) == piece;
}

bool ChessMove::isCastle() const
{
    return static_cast<MVpiece>((mvinfo>>12)&7) == MVpiece::MV_SHORT_CASTLE
        || static_cast<MVpiece>((mvinfo>>12)&7) == MVpiece::MV_LONG_CASTLE;
}

int ChessMove::getStartRow() const
{
    return (mvinfo>>9)&7;
}

int ChessMove::getStartCol() const
{
    return (mvinfo>>6)&7;
}

int ChessMove::getDestRow() const
{
    return (mvinfo>>3)&7;
}

int ChessMove::getDestCol() const
{
    return mvinfo&7;
}

BoardCoord ChessMove::getStart() const
{
    return BoardCoord{(mvinfo>>9)&7,(mvinfo>>6)&7};
}

BoardCoord ChessMove::getDest() const
{
    return BoardCoord{(mvinfo>>3)&7,mvinfo&7};
}

// Okay I just realised I could just directly AND the values into the correct bits without setting them to zero, but whatevs


void ChessMove::setStartPos(int start_row, int start_col)
{
    mvinfo = mvinfo&(~(63<<6));
    mvinfo += (start_row&7)<<9;
    mvinfo += (start_col&7)<<6;
}

void ChessMove::setDestPos(int dest_row, int dest_col)
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

std::string ChessMove::AsString(bool whitemove) const
{
    if(isCastle())
    {
        if(isPieceType(MV_SHORT_CASTLE))
        {
            return "O-O";
        }
        if(isPieceType(MV_LONG_CASTLE))
        {
            return "O-O-O";
        }
    }
    std::string output;
    std::string start = indicesToCoord(getStartRow(),getStartCol());
    std::string end = indicesToCoord(getDestRow(),getDestCol());
    std::string piece = BoardSquare(whitemove,getPieceType()).getFEN();

    if(isPawnMove())
    {
        output += start[0];// Pawns on the same rank cannot move into the same square
        output += end;
        if(!isPieceType(MV_PAWN))// If it is a pawn promotion
        {
            output += "=" + piece;
        }
        return output;
    }
    output += piece;
    output += start;
    output += end;

    return output;
}

std::string ChessMove::DBstring() const
{
    std::string output = "";
    output += (isPawnMove()?"1":"0");
    output += " " + std::to_string(static_cast<int>(getPieceVal()));
    output += " from " + indicesToCoord(getStartRow(),getStartCol());
    output += " to " + indicesToCoord(getDestRow(),getDestCol());
    // output += " " + std::to_string(getStartRow());
    // output += "," + std::to_string(getStartCol());
    // output += " " + std::to_string(getDestRow());
    // output += "," + std::to_string(getDestCol());
    return output;
}