#include "board.h"
#include "pieces.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

#define RESET       "\033[0m"
#define BOLD        "\033[1m"

// Piece colors
#define WHITE_PC    "\033[97m"   // bright white for white pieces
#define BLACK_PC    "\033[30m"   // black for black pieces

// Board square backgrounds (classic green & white)
#define LIGHT_SQ    "\033[47m"   // white
#define DARK_SQ     "\033[42m"   // green

// Check highlight  – vivid red background + bold white text
#define CHECK_BG    "\033[48;5;196m"  // bright red background
#define CHECK_FG    "\033[97;1m"      // bold bright white text

#define BORDER_CLR  "\033[38;5;245m" 
#define LABEL_CLR   "\033[38;5;214m"  

Board::Board() { init(); }

void Board::init(){
    g.assign(8, vector<Piece*>(8, nullptr));

    for(int i = 0; i < 8; i++){
        g[1][i] = new Pawn(false);
        g[6][i] = new Pawn(true);
    }

    g[0][0] = new Rook(false);   g[0][7] = new Rook(false);
    g[7][0] = new Rook(true);    g[7][7] = new Rook(true);

    g[0][1] = new Knight(false); g[0][6] = new Knight(false);
    g[7][1] = new Knight(true);  g[7][6] = new Knight(true);

    g[0][2] = new Bishop(false); g[0][5] = new Bishop(false);
    g[7][2] = new Bishop(true);  g[7][5] = new Bishop(true);

    g[0][3] = new Queen(false);
    g[7][3] = new Queen(true);

    g[0][4] = new King(false);
    g[7][4] = new King(true);
}

// ─── helper: print one cell (5 chars wide) ────────────────────
static void printCellTop(bool lightSquare, bool isCheckKing){
    string bg;
    if(isCheckKing)
        bg = CHECK_BG;
    else
        bg = lightSquare ? LIGHT_SQ : DARK_SQ;

    cout << bg << "     " << RESET;
}

static void printCellMid(Piece* p, bool lightSquare, bool isCheckKing){
    string bg;
    string fg;

    if(isCheckKing){
        bg = CHECK_BG;
        fg = CHECK_FG;
    } else {
        bg = lightSquare ? LIGHT_SQ : DARK_SQ;
        fg = (p && p->color()) ? WHITE_PC : BLACK_PC;
    }

    if(p){
        cout << bg << BOLD << fg << "  " << p->symbol() << "  " << RESET;
    } else {
        cout << bg << "     " << RESET;
    }
}

static bool isCheckKingAt(Piece* p, bool whiteInCheck, bool blackInCheck){
    if(!p) return false;
    King* k = dynamic_cast<King*>(p);
    if(!k) return false;
    if(k->color() && whiteInCheck) return true;
    if(!k->color() && blackInCheck) return true;
    return false;
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

    cout << "\n";

    cout << LABEL_CLR << "      a    b    c    d    e    f    g    h" << RESET << "\n";

    for(int i = 0; i < 8; i++){
    
        cout << "    ";
        for(int j = 0; j < 8; j++){
            bool light = (i + j) % 2 == 0;
            bool ck = isCheckKingAt(g[i][j], whiteInCheck, blackInCheck);
            printCellTop(light, ck);
        }
        cout << "\n";

        cout << LABEL_CLR << " " << (8 - i) << "  " << RESET;
        for(int j = 0; j < 8; j++){
            bool light = (i + j) % 2 == 0;
            bool ck = isCheckKingAt(g[i][j], whiteInCheck, blackInCheck);
            printCellMid(g[i][j], light, ck);
        }
        cout << LABEL_CLR << "  " << (8 - i) << RESET << "\n";

        cout << "    ";
        for(int j = 0; j < 8; j++){
            bool light = (i + j) % 2 == 0;
            bool ck = isCheckKingAt(g[i][j], whiteInCheck, blackInCheck);
            printCellTop(light, ck);
        }
        cout << "\n";
    }

    // Column labels on bottom
    cout << LABEL_CLR << "      a    b    c    d    e    f    g    h" << RESET << "\n";

    // Check warnings
    if(whiteInCheck)
        cout << "\n" << CHECK_BG << CHECK_FG << " ⚠  WHITE KING IS IN CHECK! " << RESET << "\n";
    if(blackInCheck)
        cout << "\n" << CHECK_BG << CHECK_FG << " ⚠  BLACK KING IS IN CHECK! " << RESET << "\n";
}

pair<int,int> Board::findKing(bool col){
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(g[i][j] && dynamic_cast<King*>(g[i][j]) && g[i][j]->color() == col)
                return {i, j};
    return {-1, -1};
}

bool Board::check(bool col){
    auto k = findKing(col);
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(g[i][j] && g[i][j]->color() != col)
                if(g[i][j]->valid(i, j, k.first, k.second, g))
                    return true;
    return false;
}

Board::~Board(){
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(g[i][j]) delete g[i][j];
}
