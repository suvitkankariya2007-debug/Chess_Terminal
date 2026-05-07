#ifndef AI_H
#define AI_H

#include "board.h"
#include <vector>

class AI {
public:
    static int evaluateBoard(Board& b, bool isWhite);
    static std::vector<Move> generateAllMoves(Board& b, bool isWhite);
    static int minimax(Board& b, int depth, int alpha, int beta, bool isMaximizingPlayer, bool isWhite);
    static Move getBestMove(Board& b, bool isWhite, int depth);
};

#endif
