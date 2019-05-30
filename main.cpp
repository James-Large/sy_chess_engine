#include <iostream>
#include <cstdio>
#include "board.h"
#include "data.h"
#include "bitBoards.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#define NDEBUG


#include <cassert>
#include <stdlib.h>
#include "timer.h"

using namespace std;

typedef unsigned long long ULL;

void exhaustivePerft(int, bool);
void performPerft(int, string, long);
ULL perft(int, Board);
void stdinGame(bool, string);

Timer innerPerftTimer;
ULL totalPerftTime;
int maxDepth;
bool fulloutput;
bool divide = true;

int main()
{
    Board board;

        /*TESTING BOARDINIT() */
//    for (int i = 0; i < 64; i++) {
//        if (i % 8 == 0) cout << endl;
//        printf("%5d",convBoard_64To120[i]);
//    }
//
//    cout << "\n\n\n" << endl;
////
//    for (int i = 0; i < 120; i++) {
//        if (i % 10 == 0) cout << endl;
//        printf("%5d",convBoard_120To64[i]);
//    }
//    exit(0);
//
//    cout << "\n\n\n" << endl;
//
//    for (int i = 0; i < 120; i++) {
//        if (i % 10 == 0) cout << endl;
//        printf("%5d",board.fullBoard[i]);
//    }

//    cout << "\n\n\n" << endl;
//
//    if (!board.loadFENPosition(FEN_START_POS)) {
//        cout << "ERROR \n\n" <<endl;
//    }
//
//    for (int i = 0; i < 120; i++) {
//        if (i % 10 == 0) printf("\n");
//        printf("%5d", board.fullBoard[i]);
//    }
//
//    board.printBoard();
//
//    cout << "\n\n\n" << endl;
//    if (!board.loadFENPosition("4r1k1/pppq1p1p/5rpQ/2p5/5PP1/P1PP4/1P1R1K2/7R b - -")) {
//        cout << "ERROR \n\n" <<endl;
//    }
//
//
//    board.printBoard();

//    bitBoard bb = 0;
//
//    setBit(bb,37);
//    printBitBoard(bb);
//
//    cout << countBits(bb) << endl;
//
//    clearBit(bb,37);
//    printBitBoard(bb);
//
//    cout << countBits(bb) << endl;
//
//    bb |= 0xa131b10003f1c101;
//    printBitBoard(bb);
//
//    while (bb) {
//        popBit(bb);
//        printBitBoard(bb);
//        cout << countBits(bb) << endl;
//    }
//    return 0;

//    board.loadFENPosition(FEN_START_POS);
//    board.printBoard();
//
//    cout << "\n\n";
//
//    for (int i = wP; i <= bQ; i++)
//        printBitBoard(board.pieceBBs[i]);
//    for (int i = WHITE; i <= NONE; i++)
//        printBitBoard(board.colourBBs[i]);

//    board.loadFENPosition(FEN_START_POS);
//    board.printBoard();
//
//    if (board.checkIntegrity()) {
//        cout << "all good" <<endl;
//    }

//    board.loadFENPosition(FEN_START_POS);
//    cout << board.posKey <<endl;
//
//    board.loadFENPosition("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
//    cout << board.posKey <<endl;
//
//    board.loadFENPosition(FEN_START_POS);
//    cout << board.posKey <<endl;
//
//    board.loadFENPosition("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
//    cout << board.posKey <<endl;
//
//    board.loadFENPosition("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
//    cout << board.posKey <<endl;
//
//    board.loadFENPosition(FEN_START_POS);
//    cout << board.posKey <<endl;


//    for (unsigned piece = wP; piece <= wP; piece++) {
//        cout << "\n\n *************************" << piece << " *********************" << endl;
//        for (unsigned sq = 0; sq < 64; sq++) {
//            cout << "\t\tsq:  " << sq <<endl;
//            printBitBoard(pieceAttackBBs[piece][sq]);
//        }
//    }

//    for (int i = 0; i < 8; i++) {
//        printBitBoard(rankMask[i]);
//        printBitBoard(fileMask[i]);
//    }


//    cout << genSqPair(0,63);
//    printBitBoard(blockingBBs[genSqPair(0,63)]);
//
//    cout << genSqPair(0,7);
//    printBitBoard(blockingBBs[genSqPair(0,7)]);
//
//    cout << genSqPair(0, 18);
//    printBitBoard(blockingBBs[genSqPair(0,18)]);
//
//    cout << genSqPair(42,28);
//    printBitBoard(blockingBBs[genSqPair(42,28)]);
//
//    cout << genSqPair(15,39);
//    printBitBoard(blockingBBs[genSqPair(15,39)]);
//
//    cout << genSqPair(57,62);
//    printBitBoard(blockingBBs[genSqPair(57,62)]);
//
//    cout << genSqPair(62,57);
//    printBitBoard(blockingBBs[genSqPair(62,57)]);
//
//    cout << genSqPair(0,11); //exception out of bounds
//    printBitBoard(blockingBBs.at(genSqPair(0,11)));

//    board.loadFENPosition("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
//    board.printBoard();
//    cout << board.isSqAttacked(0, BLACK) << " "; //0
//    cout << board.isSqAttacked(0, WHITE) << " "; //0
//    cout << board.isSqAttacked(16, WHITE) << " "; //2
//    cout << board.isSqAttacked(12, WHITE) << " "; //4
//    cout << board.isSqAttacked(26, WHITE) << " "; //1
//    cout << board.isSqAttacked(37, WHITE) << " "; //1
//    cout << board.isSqAttacked(40, WHITE) << " "; //1
//    cout << board.isSqAttacked(40, BLACK) << " "; //2
//    cout << board.isSqAttacked(27, WHITE) << " "; //0

//    Move m;
//    m.setFrom(5);
//    m.printMove();
//    m.setTo(13);
//    m.printMove();

//    board.loadFENPosition("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
//    printBitBoard(board.pieceBBs[wP]);
//    cout << ms1b(board.pieceBBs[wP]);
//    printBitBoard(board.pieceBBs[bK]);
//    cout << ms1b(board.pieceBBs[bK]);
//    printBitBoard(board.colourBBs[WHITE]);
//    cout << ms1b(board.colourBBs[WHITE]);

//    for (int sq1 = 0; sq1 < 64; sq1++) {
//        for (int sq2 = 0; sq2 < 64; sq2++) {
//            if (blockingBBs.count(genSqPair(sq1,sq2))) {
//                cout << "sq1:" << sq1 <<" sq2:" << sq2 <<endl;
//                printBitBoard(blockingBBs[genSqPair(sq1,sq2)]);
//            }
//        }
//    }

//    for (int sq = 0; sq < 64; sq++) {
//        for (int dir = 0; dir < 8; dir++) {
//            cout << "sq:" << sq <<" dir:" << dir << " actualdir:" << KiDir64[dir] <<endl;
//            printBitBoard(rayAttackBBs[sq][dir]);
//        }
//    }

//    board.loadFENPosition(FEN_START_POS);
//    board.checkIntegrity();
//    board.printBoard();
//    board.generateMoves(WHITE);
//
//    for (int i = 0; i < board.moveList.size(); i++) {
//        cout << board.moveList[i].getFrom() << " " << board.moveList[i].getTo() << endl;
//    }
//
//    cout << "\n\n" <<endl;
//
//    board.loadFENPosition("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
//    board.printBoard();
//
//    board.generateMoves(WHITE);
//    for (int i = 0; i < board.moveList.size(); i++)
//        cout << board.moveList[i].getFrom() << " " << board.moveList[i].getTo() << endl;
//
//    cout << "\n\n" <<endl;
//    board.loadFENPosition("rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/2P1P3/RNBQKBNR w KQkq e6 0 1");
//    board.printBoard();
//   // board.printPieceList();
//    board.checkIntegrity();
//
//    board.generateMoves(WHITE);
//    cout << board.moveList.size() <<endl;
//    for (int i = 0; i < board.moveList.size(); i++)
//        board.moveList[i].printMove();
//    cout << "\n\n" <<endl;
//    board.generateMoves(BLACK);
//    cout << board.moveList.size() <<endl;
//    for (int i = 0; i < board.moveList.size(); i++)
//        board.moveList[i].printMove();

    cout << "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P2r1/3P3P/2P1P3/RNBQK2R w KQkq e6 0 1" <<endl;
	board.loadFENPosition("rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P2r1/3P3P/2P1P3/RNBQK2R w KQkq e6 0 1");
	board.printCurrentFENPosition();
	board.printBoard();
//    board.checkIntegrity();
//	cout << "\n\n" <<endl;
//	board.generateMoves(WHITE);
//    cout << board.moveList.size() <<endl;
//    for (int i = 0; i < board.moveList.size(); i++)
//        board.moveList[i].printMove();

//    string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
//    board.loadFENPosition(fen);

//    printBitBoard(castlingBlockMasks[WHITE_CASTLE_SHORT]);
//    printBitBoard(castlingBlockMasks[WHITE_CASTLE_LONG]);
//    printBitBoard(castlingBlockMasks[BLACK_CASTLE_SHORT]);
//    printBitBoard(castlingBlockMasks[BLACK_CASTLE_LONG]);
//
//    printBitBoard(board.colourBBs[BOTH]);
//
//    board.generateMoves(board.sideToMove);
//    board.printBoard();
//    board.printMoveListSimple();

//    board.printBoard();
//    board.generateMoves(board.sideToMove);
//
//    Move move = board.moveList[0];
//
//    cout << "from: " << move.getFrom() <<endl;
//    cout << "to: " << move.getTo() <<endl;
//    cout << "cap: " << move.getCap() <<endl;
//    cout << "prom: " << move.getProm() <<endl;
//    cout << "cas: " << move.getCas() <<endl;
//    cout << "ep: " << move.isEnPas() <<endl;

//    cout << board.hisPly << " " << board.posHistory.size() << std::endl;
//    board.printPosHistory();



//    string fen = "1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1";
//    string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
//    string fen = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1";
//    string fen = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
//    performPerft(2, fen, -1);
//    for (int i = 1; i <= 1; ++i) {
//        fulloutput = false;
//        performPerft(i, fen);
//    }
//    stdinGame(false, fen);

//    exhaustivePerft(5, false);

}

void exhaustivePerft(int depth = -1, bool b = false) {
    fulloutput = b;
    ifstream in;
    in.open("perftsuite.epd");

    long nodeCounts[6];

    while (in.good()) {
        string temp;
        getline(in, temp);
        if (in.eof())
            break;
        stringstream line(temp);

        string parts[7];
        for (int i = 0; i < 7 && line.good(); ++i) {
            getline(line, parts[i], ';');
//            cout<< parts[i] <<endl;
        }

        for (int j = 1; j < 7; ++j) {
             line = stringstream(parts[j].substr(3));
             line >> nodeCounts[j-1];
//             cout << nodeCounts[j-1] <<endl;
        }
//        cout <<endl;


        if (depth==-1) {
            for (int i = 1; i <= depth; ++i)
                performPerft(i, parts[0], nodeCounts[i-1]);
        } else {
            performPerft(depth, parts[0], nodeCounts[depth-1]);
        }
    }

}

void performPerft(int depth, string fen, long targetnodes = -1) {
    Board board;
    board.loadFENPosition(fen);

    if (fulloutput) {
        board.printBoard();
        cout << "\nStarting perft of " << fen << "\n" <<endl;
    }

    maxDepth = depth;

    innerPerftTimer.start();
    long nodes = perft(depth, board);

    if (targetnodes == -1)
        cout << "Total leaf nodes found at depth " << depth << ": " << nodes << endl;
    else {
        if (targetnodes != nodes)
            cout << "********";
        cout << targetnodes << " nodes expected, found: " << nodes <<endl;
        if (targetnodes != nodes) {
            cout << board.startPosition <<endl;
            board.printFullDebug();
            cout << "\n" <<endl;
        }
    }
}

ULL perft(int depth, Board board) {
    assert(board.checkIntegrity());

	if(depth == 0)
        return 1;

    board.generateMoves(board.sideToMove);

    ULL leafNodes = 0;
	for(unsigned i = 0; i < board.moveList.size(); ++i) {
        if ( !board.makeMove(board.moveList[i]))
            continue;

        long nodes = perft(depth - 1, board);
        leafNodes += nodes;
        board.takeMove();

        if (depth == maxDepth) {
            if (fulloutput) {
                ULL time= innerPerftTimer.restart();
                totalPerftTime += time;
                cout<< setprecision(3) << (((double)i/board.moveList.size())*100)<< "% complete | ";
                cout<< setprecision(10) << time;
                cout << "s | ";
                cout << totalPerftTime;
                cout << "s" <<endl;
            }
        }

//        if (divide) {
//            int t = depth;
//            while (t-- > 0)
//                cout << "\t";
////            cout << board.moveList[i] << " " << nodes <<endl;
//            cout << board.moveList[i] <<endl;
//        }

    }
//    cout << endl;

    return leafNodes;
}

void stdinGame(bool debug = false, string fen = FEN_START_POS) {
    Board board;
    board.loadFENPosition(fen);

    string input;

    bool draw = true;

    while (1) {
        if (draw)
            if (debug)
                board.printCurrentDebug();
            else
                board.printBoard();
        else
            draw = true;

        cout << "\n>> ";
        cin >> input;

        if (input.size() == 1) {
            switch (input[0]) {
                case 'q':
                    return;
                case 't':
                    board.takeMove();
                    break;
                case 'd':
                    board.printFullDebug();
                    draw = false;
                    break;
                case 's':
                    board.generateMoves(board.sideToMove);
                    board.search(3).printMove();
                    draw = false;
                    break;
                case 'm':
                    board.generateMoves(board.sideToMove);
                    board.printMoveListSimple();
                    draw = false;
                    break;
                case 'r':
                    board.loadFENPosition(fen);
                    break;
            }
        } else {
            Move move = board.parseMove(input);

            if (move.move == Move().move) {
                cout << "Invalid input" <<endl;
            } else {
                if (!board.makeMove(move))
                    cout << "Illegal move" <<endl;

            }
        }
        cin.clear();
        fflush(stdin);
    }
}
