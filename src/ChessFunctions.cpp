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

std::string indicesToCoord(int row,int col)
{
    std::string files = "abcdefgh";
    std::string output = "";
    output += files[col];
    output += std::to_string(8-row);
    return output;
}


// == RayStack stuff == //

bool RayStack::Push(const Ray& value)
{
    if(top >= 8) { return false; }
    ray_list[top++] = value;
    return true;
}

bool RayStack::Pop()
{
    if(top <= 0) { return false; }
    --top;
    return true;
}

const Ray* const RayStack::Peek() const
{
    if(top<1 || top >8) { return nullptr; }
    return &ray_list.at(top-1);
}

Ray RayStack::Top() const
{
    return ray_list.at(top-1);
}

std::size_t RayStack::size() const
{
    return top;
}

bool RayStack::empty() const
{
    return top <= 0;
}