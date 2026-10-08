/**
 * @file Tracer.h
 * @brief Contains stuff for Analyzer.h to use for calculating piece movement
 */
#pragma once
// #include "Analyzer.h"
class Analyzer;
// == Helper Classes == //

/**
 * @brief This is just used for the attack matrix. It stores the piece type attacking this square as well as the location of the attacking piece.
 * @param piece Pointer to BoardSquare object at the location of the attacking piece
 */
struct Attacker
{
    BoardSquare* piece;
    std::uint8_t row;
    std::uint8_t col;
    Attacker();
    Attacker(BoardSquare* ptrPiece, std::uint8_t rownum, std::uint8_t colnum);
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
    int getTotalAttackers();

    

    /**
     * @brief Returns a copy of the internal attacks vector
     */
    std::vector<Attacker> getAttackVector();

    /**
     * @brief Clears the attacks vector
     */
    void clear();
};


// == Tracer Class == //
/**
 * @brief Used for calculating piece moves and attacks.
 */
class Tracer
{
private:
    friend class Analyzer;

    Analyzer* evalboard;
    int start_row;
    int start_col;
    int d_row;
    int d_col;
    int trace_range;
    bool skip_empty;

    Tracer(Analyzer* evalboard,
                int start_row, int start_col,
                int d_row, int d_col,
                int trace_range,
                bool skip_empty);

public:
    class Iterator
    {
    public:
        Attacker operator*() const;
        Iterator& operator++();

        bool operator!=(const Iterator& other) const;
    
    private:
        friend class Tracer;
        Iterator() = default;

        Iterator(Analyzer* evalboard,
                int row, int col,
                int row_step, int col_step,
                bool skip_empty);

        /**
         * @brief Constructor to use for when we want to figure out the traces based on the piece type
         */
        Iterator(Analyzer* evalboard,
                int row, int col,
                bool skip_empty);
        
        Analyzer* evalboard = nullptr;
        int row = 0;
        int col = 0;
        int row_step = 0;
        int col_step = 0;
        bool skip_empty;
        std::vector<Ray> rayQueue;
        
    };
    /**
     * @brief Creates the Iterator object that will be used for tracing
     */
    Iterator begin() const;
    /**
     * @brief Returns a default iterator. When the begin Iterator equals this one (in this case, when evalboard becomes nullptr), the loop ends
     */
    Iterator end() const;
};