#include <iostream>
#include <iomanip>
#include "bitBoards.h"
#include "enums.h"
#include "macros.h"
#include <cassert>

const int leastSigBitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

const int mostSigBitTable[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

bitBoard setMask[64];
bitBoard clearMask[64];
bitBoard fileMask[8];
bitBoard rankMask[8];

RayMap blockingBBs;
bitBoard rayAttackBBs[64][8];
bitBoard castlingBlockMasks[15]; //sparse array, icky but meh
bitBoard castlingCheckMasks[15]; //sparse array, icky but meh

bitBoard pieceAttackBBs[13][64];

void clearBit(bitBoard &bb, int i) {
    bb &= clearMask[i];
}
void setBit(bitBoard &bb, int i) {
    bb |= setMask[i];
}

bool testBit(const bitBoard &bb, int i) {
    return bb & (1ULL << i);
}

int countBits(bitBoard bb) {
  int r;
  for(r = 0; bb; r++, bb &= bb - 1);
  return r;
}

int ms1b(bitBoard bb) {
   bb |= bb >> 1;
   bb |= bb >> 2;
   bb |= bb >> 4;
   bb |= bb >> 8;
   bb |= bb >> 16;
   bb |= bb >> 32;
   return mostSigBitTable[(bb * 0x03f79d71b4cb0a89ULL) >> 58];
}

int ls1b(bitBoard bb) {
    bb ^= (bb - 1);
    unsigned int fold = (unsigned) ((bb & 0xffffffff) ^ (bb >> 32));
    return leastSigBitTable[(fold * 0x783a9b23) >> 26];
}

int popBit(bitBoard &bb) {
    int i = ls1b(bb);
    clearBit(bb, i);
    return i;
}

int popHighBit(bitBoard &bb) {
    int i = ms1b(bb);
    clearBit(bb, i);
    return i;
}

int popLowBit(bitBoard &bb) { //same imp. as the generic popBit, but just different name
    int i = ls1b(bb); //so it's easier to use when you SPECIFICALLY want the lowest bit popped
    clearBit(bb, i);
    return i;
}

void printBitBoard(const bitBoard &bb) {
    bitBoard shiftMe = 1ULL;
	int file,sq64;

	std::cout << "\n";
	for(int rank = RANK_8; rank >= RANK_1; --rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq64 = FR2SQ64(file,rank);
			if((shiftMe << sq64) & bb)
				std::cout << "X";
			else
				std::cout << "-";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

int genSqPair(int sq1, int sq2) {
    assert(sq1 >= 0 && sq1 < 64);
    assert(sq2 >= 0 && sq2 < 64);
    if (sq1 < sq2)
        return sq1 | (sq2 << 8);
    else
        return sq2 | (sq1 << 8);
}

int getSq1(int sqPr) {
    return sqPr & 0x3f; // 111111
}

int getSq2(int sqPr) {
    return (sqPr >> 8) & 0x3f;
}






