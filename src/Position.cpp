/**
 * @file Position.cpp
 * @brief Implementation of the Position class declared in CompactPosition.h
 */

#include "ChessFunctions.h"
#include "CompactPosition.h"
#include "Analyzer.h"
// #include <iostream>

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
            SetSquare(row,col,currentPiece);
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

void Position::ClearSquare(int row, int col)
{
    if (row > 7 || row < 0 || col > 7 || col < 0)
    { return; }
    int shift_amount = (7-col)*4;
    board[row] &= ~(15<<shift_amount); // Clear the 4 bits at that square
}

void Position::PlacePiece(int row, int col, SQtype piecetype)
{
    if (row > 7 || row < 0 || col > 7 || col < 0)
    { return; }
    int shift_amount = (7-col)*4;
    board[row] &= ~(15<<shift_amount); // Clear the 4 bits at that square
    if (piecetype == SQtype::TYPE_EMPTY){ return; } // Empty squares are empty squares
    board[row] += (((isWhiteToMove()&1)<<3) + piecetype) <<shift_amount;
}

BoardSquare Position::GetSquareAt(int row, int col) const
{
    if (row > 7 || row < 0 || col > 7 || col < 0)
    { return BoardSquare(SQstate::INVALID_SQUARE); }
    int shift_amount = (7-col)*4;
    std::uint8_t piece_val = ((board[row]>>shift_amount)&15);
    return BoardSquare(piece_val);
}

// === Transposition === //

Position Position::transpose(const ChessMove move) const
{
    Position copy = *this; // We create a copy of the position so we don't make changes to this one

    // Step 1: Try castling
    if(move.isCastle())
    {
        // std::cout<< "Trying to castle" <<std::endl;
        // Get the back rank based on color turn
        int back_row = (isWhiteToMove()?7:0);
        if(move.isPieceType(MVpiece::MV_SHORT_CASTLE) && copy.CanShortCastle())
        {
            copy.ClearSquare(back_row,4); // Remove the King
            copy.ClearSquare(back_row,7); // Remove the Rook
            copy.PlacePiece(back_row,5,SQtype::TYPE_ROOK); // Place the rook on f
            copy.PlacePiece(back_row,5,SQtype::TYPE_KING); // Place the king on g

            // Return the copy
            copy.WhiteToMove(!isWhiteToMove());
            return copy;
        }
        if(move.isPieceType(MVpiece::MV_LONG_CASTLE) && copy.CanLongCastle())
        {
            copy.ClearSquare(back_row,4); // Remove the King
            copy.ClearSquare(back_row,0); // Remove the Rook
            copy.PlacePiece(back_row,3,SQtype::TYPE_ROOK); // Place the rook on d
            copy.PlacePiece(back_row,2,SQtype::TYPE_KING); // Place the king on c
            // Return the copy
            copy.WhiteToMove(!isWhiteToMove());
            return copy;
        }
        // If neither of those work, the move is invalid, so we return an invalid position
        // std::cout<< "INVALID" <<std::endl;
        copy.board = {0,0,0,0,0,0,0,0};
        return copy;
    }

    // For the remaining move types, we need to make sure that the piece at the start can capture the piece at the dest. 
    // We are not concerned with whether or not the piece can actually move in that direction, just that it can be placed on that square at all.
    BoardSquare startSquare = copy.GetSquareAt(move.getStartRow(),move.getStartCol());
    BoardSquare targetSquare = copy.GetSquareAt(move.getDestRow(),move.getDestCol());

    if(startSquare.isEmptySquare() // Empty Squares can't move
    || startSquare.isWhitePiece() != copy.isWhiteToMove() // You cannot move your opponent's pieces
    || !startSquare.canCapture(targetSquare) // You can only capture opponent pieces or empty squares
    )
    {
        copy.board = {0,0,0,0,0,0,0,0};
        // std::cout<< "INVALID: " << startSquare.getUnicode() 
        // << " from " << indicesToCoord(move.getStartRow(),move.getStartCol())
        // << "Can't take " << targetSquare.getUnicode()
        // << " on "
        // << indicesToCoord(move.getDestRow(),move.getDestCol())
        //  <<std::endl;
        return copy;
    }
    // Step 2: Try pawn moves (Pawn promotions, en passant, pawn forward moves)

    if(move.isPawnMove())
    {
        // First check if the start piece is indeed a pawn
        if(!startSquare.isPiece(SQtype::TYPE_PAWN))
        {
            copy.board = {0,0,0,0,0,0,0,0};
            // std::cout<< "INVALID: No pawn on "
            // << indicesToCoord(move.getStartRow(),move.getStartCol())
            //  <<std::endl;
            return copy;
        }
        // Next check if the destination piece is not a king or a castle move
        if(static_cast<int>(move.getPieceVal()) > 4)
        {
            copy.board = {0,0,0,0,0,0,0,0};
            // std::cout<< "INVALID: Pawn cannot promote to " 
            // << move.getPieceVal()
            // <<std::endl;
            return copy;
        }

        int pawn_direction = (copy.isWhiteToMove()?-1:1); // White pawns move in the negative direction
        BoardSquare passing_pawn = copy.GetSquareAt(move.getDestRow() - pawn_direction,move.getDestCol());
        // En Passant
        if(targetSquare.isPiece(SQtype::TYPE_EN_PASSANT)
        && passing_pawn.isPiece(SQtype::TYPE_PAWN)
        && startSquare.canCapture(passing_pawn))
        {
            copy.ClearSquare(move.getStartRow(),move.getStartCol()); // Remove the pawn
            copy.PlacePiece(move.getDestRow(),move.getDestCol(),SQtype::TYPE_PAWN); // Place the pawn
            copy.ClearSquare(move.getDestRow() - pawn_direction,move.getDestCol()); // Remove the captured pawn
            copy.WhiteToMove(!isWhiteToMove());
            // std::cout<< "SUCCESS" <<std::endl;
            return copy;
        }

        // Pawn Promotion
        int promotion_row = (copy.isWhiteToMove()?0:7);
        // Pawns cannot remain as pawns in their promotion square
        if(move.getDestRow() == promotion_row
        && move.getPieceVal() == MV_PAWN)
        {
            copy.board = {0,0,0,0,0,0,0,0};
            // std::cout<< "INVALID: Pawns must promote" <<std::endl;
            return copy;
        }

        // We assume the Analyzer does not queue invalid pawn moves, so we will parse the move as normal piece move
    }

    // Step 3: Normal Piece moves

    if(move.isPieceType(MV_KING)) // Any king moves immediately forfeit castling rights
    {
        copy.removeLongCastle();
        copy.removeShortCastle();
    }

    if(move.isPieceType(MV_ROOK)) // Moving a rook forfeits castling on that rook's side
    {
        if(move.getStartCol() == 0) // Queen's rook
        {
            copy.removeLongCastle();
        }
        if(move.getStartCol() == 7) // King's rook
        {
            copy.removeShortCastle();
        }
    }
    
    // Remove the piece (i.e. put an empty square at the start)
    copy.SetSquare(move.getStartRow(),move.getStartCol(),BoardSquare(SQstate::EMPTY_SQUARE));

    // Place the piece on the destination
    BoardSquare moved_piece = BoardSquare(((isWhiteToMove()&1)<<3) // Piece color to current player
                            + move.getPieceType()); // Piece type
    copy.SetSquare(move.getDestRow(),move.getDestCol(),moved_piece);

    // Toggle player turn
    copy.WhiteToMove(!isWhiteToMove());
    // std::cout<< "SUCCESS" <<std::endl;
    return copy;
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

void Position::removeLongCastle()
{
    if(isWhiteToMove())
    {
        WhiteLongCastle(false);
    } else {
        BlackLongCastle(false);
    }
}

void Position::removeShortCastle()
{
    if(isWhiteToMove())
    {
        WhiteShortCastle(false);
    } else {
        BlackShortCastle(false);
    }
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

bool Position::CanShortCastle() const
{
    if(isWhiteToMove())
    {
        return CanWhiteShortCastle()
            && GetSquareAt(7,4).isState(SQstate::WHITE_KING)
            && GetSquareAt(7,5).isEmptySquare()
            && GetSquareAt(7,6).isEmptySquare()
            && GetSquareAt(7,7).isState(SQstate::WHITE_ROOK);
    } else {
        return CanWhiteShortCastle()
            && GetSquareAt(0,4).isState(SQstate::BLACK_KING)
            && GetSquareAt(0,5).isEmptySquare()
            && GetSquareAt(0,6).isEmptySquare()
            && GetSquareAt(0,7).isState(SQstate::BLACK_ROOK);
    }
}

bool Position::CanLongCastle() const
{
    if(isWhiteToMove())
    {
        return CanWhiteShortCastle()
            && GetSquareAt(7,4).isState(SQstate::WHITE_KING)
            && GetSquareAt(7,3).isEmptySquare()
            && GetSquareAt(7,2).isEmptySquare()
            && GetSquareAt(7,1).isEmptySquare()
            && GetSquareAt(7,0).isState(SQstate::BLACK_ROOK);
    } else {
        return CanWhiteShortCastle()
            && GetSquareAt(0,4).isState(SQstate::BLACK_KING)
            && GetSquareAt(0,3).isEmptySquare()
            && GetSquareAt(0,2).isEmptySquare()
            && GetSquareAt(0,1).isEmptySquare()
            && GetSquareAt(0,0).isState(SQstate::BLACK_ROOK);
    }
}
