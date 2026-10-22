/**
 * @file Eval.cpp
 * @brief Implementation of the Eval class, used for keeping track of moves and their associated scores
 */

#include "ChessFunctions.h"
#include "Analyzer.h"



Eval::Eval(const ChessMove move)
    : m_Move(move)
{}

Eval::Eval(const int mate)
    : mate(mate)
{}

void Eval::incrementMate()
{
    if(mate>0) {++mate;}
    if(mate<0) {--mate;}
}

void Eval::update(const Eval& other)
{
    illegal = other.illegal;
    mate = other.mate;
    heuristic = other.heuristic;
}

bool Eval::operator<(const Eval& other) const
{
    if(illegal)
    {
        return true;// Illegal moves are always the lowest, no matter what
    }
    if(other.illegal)
    {
        return false;
    }

    // == Both have mate == //
    if((mate<0 && other.mate<0) // If both evaluate a win for black, black will always choose the faster win
    || (mate>0 && other.mate>0)) // Also, if both evaluate a loss for black, black will always choose the longer checkmate
    {
        return mate > other.mate; // Either way, we choose the greater value instead of the lesser.
    }
    if((mate<0 && other.mate>0)
    || (mate<0 && other.mate>0))
    {
        return mate < other.mate; // Opposite signs mean we choose the negative one every time
    }

    // == One of them is mate = //
    if((mate==0 && other.mate!=0)
    || (mate!=0 && other.mate==0))
    {
        return mate < other.mate; // Always the one that favors black
    }

    // == Heuristics == //
    return heuristic < other.heuristic;
}

std::string Eval::AsString() const
{
    if(illegal)
    {
        return "Illegal";
    }
    if(mate != 0)
    {
        return "Mate in " + std::to_string(mate>>1);
    }
    return "" + std::to_string(static_cast<int>(heuristic));
    // return "Fuck man, idk how to print floats";
}

const Eval& preferredEval(const Eval& a, const Eval& b, bool white_player)
{
    if(a.illegal && !b.illegal)
    {
        return b;
    }
    if(!a.illegal && b.illegal)
    {
        return a;
    }
    if(white_player)//Maximizing player
    {
        return (a < b) ? b : a;
    } else {        // Minimizing player (black)
        return (b < a) ? b : a;
    }
}