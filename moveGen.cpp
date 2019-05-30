#include "board.h"
#include "enums.h"
#include "macros.h"
#include <vector>
#include "data.h"
#include "bitBoards.h"
#include <cassert>
#include <iostream>

typedef unsigned uint;

uint Board::isSqAttacked(uint sq, uint attackingSide) const {

//    general idea: if a piece on sq x could attack sq y, the same piece on sq y could attack sq x

//    for each possible attacking piece
//        pretend that this piece is on the square in question (sq)
//        get the relevant BB from pieceAttackBBs
//        get it's intersection with the BB of ACTUAL locations of this piecetype
//        for each bit set on this bb
//            popBit
//            timesAttkd++

//    fine for non sliding pieces (pawns slightly special though)
//    sliding pieces tricky, what if blocking pieces


    uint timesAttkd = 0;
    bitBoard att = 0ULL;

    //Pawns
    //special case since direction of movement is important
    //suppose attackingSide = white
    //pretend *BLACK* pawn is on square in question, x
    //get [bP][x] attack BB, bits set will be x-9, x-11 (assuming on-board)
    //if white pawn on either (or both) of these squares, then x is attacked by white
    if (attackingSide==WHITE) {
        att = pieceAttackBBs[bP][sq] & pieceBBs[wP];
        while (att) {
            popBit(att);
            timesAttkd++;
        }
    }
    else {
        att = pieceAttackBBs[wP][sq] & pieceBBs[bP];
        while (att) {
            popBit(att);
            timesAttkd++;
        }
    }

    //King / Knight (non-sliding pieces)
    uint endPiece = (attackingSide==WHITE?wN:bN);
    for (uint piece = (attackingSide==WHITE?wK:bK) ; piece <= endPiece ; piece++) {
        att = pieceAttackBBs[piece][sq] & pieceBBs[piece];
        while (att) {
            popBit(att);
            timesAttkd++;
        }
    }

    //Bishop Rook Queen - need to check if a piece could reach that sq (as before), but also if there is anything blocking it
    int b = 0;
    bitBoard los = 0ULL;
    endPiece = (attackingSide==WHITE?wQ:bQ);
    for (uint piece = (attackingSide==WHITE?wB:bB) ; piece <= endPiece ; piece++) {
        att = pieceAttackBBs[piece][sq] & pieceBBs[piece];
        while (att) {
            b = popBit(att);
            los = (1ULL << sq) | (1ULL << b);
            if (los == ((colourBBs[BOTH] | 1ULL << sq) & blockingBBs[genSqPair(sq,b)]))
                timesAttkd++;
        }
    }

    return timesAttkd;
}

void Board::addQuietMove(uint from, uint to) {
    moveList.push_back(Move(from, to));
}

void Board::addCaptureMove(uint from, uint to, uint piece) {
    Move move(from,to);
    move.setCap(piece);
    moveList.push_back(move);
}

void Board::addSpecialMove(uint from, uint to, uint piece, uint prom, uint cas, uint enPas){
    //a general catch-all method, for stuff like promotions, promotions+captures, enPas etc
    Move move(from,to);
    move.setCap(piece);
    move.setProm(prom);
    move.setCas(cas);
    move.setEnPas(enPas);
    moveList.push_back(move);
}

void Board::generateMoves(uint side) {
    moveList.clear();

    generatePawnMoves(side);
    generateNonSlidingMoves(side);
    generateSlidingMoves(side);
    generateCastlingMoves(side);
}

bool Board::isCastleThroughCheck(uint casType) const { //returns true if any square along castlling route is attacked
	bitBoard castleBB = castlingCheckMasks[casType];
	uint attackingSide = casType & WHITE_CASTLE_BOTH ? BLACK : WHITE;
	while (castleBB)
		if (isSqAttacked(popBit(castleBB), attackingSide))
			return true;
	return false;
}


void Board::generateCastlingMoves(uint side) {
    if (side == WHITE) {
		if (castlePerm & WHITE_CASTLE_BOTH) {
			if ((castlePerm & WHITE_CASTLE_SHORT)&& !(castlingBlockMasks[WHITE_CASTLE_SHORT] & colourBBs[BOTH]) && !isCastleThroughCheck(WHITE_CASTLE_SHORT))//allowed to castle short, nothing in the way, and not castling through check
				addSpecialMove(convBoard_120To64[E1], convBoard_120To64[G1], 0, 0, WHITE_CASTLE_SHORT, 0);
			if ((castlePerm & WHITE_CASTLE_LONG) && !(castlingBlockMasks[WHITE_CASTLE_LONG] & colourBBs[BOTH]) && !isCastleThroughCheck(WHITE_CASTLE_LONG))
				addSpecialMove(convBoard_120To64[E1], convBoard_120To64[C1], 0, 0, WHITE_CASTLE_LONG, 0);
		}
	}
	else {
		if (castlePerm & BLACK_CASTLE_BOTH) {
			if ((castlePerm & BLACK_CASTLE_SHORT) && !(castlingBlockMasks[BLACK_CASTLE_SHORT] & colourBBs[BOTH]) && !isCastleThroughCheck(BLACK_CASTLE_SHORT))
				addSpecialMove(convBoard_120To64[E8], convBoard_120To64[G8], 0, 0, BLACK_CASTLE_SHORT, 0);
			if ((castlePerm & BLACK_CASTLE_LONG) && !(castlingBlockMasks[BLACK_CASTLE_LONG] & colourBBs[BOTH]) && !isCastleThroughCheck(BLACK_CASTLE_LONG))
				addSpecialMove(convBoard_120To64[E8], convBoard_120To64[C8], 0, 0, BLACK_CASTLE_LONG, 0);
		}
	}
}

void Board::generatePawnMoves(uint side){
    uint sq, capsq;
    uint oppSide = side == WHITE ? BLACK : WHITE;
    uint sideOffset = side == WHITE ? 0 : 6;
    uint dir = side == WHITE ?  8 : -8;
    uint pieceType = wP + sideOffset;

    for (std::vector<uint>::const_iterator piece = pieceLists[pieceType].begin();
            piece < pieceLists[pieceType].end(); piece++) {
        sq = convBoard_120To64[*piece];

        //Pushes
        if (testBit(colourBBs[NONE],sq+dir)) { //if sq immediately in front is empty
            if (ROW64(sq) == (side==WHITE ? RANK_7:RANK_2)) {//if about to promote
                addSpecialMove(sq, sq+dir, 0, wN+sideOffset, 0, 0);
                addSpecialMove(sq, sq+dir, 0, wB+sideOffset, 0, 0);
                addSpecialMove(sq, sq+dir, 0, wR+sideOffset, 0, 0);
                addSpecialMove(sq, sq+dir, 0, wQ+sideOffset, 0, 0);
            }
            else {
                addQuietMove(sq, sq+dir);
                if (ROW64(sq) == (side==WHITE ? RANK_2:RANK_7)) //if on starting square
                    if (testBit(colourBBs[NONE],sq+dir+dir)) //and two squares forward is clear
                        addQuietMove(sq, sq+dir+dir);
            }
        }

        //Captures
        bitBoard captureAttackBB = pieceAttackBBs[pieceType][sq] & colourBBs[oppSide];
        if (captureAttackBB) {
            if (ROW64(sq) == (side==WHITE ? RANK_7:RANK_2)) {//if about to promote as well
                while (captureAttackBB) {
                    capsq = popBit(captureAttackBB);
                    addSpecialMove(sq, capsq, fullBoard[convBoard_64To120[capsq]], wN+sideOffset, 0, 0);
                    addSpecialMove(sq, capsq, fullBoard[convBoard_64To120[capsq]], wB+sideOffset, 0, 0);
                    addSpecialMove(sq, capsq, fullBoard[convBoard_64To120[capsq]], wR+sideOffset, 0, 0);
                    addSpecialMove(sq, capsq, fullBoard[convBoard_64To120[capsq]], wQ+sideOffset, 0, 0);
                }
            }
            else {
                while (captureAttackBB) {
                    capsq = popBit(captureAttackBB);
                    addCaptureMove(sq, capsq, fullBoard[convBoard_64To120[capsq]]);
                }
            }
        }

        //Enpassent
        if (enPassentSq != INVALID_SQ)
            if (pieceAttackBBs[pieceType][sq] & (1ULL << convBoard_120To64[enPassentSq]))
                addSpecialMove(sq, convBoard_120To64[enPassentSq], 0, 0, 0, 1);
    }
}

void Board::generateNonSlidingMoves(uint side){
    uint oppSide = side == WHITE ? BLACK : WHITE;
    uint sideOffset = side == WHITE ? 0 : 6; //white pieces start at 0 in enum, black at 6
    uint pieceType, sq;
    std::vector<uint>::const_iterator piece;
    bitBoard quietAttackBB,captureAttackBB;

    for (pieceType = wK+sideOffset; pieceType <= wN+sideOffset; pieceType++) {
        for (piece = pieceLists[pieceType].begin(); piece < pieceLists[pieceType].end(); piece++) {
            sq = convBoard_120To64[*piece];

            quietAttackBB = pieceAttackBBs[pieceType][sq] & colourBBs[NONE];
            while (quietAttackBB) {
                addQuietMove(sq, popBit(quietAttackBB));
            }

            captureAttackBB = pieceAttackBBs[pieceType][sq] & colourBBs[oppSide];
            while (captureAttackBB) {
                uint capsq = popBit(captureAttackBB);
                addCaptureMove(sq, capsq, fullBoard[convBoard_64To120[capsq]]);
            }
        }
    }
}

void Board::generateSlidingMoves(uint side) {
    uint oppSide = side == WHITE ? BLACK : WHITE;
    uint sideOffset = side == WHITE ? 0 : 6; //white pieces start at 0 in enum, black at 6
    uint pieceType, sq;
    std::vector<uint>::const_iterator piece;

    uint startIndices[3] = { 4, 0, 0 }; //where in king directions (aka 'all' directions)
    //do the relevent directions for rook, bishop, queen start
	uint dir, closestPieceSq;
	bitBoard rayAttackBB, blockingPieces, emptySqsAttacked;

    //Sliding (rook/bishop/queen) holy shit this code if awful
    for (pieceType = wB+sideOffset; pieceType <= wQ+sideOffset; pieceType++) {
        for (piece = pieceLists[pieceType].begin(); piece < pieceLists[pieceType].end(); piece++) {
            sq = convBoard_120To64[*piece];

            int ind = pieceType - wB - sideOffset;
            for (dir = startIndices[ind]; dir < startIndices[ind] + numDirs[pieceType]; dir++) {
                rayAttackBB = rayAttackBBs[sq][dir];
                blockingPieces = rayAttackBB & colourBBs[BOTH];

                if (blockingPieces) {
                    closestPieceSq = KiDir64[dir] > 0 ? ls1b(blockingPieces) : ms1b(blockingPieces);
                    emptySqsAttacked = blockingBBs[genSqPair(sq,closestPieceSq)];
                    clearBit(emptySqsAttacked, closestPieceSq); //ignore the blocking piece sq, dealt with later
                    clearBit(emptySqsAttacked, sq); //ignore own square

                    while (emptySqsAttacked)
                        addQuietMove(sq, popBit(emptySqsAttacked));

                    if (testBit(colourBBs[oppSide], closestPieceSq)) //enemies piece
                        addCaptureMove(sq, closestPieceSq, fullBoard[convBoard_64To120[closestPieceSq]]);
                    //else own piece, do nothing
                }
                else //whole ray is empty
                    while (rayAttackBB)
                        addQuietMove(sq, popBit(rayAttackBB));
            }
        }
    }
}
