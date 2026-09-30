#pragma once
#include <string>
#include <array>
#ifndef CHESS_FUNCTIONS_H
#define CHESS_FUNCTIONS_H

// This is the header file for out chess bot
std::string testInputFunction(const std::string& input);

/**
 * @brief Enum used for bot difficulties
 * @note The current values are just placeholders
 */
enum class Difficulty{
    EASY,
    MEDIUM,
    HARD
};

/**
 * @brief Used in the output of coordToIndices
 */
struct BoardCoord
{
    int row;
    int col;
};

/**
 * @brief Converts a string containing a chessboard coordinate in algebraic notation (e.g. "e4") and returns the indices to be used in the board array
 * @param coord string containing a chessboard coordinate in algebraic notation
 * @return BoardCoord object where row is the row index of the board matrix and col is the column index of the board matrix
 */
BoardCoord coordToIndices(const std::string& coord);

/**
 * @brief Takes the row and column of an element in the board matrix and returns it in algebraic notation.
 */
std::string indicesToCoord(int row, int col);

#endif