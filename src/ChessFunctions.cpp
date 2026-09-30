#include "ChessFunctions.h"

BoardCoord coordToIndices(const std::string& coord)
{
    if(coord.length() != 2){ // Coord must be exactly 2 characters
        std::__throw_invalid_argument("coord must be in algebraic notation");
    }
    int rank = coord[1] - '0';
    char file = coord[0];
    std::string files = "abcdefgh";



    BoardCoord output;
    output.row = 8 - rank;
    output.col = files.find(file);
    return output;
}