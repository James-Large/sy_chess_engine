#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

//Conversions between board bases convX means convert TO x, e.g conv120 = convert TO 120 from 64
#define conv120(x) ( convBoard_64To120[(x)] )
#define conv64(x) ( convBoard_120To64[(x)] )

//Finding row/file of an index on a (8x8) board
#define ROW64(x)			((x) >> 3)
#define FILE64(x)			((x) & 7)

//Finding row/file of an index on a (10x12) board
#define ROW120(x)			(((x) / 10) - 2) //-2 removes padding of 120 sq board
#define FILE120(x)			(((x) % 10) - 1) //-1 removes padding of 120 sq board

//Finding index on a (8x8) board of a file and rank
#define FR2SQ64(f,r)        ((f) + ((r) * 8))

//Finding index on a (10x12) board of a file and rank
#define FR2SQ120(f,r)        ((21 + (f)) + ((r) * 10))

//quick validation
#define inBounds64(x) ( ((x) >= 0) && ((x) <= 63) )
#define inBounds120(x) ( ((convBoard_120To64[(x)]) >= 0) && ((convBoard_120To64[(x)]) <= 63) )
#define pieceValid(pce) ( ((pce) >= wP) && ((pce) <= bQ) )
#define rankValid(x) ( ((x) >= RANK_1) && ((x) <= RANK_8) )
#define fileValid(x) ( ((x) >= FILE_A) && ((x) <= FILE_H) )

//hashing shortcuts
#define HASH_PIECE(pce,sq) ((posKey) ^= (piecePosKeys[(pce)][(sq)]))
#define HASH_CASTLING ((posKey) ^= (castlingKeys[(castlePerm)]))
#define HASH_SIDE ((posKey) ^= (sideKeys[(sideToMove)]))
#define HASH_ENPAS ((posKey) ^= (piecePosKeys[EMPTY][(enPassentSq)]))

#endif // MACROS_H_INCLUDED
