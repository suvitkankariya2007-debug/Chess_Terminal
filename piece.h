#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>
using namespace std;

class Piece {
protected:
    bool white;
public:
    Piece(bool w):white(w){}
    virtual string symbol()=0;
    virtual string name()=0;
    virtual bool valid(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b)=0;
    bool color(){return white;}
    virtual ~Piece(){}
};

#endif
