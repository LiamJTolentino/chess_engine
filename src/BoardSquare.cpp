#include "ChessFunctions.h"



// == Arrays for conversions == //
float material[16] = {0,
    -1,-3,-3,-5,-9,-20,0,
    0,
    1,3,3,5,9,20,0
};
std::string fen[16] = {" ",
    "p","n","b","r","q","k"," ",
    " ",
    "P","N","B","R","Q","K"," "
};

std::string unicodes[16] = {" ",
    "♟","♞","♝","♜","♛","♚"," ",
    " ",
    "♙","♘","♗","♖","♕","♔"," "
};

// == Constructors == //

BoardSquare::BoardSquare()
    : state(0)
{
}

BoardSquare::BoardSquare(std::uint8_t initstate)
{
    state = initstate;
}

BoardSquare::BoardSquare(bool white, SQtype piecetype)
{
    if(piecetype == SQtype::TYPE_EMPTY) // to avoid making invalid squares
    {
        state = SQstate::EMPTY_SQUARE;
    }
    state =((white&1)<<3) + piecetype;
}

// == Getters == //

bool BoardSquare::isWhitePiece() const
{
    return state >> 3; // We just need to bitshift to the right by 3 to get the piece color
}

bool BoardSquare::isValid() const
{
    return state != SQstate::INVALID_SQUARE;
}

bool BoardSquare::canCapture(const BoardSquare& other) const
{
    if(isEmptySquare()){return false;} // Obviously empty squares can't capture
    if(other.isEmptySquare()){return true;} // Pieces can always move to empty squares (unless they're pinned to the king, but we're not worrying about that rn)
    return isWhitePiece() != other.isWhitePiece(); // Pieces can only capture opponent pieces
}

bool BoardSquare::isEmptySquare(bool includeEnPassant) const
{
    return getPiece()==SQstate::EMPTY_SQUARE || (includeEnPassant && (state&7)==7);
}

bool BoardSquare::isOutOfBounds() const
{
    return state == SQstate::INVALID_SQUARE;
}

bool BoardSquare::isPiece(SQtype piecetype) const
{
    return getPiece() == piecetype;
}

bool BoardSquare::isState(SQstate piecestate) const
{
    return state == piecestate;
}

std::uint8_t BoardSquare::getPiece() const
{
    return state&7;
}

std::uint8_t BoardSquare::getState() const
{
    return state;
}

// == Setters == //

void BoardSquare::setPiece(std::uint8_t piece)
{
    if (piece == 0){ clearSquare(); return;} // If a piece is white, we might get 1000 which we don't want, so we just clear the square

    state = (state >> 3) << 3; // Clears the last three bits so we can keep the color
    state += (piece << 5) >> 5; // Just add the last three bits of piece
}

void BoardSquare::setState(SQstate newstate)
{
    state = newstate;
}

void BoardSquare::setFromFEN(char piece)
{
    switch (piece)
    {
    case 'p':
        state = SQstate::BLACK_PAWN;
        break;
    case 'n':
        state = SQstate::BLACK_KNIGHT;
        break;
    case 'b':
        state = SQstate::BLACK_BISHOP;
        break;
    case 'r':
        state = SQstate::BLACK_ROOK;
        break;
    case 'q':
        state = SQstate::BLACK_QUEEN;
        break;
    case 'k':
        state = SQstate::BLACK_KING;
        break;
    
    case 'P':
        state = SQstate::WHITE_PAWN;
        break;
    case 'N':
        state = SQstate::WHITE_KNIGHT;
        break;
    case 'B':
        state = SQstate::WHITE_BISHOP;
        break;
    case 'R':
        state = SQstate::WHITE_ROOK;
        break;
    case 'Q':
        state = SQstate::WHITE_QUEEN;
        break;
    case 'K':
        state = SQstate::WHITE_KING;
        break;
    
    default:
        break;
    }
}

void BoardSquare::clearSquare()
{
    state = EMPTY_SQUARE;
}

float BoardSquare::getMaterialValue() const
{
    return material[state];
}

std::string BoardSquare::getFEN() const
{
    return fen[state];
}

std::string BoardSquare::getUnicode() const
{
    return unicodes[state];
}

RayStack BoardSquare::getPieceRays() const
{
    RayStack output;
    int piecetype = getPiece();
    int row_step;
    
    switch (piecetype)
    {
    case SQtype::TYPE_PAWN:
        // Pawns can only move forward, so row_step depends on piece color. White pawns move in the negative, while black pawns move in the positive
        row_step = (isWhitePiece()? -1 : 1);
        
        output.Push(Ray{row_step,-1,1}); // Pawn captures to the left
        output.Push(Ray{row_step,1,1}); // Pawn captures to the right
        // output.Push(Ray{row_step,0,2}); // Forward pawn move
        break;
    case SQtype::TYPE_KNIGHT:
        // Knights move two steps in one direction and one step orthogonally
        output.Push(Ray{1,2,1});
        output.Push(Ray{2,1,1});
        output.Push(Ray{1,-2,1});
        output.Push(Ray{2,-1,1});
        output.Push(Ray{-1,2,1});
        output.Push(Ray{-2,1,1});
        output.Push(Ray{-1,-2,1});
        output.Push(Ray{-2,-1,1});
        break;
    case SQtype::TYPE_BISHOP:
        // Bishops move along diagonals for any distance. 
        output.Push(Ray{1,1,8});
        output.Push(Ray{1,-1,8});
        output.Push(Ray{-1,1,8});
        output.Push(Ray{-1,-1,8});
        break;
    case SQtype::TYPE_ROOK:
        // Rooks can move along ranks and files for any distance
        output.Push(Ray{0,1,8});
        output.Push(Ray{1,0,8});
        output.Push(Ray{0,-1,8});
        output.Push(Ray{-1,0,8});
        break;
    case SQtype::TYPE_QUEEN:
        // Queens can move like bishops and rooks
        output.Push(Ray{1,1,8});
        output.Push(Ray{1,-1,8});
        output.Push(Ray{-1,1,8});
        output.Push(Ray{-1,-1,8});
        output.Push(Ray{0,1,8});
        output.Push(Ray{1,0,8});
        output.Push(Ray{0,-1,8});
        output.Push(Ray{-1,0,8});
        break;
    case SQtype::TYPE_KING:
        // Kings can move in any of the 8 directions, but for only one square
        output.Push(Ray{1,1,1});
        output.Push(Ray{1,-1,1});
        output.Push(Ray{-1,1,1});
        output.Push(Ray{-1,-1,1});
        output.Push(Ray{0,1,1});
        output.Push(Ray{1,0,1});
        output.Push(Ray{0,-1,1});
        output.Push(Ray{-1,0,1});
        break;
    default:
        break;
    }
    return output;
}