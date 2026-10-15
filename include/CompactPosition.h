/**
 * @file CompactPosition.h
 * @brief Contains all the Zobrist hashing stuff and the compact position class
 */

#pragma once
#include <array>
#include "ChessFunctions.h"
class Analyzer; // From Analyzer.h

/**
 * @brief Compacted representation of a chess board. Each square is represented in 4 bits 
 */
class Position
{
private:
    std::array<std::uint64_t,8> board; // Board stored as an array of long ints where each element is a whole row
    std::uint8_t extra_info; // Contains info about castling and which side is playing
public:
    /**
     * @brief Constructs the position from FEN
     */
    Position(std::string FEN);

    Position(const Analyzer* const pAnalyzer);

    // Returns a BoardSquare object representing the piece at row,col
    BoardSquare GetSquareAt(std::uint8_t row, std::uint8_t col) const;
    
    /**
     * @brief Returns a new Position object that results from performing a move on the current position
     */
    Position transpose(ChessMove move);

    bool isWhiteToMove() const;

    // Simply returns true if there is an En Passant square in the position
    bool isEnPassantAvailable() const;

    bool CanBlackShortCastle() const;

    bool CanBlackLongCastle() const;

    bool CanWhiteShortCastle() const;

    bool CanWhiteLongCastle() const;
};

/**
 * @brief This is what we'll use to store all the analyzed positions. We use Zobrist hashing to get the key for a position and then check if it exists already. 
 */
class TranspositionTable
{

};