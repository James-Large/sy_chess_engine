#ifndef POSHISTORY_H_INCLUDED
#define POSHISTORY_H_INCLUDED

#include "move.h"
#include "board.h"
#include "posHash.h"

class PosHistory {
public:

    Move move;
    PosHash::Key posKey;
    uint castlePerm;
	uint enPassentSq;
	uint fiftyMove;

    PosHistory() { }
};

#endif // POSHISTORY_H_INCLUDED
