#include "data.h"
#include "enums.h"

const char pieceChars[]       = ".PKNBRQpknbrq";
const char fileChars[]        = "ABCDEFGH";
const char rankChars[]        = "12345678";
const char colourChars[]      = "wb-";
const char castlePermStrs[][5] = {"----", "K---", "-Q--", "KQ--", "--k-", "K-k-", "-Qk-", "KQk-",
                                 "---q", "K--q", "-Q-q", "KQ-q", "--kq", "K-kq", "-Qkq", "KQkq"};

const std::string FEN_START_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

const int numDirs[13] = {0, 0, 8, 8, 4, 4, 8, 0, 8, 8, 4, 4, 8 };

const int KnDir120[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int RkDir120[4] = { -1, -10, 1, 10 };
const int BiDir120[4] = { -9, -11, 11, 9 };
const int KiDir120[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

const int KnDir64[8] = { -6, -15, -17, -10, 6, 15, 17, 10 };
const int RkDir64[4] = { -1, -8, 1, 8 };
const int BiDir64[4] = { -7, -9, 9, 7 };
const int KiDir64[8] = { -1, -8, 1, 8, -7, -9, 9, 7 };

unsigned convBoard_120To64[120];
unsigned convBoard_64To120[64];

const int pieceBig[13] = { 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 };
const int pieceMaj[13] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1 };
const int pieceMin[13] = { 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 };
const int pieceVal[13]= { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000  };
const int pieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
	BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

const int piecePawn[13] = { 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };
const int pieceKnight[13] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 };
const int pieceKing[13] = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
const int pieceRookQueen[13] = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0 };
const int pieceBishopQueen[13] = { 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0 };
const int pieceSlides[13] = { 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0 };

const char printableSqs64[64][3] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};
