#include "board.h"
#include "data.h"
#include "bitBoards.h"
#include "posHash.h"
#include "macros.h"
#include "move.h"
#include "posHistory.h"
#include <cassert>
#include <stdlib.h>
#include <iostream>

const int castlePermMask[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};


void Board::clearPiece(const uint sq120) {
    assert(inBounds120(sq120));

    //get the info we need about the piece on this square
    unsigned piece = fullBoard[sq120];
	assert(pieceValid(piece));
	unsigned col = pieceCol[piece];

    //unhash the piece from the position key
    HASH_PIECE(piece,sq120);

    //remove from the full board
	fullBoard[sq120] = EMPTY;

    //remove from the bitboards
    const int sq64 = conv64(sq120);
    clearBit(pieceBBs[piece],sq64);
    clearBit(colourBBs[col],sq64);
    clearBit(colourBBs[BOTH],sq64);
    setBit(colourBBs[NONE],sq64);

    //remove from the piecelists
	bool found = false;
	for(std::vector<uint>::iterator pieceIter = pieceLists[piece].begin(); pieceIter < pieceLists[piece].end(); ++pieceIter) {
		if((*pieceIter) == sq120) {
			found = true;
			pieceLists[piece].erase(pieceIter);
			break;
		}
	}
	assert(found); //assert that the piece to remove was found in the piecelists
	assert(checkIntegrity()); //check that everything is still in proper order
}

void Board::addPiece(const uint sq120, const uint piece) {
    assert(inBounds120(sq120));
	assert(pieceValid(piece));

	unsigned col = pieceCol[piece];

	//hash the piece into the position key
    HASH_PIECE(piece,sq120);

	//add to the full board
	assert(fullBoard[sq120] == EMPTY);
    fullBoard[sq120] = piece;

    //add to the bitboards
    const uint sq64 = conv64(sq120);
    setBit(pieceBBs[piece],sq64);
    setBit(colourBBs[col],sq64);
    setBit(colourBBs[BOTH],sq64);
    clearBit(colourBBs[NONE],sq64);

    //add to the piecelists
	pieceLists[piece].push_back(sq120);

	assert(checkIntegrity()); //check that everything is still in proper order
}

//this is the obvious encapsulated way, however is in reality quite inefficient, here incase needed for testing purposes
//void Board::movePiece(const uint from120, const uint to120) {
//    assert(inBounds120(from120));
//    assert(inBounds120(to120));
//    unsigned piece = fullBoard[from120];
//    assert(pieceValid(piece));
//
//    clearPiece(from120);
//    addPiece(to120, piece);
//}

void Board::movePiece(const uint from120, const uint to120) {
    //currently works on assumption that the 'to' sq is empty, i.e if move is a capture,
    //the captured piece is removed (clearPiece()) before the capturing piece is moved
    assert(inBounds120(from120));
    assert(inBounds120(to120));
    assert(fullBoard[to120] == EMPTY);

    unsigned piece = fullBoard[from120];
    assert(pieceValid(piece));
    unsigned col = pieceCol[piece];

    //unhash the piece in the from sq, and hash it in on the to sq
    HASH_PIECE(piece,from120);
    HASH_PIECE(piece,to120);

    //update full board
    fullBoard[from120] = EMPTY;
    fullBoard[to120] = piece;

    //update bit boards
    const int from64 = conv64(from120);
    clearBit(pieceBBs[piece],from64);
    clearBit(colourBBs[col],from64);
    clearBit(colourBBs[BOTH],from64);
    setBit(colourBBs[NONE],from64);

    const int to64 = conv64(to120);
    setBit(pieceBBs[piece],to64);
    setBit(colourBBs[col],to64);
    setBit(colourBBs[BOTH],to64);
    clearBit(colourBBs[NONE],to64);

    //update piece lists
    bool found = false;
	for(uint index = 0; index < pieceLists[piece].size(); ++index) {
		if(pieceLists[piece][index] == from120) {
			found = true;
			pieceLists[piece][index] = to120;
			break;
		}
	}
	assert(found); //assert that the piece to remove was found in the piecelists
    assert(checkIntegrity()); //check that everything is still in proper order
}

int Board::makeMove(const Move move) { //the meat
    //returns 1 if legal, 0 if move puts the moving side in check

    assert(checkIntegrity());

    uint from64 = move.getFrom();
    uint to64 = move.getTo();

    uint from120 = conv120(from64);
    uint to120 = conv120(to64);

    PosHistory pos; //to add onto the posHistory stack
    pos.posKey = this->posKey; //this included just for clarity, aka 'current'
    pos.move = move;
    pos.fiftyMove = fiftyMove;
    pos.enPassentSq = enPassentSq;
    pos.castlePerm = castlePerm;

    //couple of special cases
    if (move.isEnPas()) { //if move is enpassent
        if (sideToMove == WHITE)
            clearPiece(to120 - 10); //remove the pawn that passed this one
        else
            clearPiece(to120 + 10);
    }
    else if (move.getCas()) { //if castling
        switch(move.getCas()) { //need to move the rook as well
            case WHITE_CASTLE_LONG:
                movePiece(A1, D1);
                break;
            case WHITE_CASTLE_SHORT:
                movePiece(H1, F1);
                break;
            case BLACK_CASTLE_LONG:
                movePiece(A8, D8);
                break;
            case BLACK_CASTLE_SHORT:
                movePiece(H8, F8);
                break;
            default: assert(0);
        }
    }

    if (enPassentSq != INVALID_SQ)
        HASH_ENPAS; //if the square was set, hash it out
    enPassentSq = INVALID_SQ; //force it off either way, will be set later if new move is a double pawn push

    HASH_CASTLING; //hash OUT the current castling perms
    //if either the rook or king moved (from) their squares, or another piece moved (to) their squares, castling permissions are affected
    //applies clear masks (defined at top of file) depending on the squares involved in the move
    castlePerm &= castlePermMask[from120];
    castlePerm &= castlePermMask[to120];
    HASH_CASTLING; //hash IN the new castling perms (may be identical)

    //now handle 'normal' captures
    uint pieceCapped = move.getCap();
    fiftyMove++; //by default, this increments

    if (pieceCapped) {
        assert(pieceValid(pieceCapped));
        fiftyMove = 0; //however resets when piece is capped
        clearPiece(to120);
    }

    if (piecePawn[fullBoard[from120]]) { //if the piece moved is a pawn
        fiftyMove = 0; //also resets
        if (sideToMove == WHITE) {
            if (to64-from64 == 16) { //if it pushed two ranks
                enPassentSq = from120+10;
                assert(ROW120(enPassentSq) == RANK_3);
            }
        }
        else { //black
            if (from64-to64 == 16) { //if it pushed two ranks
                enPassentSq = from120-10;
                assert(ROW120(enPassentSq) == RANK_6);
            }
        }

    }

    //now to do the 'actual' move
    movePiece(from120, to120);

    //promoted?
	int promPiece = move.getProm();
    if(promPiece)   {
        assert(pieceValid(promPiece) && !piecePawn[promPiece]);
        clearPiece(to120); //change the piece type
        addPiece(to120, promPiece);
    }

    //swap sides to move
    uint side = sideToMove;
    sideToMove = oppSideToMove;
    oppSideToMove = side;
    HASH_SIDE;

    posHistory.push_back(pos);
    hisPly++; ply++;
    assert(checkIntegrity());


    //check that the moving player has not left themselves in check, if so, undo everything
    uint offset = side == WHITE ? 0 : 6;
    if(isSqAttacked(conv64(pieceLists[wK+offset][0]),sideToMove))  { //sideToMove will be flipped back if/when takeMove() is called, jsut looks odd here
        takeMove();
        return 0; //move is illegal
    }

    return 1;
}

int Board::takeMove() {
    //essentially do things mostly in opposite order as makemove
    assert(checkIntegrity());

    PosHistory pos = posHistory.back();
    posHistory.pop_back();
    hisPly--; ply--;

    Move move = pos.move;

    uint t = sideToMove;
    sideToMove = oppSideToMove;
    oppSideToMove = t;

    uint to64 = move.getFrom(); //move opposite direction
    uint from64 = move.getTo();

    uint to120 = conv120(to64);
    uint from120 = conv120(from64);

    uint sideOffset = sideToMove == WHITE ? 0 : 6;
//    uint altSideOffset = sideOffset ? 0 : 6;

    movePiece(from120, to120);

    //was this a promotion?
    if (move.getProm()) {
        //clear the promoted piece, and replace with a pawn
        clearPiece(to120);
        addPiece(to120, wP+sideOffset);
    }

    //piece moved, now handling any captured piece
    //enpassent is also a capture, but not all captures are enpassents, therefore ep handled first
    if (move.isEnPas()) {
        if (sideToMove == WHITE) {
            assert(ROW120(from120-10) == RANK_5);
            addPiece(from120-10, bP);
        }
        else {
            assert(ROW120(from120+10) == RANK_4);
            addPiece(from120+10, wP);
        }
    }
    else if (move.getCap()) { //any capture otherwise, put the piece back on
        uint pieceCapped = move.getCap();
        assert(pieceValid(pieceCapped));
        fiftyMove = 0; //however resets when piece is capped
        addPiece(from120, pieceCapped);
    }

    //castling?
    if (move.getCas()) {
        switch(move.getCas()) { //need to move the rook back as well
            case WHITE_CASTLE_LONG:
                movePiece(D1, A1);
                break;
            case WHITE_CASTLE_SHORT:
                movePiece(F1, H1);
                break;
            case BLACK_CASTLE_LONG:
                movePiece(D8, A8);
                break;
            case BLACK_CASTLE_SHORT:
                movePiece(F8, H8);
                break;
            default: assert(0);
        }
    }

    posKey = pos.posKey;
    castlePerm = pos.castlePerm;
    enPassentSq = pos.enPassentSq;
    fiftyMove = pos.fiftyMove;

    assert(checkIntegrity());

    return 0;
}

