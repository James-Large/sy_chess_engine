#ifndef HASHER_H_INCLUDED
#define HASHER_H_INCLUDED

#include "data.h"
#include "enums.h"


namespace PosHash {
    typedef unsigned long long Key;
    typedef std::unordered_map<unsigned, Key> KeyMap;
}

extern PosHash::KeyMap piecePosKeys[];//Stores random keys for each piece type on each square
//[13] because keys will be included for EMPTY, to be used for the enpassent square
//umap used because will be indexing by 120 based squares (to save time converting to 64 based)
//thus indexing is not continuous

extern PosHash::Key sideKeys[]; //Stores random keys for WHITE/BLACK to move
extern PosHash::Key castlingKeys[]; //Stores random keys for each combination of castling permissions

// hasher.cpp
void initKeys();
#endif // HASHER_H_INCLUDED
