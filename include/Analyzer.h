#pragma once
#include <string>
#include <array>
#include "ChessFunctions.h"
#include <map>




// == Analyzer Class == //
/**
 * @brief Class used for calculating stuff related to chess positions
 * 
 * @param board 8x8 float array representing all the pieces on the board. Pieces are indicated by their material values with 0.0 being an empty square, 1.0=Pawn, 3.0=Knight, 3.1=Bishop, 5.0=Rook, 9.0=Queen, 20.0=King. Sign indicates the color, so positive values are white pieces while negative values are black pieces.
 */
class Analyzer
{
private:
    // std::array<std::array<int, 8>, 8> board; 
    float board[8][8];
    bool whiteToPlay;
    Difficulty diff;
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

    bool isWhiteToMove();

    /**
     * @brief Returns true if the current position is legal. This means that:
     * 1. Both kings are on the board
     * 2. Current player cannot capture opponent king
     * 3. There are no promoted pawns in their last rank
     */
    bool isPositionLegal();

    /**
     * @brief True if in the current position, the current player's king is being attacked by an enemy piece
     */
    bool isInCheck();

    void transpose(std::string move);

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
};
