#include "Tracer.h"
#include "Analyzer.h"
#include "ChessFunctions.h"

// Constructors

Tracer::Tracer(Analyzer* evalboard,
            int start_row, int start_col,
            int d_row, int d_col,
            int trace_range,
            bool skip_empty)
        : evalboard(evalboard),
        start_row(start_row),
        start_col(start_col),
        d_row(d_row),
        d_col(d_col),
        trace_range(trace_range),
        skip_empty(skip_empty)
        {}



// Dereferencing
Attacker Tracer::Iterator::operator*() const
{
    return Attacker(evalboard->getSquareAt(row,col),row,col);
}

// Increment
Tracer::Iterator& Tracer::Iterator::operator++()
{
    row += row_step;
    col += col_step;

    if(evalboard->getSquareAt(row,col) == nullptr)
    {
        evalboard = nullptr;
    }

    return *this;
}

// Comparison
bool Tracer::Iterator::operator!=(const Iterator& other) const
{
    return evalboard != other.evalboard;
}

Tracer::Iterator Tracer::begin() const
{
    return Iterator(evalboard, start_row, start_col, d_row, d_col);
}

Tracer::Iterator Tracer::end() const
{
    return Iterator{};
}