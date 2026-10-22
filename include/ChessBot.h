/**
 * @mainpage The main functions to use for the Chess bot
 * @file ChessBot.h
 * @brief All the minimax stuff
 */

#pragma once
#include "ChessFunctions.h"
#include "Analyzer.h"


/**
 * @brief This will be used for the first implementation of the bot that will just use the regular minimax algorithm. I plan to implement something slightly different and more memory-efficient, but for now this is what we'll use.
 */
class TESTbot
{
private:
    // The bot will have its own analyzer object for analyzing position. Every position analyzed will be loaded into the same Analyzer object so we don't bloat the call stack.
    Analyzer m_analyzer;

    // Max depth to search. Depends on difficulty.
    int m_depth;

    // Difficulty level of the both. Determines max search depth and what heuristics it can calculate
    Difficulty m_diff;

    Eval MiniMax(const Position pos, int depth, Eval alpha, Eval beta);
public:
    /**
     * @brief Constructs a bot with the given difficulty
     */
    TESTbot(Difficulty diff);

    void SetDifficulty(Difficulty diff);
    /**
     * @brief Simply takes the current position as FEN and runs the minimax algorithm to determine the best move in that position which it will return as a string in algebraic notation. To avoid ambiguity, moves will always output with the starting square, so something like Qb6 would be represented as Qd8b6 or Qe6b6 even if there are no other queens that can move to b6.
     * @param FEN Current board position formatted in Forsyth-Edwards Notation (FEN).
     * @returns String representing the algebraic notation of the next move to play.
     */
    const std::string GetNextMove(const std::string& FEN);
};