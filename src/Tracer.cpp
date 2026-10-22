#include "Tracer.h"
#include "Analyzer.h"
#include "ChessFunctions.h"
// #include <iostream>
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

Tracer::Tracer(Analyzer* pAnalyzer,
            int start_row, int start_col)
        : pAnalyzer(pAnalyzer),
        start_row(start_row),
        start_col(start_col),
        piece_mode(true)
        {}

/**
 * @todo Make it return for different overloads of Iterator based on certain conditions
 */
Tracer::Iterator Tracer::begin() const
{
    if(piece_mode)
    {
        return Iterator(pAnalyzer, start_row, start_col);
    }
    return Iterator(pAnalyzer, start_row, start_col, d_row, d_col, trace_range);
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
          trace_range(trace_range),
          rotate_now(false)
{
    if(pAnalyzer != nullptr && !pAnalyzer->SquareAt(row,col).isValid())
    {
        this->pAnalyzer = nullptr;
    }
    start_row = row;
    start_col = col;
    startSquare = pAnalyzer->SquareAt(start_row,start_col);
    // if(startSquare.isPiece(TYPE_PAWN))
    // {
    // std::cout<< startSquare.getUnicode() << " started on " << indicesToCoord(start_row,start_col) << std::endl;
    // }
}

// TODO: Clean up this logic it's a damn mess
Tracer::Iterator::Iterator(Analyzer* pAnalyzer,
                int row, int col)
        : pAnalyzer(pAnalyzer),
          row(row),
          col(col),
          rotate_now(false)
{
    if(pAnalyzer != nullptr && !pAnalyzer->SquareAt(row,col).isValid())
    {
        this->pAnalyzer = nullptr;
    }
    BoardSquare current_square = pAnalyzer->SquareAt(row,col);
    
    if(!current_square.isEmptySquare())
    {
        rayQueue = current_square.getPieceRays();
        Ray firstray = rayQueue.Top();
        // rayQueue.Pop();
        rotate_now = true;

        row_step = firstray.row_step;
        col_step = firstray.col_step;
        trace_range = firstray.range;
    } else {
        this->pAnalyzer = nullptr; 
    }
    start_row = row;
    start_col = col;
    startSquare = pAnalyzer->SquareAt(start_row,start_col);
    // if(startSquare.isPiece(TYPE_BISHOP))
    // {
    //     std::cout<< startSquare.getUnicode() << " started on " << indicesToCoord(start_row,start_col) << std::endl;
    // }
}


// Dereferencing
Attacker Tracer::Iterator::operator*() const
{
    // if(startSquare.isPiece(TYPE_BISHOP)){
    //     std::cout<< "__Returned: " << CurrentSquare().getUnicode()
    //     << indicesToCoord(row,col) 
    //     << "\tStep:\t" << step << "/" << trace_range
    //     <<std::endl;
    // }
    return Attacker(pAnalyzer->SquareAt(row,col),row,col);
    // return Attacker(CurrentSquare(),row,col);
}

// Increment
Tracer::Iterator& Tracer::Iterator::operator++()
{
    // if(startSquare.isPiece(TYPE_BISHOP))
    // {
    //     std::cout << "++" <<std::endl;
    //     std::cout<< "\trotate_now = " << rotate_now <<std::endl;
    // }
    if(rotate_now){ 
        Rotate(); 
        if(rayQueue.empty()) { 
            // std::cout<< "DJFSLSJDFLJSFDLKJ" <<std::endl;
            return *this;
        }
        // std::cout<< "\trotate_now = " << rotate_now <<std::endl;
    }
    row += row_step;
    col += col_step;
    step += 1;
    // if(startSquare.isPiece(TYPE_PAWN))
    // {
    // std::cout<< "\t" << startSquare.getUnicode() << " takes " << indicesToCoord(row,col) << std::endl;
    // std::cout<< "\t\trayQueue size: " << rayQueue.size() << std::endl;
    // }

    // if(CurrentSquare()->isPiece(SQtype::TYPE_ROOK))
    // {
    //     throw std::runtime_error("Hi");
    //     std::cout<< indicesToCoord(row,col)
    //     << " rayQueue: " << std::to_string(rayQueue.size())
    //     <<std::endl;
    // }
    // std::string pp = (CurrentSquare()).getUnicode();

    // if(startSquare.isPiece(TYPE_BISHOP))
    // {
    //     std::cout<< indicesToCoord(start_row,start_col) << " atk:\t"
    //     << (!CurrentSquare().isValid()?" ":CurrentSquare().getUnicode())
    //     << indicesToCoord(row,col)
    //     << " rayQueue: " << std::to_string(rayQueue.size()) << "|\t"
    //     << step << " of " << trace_range << "\t"
    //     << row_step << "," << col_step
    //     <<std::endl;
    // }
    
    if(!CurrentSquare().isValid()
        || step > trace_range
        // || !CurrentSquare()->isEmptySquare()
        // || !startSquare->canCapture(*(CurrentSquare()))
        )
    {
        // std::cout<<"\tOUTTABOUNDS"<<std::endl;
        while(!rayQueue.empty()) // If the piece still has more traces
        {
            // std::cout<<"\tRotated"<<std::endl;
            Rotate();
            row += row_step;
            col += col_step;
            step += 1;
            if(CurrentSquare().isValid())
            {
                // std::cout<<"\tBUMPED2"<<std::endl;
                // std::cout<<"\tReturned"<< CurrentSquare()->getUnicode() <<std::endl;
                // if(startSquare!=nullptr && startSquare->isPiece(TYPE_QUEEN)){std::cout<<"==Returned=="<<std::endl;} 
                if(!CurrentSquare().isEmptySquare()){ rotate_now = true; }
                return *this;
            }
            // std::cout<<"\tOUTTABOUNDS AGAIN"<<std::endl;
        }
        
        pAnalyzer = nullptr;
        // if(startSquare!=nullptr && startSquare->isPiece(TYPE_QUEEN)){std::cout<<"==Returned null=="<<std::endl;} 
        return *this;
    }
    if(!CurrentSquare().isEmptySquare())
    {
        // std::cout<<"\tBUMPED"<<std::endl;
        if(rotate_now) // At this point in the code, rotate_now can only be true if Rotate failed
        {
            pAnalyzer = nullptr;
            // if(startSquare!=nullptr && startSquare->isPiece(TYPE_QUEEN)){std::cout<<"==Returned null=="<<std::endl;} 
            return *this;
        }
        // std::cout<<"\tRotated1"<<std::endl;
        // Rotate();
        rotate_now = true;
        // std::cout<<"\tReturned1"<< CurrentSquare()->getUnicode() <<std::endl;
        // return *this;
    }
    // std::cout<<"\tReturned2"<< CurrentSquare()->getUnicode() <<std::endl;
    // if(startSquare!=nullptr && startSquare->isPiece(TYPE_QUEEN)){std::cout<<"==Returned=="<<std::endl;} 
    return *this;
}

// Comparison
bool Tracer::Iterator::operator!=(const Iterator& other) const
{
    return pAnalyzer != other.pAnalyzer;
}

BoardSquare Tracer::Iterator::CurrentSquare() const
{
    if(pAnalyzer == nullptr){ return BoardSquare(SQstate::INVALID_SQUARE); }
    return pAnalyzer->SquareAt(row,col);
}

void Tracer::Iterator::Rotate()
{
    if(rayQueue.empty()){ 
        // std::cout<< "\t====DEATH" <<std::endl;
        pAnalyzer = nullptr;
        return; 
    }
    Ray nextRay = rayQueue.Top();
    rayQueue.Pop();
    step = 0;
    row_step = nextRay.row_step;
    col_step = nextRay.col_step;
    row = start_row;
    col = start_col;
    trace_range = nextRay.range;
    rotate_now = false;
    // if(CurrentSquare().isPiece(TYPE_BISHOP))
    // if(startSquare.isPiece(TYPE_BISHOP))
    // {
    //     std::cout<< "\t==Rotated" <<std::endl;
    // // std::cout<< CurrentSquare()->getUnicode() << indicesToCoord(start_row,start_col) << std::endl;
    //     std::cout<< "\t==rayQueue size: " << rayQueue.size() << std::endl;
    //     std::cout<< "\t==\tNow scanning in "
    //     << row_step << "," << col_step << std::endl;
    // }
    // return true;
}