#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <stack>
#include <utility>
#include "board.h"
using namespace std;

struct Move {
    int sx,sy,ex,ey;
    Piece* captured;
    string notation;
};

class Game {
private:
    Board b;
    bool turn;
    stack<Move> history;
    vector<string> moves;

public:
    Game();
    pair<int,int> conv(string s);
    void undo();
    void promote(int x,int y);
    bool move(int sx,int sy,int ex,int ey,string a,string c);
    void save();
    void load();
    void showHistory();
    void play();
};

#endif
