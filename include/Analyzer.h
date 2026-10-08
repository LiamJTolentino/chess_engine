/**
 * @file Analyzer.h
 * @brief Provides functionality for loading and calculating chess positions
 */
#pragma once
#include <string>
#include <array>
#include <vector>
#include "ChessFunctions.h"
#include "Tracer.h"
#include <map>


// == Analyzer Class == //
/**
 * @brief Class used for calculating stuff related to chess positions
 * 
 * @param board 8x8 BoardSquare array representing all the pieces on the board. 
 * @param whiteToPlay True if in the currently loaded position white is to play the next move
 * @param diff Difficulty enum used to adjust the difficulty of the chess bot by enabling or disabling certain calculations and setting depth of the minimax search.
 */
class Analyzer
{
private:
    // std::array<std::array<int, 8>, 8> board; 
    BoardSquare board[8][8];
    bool whiteToPlay;
    Difficulty diff;
    AttackSquare attack_matrix[8][8];
    bool whiteShortCastle;
    bool whiteLongCastle;
    bool blackShortCastle;
    bool blackLongCastle;

    

    /**
     * @brief For each square on the board, we want to know what pieces are able to move into that square given the current position. For that, we have the attack_matrix which contains an AttackSquare object at each corresponding index which will be useful for finding legal moves, calculating tactics, etc.
     * @note Only call this ONCE after a position has been loaded. 
     */
    void calculateAttackMatrix();

    // Tracer stuff
    Tracer tracerRange(int from_row, int from_col, int d_row, int d_col);
    friend class Tracer;
protected:
    friend class Tracer::Iterator;
    /**
     * @brief Returns a pointer to the BoardSquare object stored at the given coordinate
     */
    BoardSquare* getSquareAt(int row, int col);
public:
    /**
     * @brief Construct a new Analyzer from the starting position
     */
    Analyzer();

    /**
     * @brief Construct a new Analyzer using the given Forsyth-Edwards Notation (FEN)
     * @param FEN String containing the FEN of the position
     */
    Analyzer(std::string& FEN);

    /**
     * @brief Reads the FEN and sets up all the necessary variables
     */
    void loadFromFen(std::string& FEN);

    std::string getFEN();

    std::string boardAsString();

    bool isWhiteToMove();

    

    /**
     * @brief Returns true if the current position is legal. This means that:
     * 1. Both kings are on the board
     * 2. Current player cannot capture opponent king
     * 3. There are no promoted pawns in their last rank
     * @note TODO: Redo the logic for this using attack_matrix once that has been implemented
     */
    bool isPositionLegal();

    /**
     * @brief True if in the current position, the current player's king is being attacked by an enemy piece
     */
    bool isInCheck();

    /**
     * @brief Used for move calculations.
     * @param from_row Row to start tracing from
     * @param from_col Column to start tracing from
     * @param d_row Number of squares to add to the rank/row in a single step
     * @param d_col Number of squares to add to the file/column in a single step
     * @param range Number of steps to count before stopping
     * @param includeEmpty False by default. Set to true if emtpy squares and en passants should be included.
     * @param stopAtPiece False by default. Set to true if trace should stop at the first piece it finds. This essentially returns a vector of legal moves in that direction.
     * @return Vector of all Attacker objects detected in that direction. Attacker objects contain a pointer to the piece at their location as well as their row and column to help find them in the board matrix.
     */
    std::vector<Attacker> traceFrom(int from_row, int from_col, int d_row,int d_col, int range, bool includeEmpty=false, bool stopAtPiece=false);

    /**
     * @brief Uses traceFrom to get a vector of all possible squares that a piece at a given square can move to
     * @param include_defense True if you want to include the squares occupied by pieces of the same color as the specified piece. This is useful for calculating the attack matrix to see if a piece has enough defenders.
     */
    std::vector<Attacker> getMovesFromSquare(int sourcerow, int sourcecol, bool include_defense=true);

    void transpose(std::string move);

// == Everything below is related to heuristic calculations == //

    /**
     * @brief Uses a bunch of other calculations to determine the overall heuristic of the position.
     * @note TODO: Figure out what to do with checks and checkmate and stuff.
     */
    float getHeuristic();

    /**
     * @brief Returns the material difference of the current position by adding up all the numbers in the array except for -20 and 20 as these are the kings.
     * @return float Material value of white added with material value of black (because black pieces are negative.) 
     * @note Returns as float because we might implement something later on that modifies the values of individual pieces like passed pawns.
     */
    float getMaterialDifference();

    /**
     * @brief Used for the heuristic calculation. Uses the attack_matrix
     */
    float getHangingScore();
};


