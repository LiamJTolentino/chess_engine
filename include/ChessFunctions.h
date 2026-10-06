#pragma once
#include <string>
#include <array>
#include <vector>
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

    /**
     * @brief Constructor that initializes the piece value
     */
    BoardSquare(std::uint8_t initstate);
    
    bool isWhitePiece();

    /**
     * @brief True if this square is unoccupied
     * @param includeEnPassant Set to true if en passant squares should count as empty
     */
    bool isEmptySquare(bool includeEnPassant = true);

    /**
     * @brief Just returns the last 3 bits
     */
    std::uint8_t getPiece();

    std::uint8_t getState();

    /**
     * @brief Changes the piece on the square without changing the color
     * @param piece int value of the piece
     */
    void setPiece(std::uint8_t piece);

    void setState(SQstate newstate);

    void setFromFEN(char piece);

    void clearSquare();

    float getMaterialValue();

    std::string getUnicode();
    
    std::string getFEN();
};

/**
 * @brief Data structure for storing chess move info and functions to extract information. This is to make sure that we can store move information in as little space as possible to save on memory.
 * @param mvinfo 16 bit unsigned integer containing all the necessary information to make a legal move on a chess board.
 * This integer is made up of smaller numbers that tell information about a move.
 *  
 * First is a 1-bit value that is only 1 (true) if the move is a pawn promotion. If it is, the next value has to represent a piece that a pawn can promote to.
 * 
 * Second is a 3-bit value that tells what piece to set at the destination square, or which direction to castle. 000 (0) - Pawn, 001 (1) - Knight, 010 (2) - Bishop, 011 (3) - Rook, 100 (4) - Queen, 101 (5) - King, 110 (6) - Castle kingside/Short castle ("O-O"), 111 (7) - Castle queenside/Long castle ("O-O-O"). If this value is a castle, we can simply ignore the other values.
 * 
 * Remaining four numbers are 3-bit values that tell the start row, start column, destination row, and destination column respectively. Since the chess board is only 8x8, each of these only needs to have 8 distinct values, hence the 3-bits.
 */
class ChessMove
{
private:
    std::uint16_t mvinfo;
public:
    ChessMove();
    ChessMove(bool promotion, int piece, int start_row, int start_col, int dest_row, int dest_col);
    ChessMove(std::string PGN);

    bool isPiecePromotion();

    int getPieceVal();

    int getStartRow();

    int getStartCol();

    int getDestRow();

    int getDestCol();

    void setStartPos(int start_row, int start_col);

    void setDestPos(int dest_row, int dest_col);

    void setPieceVal(int piece_val);

    void setPawnPromotion(bool isPromotion);
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