#include "Analyzer.h"
#include "ChessFunctions.h"
#include "Tracer.h"
#include "CompactPosition.h"
#include <cctype>
#include <vector>
#include <utility>
#include <cmath>
// #include <fstream>
// #include <iostream>
// #include <cstdint>

Analyzer::Analyzer()
    : board {
        {BoardSquare(4),BoardSquare(2),BoardSquare(3),BoardSquare(5),BoardSquare(6),BoardSquare(3),BoardSquare(2),BoardSquare(4)},
        {BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1),BoardSquare(1)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0),BoardSquare(0)},
        {BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9),BoardSquare(9)},
        {BoardSquare(12),BoardSquare(10),BoardSquare(11),BoardSquare(13),BoardSquare(14),BoardSquare(11),BoardSquare(10),BoardSquare(12)}

    },
    whiteToPlay(true),
    diff(Difficulty::EASY)
{
    calculateAttackMatrix();
}

BoardSquare* Analyzer::getSquareAt(int row, int col)
{
    if (row > 7 || row < 0 || col > 7 || col < 0) { return nullptr;} // Out of range

    return &board[row][col];
}

BoardSquare Analyzer::SquareAt(int row,int col) const
{
    if (row > 7 || row < 0 || col > 7 || col < 0) // Out of Range
    {
        return BoardSquare(SQstate::INVALID_SQUARE);
    }
    return board[row][col];
}

// Prepare for some really shitty ass code
void Analyzer::calculateAttackMatrix()
{
    // Reset matrix
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            attack_matrix[row][col].clear();
        }
    }

    // Iterate through board
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            attacksFromSquare(row,col);
        }
    }
}

// TODO: Implement pawn moves
void Analyzer::attacksFromSquare(int row, int col)
{
    BoardSquare currentSquare = SquareAt(row,col);
    int row_step;
    int step_range;
    if(currentSquare.isEmptySquare()){ return; } // Empty squares obviously can't attack
    // Probably the dumbest way to implement pawn moves
    if(currentSquare.isPiece(SQtype::TYPE_PAWN))
    {
        row_step = (currentSquare.isWhitePiece()?-1:1); // Pawn move direction
        step_range = (2*row + 5*row_step == 7 ? 2:1); // Really stupid and convoluted way of determinining if a pawn is on its starting rank, but it works somehow.
        // std::cout<< "\tAL> " << pSquare->getUnicode() << indicesToCoord(row,col) << " " << row_step << " " << step_range <<std::endl;
        for(Attacker target : tracerRange(row,col,row_step,0,step_range))
        {
            if(target.piece.isEmptySquare()) // Pawns can only move forward into empty squares
            {
                attack_matrix[target.row][target.col].addAttacker(currentSquare,row,col);
            }
        }
        // Diagonal pawn moves can be handled by the regular tracerRange
        for(Attacker target : tracerRange(row,col))
        {
            // std::cout<< "\tAL:" << pSquare->getUnicode() << indicesToCoord(row,col) << " attacks " << target.piece.getUnicode() << indicesToCoord(target.row,target.col)  <<std::endl;
            if(row != target.row && col != target.col
                && !target.piece.isEmptySquare(false) // Since pawns can En Passant, we don't count En Passant squares as empty
                )
            {
                attack_matrix[target.row][target.col].addAttacker(currentSquare,row,col);
            }
        }
        return;
    }
    for(Attacker target : tracerRange(row,col))
    {
        if(currentSquare.getState() != target.piece.getState()
            // && !target.piece.isEmptySquare() 
            // && pSquare->canCapture(*target.piece)
            )
        {
            attack_matrix[target.row][target.col].addAttacker(currentSquare,row,col);
            // if(pSquare->isPiece(SQtype::TYPE_ROOK))
            // {
            //     std::cout<< "R" << indicesToCoord(row,col) << "x" << target.piece.getUnicode() << indicesToCoord(target.row,target.col) <<std::endl;
            // }
        }
    }
}

// TODO: Redo the king check stuff using the attack matrix instead
bool Analyzer::isPositionLegal() const
{
    bool bking = false;
    bool wking = false;
    int numkings = 0;

    BoardCoord bkloc;
    BoardCoord wkloc;
    // == First we check if both kings are on the board and there are only 2
    for (int row=0; row<8; row++)
    {
        for (int col=0; col<8; col++)
        {
            // 
            if(board[row][col].getState() == SQstate::BLACK_KING) { bking = true; bkloc.row = row; bkloc.col = col; numkings++;}
            if(board[row][col].getState() == SQstate::WHITE_KING) { wking = true; wkloc.row = row; wkloc.col = col; numkings++;}
        }
    }
    if (!(bking && wking) || numkings != 2) { return false; }

    // == Then we check if the current player can capture the opponent king with a piece
    // First make sure the kings aren't touching
    
    if(std::abs(bkloc.row - wkloc.row) <= 1 && std::abs(bkloc.col - wkloc.col) <= 1)
    { return false; }
    BoardCoord& ekloc = (whiteToPlay ? bkloc : wkloc); // Which one is the enemy king?

    // Looking for attacks on the enemy king
    AttackSquare kingatk = attack_matrix[ekloc.row][ekloc.col];
    if(kingatk.getTotalAttackers(whiteToPlay) > 0){
        return false;
    }

    // Then we see if there are any unpromoted pawns in their final rank
    for (int col=0; col<8; col++){
        if(board[0][col].getState() == SQstate::WHITE_PAWN || board[7][col].getState() == SQstate::BLACK_PAWN)
        {
            return false;
        }
    }

    return true;
}

bool Analyzer::isInCheck() const
{
    BoardSquare currentSquare;
    for (int row=0; row<8; row++)
    {
        for (int col=0; col<8; col++)
        {
            currentSquare = board[row][col];
            // Find friendly king
            if(currentSquare.isPiece(SQtype::TYPE_KING)
            && currentSquare.isWhitePiece() == whiteToPlay)
            {
                // Return true if there are more than one attackers on the king
                return attack_matrix[row][col].getTotalAttackers(!whiteToPlay) > 0;
            }
        }
    }
    return false;
}





std::vector<ChessMove> Analyzer::getAllMoves() const
{
    std::vector<ChessMove> output;
    std::vector<Attacker> attackers;
    BoardSquare dummy(whiteToPlay,SQtype::TYPE_KING); // dummy piece to check if a piece can be captured

    // Variables for creating the moves
    MVpiece endpiece;
    for (int row=0; row<8; row++)
    {
        for (int col=0; col<8; col++)
        {
            if(!dummy.canCapture(board[row][col])) { continue; } // Skip friendly pieces
            
            attackers = attack_matrix[row][col].getAttackers(whiteToPlay); // Gets all the friendly pieces that can move into this square
            for(Attacker atk : attackers)
            {
                if(atk.piece.isPiece(SQtype::TYPE_PAWN))
                {
                    if(row==0 || row==7)// If the pawn ends up in a promotion rank, we queue all the possible promotions
                    {
                        output.push_back(ChessMove(true,MV_QUEEN,atk.row,atk.col,row,col));
                        output.push_back(ChessMove(true,MV_ROOK,atk.row,atk.col,row,col));
                        output.push_back(ChessMove(true,MV_BISHOP,atk.row,atk.col,row,col));
                        output.push_back(ChessMove(true,MV_KNIGHT,atk.row,atk.col,row,col));
                        continue;
                    }
                    // En passants are already handled by Position class, so we can just add pawn as normal
                    output.push_back(ChessMove(true,atk,row,col));
                    continue; // Just continue the loop
                }
                //Normal piece moves
                output.push_back(ChessMove(false,atk,row,col));
            }
        }
    }
    return output;
}





Tracer Analyzer::tracerRange(int from_row, int from_col, int d_row, int d_col,int range)
{
    return Tracer(this, from_row, from_col,
                d_row, d_col,range);
}

Tracer Analyzer::tracerRange(int from_row, int from_col)
{
    return Tracer(this,from_row,from_col);
}

std::string Analyzer::TESTprintMoves()
{
    std::string output;
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            BoardSquare* pSquare = getSquareAt(row,col);
            if(pSquare && !pSquare->isEmptySquare())
            {
                AttackSquare& atk = attack_matrix[row][col];
                int numDefenders = atk.getTotalAttackers(pSquare->isWhitePiece());
                int numAttackers = atk.getTotalAttackers(!pSquare->isWhitePiece());
                output += pSquare->getUnicode() + " on " + indicesToCoord(row,col) + " is ";
                if(numDefenders==0 && numAttackers > 0)
                {
                    output += "hanging";
                }else if(numDefenders<numAttackers){
                    output += "underdefended";
                }else{
                    output += "Okay";
                }
                
                // output += "\t" + std::to_string(atk.getAttackImbalance());
                output += "\n\t" + atk.TESTshow();
                output += "\n";
            }
            // if (!getSquareAt(row,col)->isEmptySquare())
            // {    
            //     output += getSquareAt(row,col)->getUnicode();
            //     output += " at " + indicesToCoord(row,col);
            //     output += " defends:\n";
            //     for(Attacker atk : tracerRange(row,col))
            //     {
            //         if(getSquareAt(row,col) != atk.piece
            //             && !atk.piece.isEmptySquare() && !getSquareAt(row,col)->canCapture(*atk.piece))
            //         {
            //             output += "\t";
            //             output += atk.piece.getUnicode();
            //             output += " at ";
            //             output += indicesToCoord(atk.row,atk.col);
            //             output += "\n";
            //         }
            //     }
            //     output += "\n";
            // }
        }
    }
    return output;
}


std::vector<Attacker> Analyzer::traceFrom(int from_row, int from_col, int d_row, int d_col, int range, bool includeEmpty,bool stopAtPiece)
{
    std::vector<Attacker> output;
    int i = 0;
    std::uint8_t current_row = from_row + d_row;
    std::uint8_t current_col = from_col + d_col;
    BoardSquare currentSquare = SquareAt(current_row,current_col);
    while(currentSquare.isValid() && i < range)
    {
        if(includeEmpty || !currentSquare.isEmptySquare())
        {
            output.push_back(Attacker(currentSquare,current_row,current_col));
            if(stopAtPiece && !currentSquare.isEmptySquare()){ break; }
        }
        i++;
        current_row += d_row;
        current_col += d_col;
        currentSquare = SquareAt(current_row,current_col);
    }

    return output;
}


// === Loading Positions == //

void Analyzer::loadFromPosition(const Position* const pPos)
{
    whiteShortCastle = pPos->CanWhiteShortCastle();
    whiteLongCastle = pPos->CanWhiteLongCastle();
    blackShortCastle = pPos->CanBlackShortCastle();
    blackLongCastle = pPos->CanBlackLongCastle();

    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            board[row][col] = pPos->GetSquareAt(row,col);
        }
    }

    calculateAttackMatrix();
}


void Analyzer::loadFromFen(const std::string& FEN)
{
    // First set everything to 0
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            // board[row][col] = 0.0;
            board[row][col].clearSquare();
        }
    }
    whiteShortCastle = false;
    whiteLongCastle = false;
    blackShortCastle = false;
    blackLongCastle = false;
    // Iterate over each character
    int charIndex = 0;
    int FENlength = FEN.length();
    int row = 0;
    int col = 0;

    // First the board
    while (row < 8 && charIndex < FENlength)
    {
        char currentChar = FEN[charIndex];

        // Slash means next rank and space means we're done with the board
        if (currentChar == '/' || currentChar == ' ')
        {
            row++;
            col = 0;
            charIndex++;
            continue;
        }

        // Number means empty squares so we skip that many
        if (currentChar >= '0' && currentChar <= '9')
        {
            col += currentChar - '0';
            charIndex++;
            continue;
        }
        
        // Pieces
        // board[row][col] = FENmap[currentChar];
        board[row][col].setFromFEN(currentChar);

        col++;
        charIndex++;
    }
    int en_passant_index = -1;
    // Now the other stuff
    while (charIndex < FENlength && en_passant_index<0)
    {
        char currentChar = FEN[charIndex];
        switch (currentChar)
        {
        case 'w':
            whiteToPlay = true;
            break;
        case 'b':
            whiteToPlay = false;
            break;
        case 'K':
            whiteShortCastle = true;
            break;
        case 'Q':
            whiteLongCastle = true;
            break;
        case 'k':
            blackShortCastle = true;
            break;
        case 'q':
            blackLongCastle = true;
            break;
        case ' ':
            break;
        case '-':
            break;
        default:
            en_passant_index = charIndex;
            break;
        }
        charIndex++;
    }

    if (en_passant_index>=0)
    {
        std::string EPcoord{FEN[en_passant_index],FEN[en_passant_index+1]};
        BoardCoord EP = coordToIndices(EPcoord);
        board[EP.row][EP.col] = BoardSquare(!whiteToPlay,SQtype::TYPE_EN_PASSANT);
    }

    calculateAttackMatrix();
}

std::string Analyzer::boardAsString()
{
    std::string output = "================\n";
    for (int row=0; row< 8; row++)
    {
        output += "|";
        for (int col=0; col<8; col++)
        {
            // output += unicodemap[board[row][col]];
            output += board[row][col].getUnicode();
            output += "|";
        }
        output += "\n================\n";
    }
    output += (whiteToPlay ? "White" : "Black");
    output += " to move";
    return output;
}

// == Booleans == //

bool Analyzer::isWhiteToMove() const
{
    return whiteToPlay;
}

bool Analyzer::CanBlackShortCastle() const
{
    return blackShortCastle;
}

bool Analyzer::CanBlackLongCastle() const
{
    return blackLongCastle;
}

bool Analyzer::CanWhiteShortCastle() const
{
    return whiteShortCastle;
}

bool Analyzer::CanWhiteLongCastle() const
{
    return whiteLongCastle;
}


// == Heuristics == //

float Analyzer::getMaterialDifference()
{
    float total = 0.0;
    for (int row=0; row< 8; row++)
    {
        for (int col=0; col<8; col++)
        {
            // total += board[row][col];
            total += board[row][col].getMaterialValue();
        }
    }
    return total;
}


// == Attack Square Stuff == //

Attacker::Attacker()
    : piece(BoardSquare(0)),
    row(0),
    col(0)
{}

Attacker::Attacker(BoardSquare piece, std::uint8_t rownum, std::uint8_t colnum)
    : piece(piece),
    row(rownum),
    col(colnum)
{
}

int AttackSquare::getTotalAttackers() const
{
    return attacks.size();
}

int AttackSquare::getTotalAttackers(bool whitePieces) const
{
    int total = 0;
    for(const Attacker& atk : attacks)
    {
        total += (atk.piece.isWhitePiece() == whitePieces ? 1 : 0);
    }
    return total;
}

float AttackSquare::getAttackImbalance() const
{
    float total = 0.0;
    for(const Attacker& atk : attacks)
    {
        total += (atk.piece.isWhitePiece() ? 1.0 : -1.0);
    }
    return total;
}

std::vector<Attacker> AttackSquare::getAttackers(bool whitePieces) const
{
    std::vector<Attacker> output;
    for(const Attacker atk : attacks)
    {
        if(atk.piece.isWhitePiece() == whitePieces)
        {
            output.push_back(atk);
        }
    }
    return output;
}


void AttackSquare::addAttacker(BoardSquare piece, std::uint8_t rownum, std::uint8_t colnum)
{
    Attacker to_add = Attacker(piece,rownum,colnum);
    attacks.push_back(to_add);
}

void AttackSquare::addAttacker(Attacker atk)
{
    attacks.push_back(atk);
}

void AttackSquare::clear()
{
    attacks.clear();
}

std::string AttackSquare::TESTshow() const
{
    std::string output = "";
    for(const Attacker& atk : attacks)
    {
        output += atk.piece.getUnicode() + indicesToCoord(atk.row,atk.col);
        output += "\t";
    }
    return output;
}