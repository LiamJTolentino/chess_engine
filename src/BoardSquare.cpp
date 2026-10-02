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

BoardSquare::BoardSquare()
    : state(0)
{
}

bool BoardSquare::isWhitePiece()
{
    return state >> 3; // We just need to bitshift to the right by 3 to get the piece color
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