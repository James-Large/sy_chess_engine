#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "data.h"
#include "enums.h"
#include "move.h"
#include "bitBoards.h"
#include "posHash.h"
#include "posHistory.h"
#include <stack>
#include <string>
#include <vector>
//std::unordered_multiset to store hash keys?

class Board {

//MEMBERS
public:
    const static uint STD_BOARD_SZ = 64;
    const static uint PADDED_BOARD_SZ = 120;
    const static uint NUM_PIECES = 13;

    //State of position
    uint fullBoard[PADDED_BOARD_SZ]; //120sq board storing Pieces, the 'overall' board
    bitBoard pieceBBs[NUM_PIECES]; //stores bitBoards detailing positions of every type of piece, and EMPTY squares
    bitBoard colourBBs[4]; //stores bitboards detailing position of all WHITE, BLACK, BOTH, NONE pieces
    //BOTH essentially = BLACK | WHITE  = all occupied squares
    //NONE essentially = ~BOTH          = all empty squares = pieceBBs[EMPTY] also
    std::vector<uint> pieceLists[NUM_PIECES]; //stores squares (120 based) that every piece is sitting on, by piece type [12]
    //and by instances of that type (vector<int>), e.g if 5 wP on board, pieceLists[wP].size() == 5
    //in most extreme case, at most 10 of same piece type on board at once (2 rooks, 8 pawns promote to rook)

    uint sideToMove;      //WHITE or BLACK
    uint oppSideToMove;   //opposite of sideToMove, just for ease
    //(since Colour enum contains more than just BLACK/WHITE values (i.e BOTH/NONE), cannot just do something like !sideToMove)

    uint castlePerm;
    uint fiftyMove; //if == 100 (halfmoves), then game is drawn, reset on each pawn move/capture
    uint enPassentSq; //if a pawn moves two squares from its initial position, the 'skipped' square is stored here to allow
    //checks for the availability of en passent captures, in 120 form

    uint ply;        //depth, in half-moves, of current search
    uint hisPly;    //length, in half-moves, of game so far
    std::vector<PosHistory> posHistory; //size = hisPly, actual moves played 'on board' (hisPly - ply) + moves being searched (ply)

    std::vector<Move> moveList; //moveGen generates moves and stores them here, list of all legal moves in a position

    PosHash::Key posKey;
    std::string startPosition;

//FUNCS
public:
    //Setup
    Board();

    /* init.cpp */
    void initAll();
    void initBoard();

    /* board.cpp */
    bool loadFENPosition(std::string FENstring);
    void dassertFail(const char* test, const char* file, int line);
    void dassertFailEXIT(const char* test, const char* file, int line);
    void printFullDebug(); //prints all debug info for the entire position history
    void printCurrentDebug(); //prints debug info for the current position and a simple movehistory
    void printCurrentFENPosition();
    void printBoard();
    void printPieceList();
    void printMoveHistory();
    void printPosHistory();
    void printMoveList();
    void printMoveListSimple();
    void printPieceBitBoards();
    void buildBitBoards(); //build all bbs from the fullBoard from scratch - e.g after a position is initially loaded
    void buildPieceLists();
    bool checkIntegrity(); //checks that all data in this board is consistent - bitboards are consistent with fullboard etc
    PosHash::Key genPosKey();
    Move parseMove(std::string move);

    /* moveGen.cpp */
    uint isSqAttacked(uint sq, uint attackingSide) const; //uint so that can return not only true/false, but HOW MANY times the sq is attacked
    void generateMoves(uint side);
    void generateCastlingMoves(uint side);
    void generatePawnMoves(uint side);
    void generateNonSlidingMoves(uint side);
    void generateSlidingMoves(uint side);
    void addQuietMove(uint from, uint to);
    void addCaptureMove(uint from, uint to, uint piece);
    void addSpecialMove(uint from, uint to, uint piece, uint prom, uint cas, uint enPas);
	bool isCastleThroughCheck(uint casType) const;

    /* makeMove.cpp */
    void clearPiece(const uint sq120);
    void addPiece(const uint sq120, const uint piece);
    void movePiece(const uint from120, const uint to120);
    int makeMove(const Move move);
    int takeMove();

    /* search.cpp */
    Move search(int depth);
    Move alphaBeta(int depth, int alpha, int beta);
    int isRepetition();

};

#endif // BOARD_H_INCLUDED
