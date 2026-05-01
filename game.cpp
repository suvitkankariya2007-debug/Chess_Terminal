#include "game.h"
#include "pieces.h"
#include <iostream>
#include <fstream>
#include <exception>
using namespace std;

#define BOLD "\033[1m"
#define RESET "\033[0m"

class ChessException : public exception {
    string msg;
public:
    ChessException(string m):msg(m){}
    const char* what() const noexcept override { return msg.c_str(); }
};

Game::Game() : turn(true) { }

pair<int,int> Game::conv(string s){
    return {8-(s[1]-'0'), s[0]-'a'};
}

void Game::undo(){
    if(history.empty()){ cout<<"No undo\n"; return; }
    Move m=history.top(); history.pop();

    b.g[m.sx][m.sy]=b.g[m.ex][m.ey];
    b.g[m.ex][m.ey]=m.captured;

    moves.pop_back();
    turn=!turn;
}

void Game::promote(int x,int y){
    if(dynamic_cast<Pawn*>(b.g[x][y])){
        if(x==0 || x==7){
            cout<<"Promote to (Q/R/B/N): ";
            char ch; cin>>ch;
            delete b.g[x][y];
            if(ch=='Q') b.g[x][y]=new Queen(turn);
            else if(ch=='R') b.g[x][y]=new Rook(turn);
            else if(ch=='B') b.g[x][y]=new Bishop(turn);
            else b.g[x][y]=new Knight(turn);
        }
    }
}

bool Game::move(int sx,int sy,int ex,int ey,string a,string c){
    if(!b.g[sx][sy] || b.g[sx][sy]->color()!=turn)
        throw ChessException("Invalid selection");

    if(b.g[ex][ey] && b.g[ex][ey]->color()==turn)
        throw ChessException("Destination occupied by own piece");

    if(!b.g[sx][sy]->valid(sx,sy,ex,ey,b.g))
        throw ChessException("Invalid move for this piece");

    Move m={sx,sy,ex,ey,b.g[ex][ey],a+"->"+c};

    b.g[ex][ey]=b.g[sx][sy];
    b.g[sx][sy]=nullptr;

    promote(ex,ey);

    history.push(m);
    moves.push_back(m.notation);

    return true;
}

void Game::save(){
    ofstream f("save.txt");
    for(auto &m:moves) f<<m<<"\n";
    cout<<"Game saved\n";
}

void Game::load(){
    ifstream f("save.txt");
    if(!f){ cout<<"No save\n"; return; }
    b.init();
    moves.clear();
    history=stack<Move>();

    string mv;
    while(getline(f,mv)){
        string a=mv.substr(0,2), c=mv.substr(3,2);
        auto s=conv(a), e=conv(c);
        move(s.first,s.second,e.first,e.second,a,c);
        turn=!turn;
    }
    cout<<"Game loaded\n";
}

void Game::showHistory(){
    cout<<"\n== Moves:\n";
    for(int i=0;i<moves.size();i++)
        cout<<i+1<<". "<<moves[i]<<endl;
    cout<<"\n== Board Status:\n";
    if(b.whiteInCheck) cout<<BOLD<<"-> White King is in CHECK!"<<RESET<<"\n";
    if(b.blackInCheck) cout<<BOLD<<"-> Black King is in CHECK!"<<RESET<<"\n";
}

void Game::play(){
    b.init();
    while(true){
        b.show();
        showHistory();

        string player = turn ? "White" : "Black";
        cout<<"\n"<<player<<" move (format: e2 e4) or (undo/save/load/exit): ";
        string a,c;
        if(!(cin >> a)) break;

        if(a=="undo"){ undo(); continue; }
        if(a=="save"){ save(); continue; }
        if(a=="load"){ load(); continue; }
        if(a=="exit") break;

        if(!(cin >> c)){
            cout<<"ERROR: Invalid input format. Use: e2 e4\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if(a.length() != 2 || c.length() != 2 || 
           a[0] < 'a' || a[0] > 'h' || a[1] < '1' || a[1] > '8' ||
           c[0] < 'a' || c[0] > 'h' || c[1] < '1' || c[1] > '8'){
            cout<<"ERROR: Invalid square notation (use a1-h8)\n";
            continue;
        }

        try{
            auto s=conv(a), e=conv(c);
            if(b.g[s.first][s.second]){
                cout<<"Moving "<<b.g[s.first][s.second]->name()<<" from "<<a<<" to "<<c<<"\n";
            }
            move(s.first,s.second,e.first,e.second,a,c);

            turn=!turn;
        }
        catch(exception &e){
            cout<<"ERROR: "<<e.what()<<endl;
        }
    }
}
