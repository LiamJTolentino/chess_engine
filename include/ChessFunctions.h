/**
 * @file ChessFunctions.h
 * @brief Provides data structures for storing information on a chess position
 */
#pragma once
#include <string>
#include <array>
#include <vector>
#include <cstdint>
#ifndef CHESS_FUNCTIONS_H
#define CHESS_FUNCTIONS_H

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

    INVALID_SQUARE = 8,     // 1000

    WHITE_PAWN = 9,         // 1001
    WHITE_KNIGHT = 10,      // 1010
    WHITE_BISHOP = 11,      // 1011
    WHITE_ROOK = 12,        // 1100
    WHITE_QUEEN = 13,       // 1101
    WHITE_KING = 14,        // 1110
    WHITE_EN_PASSANT = 15   // 1111
};

/**
 * @brief Same as SQstate but without piece color. Useful for when you just want the piece type
 */
enum SQtype : std::uint8_t{
    TYPE_EMPTY = 0,         // 000
    TYPE_PAWN = 1,         // 001
    TYPE_KNIGHT = 2,       // 010
    TYPE_BISHOP = 3,       // 011
    TYPE_ROOK = 4,         // 100
    TYPE_QUEEN = 5,        // 101
    TYPE_KING = 6,         // 110
    TYPE_EN_PASSANT = 7,   // 111
};

/**
 * @brief Used for ChessMove
 */
enum MVpiece : std::uint8_t{
    MV_PAWN = 0,            // 000
    MV_KNIGHT = 1,          // 001
    MV_BISHOP = 2,          // 010
    MV_ROOK = 3,            // 011
    MV_QUEEN = 4,           // 100
    MV_KING = 5,            // 101
    MV_SHORT_CASTLE = 6,    // 110
    MV_LONG_CASTLE = 7      // 111
};

/**
 * @brief Used by the Tracer class to indicate directions to scan in
 * @param row_step Number of squares along the vertical to scan in one step
 * @param col_step Number of squares along the horizontal to scan in one step
 * @param range Number of steps to scan for
 */
struct Ray
{
    int row_step;
    int col_step;
    int range;
};

/**
 * @brief Used by the Tracer class to keep track of which Rays need to be cast. Previously we were using vectors, but in this way, it allocates to stack instead of heap which should be faster.
 */
class RayStack
{
private:
    std::array<Ray, 8> ray_list{}; // Array to hold the Ray objects. Limited to 8.
    std::size_t top = 0; // Number of rays stored
public:
    // Return true if successful
    bool Push(const Ray& value);//
    // Decrements top. Return true if successful.
    bool Pop();
    // Returns a read-only pointer to the top of the stack
    const Ray* const Peek() const;
    // Returns the ray at the top of the stack
    Ray Top() const;

    std::size_t size() const;
    bool empty() const;
};

/**
 * @brief Used for retrieving information about a square on the board. 
 * @file BoardSquare.cpp
 * @param state 8-bit int defined by the SQstate enum
 * @todo In every implementation that returns or passes a pointer to a BoardSquare object, have it use the value of the BoardSquare itself. This is because pointers/addresses are 64 bits while a single BoardSquare object is only 8 bits, so we're actually wasting memory.
 */
class BoardSquare
{
private:
    std::uint8_t state;
public:
    /**
     * @brief Constructor that initializes as empty square
     */
    BoardSquare();//

    /**
     * @brief Constructor that initializes the piece value
     */
    BoardSquare(std::uint8_t initstate);//

    /**
     * @brief Constructore that initializes the piece with the given color
     */
    BoardSquare(bool white, SQtype piecetype);

    BoardSquare(bool white, MVpiece movedpiece);
    
    bool isWhitePiece() const;//

    bool isValid() const;

    /**
     * @brief Return true if this piece can capture other (other must be either empty, en passant, or opposite color piece)
     */
    bool canCapture(const BoardSquare& other) const;//

    /**
     * @brief True if this square is unoccupied
     * @param includeEnPassant Set to true if en passant squares should count as empty
     */
    bool isEmptySquare(bool includeEnPassant = true) const;//

    // Returns true if state = 8. THis is to indicate that a square is invalid.
    bool isOutOfBounds() const;//

    bool isPiece(SQtype piecetype) const;//

    bool isState(SQstate piecestate) const;//

    /**
     * @brief Just returns the last 3 bits
     */
    SQtype getPiece() const;//

    SQstate getState() const;//

    MVpiece getMVpiece() const;

    /**
     * @brief Changes the piece on the square without changing the color
     * @param piece int value of the piece
     */
    void setPiece(std::uint8_t piece);//

    void setState(SQstate newstate);//

    void setFromFEN(char piece);//

    void clearSquare();//

    float getMaterialValue() const;//

    /**
     * @brief If this square has a chess piece, this will return the directions in which the piece can move. If it is an empty square or en passant, it returns an empty stack.
     */
    RayStack getPieceRays() const;//

    std::string getUnicode() const;//
    
    std::string getFEN() const;//

    bool operator==(const BoardSquare& other) const;//
};

class Attacker;
/**
 * @brief Data structure for storing chess move info and functions to extract information. This is to make sure that we can store move information in as little space as possible to save on memory.
 * @todo Like with the BoardSquare class, the ChessMove class takes up less memory than a pointer to its object. We need to make sure we're only passing by value.
 * @file ChessMove.cpp
 * @param mvinfo 16 bit unsigned integer containing all the necessary information to make a legal move on a chess board.
 * This integer is made up of smaller numbers that tell information about a move.
 *  
 * First is a 1-bit value that is only 1 (true) if the move is a pawn promotion (or en passant). If it is, the next value has to represent a piece that a pawn can promote to.
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
    ChessMove(); // Constructs an illegal move
    ChessMove(MVpiece mvinfo);
    ChessMove(bool promotion, MVpiece piece, int start_row, int start_col, int dest_row, int dest_col);//
    ChessMove(bool promotion, Attacker attacker, int target_row, int target_col);//
    ChessMove(std::string PGN);

    /**
     * @brief Returns the first bit which determines if the specified move is a pawn promotion or en passant. This is to avoid ambiguity with moves like d8=Q vs Qd8 which both end up with a queen on the d8 square.
     * En passant is not ambiguous, but we indicate it with this bit anyway because it involves three changes as the captured pawn is not on the destination square.
     */
    bool isPawnMove() const;//

    bool isLegal() const;//


    MVpiece getPieceVal() const;//

    // ChessMove stores the piece type in a weird way, so we have this to return it to the normal type. 
    SQtype getPieceType() const;//

    bool isPieceType(MVpiece piece) const;

    bool isCastle() const;

    int getStartRow() const;//

    int getStartCol() const;//

    int getDestRow() const;//

    int getDestCol() const;//

    BoardCoord getStart() const;//

    BoardCoord getDest() const;//

    void setStartPos(int start_row, int start_col);//

    void setDestPos(int dest_row, int dest_col);//

    void setPieceVal(std::uint8_t piece_val);//

    void setPawnMove(bool isPromotion);//

    std::string AsString(bool whitemove) const;//

    std::string DBstring() const;//
};

/**
 * @brief Converts a string containing a chessboard coordinate in algebraic notation (e.g. "e4") and returns the indices to be used in the board array
 * @param coord string containing a chessboard coordinate in algebraic notation
 * @return BoardCoord object where row is the row index of the board matrix and col is the column index of the board matrix
 */
BoardCoord coordToIndices(const std::string& coord);//

/**
 * @brief Takes the row and column of an element in the board matrix and returns it in algebraic notation.
 */
std::string indicesToCoord(int row, int col);//


#endif