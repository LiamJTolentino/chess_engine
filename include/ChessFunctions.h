#pragma once
#include <string>
#include <array>
#include <cstdint>
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
 * @brief Enum used for the 15 possible states a square on the board can be in
 * @note Only the last 4 bits matter. The most significant bit tells what color that piece belongs to, if there is no piece. 0 is an empty square. The last 3 bits tell what piece is in that square. EN_PASSANT is just used to indicate that a pawn can capture in that square, and also makes it so we can use all 8 possible values with those 3 bits.
 */
enum SQstate : std::uint8_t{
    EMPTY_SQUARE = 0,       // 0000
    BLACK_PAWN = 1,         // 0001
    BLACK_KNIGHT = 2,       // 0010
    BLACK_BISHOP = 3,       // 0011
    BLACK_ROOK = 4,         // 0100
    BLACK_QUEEN = 5,        // 0101
    BLACK_KING = 6,         // 0110
    BLACK_EN_PASSANT = 7,   // 0111
    
    WHITE_PAWN = 9,         // 1001
    WHITE_KNIGHT = 10,      // 1010
    WHITE_BISHOP = 11,      // 1011
    WHITE_ROOK = 12,        // 1100
    WHITE_QUEEN = 13,       // 1101
    WHITE_KING = 14,        // 1110
    WHITE_EN_PASSANT = 15   // 1111
};

/**
 * @brief Used for retrieving information about a square on the board. 
 * 
 * @param state 8-bit int defined by the SQstate enum
 */
class BoardSquare
{
private:
    std::uint8_t state;
public:
    /**
     * @brief Constructor that initializes as empty square
     */
    BoardSquare();
    
    bool isWhitePiece();

    /**
     * @brief Changes the piece on the square without changing the color
     * @param piece int value of the piece
     */
    void setPiece(std::uint8_t piece);

    void setState(SQstate newstate);

    void clearSquare();

    float getMaterialValue();

    std::string getUnicode();
    
    std::string getFEN();
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