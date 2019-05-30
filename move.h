#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "data.h"
#include "enums.h"
#include "posHash.h"
#include "bitBoards.h"
#include "macros.h"
#include <iostream>
#include <bitset>

class Move {
public:
    //all assuming 64 square board
    // bits: 0-5 from sq
    //       6-11 to sq
    //       12-15 piece captured
    //       16-19 piece promoted to
    //       20-23 castling
    //       24 enpassent

    // 0000 0000 0000 0000 0000 0000 0011 1111 from
    // 0000 0000 0000 0000 0000 1111 1100 0000 to
    // 0000 0000 0000 0000 1111 0000 0000 0000 capped piece
    // 0000 0000 0000 1111 0000 0000 0000 0000 promotion piece
    // 0000 0000 1111 0000 0000 0000 0000 0000 castling permitions
    // 0000 0001 0000 0000 0000 0000 0000 0000 enpassent flag
    // 1111 1110 0000 0000 0000 0000 0000 0000 empty


    int move;
    int score;

    Move(): move(0), score(0) { };
    Move(int from, int to): move(0) { setFrom(from); setTo(to); }

    friend std::ostream& operator<<(std::ostream& os, const Move& m);

    int getFrom() const   { return (move & 0x3f); }
    int getTo() const     { return ((move >> 6) & 0x3f); }
    int getCap() const    { return ((move >> 12) & 0xf); }
    int getProm() const   { return ((move >> 16) & 0xf); }
    int getCas() const    { return ((move >> 20) & 0xf); }
    int isEnPas() const   { return ((move >> 24) & 0x1); }

    void setFrom(int sq)    { move &= 0xffffffc0; move |= sq; } //clear, then set
    void setTo(int sq)      { move &= 0xfffff03f; move |= (sq<<6); }
    void setCap(int piece)  { move &= 0xffff0fff; move |= (piece<<12); }
    void setProm(int piece) { move &= 0xfff0ffff; move |= (piece<<16); }
    void setCas(int cstl)   { move &= 0xff0fffff; move |= (cstl<<20); }

    void setEnPas(int t = 1) {
        if (t) move |= (1ULL<<24);
        else move &= 0xfeffffff;
    }

    void printMove() const {
        int f = getFrom(), r = getTo();
        std::cout << static_cast<char>(FILE64(f) + 'a') << ROW64(f) + 1;
        if (getCap() || isEnPas()) std::cout << "X";
        std::cout << static_cast<char>(FILE64(r) + 'a') << ROW64(r) + 1;

        if (getProm()) {
            switch (getProm()) {
                case 3: case 9:
                    std::cout << 'N'; break;
                case 4: case 10:
                    std::cout << 'B'; break;
                case 5: case 11:
                    std::cout << 'R'; break;
                case 6: case 12:
                    std::cout << 'Q'; break;
            }
        }
        std::cout << std::endl;
    }

    void printMoveDetailed() const {
        std::cout << std::bitset<32>(move) << std::endl;
        std::cout << "from:"<<printableSqs64[getFrom()] <<
            " to:"<<printableSqs64[getTo()] <<
            " cap:"<<pieceChars[getCap()] <<
            " prom:"<<pieceChars[getProm()] <<
            " cas:"<<getCas() <<
            " ep:"<<isEnPas() <<std::endl;
    }

};

std::ostream& operator<<(std::ostream& os, const Move& m);


//class Move {
//    Square source; //Where did piece move from
//    Square target; //And where to
//
//    MoveType moveType; //(if castling move, source/target will detail king positions, must do rook manually)
//    Piece promote; //If move was a promotion, what piece to promote to
//    Piece capture; //Stores captured piece, when move is undone, put it back onto target square
//
//    CastleRights castlingRights; //What were castle rights before move was played
//    Square enPassentSq; //Was there potential for/what was the enpassent square before this move
//    int fiftyMove; //What was the fiftyMove counter at before this move
//
//    PosHash::Key posKey; //of position before or after move?
//};

#endif // MOVE_H_INCLUDED
