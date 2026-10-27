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
#include "CompactPosition.h"
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

    /**
     * @brief Only run this in calculateAttackMatrix
     */
    void attacksFromSquare(int row, int col);

    // Tracer stuff
    Tracer tracerRange(int from_row, int from_col, int d_row, int d_col, int range);
    /**
     * @brief Given the piece at board[from_row,from_col] return a Tracer object that iterates through all the squares that the piece can move to.
     */
    Tracer tracerRange(int from_row, int from_col);
    friend class Tracer;
protected:
    friend class Tracer::Iterator;
    /**
     * @brief Returns a pointer to the BoardSquare object stored at the given coordinate
     * @todo Redo this to return a BoardSquare object instead of a pointer. BoardSquares only have 8 bits as opposed to the 64 bits of a pointer, so this will be a lot more efficient and will be safer. Instead of nullptr, out of bounds coordinates will return the value 8 which is an invalid piece.
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
    void loadFromFen(const std::string& FEN);

    /**
     * @brief Loads the provided compact position object into this analyzer.
     * @param pPos Read-only pointer to a Position object. Use pPos->GetSquareAt(row,col) to get the piece.
     */
    void loadFromPosition(const Position* const pPos);



    // Simply returns the 8-bit object at board[row][col]
    BoardSquare SquareAt(int row, int col) const;

    std::string getFEN();

    std::string boardAsString();//

    /**
     * @brief Parses a move written in Standard Algebraic Notation (SAN) and returns the position that results from the move. Returns an invalid position if the resulting position is illegal.
     * @note This is a temporary solution as it changes the stored position.
     */
    Position parseMove(const std::string& SAN);

    bool isWhiteToMove() const;//

    bool CanBlackShortCastle() const;//

    bool CanBlackLongCastle() const;//

    bool CanWhiteShortCastle() const;//

    bool CanWhiteLongCastle() const;//

    bool ShortCastleAvailable() const;

    bool LongCastleAvailable() const;
    

    /**
     * @brief Returns true if the current position is legal. This means that:
     * 1. Both kings are on the board
     * 2. Current player cannot capture opponent king
     * 3. There are no promoted pawns in their last rank
     */
    bool isPositionLegal() const;//

    /**
     * @brief True if in the current position, the current player's king is being attacked by an enemy piece
     */
    bool isInCheck() const;//

    /**
     * @brief Returns all the possible moves that can be made in the current position.
     * @todo Implement with priority queue instead so that important moves are calculated first.
     */
    std::vector<ChessMove> getAllMoves() const;

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

    //DELETE LATER
    std::string TESTprintMoves();

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
    float getHeuristic() const;

    /**
     * @brief Returns the material difference of the current position by adding up all the numbers in the array except for -20 and 20 as these are the kings.
     * @return float Material value of white added with material value of black (because black pieces are negative.) 
     * @note Returns as float because we might implement something later on that modifies the values of individual pieces like passed pawns.
     */
    float getMaterialDifference() const;//

    /**
     * @brief Used for the heuristic calculation. 
     * Uses the attack_matrix to detect pieces that are underdefended.
     */
    float getHangingScore() const;

    /**
     * @brief Used for the heuristic calculation. 
     * Detects strong chess structures like pawn structures, fianchettos, connected rooks, knight and bishop outposts, Queen+bishop/Queen+rook batteries, etc. The idea is to reward the bot for reaching positions with these basic chess principles while also encouraging it to dismantle the opponent's position.
     */
    float getStructureScore() const;

    /**
     * @brief Used for the heuristic calculation early into the game. 
     * The idea is to detect Chess opening principles like control of the center, knights and bishops out of the back rank, etc.
     * In this way, we program the bot to follow basic opening principles rather than hardcoding book moves like with Stockfish.
     */
    float getDevelopmentScore() const;

    /**
     * @brief Used for the heuristic calculation.
     * Uses the attack_matrix to calculate how much of the empty squares on the board are controlled by each side. Highly strategic squares like the four central squares (e4, d4, e5, d5) and long diagonals will be weighted higher.
     */
    float getTerritorialScore() const;
};



/**
 * @brief This is what is actually output by the minimax and is used to compare moves by the heuristics of their resulting positions. The important thing to note for now is that any Eval that doesn't result in a legal position will always be preferred.
 */
struct Eval
{
    ChessMove m_Move;
    float heuristic = 0.0;
    int mate = 0; // Number of half-moves until checkmate. Positive if mate is in favor of white (i.e. black is checkmated), negative for the other way. Values closer to zero are favored.
    bool illegal = false;
    Eval() = default;
    Eval(const ChessMove move);
    Eval(const int mate);
    // Eval(float heuristic);

    /**
     * @brief If we intialized an Eval with a move and want to update its score, we copy over the attributes from other.
     */
    void update(const Eval& other);

    /**
     * @brief Override of the less-than operator. Think of it as returning true if black would choose the current Eval over the other
     */
    bool operator<(const Eval& other) const;

    void incrementMate();

    std::string AsString() const;
};

/**
 * @brief Returns the min or max eval depending on the specified side. This avoids needing to have separate conditions for the minimizing and maximizing player
 */
const Eval& preferredEval(const Eval& a, const Eval& b, bool white_player);