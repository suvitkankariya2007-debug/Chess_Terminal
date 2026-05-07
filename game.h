#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
//dor dynamic array storing moves hist.
#include <stack>// LIFO- latest move on top
#include <utility>//gav ethe pair function
#include "board.h"
using namespace std;

class Game {
private:
    Board b;
    bool turn;
    stack<Move> history;
    vector<string> moves;

public:
    Game();
    pair<int,int> conv(string s);//stores two characters together

    void undo();//removes last move from stack
    void promote(int x,int y);
    bool move(int sx,int sy,int ex,int ey,string a,string c);
    //every move stored in stack and last move removed during undo
    //sx-start x   ey-end y
    void save();
    void load();
    void showHistory();
    void play();
};

#endif
