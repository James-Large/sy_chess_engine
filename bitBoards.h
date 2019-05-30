#ifndef BITBOARDS_H_INCLUDED
#define BITBOARDS_H_INCLUDED

#include "data.h"
#include <stdint.h> //uint64_t

typedef uint64_t bitBoard; //exactly 64 bits guaranteed on all systems
typedef std::unordered_map<int, bitBoard> RayMap;

extern bitBoard setMask[];
extern bitBoard clearMask[];
extern bitBoard fileMask[];
extern bitBoard rankMask[];

//Masks to check for blocked squares between sourse/target squares for each type of castling (king rook squares themselves not included)
extern bitBoard castlingBlockMasks[]; //sparse array, icky but meh
//Masks to check for squares that the king will move through while castling, incase of castling through check (almost identical to block masks, however castling long has one less square set (the b file)
extern bitBoard castlingCheckMasks[]; //sparse array, icky but meh

void clearBit(bitBoard &bb, int i); //Clear the bit at index i in bb
void setBit(bitBoard &bb, int i); //Set the bit at index i in bb
bool testBit(const bitBoard &bb, int i); //Test whether bit i in bb is set
int countBits(bitBoard bb); //Count and return the number of set bits in bb
int ms1b(bitBoard bb);
int ls1b(bitBoard bb);
int popBit(bitBoard &bb); //Find and clear the lowest-order set bit in bb, and return it's index
void printBitBoard(const bitBoard &bb); //Print the bitboard in an 8x8 fashion to cout


extern RayMap blockingBBs;
extern bitBoard rayAttackBBs[64][8];

int genSqPair(int sq1, int sq2);
int getSq1(int sqPr);
int getSq2(int sqPr);


extern bitBoard pieceAttackBBs[][64]; //for each piece [12], store a BB containing all possible target squares from all possible positions [64]
    //BBs 1-5 and 7-11 are actually the same, since the only different is white/balck which makes no difference in moves (pawns differ in vertical direction)
    //but will keep them to allow for ease of indexing, not much space is used, 6*64*64 bits duplicated = 3Kb
    //ONLY ATTACKING MOVES ARE STORED FOR PAWNS, i.e whitePawnDirs64Based = { +7, +9 }
    //NORMAL FILE PUSHES AND DOUBLE PUSHES HANDLED SEPERATELY



#endif // BITBOARDS_H_INCLUDED
