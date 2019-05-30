#ifndef ENUMS_H_INCLUDED
#define ENUMS_H_INCLUDED

enum Piece {
    EMPTY,
    wP, wK, wN, wB, wR, wQ,
    bP, bK, bN, bB, bR, bQ,
    INVALID_PCE = 15
};

enum Colour {
    WHITE, BLACK, BOTH, NONE
};

enum Square { //on a 120 based board
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8,
    INVALID_SQ = 120
};

enum Rank {
    RANK_1,RANK_2,RANK_3,RANK_4,RANK_5,RANK_6,RANK_7,RANK_8,
    INVALID_RANK
};

enum File {
    FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
    INVALID_FILE
};

enum CastleRights {
    WHITE_CASTLE_SHORT = 1,
    WHITE_CASTLE_LONG  = 1 << 1,
    BLACK_CASTLE_SHORT = 1 << 2,
    BLACK_CASTLE_LONG  = 1 << 3,
    WHITE_CASTLE_BOTH  = WHITE_CASTLE_SHORT | WHITE_CASTLE_LONG,
    BLACK_CASTLE_BOTH  = BLACK_CASTLE_SHORT | BLACK_CASTLE_LONG,
    ALL_CASTLING       = BLACK_CASTLE_BOTH | WHITE_CASTLE_BOTH,
    NO_CASTLING        = 0
};

enum MoveType {
    CAPTURE,       //if this, move may also be ENPASSENT *OR* PROMOTION
    PAWN_2SQ_PUSH, //enPassent square needs to be set up
    ENPASSENT,     //previous move was PAWN_2SQ_PUSH, if ENPASSENT, also CAPTURE
    PROMOTION,     //if this, may also be capture
    CASTLE         //the source/target of a move will detail the king movement, type of castling and rook movement can be inferred from there
};

#endif // ENUMS_H_INCLUDED
