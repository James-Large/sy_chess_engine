#include "board.h"

Move Board::search(int depth) {
    return Move();
}


Move Board::alphaBeta(int depth, int alpha, int beta) {
    return Move();
}

//maybe implement a hashmap, mapping poskeys -> ints, which counts the number of times this position is reached
//in the current game+search, and is updated at each make/take move
//this function as is simply looks back into the posHistory to find how many times this position occurred
int Board::isRepetition() {
    int count = 0;
    for (int i = posHistory.size()-1-fiftyMove; i >=0; --i) {
        if (posHistory[i].posKey == this->posKey)
            ++count;
    }
    return count;
}
