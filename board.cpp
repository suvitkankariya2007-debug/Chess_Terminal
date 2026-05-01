#include "board.h"
#include "pieces.h"
#include <iostream>
#include <windows.h>
using namespace std;

// Colors
#define RESET "\033[0m"
#define WHITE_P "\033[96m"
#define BLACK_P "\033[33m"
#define BG1 "\033[47m"
#define BG2 "\033[42m"
#define CHECK_BG "\033[41m"
#define BOLD "\033[1m"

Board::Board() { init(); }

void Board::init(){
    g.assign(8, vector<Piece*>(8,nullptr));

    for(int i=0;i<8;i++){
        g[1][i]=new Pawn(false);
        g[6][i]=new Pawn(true);
    }

    g[0][0]=g[0][7]=new Rook(false);
    g[7][0]=g[7][7]=new Rook(true);

    g[0][1]=g[0][6]=new Knight(false);
    g[7][1]=g[7][6]=new Knight(true);

    g[0][2]=g[0][5]=new Bishop(false);
    g[7][2]=g[7][5]=new Bishop(true);

    g[0][3]=new Queen(false);
    g[7][3]=new Queen(true);

    g[0][4]=new King(false);
    g[7][4]=new King(true);
}

void Board::show(){
    #ifdef _WIN32
        system("cls");
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #else
        system("clear");
    #endif
    
    whiteInCheck = check(true);
    blackInCheck = check(false);
    
    cout<<"\n    a b c d e f g h\n";
    for(int i=0;i<8;i++){
        cout<<"  "<<8-i<<" ";
        for(int j=0;j<8;j++){
            string bg=((i+j)%2==0)?BG1:BG2;

            if(g[i][j]){
                bool isKing = dynamic_cast<King*>(g[i][j]) != nullptr;
                bool inCheck = (isKing && g[i][j]->color() && whiteInCheck) || 
                                (isKing && !g[i][j]->color() && blackInCheck);
                
                string col=g[i][j]->color()?WHITE_P:BLACK_P;
                if(inCheck){
                    cout<<CHECK_BG<<BOLD<<col<<g[i][j]->symbol()<<RESET;
                } else {
                    cout<<bg<<col<<g[i][j]->symbol()<<RESET;
                }
            } else cout<<bg<<" "<<RESET;
            cout<<" ";
        }
        cout<<" "<<8-i<<"\n";
    }
    cout<<"    a b c d e f g h\n";
}

pair<int,int> Board::findKing(bool col){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(g[i][j] && dynamic_cast<King*>(g[i][j]) && g[i][j]->color()==col)
                return {i,j};
    return {-1,-1};
}

bool Board::check(bool col){
    auto k=findKing(col);
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(g[i][j] && g[i][j]->color()!=col)
                if(g[i][j]->valid(i,j,k.first,k.second,g))
                    return true;
    return false;
}

Board::~Board(){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(g[i][j]) delete g[i][j];
}
