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
        m_depth = 3;
        break;
    
    default:
        m_depth = 3;
        break;
    }
}



Eval TESTbot::MiniMax(const Position pos, int depth, Eval alpha, Eval beta)
{
    Position newPos; // Resulting position from a move that we will evaluate
    // std::cout<< "Loading" <<"\n";
    m_analyzer.loadFromPosition(&pos);
    // std::cout<< "Checking which color is playing" <<"\n";
    // bool white_player = m_analyzer.isWhiteToMove();
    bool white_player = pos.isWhiteToMove();
    // std::cout<< "now initializing output" <<"\n";
    // Eval output(ChessMove()); // Contains the move and heuristic we output. Defaults to zero.
    Eval output{ChessMove()};
    // std::cout<< "output initialized" <<"\n";
    output.illegal = true;
    if(!m_analyzer.isPositionLegal())
    {
        // std::cout<< "Illegal Pos" <<"\n";
        output.m_Move = ChessMove(false,MV_LONG_CASTLE,0,0,0,0);
        output.illegal = true;
        // std::cout<< "Illegal Pos at depth=" << depth <<"\n";
        return output;
    }
    if(m_analyzer.isInCheck()) // If no legal moves are found after this turn, we basically guarantee this position evaluates as checkmate.
    {
        output.illegal = false;
        output.mate = (white_player?-1:1);
    }
    if(depth <= 0) {
        output.mate = 0;
        output.illegal = false;
        output.heuristic = m_analyzer.getHeuristic();
        // std::cout<< "Depth reached: " << depth <<"\n";
        return output;
    }

    std::vector<ChessMove> moveList;
    // std::cout<< "Getting moves" <<"\n";
    moveList = m_analyzer.getAllMoves();
    size_t total_moves = moveList.size();
    // std::cout<< "Found " << total_moves << " moves"<< "\n";
    Eval temp;
    for(size_t i=0; i<total_moves; i++)
    {
        // std::cout<< "Move " << i << " of " << total_moves <<"\n";
        ChessMove move = moveList.at(i);
        newPos = pos.transpose(move);
        temp.m_Move = move;
        // std::cout<< "Minimax" <<"\n";
        temp.update(MiniMax(newPos,depth-1,alpha,beta));
        temp.incrementMate();
        // std::cout<< "\t"
        // << depth << "|\t"
        // << (white_player?"White":"Black") << " to play\t"
        // << temp.m_Move.AsString(white_player) << " (" << temp.AsString() << ")"
        // << " vs "
        // << output.m_Move.AsString(white_player) << " (" << output.AsString() << ")";
        output = preferredEval(output,temp,white_player);
        // std::cout<< "\t\tBEST: " << output.m_Move.AsString(white_player) <<"\n";
        
        if(white_player)
        {
            alpha = std::max(alpha,output);
        } else {
            beta = std::min(beta,output);
        }
        if(beta < alpha) {break; }// Alpha-Beta pruning
    }
    // std::cout<< "Default return: " << output.m_Move.AsString(white_player) <<"\n";
    return output;
}


const std::string TESTbot::GetNextMove(const std::string& FEN)
{
    m_analyzer.loadFromFen(FEN);
    Position startPos = Position(&m_analyzer);
    Eval best = MiniMax(startPos, m_depth, Eval(-1),Eval(1));
    return best.m_Move.AsString(startPos.isWhiteToMove());
}