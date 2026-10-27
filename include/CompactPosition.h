/**
 * @file CompactPosition.h
 * @brief Contains all the Zobrist hashing stuff and the compact position class
 */

#pragma once
#include <array>
#include "ChessFunctions.h"
class Analyzer; // From Analyzer.h

enum class POS_TYPE{
    EMPTY,
    STARTING
};

/**
 * @brief Compacted representation of a chess board with 264 bits or 33 bytes in memory for each object. Each square is represented in 4 bits, so with 8 squares per row, we represent each row with 32 bits. Another 8 bits are used to store information about the current player as well as castling rights.
 * @param board Array of 8 unsigned long integers that each represent a row/rank on the board. 
 * @param extra_info 8-bit integer containing information about which side is playing and castling rights.
 * As for now, only the last 5 bits matter, so we will dicuss in terms of bits from the right.
 * The 5th bit from the right tells if white is to play in the current position. The following two bits tell which sides white can castle and the last two bits are which sides black can castle.
 * 
 * Take for example the FEN rnb1kbnr/pp1q1ppp/2pp4/8/3QPB2/2N5/PPP2PPP/1R2KBNR b Kkq - 3 6, specifically at the "b Kkq". 
 * b means it is black's turn to move ([0]0000). 
 * Capital K means white can short castle/kingside castle (0[1]000). 
 * The lack of a capital Q means white cannot castle queenside (in this case, because the rook on a1 already moved) (01[0]00).
 * Lowercase k means black can short castle/kingside castle (010[1]0).
 * Finally, lowercase q means black can long castle/queenside castle (0101[1])
 * And so the value of extra_info for that position is 01011 or 13 in decimal.
 */
class Position
{
private:
    std::array<std::uint32_t,8> board; // Board stored as an array of long ints where each element is a whole row
    std::uint8_t extra_info; // Contains info about castling and which side is playing


    // Sets the square at row,col to be the value of the given BoardSquare. Use bitwise operations.
    void SetSquare(int row, int col, BoardSquare square);//

    // Places a piece of the current player's color at row,col
    void PlacePiece(int row, int col, SQtype piecetype);

    // Sets the square at row,col to be an empty square
    void ClearSquare(int row, int col);

    void WhiteToMove(bool whitemove);//

    void WhiteShortCastle(bool castle);//

    void WhiteLongCastle(bool castle);//

    void BlackShortCastle(bool castle);//

    void BlackLongCastle(bool castle);//

    void removeLongCastle();

    void removeShortCastle();
public:
    /**
     * @brief Default constructor. Initializes position with starting position.
     */
    Position();//
    Position(POS_TYPE type);
    /**
     * @brief Constructs the position from FEN
     */
    Position(std::string FEN);

    /**
     * @brief Constructs the position from the currently loaded position in the provided Analyzer object
     * @param pAnalyzer Read-only pointer to an Analyzer object that has the position loaded.
     */
    Position(const Analyzer* const pAnalyzer);//

    // Returns a BoardSquare object representing the piece at row,col
    BoardSquare GetSquareAt(int row, int col) const;//
    
    /**
     * @brief Returns a new Position object that results from performing a move on the current position. Returns a blank board if it cannot legally perform the move. The Analyzer will count this as an illegal position.
     */
    Position transpose(const ChessMove move) const;

    bool isWhiteToMove() const;//

    bool isEmpty() const;

    // True if in the current position, the current player has short castle rights and can perform short castle
    bool CanShortCastle() const;//

    // True if in the current position, the current player has long castle rights and can perform long castle
    bool CanLongCastle() const;

    // Simply returns true if there is an En Passant square in the position
    bool isEnPassantAvailable() const;//

    bool CanBlackShortCastle() const;//

    bool CanBlackLongCastle() const;//

    bool CanWhiteShortCastle() const;//

    bool CanWhiteLongCastle() const;//
};

/**
 * @brief This is what we'll use to store all the analyzed positions. We use Zobrist hashing to get the key for a position and then check if it exists already. 
 */
class TranspositionTable
{

};