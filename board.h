#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>
#include "piece.h"
using namespace std;

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
    ~Board();
};

#endif
