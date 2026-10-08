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

BoardSquare::BoardSquare()
    : state(0)
{
}

BoardSquare::BoardSquare(std::uint8_t initstate)
{
    state = initstate;
}

bool BoardSquare::isWhitePiece()
{
    return state >> 3; // We just need to bitshift to the right by 3 to get the piece color
}

bool BoardSquare::isEmptySquare(bool includeEnPassant)
{
    return getPiece()==SQstate::EMPTY_SQUARE || (includeEnPassant && (state&7)==7);
}

bool BoardSquare::isPiece(SQtype piecetype)
{
    return getPiece() == piecetype;
}

std::uint8_t BoardSquare::getPiece()
{
    return state&7;
}

std::uint8_t BoardSquare::getState()
{
    return state;
}

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

float BoardSquare::getMaterialValue()
{
    return material[state];
}

std::string BoardSquare::getFEN()
{
    return fen[state];
}

std::string BoardSquare::getUnicode()
{
    return unicodes[state];
}

std::vector<Ray> BoardSquare::getPieceRays()
{
    std::vector<Ray> output;
    output.reserve(8); // A piece can have at most 8 directions to move in, so we only need that much.
    int piecetype = getPiece();
    
    switch (piecetype)
    {
    case SQtype::TYPE_PAWN:
        // Pawns can only move forward, so row_step depends on piece color. White pawns move in the negative, while black pawns move in the positive
        int row_step = (isWhitePiece()? -1 : 1);
        output.push_back(Ray{row_step,0,1}); // Forward pawn move
        output.push_back(Ray{row_step,-1,1}); // Pawn captures to the left
        output.push_back(Ray{row_step,1,1}); // Pawn captures to the right
        break;
    case SQtype::TYPE_KNIGHT:
        // Knights move two steps in one direction and one step orthogonally
        output.push_back(Ray{1,2,1});
        output.push_back(Ray{2,1,1});
        output.push_back(Ray{1,-2,1});
        output.push_back(Ray{2,-1,1});
        output.push_back(Ray{-1,2,1});
        output.push_back(Ray{-2,1,1});
        output.push_back(Ray{-1,-2,1});
        output.push_back(Ray{-2,-1,1});
        break;
    case SQtype::TYPE_BISHOP:
        // Bishops move along diagonals for any distance. 
        output.push_back(Ray{1,1,8});
        output.push_back(Ray{1,-1,8});
        output.push_back(Ray{-1,1,8});
        output.push_back(Ray{-1,-1,8});
        break;
    case SQtype::TYPE_ROOK:
        // Rooks can move along ranks and files for any distance
        output.push_back(Ray{0,1,8});
        output.push_back(Ray{1,0,8});
        output.push_back(Ray{0,-1,8});
        output.push_back(Ray{-1,0,8});
        break;
    case SQtype::TYPE_QUEEN:
        // Queens can move like bishops and rooks
        output.push_back(Ray{1,1,8});
        output.push_back(Ray{1,-1,8});
        output.push_back(Ray{-1,1,8});
        output.push_back(Ray{-1,-1,8});
        output.push_back(Ray{0,1,8});
        output.push_back(Ray{1,0,8});
        output.push_back(Ray{0,-1,8});
        output.push_back(Ray{-1,0,8});
        break;
    case SQtype::TYPE_KING:
        // Kings can move in any of the 8 directions, but for only one square
        output.push_back(Ray{1,1,1});
        output.push_back(Ray{1,-1,1});
        output.push_back(Ray{-1,1,1});
        output.push_back(Ray{-1,-1,1});
        output.push_back(Ray{0,1,1});
        output.push_back(Ray{1,0,1});
        output.push_back(Ray{0,-1,1});
        output.push_back(Ray{-1,0,1});
        break;
    default:
        break;
    }
    return output;
}