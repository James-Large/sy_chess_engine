#include "board.h"
#include "enums.h"
#include "macros.h"
#include "data.h"
#include "bitBoards.h"
#include <iostream>
#include <iomanip>
#include "posHash.h"
#include <stdlib.h>

//#define NDEBUG
#include <cassert>

//#define dassert(e)       ((e) ? (void)0 : dassertFailEXIT(#e, __FILE__, __LINE__))
#define dassert(e)      do { \
                            if (!(e)) { \
                                dassertFail(#e, __FILE__, __LINE__); \
                                return false; \
                            } \
                        } while (0)

void Board::dassertFailEXIT(const char* test, const char* file, int line) {
    std::cerr << "Detailed assertion failed (" << test << "), file " << file << ", line "<< line << std::endl;
    printFullDebug();
    exit(0);
}

void Board::dassertFail(const char* test, const char* file, int line) {
    std::cerr << "Detailed assertion failed (" << test << "), file " << file << ", line "<< line << std::endl;
    printFullDebug();
}

bool Board::loadFENPosition(std::string FENstring) {
    initBoard(); //completely resets everything in board

    startPosition = FENstring;

    //now to rebuild it from the FEN
    std::string::const_iterator character = FENstring.begin(), end = FENstring.end();

    int rank = RANK_8, file = FILE_A, count = 1;
    Piece piece = EMPTY;

    while (rank >= RANK_1 && character < end) {
        count = 1;
        switch (*character) {
            case 'p': piece = bP; break;
            case 'k': piece = bK; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'r': piece = bR; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'K': piece = wK; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'R': piece = wR; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *character - '0'; //ASCII value of number - ASCII value of 0 = actual value of number
                break;

            case ' ':
            case '/':
                rank--;
                file = FILE_A;
                character++;
                continue;

            default:
                return false; //error
        }

        for (int i = 0; i < count; i++) {
            fullBoard[FR2SQ120(file,rank)] = piece;
            file++;
        }

        character++;
    }

    //side to move
    sideToMove = *character == 'w' ? WHITE : BLACK;
    oppSideToMove = *character == 'w' ? BLACK : WHITE;
    character += 2;

    //castling rights
    for (int i = 0; i < 4; i++) {
        if (character >= end) return false;
        if (*character == ' ') break;
        switch (*character) {
            case 'K': castlePerm |= WHITE_CASTLE_SHORT; break;
            case 'Q': castlePerm |= WHITE_CASTLE_LONG; break;
            case 'k': castlePerm |= BLACK_CASTLE_SHORT; break;
            case 'q': castlePerm |= BLACK_CASTLE_LONG; break;
        }
        character++;
    }
    character++;
    if (character >= end) return false;

    //enpassent square
    if (*character != '-') {
        file = *(character++) - 'a';
        if (character >= end) return false;
        rank = *character - '1';

        enPassentSq = FR2SQ120(file,rank);
    }
//    character += 2;
//    if (character >= end) return false;
//
//    //fifty move counter
//    int fifty = *(character++) - '0';
//
//    if (character >= end) return false;
//    if (*character != ' ') //two digit number
//        fifty = 10*fifty + (*character - '0');
//
//    fiftyMove = fifty;

    /*
        FEN string completely parsed, now need to synchronize board internally
        e.g bit boards
    */

    buildBitBoards();
    buildPieceLists();

    posKey = genPosKey();

    return true;
}


void Board::printBoard() {
    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        std::cout << std::setw(4) << std::left << rankChars[rank];
        for (int file = FILE_A; file <= FILE_H; file++)
            std::cout << std::setw(3) << pieceChars[fullBoard[FR2SQ120(file,rank)]];
        std::cout << std::endl;
    }
    std::cout << "\n    ";
    for (int file = FILE_A; file <= FILE_H; file++)
        std::cout << std::setw(3) << fileChars[file];

    std::cout << "\n\n     " << colourChars[sideToMove]
              << "   " << castlePermStrs[castlePerm]
              << "   " << (enPassentSq!=INVALID_SQ?static_cast<char>(FILE120(enPassentSq) + 'a'):'-')
                       << (enPassentSq!=INVALID_SQ?static_cast<char>(ROW120(enPassentSq) + '1'):' ')
              << "   " << fiftyMove <<std::endl;

}

void Board::printPieceList() {
    char labels[13][3] = { "  ",
                           "wP", "wK", "wN", "wB", "wR", "wQ",
                           "bP", "bK", "bN", "bB", "bR", "bQ" };

    for (uint pieceType = wP; pieceType <= bQ; pieceType++) {
        std::cout << labels[pieceType] << std::endl;
        for (std::vector<uint>::const_iterator piece = pieceLists[pieceType].begin(); piece < pieceLists[pieceType].end(); piece++) {
            std::cout << "\t120: " << *piece << "\t(" << printableSqs64[convBoard_120To64[*piece]] << ")" << std::endl;
        }
    }

}

void Board::buildBitBoards() {
    unsigned sq,piece,col;

    //Piece bitBoards
    for (piece = wP; piece <= bQ; piece++) {
        pieceBBs[piece] = 0ULL; //reset bb
        for (sq = A1; sq <= H8; sq++)
            if (fullBoard[sq] == piece)
                setBit(pieceBBs[piece],convBoard_120To64[sq]);
    }

    //Colour bitBoards
    for (col = WHITE; col < NONE; col++)
        colourBBs[col] = 0ULL; //resetting

    for (piece = wP; piece <= wQ; piece++)
        colourBBs[WHITE] |= pieceBBs[piece];

    for (piece = bP; piece <= bQ; piece++)
        colourBBs[BLACK] |= pieceBBs[piece];

    colourBBs[BOTH] = colourBBs[WHITE] | colourBBs[BLACK];
    colourBBs[NONE] = ~colourBBs[BOTH];
    pieceBBs[EMPTY] = colourBBs[NONE];
}

void Board::buildPieceLists() {
    int sq120;

    for (int sq64 = 0; sq64 < 64; sq64++) {
        sq120 = convBoard_64To120[sq64];
        if (fullBoard[sq120] != EMPTY)
            pieceLists[fullBoard[sq120]].push_back(sq120);
    }
}

bool Board::checkIntegrity() {

    unsigned i,sq64,sq120,piece;

    /* CONSISTENCY WITHIN THE BBs */
    //Finding whether there are any duplicate entries for a square in different BBs
    //White pieces
    bitBoard wbb = pieceBBs[wP];
    for (piece = wK; piece <= wQ; piece++) { //for each piece
        dassert(!(wbb & pieceBBs[piece])); //check that there are no shared bits with the rest of the piece BBs
        wbb |= pieceBBs[piece];
    }
    dassert(wbb == colourBBs[WHITE]);
    dassert(!(wbb & colourBBs[BLACK]));

    //Black pieces
    bitBoard bbb = pieceBBs[bP];
    for (piece = bK; piece <= bQ; piece++) { //for each piece
        dassert(!(bbb & pieceBBs[piece])); //check that there are no shared bits with the rest of the piece BBs
        bbb |= pieceBBs[piece];
    }
    dassert(bbb == colourBBs[BLACK]);
    dassert(!(bbb & colourBBs[WHITE]));

    //All pieces
    bitBoard tempbb = wbb | bbb;

    dassert(tempbb == colourBBs[BOTH]); //assert that all pieces documented in BOTH
    dassert(!(tempbb & colourBBs[NONE])); //and that there aren't any extras (since BOTH = ~NONE)
    tempbb = ~tempbb;
    dassert(tempbb == colourBBs[NONE]); //opposite of above
    dassert(!(tempbb & colourBBs[BOTH]));

    //Find whether there are any missing squares in all BBs (i.e, 0 in Colour::BOTH AND Colour::NONE)
    dassert((tempbb | ~tempbb) == 0xffffffffffffffff); //all bits should be set in BOTH | NONE

    //Check counts of individual piece types
    dassert(countBits(pieceBBs[wK]) == 1);
    dassert(countBits(pieceBBs[bK]) == 1);
    dassert(countBits(pieceBBs[wP]) < 9);
    dassert(countBits(pieceBBs[bP]) < 9);

    for (piece = wN; piece < wQ; piece++)
        dassert(countBits(pieceBBs[piece]) < 11); //10 is absolute max, e.g 2 rooks and 8 pawns promoted to rooks
        //I'd be shocked if this dassert ever failed, while all the ones before passed
    for (piece = bN; piece < bQ; piece++)
        dassert(countBits(pieceBBs[piece]) < 11);


    /* CONSISTENCY BETWEEN BBs AND FULLBOARD */
    for (sq64 = 0; sq64 < 64; sq64++) { //for all squares
        int sq120 = convBoard_64To120[sq64];
        dassert(fullBoard[sq120] != INVALID_PCE); //dassert there is either a piece here or EMPTY
        if (fullBoard[sq120] != EMPTY) { //if occupied
            dassert(testBit(pieceBBs[fullBoard[sq120]], sq64)); //dassert that the piece here is documented in its own bb
            if (fullBoard[sq120] < bP) { //if it's white piece
                dassert(testBit(colourBBs[WHITE], sq64)); //dassert that the piece is documented in its colour's bb
                dassert(!testBit(colourBBs[BLACK], sq64)); //dassert that the piece is NOT documented in opposite colour's bb
            }
            else {//black
                dassert(testBit(colourBBs[BLACK], sq64)); //same for black
                dassert(!testBit(colourBBs[WHITE], sq64));
            }
            dassert(testBit(colourBBs[BOTH], sq64));//dassert that it's in BOTH
            dassert(!testBit(colourBBs[NONE], sq64)); //and that it's NOT in NONE
        }
        else { //square == EMPTY
            for (piece = wP; piece <= bQ; piece++)
                dassert(!testBit(pieceBBs[piece], sq64)); //dassert that all piece bitboards reflects that
            for (int col = WHITE; col <= BOTH; col++)
                dassert(!testBit(colourBBs[col], sq64)); //dassert that ONLY Colour::NONE reflects that
            dassert(testBit(colourBBs[NONE], sq64));
        }
    }


    /* CONSISTENCY BETWEEN PIECELISTS AND FULLBOARD */
    //Checking all pieces on fullboard are in pieceLists
    for (sq64 = 0; sq64 < 64; sq64++) { //for all squares
        sq120 = convBoard_64To120[sq64];
        if (fullBoard[sq120] != EMPTY) { //if occupied
            bool found = false;
            for (i = 0; i < pieceLists[fullBoard[sq120]].size(); i++) //for each entry in the list for this piece
                if (pieceLists[fullBoard[sq120]][i] == sq120) {//if this entry is for this square
                    if (!found) //and we havnt already reported it as found
                        found = true;
                    else  //duplicates of this entry
                        dassert(0); //bail
                }
            dassert(found); //if not, no entries for this square found at all
        }
    }

    //And checking all pieces in pieceLists are on fullboard
    for (piece = wP; piece <= bQ; piece++) {
        for (i = 0; i < pieceLists[piece].size(); i++) {//for each entry in the list for this piece
            dassert(fullBoard[pieceLists[piece][i]] == piece);
        }
    }

    //if bbs consistent with fullboard, and piecelists consistent with fullboard so far, then shouldnt need these
    //checks, but whatever just to be sure
    dassert(pieceLists[wK].size() == 1);
    dassert(pieceLists[bK].size() == 1);

    dassert(pieceLists[wP].size() < 9);
    dassert(pieceLists[bP].size() < 9);


    /* VALIDITY OF MISC DATA */
    //Both sides should either be NONE (board just initialised), or opposite BLACK/WHITE. In no case BOTH
    if (sideToMove == NONE)
        dassert(oppSideToMove == NONE);
    else
        if (sideToMove == WHITE)
            dassert(oppSideToMove == BLACK);
        else if (sideToMove == BLACK)
            dassert(oppSideToMove == WHITE);
        else // = BOTH
            dassert(0); //should never = BOTH

    if (enPassentSq != INVALID_SQ) //if it's set
        dassert(ROW120(enPassentSq) == RANK_3 || ROW120(enPassentSq) == RANK_6);
    dassert(castlePerm >= 0 && castlePerm <= 15); //Castling
    dassert(fiftyMove >= 0 && fiftyMove <= 101); //Fifty move rule counter
    dassert(posHistory.size() == hisPly);
    dassert(ply <= hisPly);

    return true;
}

void Board::printMoveHistory() {
    assert(hisPly == posHistory.size());

    std::cout << "\nMOVE HISTORY (" << hisPly << " ply):" <<std::endl;
    for (unsigned i = 0; i < hisPly; i+=2) {
        if (i+1 < hisPly)
            std::cout << (i/2 + 1) << ": " << posHistory[i].move << " " << posHistory[i+1].move << std::endl;
        else
            std::cout << (i/2 + 1) << ": " << posHistory[i].move << " ..." << std::endl;
    }
}

void Board::printPosHistory() {
    assert(hisPly == posHistory.size());

    Board tempBoard;
    tempBoard.loadFENPosition(this->startPosition);

    std::cout << "\nPOSITION HISTORY (" << hisPly << " ply):" <<std::endl;
    tempBoard.printBoard();

    for (unsigned i = 0; i < hisPly; ++i) {
        Move move = this->posHistory[i].move;

        assert(tempBoard.checkIntegrity());
        tempBoard.makeMove(move);

        tempBoard.printBoard();
        move.printMoveDetailed();
        tempBoard.printMoveHistory();
        printCurrentFENPosition();
        assert(tempBoard.checkIntegrity());
    }
}

void Board::printMoveList() {
    std::cout << "\nMOVELIST (" << moveList.size() << "):" << std::endl;
    for (unsigned i = 0; i < moveList.size(); ++i) {
        moveList[i].printMoveDetailed();
    }
}

void Board::printMoveListSimple() {
    std::cout << "\nMOVELIST (" << moveList.size() << "):" << std::endl;
    for (unsigned i = 0; i < moveList.size(); ++i) {
        moveList[i].printMove();
    }
}

void Board::printPieceBitBoards() {
    std::cout << "\nBITBOARDS:" << std::endl;
    for (unsigned i = 0; i < NUM_PIECES; ++i) {
        std::cout << "Piece: " << pieceChars[i] <<std::endl;
        printBitBoard(pieceBBs[i]);
    }
}

void Board::printFullDebug() {
    printPosHistory();
    printMoveList();
    printPieceList();
    printPieceBitBoards();
    printCurrentFENPosition();
}

void Board::printCurrentDebug() {
    printBoard();
    printPieceList();
    printPieceBitBoards();
    printCurrentFENPosition();
}

void Board::printCurrentFENPosition() {
    std::cout << "\nFENPOSITION:" << std::endl;

    uint emptyCounter = 0;
    for (int rank = RANK_8; rank >= RANK_1; --rank) { //signed else annoying wrap-around bullshit since RANK_1 = 0
        for (uint file = FILE_A; file <= FILE_H; ++file) {
            uint piece = fullBoard[FR2SQ120(file, rank)];
            if (piece == EMPTY)
                ++emptyCounter;
            else {
                if (emptyCounter) {
                    std::cout << emptyCounter;
                    emptyCounter = 0;
                }
                std::cout << pieceChars[piece];
            }
        }
        if (emptyCounter) {
            std::cout << emptyCounter;
            emptyCounter = 0;
        }
        if (rank != RANK_1)
            std::cout << "/"; //no trailing /
    }

    std::cout << " " << colourChars[sideToMove]
            << " " << castlePermStrs[castlePerm]
            << " " << printableSqs64[conv64(enPassentSq)]
            << " " << fiftyMove
            << " " << (hisPly/2 + 1) << std::endl;
}

PosHash::Key Board::genPosKey() {
    PosHash::Key posKey = 0ULL;

    unsigned piece,i;
    for (piece = wP; piece <= bQ; piece++)
        for (i = 0; i < pieceLists[piece].size(); i++)
            posKey ^= piecePosKeys[piece][pieceLists[piece][i]]; //b.pieceLists[piece][i] returns a (120) square which that piecetype is occupying
                    //therefore ' piecePosKeys[piece][b.pieceLists[piece][i]] ' = get the key corresponding to piece on the square detailed in the pieceList

    if (sideToMove < BOTH)
        posKey ^= sideKeys[sideToMove];

    posKey ^= castlingKeys[castlePerm];

    if (enPassentSq != INVALID_SQ)
        posKey^= piecePosKeys[EMPTY][enPassentSq];

    return posKey;
}

Move Board::parseMove(std::string moveStr) {
    assert(moveStr.size() == 4 || moveStr.size() == 5);

//    std::cout << moveStr<< " ";

    int fromFile = moveStr[0] - 'a';
    int fromRank = moveStr[1] - '1';
    int toFile = moveStr[2] - 'a';
    int toRank = moveStr[3] - '1';

    assert(fileValid(fromFile));
    assert(rankValid(fromRank));
    assert(fileValid(toFile));
    assert(rankValid(toRank));

    int from = FR2SQ64(fromFile, fromRank);
    int to = FR2SQ64(toFile, toRank);

//    std::cout << from << " " << to <<std::endl;

    assert(inBounds64(from));
    assert(inBounds64(to));

    generateMoves(sideToMove);

    //find move corresponding to the one that was inputted
    for (unsigned i = 0; i < moveList.size(); ++i) {
        Move move = moveList[i];
        if (from == move.getFrom() && to == move.getTo()) {
            if (!move.getProm()) {
                return move;
            } else {
                Move m;
                switch(move.getProm()) {
                    case wN: if (moveStr[4] == 'N') m=move; break;
                    case wB: if (moveStr[4] == 'B') m=move; break;
                    case wQ: if (moveStr[4] == 'Q') m=move; break;
                    case wR: if (moveStr[4] == 'R') m=move; break;
                    case bN: if (moveStr[4] == 'n') m=move; break;
                    case bB: if (moveStr[4] == 'b') m=move; break;
                    case bQ: if (moveStr[4] == 'q') m=move; break;
                    case bR: if (moveStr[4] == 'r') m=move; break;
                    default: assert(0); //invalid piece
                }
                return m;
            }
        }
    }

    return Move();
}
