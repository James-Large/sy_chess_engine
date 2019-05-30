#include "board.h"
#include "enums.h"
#include "macros.h"
#include <stack>
#include <iostream>
#include <vector>
#include "posHash.h"
#include "data.h"
#include "bitBoards.h"

Board::Board() {
    initAll();
}

void initBitMasks(); //forward def, defined below
void initConvBoards();
void initPieceAttackBBs();

void Board::initAll() {
    initConvBoards();
    initBoard();
    initPieceAttackBBs();
    initBitMasks();
    initKeys();
}

void Board::initBoard() {
    uint i = 0;

    //Fullboard
    for (i = 0; i < PADDED_BOARD_SZ; i++)
        fullBoard[i] = INVALID_PCE;
    for (i = 0; i < STD_BOARD_SZ; i++)
        fullBoard[convBoard_64To120[i]] = EMPTY;

    //Piece bitBoards
    for (i = 0; i < NUM_PIECES; i++)
        pieceBBs[i] = 0;
    for (i = 0; i < 3; i++)
        colourBBs[i] = 0;

    //Piece lists
    for (i = 0; i < NUM_PIECES; i++) {
        pieceLists[i].clear();
        pieceLists[i].reserve(10); //most likely doesn't make difference, don't know starting capacity of vectors (ArrayList in java is 12)
        //but should make sure no internal reallocation happens due to capacity being reached
        //10 because most extreme case is 2 of a piece and 8 pawns promoting to that piece, never more than 10 of single type
    }

    //General board state data
    sideToMove = oppSideToMove = NONE;
    castlePerm = NO_CASTLING;
    enPassentSq = INVALID_SQ;
    fiftyMove = 0;

    ply = 0;
    hisPly = 0;
    posHistory = std::vector<PosHistory>();

    startPosition = "";
}

void initConvBoards() {
    for (unsigned i = 0; i < 120; i++)
        convBoard_120To64[i] = INVALID_SQ;

    int sq64 = 0, sq120 = 0;
    for (int r = RANK_1; r <= RANK_8; ++r) {
        for (int f = FILE_A; f <= FILE_H; ++f) {
            sq120 = FR2SQ120(f,r);
            convBoard_64To120[sq64] = sq120;
            convBoard_120To64[sq120] = sq64++;
        }
    }
}

void initPieceAttackBBs() {
    //FOR NOW pawns are handled separately, since so many different cases to check for

    //magic numbers, magic numbers everywhere

    unsigned dir,sq64,sq120,piece,dist;

    for (piece = 0; piece < 13; piece++)
        for (sq64 = 0; sq64 < 64; sq64++)
            pieceAttackBBs[piece][sq64] = 0ULL;


    //Pawns
    //ONLY ATTACKING MOVES ARE STORED FOR PAWNS, i.e whitePawnDirs64Based = { +7, +9 }
    //NORMAL FILE PUSHES AND DOUBLE PUSHES HANDLED SEPERATELY AS SPECIAL CASE WHERE NEEDED
    //(using (for white) pawnBB << 8 for example for all single square pushes
    for (sq64 = 0; sq64 < 64; sq64++) {
        sq120 = convBoard_64To120[sq64];
        if (inBounds64( convBoard_120To64[sq120+11] ))
            pieceAttackBBs[wP][sq64] |= (1ULL << (convBoard_120To64[sq120+11]) );
        if (inBounds64( convBoard_120To64[sq120+9] ))
            pieceAttackBBs[wP][sq64] |= (1ULL << (convBoard_120To64[sq120+9]) );
        if (inBounds64( convBoard_120To64[sq120-11] ))
            pieceAttackBBs[bP][sq64] |= (1ULL << (convBoard_120To64[sq120-11]) );
        if (inBounds64( convBoard_120To64[sq120-9] ))
            pieceAttackBBs[bP][sq64] |= (1ULL << (convBoard_120To64[sq120-9]) );
    }

    //Knights
    for (sq64 = 0; sq64 < 64; sq64++) {
        sq120 = convBoard_64To120[sq64];
        for (dir = 0; dir < 8; dir++) {
            if (inBounds64( convBoard_120To64[sq120+KnDir120[dir]] )) {
                pieceAttackBBs[wN][sq64] |= (1ULL << (convBoard_120To64[sq120+KnDir120[dir]]) );
                pieceAttackBBs[bN][sq64] = pieceAttackBBs[wN][sq64];
            }
        }
    }

    //Rooks
    for (sq64 = 0; sq64 < 64; sq64++) {
        sq120 = convBoard_64To120[sq64];
        for (dir = 0; dir < 4; dir++) {
            dist = 1; //sliding piece
            while (inBounds64( convBoard_120To64[sq120+(RkDir120[dir]*dist)] )) {
                pieceAttackBBs[wR][sq64] |= ( 1ULL << convBoard_120To64[sq120+(RkDir120[dir]*dist)] );
                pieceAttackBBs[bR][sq64] = pieceAttackBBs[wR][sq64];
                dist++;
            }
        }
    }

    //Bishops
    for (sq64 = 0; sq64 < 64; sq64++) {
        sq120 = convBoard_64To120[sq64];
        for (dir = 0; dir < 4; dir++) {
            dist = 1; //sliding piece
            while (inBounds64( convBoard_120To64[sq120+(BiDir120[dir]*dist)] )) {
                pieceAttackBBs[wB][sq64] |= ( 1ULL << convBoard_120To64[sq120+(BiDir120[dir]*dist)] );
                pieceAttackBBs[bB][sq64] = pieceAttackBBs[wB][sq64];
                dist++;
            }
        }
    }

    //Queen = bishop + rook
    for (sq64 = 0; sq64 < 64; sq64++) {
        pieceAttackBBs[wQ][sq64] = pieceAttackBBs[wB][sq64] | pieceAttackBBs[wR][sq64];
        pieceAttackBBs[bQ][sq64] = pieceAttackBBs[wQ][sq64];
    }

    //King
    for (sq64 = 0; sq64 < 64; sq64++) {
        sq120 = convBoard_64To120[sq64];
        for (dir = 0; dir < 8; dir++)
            if (inBounds64( convBoard_120To64[sq120+KiDir120[dir]] )) {
                pieceAttackBBs[wK][sq64] |= (1ULL << convBoard_120To64[sq120+KiDir120[dir]] );
                pieceAttackBBs[bK][sq64] = pieceAttackBBs[wK][sq64];
            }
    }

}

void initBitMasks() {
    //Set/Clear Masks
	for(int i = 0; i < 64; i++) {
		setMask[i] = 0ULL;
		clearMask[i] = 0ULL;
	}

	for(int i = 0; i < 64; i++) {
		setMask[i] |= (1ULL << i);
		clearMask[i] = ~setMask[i];
	}


    //Rank/File masks
	bitBoard afile = 0x0101010101010101;
	bitBoard rank1 = 0xff;

	for (int i = 0; i < 8; i++) {
        rankMask[i] = rank1 << (i*8);
        fileMask[i] = afile << i;
	}

	//Blocking Masks
	//re-learn and comment this at some point when you arent lazy as fuck
	//and get rid of the magic numbers
	//For each pair of squares, stores a BB with all sqs set between them INCLUSIVE
    int dirs[] = { 1, 9, 10, 11 };
    for (int sq64 = 0; sq64 < 64; sq64++) {
        int sq120 = conv120(sq64);
        for (int dir = 0; dir < 4; dir++) {
            bitBoard bb = 1ULL << sq64;
            int t_sq120 = sq120+dirs[dir];
            while (inBounds64(conv64(t_sq120))) {
                bb |= 1ULL << conv64(t_sq120);

                blockingBBs[genSqPair(sq64, conv64(t_sq120))] = bb;
                t_sq120 += dirs[dir];
            }
        }
    }

    //Ray Attack Masks
    //For each square and each direction, stores a BB with all sqs in a line from sq+dir to the edge of the board
    for (int sq64 = 0; sq64 < 64; sq64++) {
        int sq120 = conv120(sq64);
        for (int dir = 0; dir < 8; dir++) {
            bitBoard bb = 0ULL;
            int t_sq120 = sq120+KiDir120[dir];
            while (inBounds64(conv64(t_sq120))) {
                bb |= 1ULL << conv64(t_sq120);
                t_sq120 += KiDir120[dir];
            }
            rayAttackBBs[sq64][dir] = bb;
        }
    }

	//Castling Masks
	//Masks to check for blocked squares between sourse/target squares for each type of castling (king rook squares themselves not included)
	castlingBlockMasks[WHITE_CASTLE_SHORT] = (1ULL << 5) | (1ULL << 6);
	castlingBlockMasks[WHITE_CASTLE_LONG] = (1ULL << 1) | (1ULL << 2) | (1ULL << 3);
	castlingBlockMasks[BLACK_CASTLE_SHORT] = (1ULL << 61) | (1ULL << 62);
	castlingBlockMasks[BLACK_CASTLE_LONG] = (1ULL << 57) | (1ULL << 58) | (1ULL << 59);
	castlingBlockMasks[WHITE_CASTLE_BOTH] = castlingBlockMasks[WHITE_CASTLE_SHORT] | castlingBlockMasks[WHITE_CASTLE_LONG];
	castlingBlockMasks[BLACK_CASTLE_BOTH] = castlingBlockMasks[BLACK_CASTLE_SHORT] | castlingBlockMasks[BLACK_CASTLE_LONG];
	castlingBlockMasks[ALL_CASTLING] = castlingBlockMasks[WHITE_CASTLE_BOTH] | castlingBlockMasks[BLACK_CASTLE_BOTH];

	//Masks to check for squares that the king will move through while castling, including it's own square, incase of castling through check
    castlingCheckMasks[WHITE_CASTLE_SHORT] = (1ULL << 4) | (1ULL << 5) | (1ULL << 6);
	castlingCheckMasks[WHITE_CASTLE_LONG] = (1ULL << 2) | (1ULL << 3) | (1ULL << 4);
	castlingCheckMasks[BLACK_CASTLE_SHORT] = (1ULL << 60) | (1ULL << 61) | (1ULL << 62);
	castlingCheckMasks[BLACK_CASTLE_LONG] = (1ULL << 58) | (1ULL << 59) | (1ULL << 60);
	castlingCheckMasks[WHITE_CASTLE_BOTH] = castlingCheckMasks[WHITE_CASTLE_SHORT] | castlingCheckMasks[WHITE_CASTLE_LONG];
	castlingCheckMasks[BLACK_CASTLE_BOTH] = castlingCheckMasks[BLACK_CASTLE_SHORT] | castlingCheckMasks[BLACK_CASTLE_LONG];
	castlingCheckMasks[ALL_CASTLING] = castlingCheckMasks[WHITE_CASTLE_BOTH] | castlingCheckMasks[BLACK_CASTLE_BOTH];
}
