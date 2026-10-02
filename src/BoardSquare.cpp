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