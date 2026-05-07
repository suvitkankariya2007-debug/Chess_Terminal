#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>
#include <string>
#include "piece.h"
using namespace std;

struct Move {
    int sx, sy, ex, ey;
    Piece* captured;
    string notation;
};

class Board {
public:
    vector<vector<Piece*>> g;
    bool whiteInCheck = false;
    bool blackInCheck = false;

    Board();
    void init();
    void show();
    pair<int,int> findKing(bool col);
    bool check(bool col);
    
    Piece* applyMove(int sx, int sy, int ex, int ey);
    void undoMove(int sx, int sy, int ex, int ey, Piece* captured);
    
    ~Board();
};

#endif
