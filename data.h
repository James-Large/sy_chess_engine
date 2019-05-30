#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <string>
#include <stdint.h> //uint64_t
#include <unordered_map>

typedef unsigned uint;

extern const char pieceChars[];
extern const char fileChars[];
extern const char rankChars[];
extern const char colourChars[];
extern const char castlePermStrs[][5];

extern const std::string FEN_START_POS;

extern const int numDirs[];

extern const int KnDir120[];
extern const int RkDir120[];
extern const int BiDir120[];
extern const int KiDir120[];

extern const int KnDir64[];
extern const int RkDir64[];
extern const int BiDir64[];
extern const int KiDir64[];

extern unsigned convBoard_120To64[]; //simply stores the indices that refer to the equivalent squares in
extern unsigned convBoard_64To120[]; //the other board representation

extern const int pieceBig[13];
extern const int pieceMaj[13];
extern const int pieceMin[13];
extern const int pieceVal[13];
extern const int pieceCol[13];

extern const int piecePawn[13];
extern const int pieceKnight[13];
extern const int pieceKing[13];
extern const int pieceRookQueen[13];
extern const int pieceBishopQueen[13];
extern const int pieceSlides[13];

extern const char printableSqs64[][3];

#endif // DATA_H_INCLUDED
