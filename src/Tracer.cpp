#include "Tracer.h"
#include "Analyzer.h"
#include "ChessFunctions.h"

// == Tracer == //


Tracer::Tracer(Analyzer* pAnalyzer,
            int start_row, int start_col,
            int d_row, int d_col,
            int trace_range)
        : pAnalyzer(pAnalyzer),
        start_row(start_row),
        start_col(start_col),
        d_row(d_row),
        d_col(d_col),
        trace_range(trace_range)
        {}


/**
 * @todo Make it return for different overloads of Iterator based on certain conditions
 */
Tracer::Iterator Tracer::begin() const
{
    return Iterator(pAnalyzer, start_row, start_col, d_row, d_col);
}

Tracer::Iterator Tracer::end() const
{
    return Iterator{};
}

// == Iterator == //

// Constructors

Tracer::Iterator::Iterator(Analyzer* pAnalyzer,
                int row, int col,
                int row_step, int col_step,
                int trace_range)
        : pAnalyzer(pAnalyzer),
          row(row),
          col(col),
          row_step(row_step),
          col_step(col_step),
          trace_range(trace_range)
{
    if(pAnalyzer != nullptr && pAnalyzer->getSquareAt(row,col) == nullptr)
    {
        this->pAnalyzer = nullptr;
    }
    start_row = row;
    start_col = col;
    startSquare = pAnalyzer->getSquareAt(start_row,start_col);
}

// TODO: Clean up this logic it's a damn mess
Tracer::Iterator::Iterator(Analyzer* pAnalyzer,
                int row, int col)
        : pAnalyzer(pAnalyzer),
          row(row),
          col(col)
{
    if(pAnalyzer != nullptr && pAnalyzer->getSquareAt(row,col) == nullptr)
    {
        this->pAnalyzer = nullptr;
    }
    BoardSquare* current_square = pAnalyzer->getSquareAt(row,col);
    
    if(!current_square->isEmptySquare())
    {
        rayQueue = current_square->getPieceRays();
        Ray firstray = rayQueue.back();
        rayQueue.pop_back();

        row_step = firstray.row_step;
        col_step = firstray.col_step;
        trace_range = firstray.range;
    } else {
        this->pAnalyzer = nullptr; 
    }
    start_row = row;
    start_col = col;
    startSquare = pAnalyzer->getSquareAt(start_row,start_col);
}


// Dereferencing
Attacker Tracer::Iterator::operator*() const
{
    return Attacker(pAnalyzer->getSquareAt(row,col),row,col);
}

// Increment
Tracer::Iterator& Tracer::Iterator::operator++()
{
    row += row_step;
    col += col_step;
    step += 1;

    if(CurrentSquare() == nullptr
        || step >= trace_range
        || !startSquare->canCapture(*(CurrentSquare())))
    {
        step = 0;
        while(!rayQueue.empty()) // Find the next valid ray
        {
            Ray nextRay = rayQueue.back();
            rayQueue.pop_back();
            row_step = nextRay.row_step;
            col_step = nextRay.col_step;
            row = start_row + row_step;
            col = start_col + col_step;
            trace_range = nextRay.range;
            if(CurrentSquare()
                && startSquare->canCapture(*(CurrentSquare())))
            {
                return *this;
            }
        }
        pAnalyzer = nullptr;
    }

    return *this;
}

// Comparison
bool Tracer::Iterator::operator!=(const Iterator& other) const
{
    return pAnalyzer != other.pAnalyzer;
}

BoardSquare* Tracer::Iterator::CurrentSquare()
{
    if(pAnalyzer == nullptr){ return nullptr; }
    return pAnalyzer->getSquareAt(row,col);
}