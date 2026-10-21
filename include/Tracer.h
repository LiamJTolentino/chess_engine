/**
 * @file Tracer.h
 * @brief Contains stuff for Analyzer.h to use for calculating piece movement
 */
#pragma once
#include "ChessFunctions.h"
// #include <vector>
// #include "Analyzer.h"
class Analyzer;
// == Helper Classes == //

/**
 * @brief This is just used for the attack matrix. It stores the piece type attacking this square as well as the location of the attacking piece.
 * @param piece Pointer to BoardSquare object at the location of the attacking piece
 */
struct Attacker
{
    BoardSquare piece;
    std::uint8_t row;
    std::uint8_t col;
    Attacker();
    Attacker(BoardSquare piece, std::uint8_t rownum, std::uint8_t colnum);
};

/** 
 * @brief Wrapper class for everything related to the attack matrix
 * @param attacks Vector of Attacker objects representing all the pieces that are targetting this square.
 */
class AttackSquare
{
private:
    std::vector<Attacker> attacks;
public:
    // AttackSquare();

    /**
     * @brief Gets the total number of pieces targetting this square
     */
    int getTotalAttackers() const;

    /**
     * @brief Gets the total number of pieces of a given color targetting this square
     */
    int getTotalAttackers(bool whitePieces) const;

    /**
     * @brief Total white pieces attacking minus total black pieces attacking. Used in the heuristic calculations to determine if a piece is sufficiently defended.
     */
    float getAttackImbalance() const;

    std::vector<Attacker> getAttackers(bool whitePieces) const;
    

    /**
     * @brief Returns a copy of the internal attacks vector
     */
    std::vector<Attacker> getAttackVector() const;

    /**
     * @brief Clears the attacks vector
     */
    void clear();

    void addAttacker(Attacker atk);
    void addAttacker(BoardSquare piece, std::uint8_t rownum, std::uint8_t colnum);

    std::string TESTshow() const;
};


// == Tracer Class == //
/**
 * @brief Used for calculating piece moves and attacks.
 */
class Tracer
{
private:
    friend class Analyzer;

    Analyzer* pAnalyzer;
    int start_row;
    int start_col;
    int d_row = 0;
    int d_col = 0;
    int trace_range = 0;
    bool piece_mode = false;

public:
    Tracer(Analyzer* pAnalyzer,
                int start_row, int start_col,
                int d_row, int d_col,
                int trace_range);
    
    /**
     * @brief Constructor overload for when you want to iterate over all the squares the piece at start_row,start_col can move to and not just in the given direction.
     */
    Tracer(Analyzer* pAnalyzer,
                int start_row, int start_col);

    /**
     * @brief Nested Iterator class for the tracer
     * @param pAnalyzer Pointer to the Analyzer object containing the board
     * @param row Current row
     * @param col Current column
     * @param row_step Number of squares along the vertical to scan in one step
     * @param col_step Number of squares along the horizontal to scan in one step
     * @param trace_range Number of steps to scan in
     * @param rayQueue RayStack of ray objects containing the directions the piece at board[start_row][start_col] can move in
     * @param step Current step in the iteration
     * @param start_row Starting row of the square to return to
     * @param start_col Starting column of the square to return to
     * @param startSquare pointer to the start square
     */
    class Iterator
    {
    public:
        Attacker operator*() const;
        /**
         * @brief In the for loop, this is what is called at the end of the loop to move to the next iteration.
         * This is where most of the tracing logic stuff will go.
         */
        Iterator& operator++();

        bool operator!=(const Iterator& other) const;
    
    private:
        friend class Tracer;
        Iterator() = default;

        Iterator(Analyzer* pAnalyzer,
                int row, int col,
                int row_step, int col_step,
                int trace_range=1);

        /**
         * @brief Constructor to use for when we want to figure out the traces based on the piece type
         */
        Iterator(Analyzer* pAnalyzer,
                int row, int col);

        /**
         * @brief Returns the current square at board[row][col]
         */
        BoardSquare CurrentSquare() const;//

        /**
         * @brief Pops the next ray in queue and resets to the start square
         */
        void Rotate();
        
        Analyzer* pAnalyzer = nullptr;
        int start_row = 0;
        int start_col = 0;
        int row = 0;
        int col = 0;
        int step = 0;
        int row_step = 0;
        int col_step = 0;
        int trace_range = 0;
        bool xray = false;

        bool rotate_now;
        BoardSquare startSquare;
        RayStack rayQueue;
        
    };
    /**
     * @brief Creates the Iterator object that will be used for tracing
     */
    Iterator begin() const;
    /**
     * @brief Returns a default iterator. When the begin Iterator equals this one (in this case, when pAnalyzer becomes nullptr), the loop ends
     */
    Iterator end() const;
};