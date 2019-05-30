#include "enums.h"
#include "posHash.h"
#include <unordered_map>
#include <stdlib.h> //look up <random> again at some point and figure out how the fuck to use it
#include <time.h>

//rand() returns number ATLEAST greater than 32767 (15 bits)
//want a 64 bit key, so produce multiple shifted rands() and | them together
#define randKey()  ( (PosHash::Key)rand() | \
                     ((PosHash::Key)rand() << 15) | \
                     ((PosHash::Key)rand() << 30) | \
                     ((PosHash::Key)rand() << 45) | \
                     (((PosHash::Key)rand() * 0xf) << 60) \
                    )

PosHash::KeyMap piecePosKeys[13];//Stores random keys for each piece type on each square
//[13] because keys will be included for EMPTY, to be used for the enpassent square
//umap used because will be indexing by 120 based squares (to save time converting to 64 based)
//thus indexing is not continuous

PosHash::Key sideKeys[2]; //Stores random keys for WHITE/BLACK to move
PosHash::Key castlingKeys[16]; //Stores random keys for each combination of castling permissions

void initKeys() {
    srand(time(NULL));

    unsigned sq,piece,c;

    for (piece = wP; piece <= EMPTY; piece++)
        for (sq = A1; sq <= H8; sq++)
            piecePosKeys[piece][sq] = randKey(); //indexing with [sq] will automatically create the element in the umap

    sideKeys[0] = randKey();
    sideKeys[1] = randKey();

    for (c = 0; c < 16; c++)
        castlingKeys[c] = randKey();
}


