/**
 * @file ChessBot.cpp
 * @brief Implementation of the bot classes declared in ChessBot.h
 */
#include "Analyzer.h"
#include "ChessBot.h"
#include <vector>
// #include <iostream>


TESTbot::TESTbot(Difficulty diff)
    : m_diff(diff)
{
    switch (diff)
    {
    case Difficulty::EASY:
        m_depth = 2;
        break;
    
    default:
        m_depth = 3;
        break;
    }
}



Eval TESTbot::MiniMax(const Position pos, int depth, Eval alpha, Eval beta)
{
    Position newPos; // Resulting position from a move that we will evaluate
    // std::cout<< "Loading" <<std::endl;
    m_analyzer.loadFromPosition(&pos);
    bool white_player = m_analyzer.isWhiteToMove();
    Eval output; // Contains the move and heuristic we output. Defaults to zero.
    if(!m_analyzer.isPositionLegal())
    {
        // std::cout<< "Illegal Pos" <<std::endl;
        output.illegal = true;
        return output;
    }
    if(m_analyzer.isInCheck()) // If no legal moves are found after this turn, we basically guarantee this position evaluates as checkmate.
    {
        output.mate = (white_player?-1:1);
    }
    if(depth <= 0) {
        output.heuristic = m_analyzer.getHeuristic();
        return output;
    }

    std::vector<ChessMove> moveList;
    // std::cout<< "Getting moves" <<std::endl;
    moveList = m_analyzer.getAllMoves();
    size_t total_moves = moveList.size();
    Eval temp;
    for(size_t i; i<total_moves; i++)
    {
        // std::cout<< "Move " << i << " of " << total_moves <<std::endl;
        ChessMove move = moveList.at(i);
        newPos = pos.transpose(move);
        temp.m_Move = move;
        // std::cout<< "Minimax" <<std::endl;
        temp.update(MiniMax(newPos,depth-1,alpha,beta));
        output = preferredEval(output,temp,white_player);
        if(white_player)
        {
            alpha = std::max(alpha,output);
        } else {
            beta = std::min(beta,output);
        }
        if(beta < alpha) { break; }// Alpha-Beta pruning
    }

    return output;
}


const std::string TESTbot::GetNextMove(const std::string& FEN)
{
    m_analyzer.loadFromFen(FEN);
    Position startPos = Position(&m_analyzer);
    Eval best = MiniMax(startPos, m_depth, Eval(-1),Eval(1));
    return best.m_Move.AsString(startPos.isWhiteToMove());
}