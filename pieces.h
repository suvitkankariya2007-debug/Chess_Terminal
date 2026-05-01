#ifndef PIECES_H
#define PIECES_H

#include "piece.h"
#include "utils.h"

class Pawn: public Piece{
public:
    Pawn(bool w):Piece(w){}
    string symbol(){ return white?"♙":"♟"; }
    string name(){ return "Pawn"; }
    bool valid(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
        int d = white?-1:1;
        if(ey==sy && ex==sx+d && !b[ex][ey]) return true;
        if(ey==sy && ((white && sx==6 && ex==4) || (!white && sx==1 && ex==3)) && 
           !b[sx+d][sy] && !b[ex][ey]) return true;
        if(abs(ey-sy)==1 && ex==sx+d && b[ex][ey] && b[ex][ey]->color()!=white) return true;
        return false;
    }
};

class Rook: public Piece{
public:
    Rook(bool w):Piece(w){}
    string symbol(){ return white?"♖":"♜"; }
    string name(){ return "Rook"; }
    bool valid(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
        if(sx==ex || sy==ey) return clearPath(sx,sy,ex,ey,b);
        return false;
    }
};

class Knight: public Piece{
public:
    Knight(bool w):Piece(w){}
    string symbol(){ return white?"♘":"♞"; }
    string name(){ return "Knight"; }
    bool valid(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
        int dx=abs(sx-ex), dy=abs(sy-ey);
        return (dx==2&&dy==1)||(dx==1&&dy==2);
    }
};

class Bishop: public Piece{
public:
    Bishop(bool w):Piece(w){}
    string symbol(){ return white?"♗":"♝"; }
    string name(){ return "Bishop"; }
    bool valid(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
        if(abs(sx-ex)==abs(sy-ey)) return clearPath(sx,sy,ex,ey,b);
        return false;
    }
};

class Queen: public Piece{
public:
    Queen(bool w):Piece(w){}
    string symbol(){ return white?"♕":"♛"; }
    string name(){ return "Queen"; }
    bool valid(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
        if(sx==ex||sy==ey||abs(sx-ex)==abs(sy-ey))
            return clearPath(sx,sy,ex,ey,b);
        return false;
    }
};

class King: public Piece{
public:
    King(bool w):Piece(w){}
    string symbol(){ return white?"♔":"♚"; }
    string name(){ return "King"; }
    bool valid(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
        return abs(sx-ex)<=1 && abs(sy-ey)<=1;
    }
};

#endif
