#include "board.h"
#include "pieces.h"
#include <iostream>

// If running on Windows, include Windows-specific headers for console manipulation
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// ANSI escape codes to reset terminal colors back to default
#define RESET       "\033[0m"
#define BOLD        "\033[1m"

// Define the colors for the pieces themselves
#define WHITE_PC    "\033[97;1m" // bold bright white for white pieces
#define BLACK_PC    "\033[30m"   // solid black for black pieces

// Define the background colors for the board squares
#define LIGHT_SQ    "\033[48;5;245m" // grey background for light squares
#define DARK_SQ     "\033[42m"       // green background for dark squares

// Define the vivid red highlighting used when a King is in check
#define CHECK_BG    "\033[48;5;196m"  // bright red background
#define CHECK_FG    "\033[97;1m"      // bold bright white text for warning

// Colors used for the coordinate labels (a-h, 1-8) around the board
#define BORDER_CLR  "\033[38;5;245m" 
#define LABEL_CLR   "\033[38;5;214m"  

// Constructor: automatically initializes the board when a Board object is created
Board::Board() { init(); }

// Initializes the board with the standard starting position
void Board::init(){
    // Create an 8x8 2D vector (grid) filled with nullptrs (empty squares)
    g.assign(8, vector<Piece*>(8, nullptr));

    // Place all 8 pawns for both black (row 1) and white (row 6)
    for(int i = 0; i < 8; i++){
        g[1][i] = new Pawn(false); // Black pawns
        g[6][i] = new Pawn(true);  // White pawns
    }

    // Place Rooks in the corners
    g[0][0] = new Rook(false);   g[0][7] = new Rook(false);
    g[7][0] = new Rook(true);    g[7][7] = new Rook(true);

    // Place Knights next to the Rooks
    g[0][1] = new Knight(false); g[0][6] = new Knight(false);
    g[7][1] = new Knight(true);  g[7][6] = new Knight(true);

    // Place Bishops next to the Knights
    g[0][2] = new Bishop(false); g[0][5] = new Bishop(false);
    g[7][2] = new Bishop(true);  g[7][5] = new Bishop(true);

    // Place Queens on their own color
    g[0][3] = new Queen(false);
    g[7][3] = new Queen(true);

    // Place Kings next to the Queens
    g[0][4] = new King(false);
    g[7][4] = new King(true);
}

// ─── helper function: prints the top empty margin of a single square ───
static void printCellTop(bool lightSquare, bool isCheckKing){
    string bg;
    // If this square contains a king in check, paint the background red
    if(isCheckKing)
        bg = CHECK_BG;
    else // Otherwise, paint it the standard light/dark background color
        bg = lightSquare ? LIGHT_SQ : DARK_SQ;

    // Print 5 spaces to create the top vertical padding for the square
    cout << bg << "     " << RESET;
}

// ─── helper function: prints the middle row of a square (which holds the piece) ───
static void printCellMid(Piece* p, bool lightSquare, bool isCheckKing){
    string bg;
    string fg;

    // Determine the background and foreground colors
    if(isCheckKing){
        bg = CHECK_BG;
        fg = CHECK_FG;
    } else {
        bg = lightSquare ? LIGHT_SQ : DARK_SQ;
        // If there's a piece, determine its color. If not, fallback to black.
        fg = (p && p->color()) ? WHITE_PC : BLACK_PC;
    }

    // If a piece exists on this square, print it surrounded by spaces
    if(p){
        cout << bg << BOLD << fg << "  " << p->symbol() << "  " << RESET;
    } else {
        // If the square is empty, print 5 spaces
        cout << bg << "     " << RESET;
    }
}

// ─── helper function: checks if a specific piece is a King that is currently in check ───
static bool isCheckKingAt(Piece* p, bool whiteInCheck, bool blackInCheck){
    if(!p) return false; // Not even a piece
    
    // Try to dynamically cast the piece to a King. If it fails, it's not a King.
    King* k = dynamic_cast<King*>(p);
    if(!k) return false;
    
    // If it's a White King and White is in check, return true.
    if(k->color() && whiteInCheck) return true;
    
    // If it's a Black King and Black is in check, return true.
    if(!k->color() && blackInCheck) return true;
    
    return false;
}

// Draws the entire chess board to the terminal
void Board::show(){
    // Clear the terminal screen depending on the operating system
    #ifdef _WIN32
        system("cls");
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #else
        system("clear");
    #endif

    // Before drawing, determine if either king is currently in check
    whiteInCheck = check(true);
    blackInCheck = check(false);

    cout << "\n";

    // Print the top column labels (a, b, c, d, e, f, g, h)
    cout << LABEL_CLR << "      a    b    c    d    e    f    g    h" << RESET << "\n";

    // Loop through all 8 rows of the board
    for(int i = 0; i < 8; i++){
    
        // 1. Draw the top padding for all 8 squares in this row
        cout << "    ";
        for(int j = 0; j < 8; j++){
            // A square is light if the sum of its coordinates is even
            bool light = (i + j) % 2 == 0;
            bool ck = isCheckKingAt(g[i][j], whiteInCheck, blackInCheck);
            printCellTop(light, ck);
        }
        cout << "\n";

        // 2. Draw the middle row (which contains the actual pieces)
        // Print the left-side row number label (8 down to 1)
        cout << LABEL_CLR << " " << (8 - i) << "  " << RESET;
        for(int j = 0; j < 8; j++){
            bool light = (i + j) % 2 == 0;
            bool ck = isCheckKingAt(g[i][j], whiteInCheck, blackInCheck);
            printCellMid(g[i][j], light, ck);
        }
        // Print the right-side row number label
        cout << LABEL_CLR << "  " << (8 - i) << RESET << "\n";

        // 3. Draw the bottom padding for all 8 squares in this row
        cout << "    ";
        for(int j = 0; j < 8; j++){
            bool light = (i + j) % 2 == 0;
            bool ck = isCheckKingAt(g[i][j], whiteInCheck, blackInCheck);
            printCellTop(light, ck);
        }
        cout << "\n";
    }

    // Print the bottom column labels
    cout << LABEL_CLR << "      a    b    c    d    e    f    g    h" << RESET << "\n";

    // If a king is in check, print a massive, unmissable red warning below the board
    if(whiteInCheck)
        cout << "\n" << CHECK_BG << CHECK_FG << " ⚠  WHITE KING IS IN CHECK! " << RESET << "\n";
    if(blackInCheck)
        cout << "\n" << CHECK_BG << CHECK_FG << " ⚠  BLACK KING IS IN CHECK! " << RESET << "\n";
}

// Finds the coordinates (row, col) of the King belonging to the specified color
pair<int,int> Board::findKing(bool col){
    // Scan the whole board
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            // If we find a piece, and it's a King, and its color matches what we're looking for
            if(g[i][j] && dynamic_cast<King*>(g[i][j]) && g[i][j]->color() == col)
                return {i, j}; // Return its coordinates
                
    // Return a dummy coordinate if not found (should theoretically never happen)
    return {-1, -1};
}

// Determines if the player of the specified color is currently in check
bool Board::check(bool col){
    // First, locate the King we want to protect
    auto k = findKing(col);
    
    // Scan the entire board for enemy pieces
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            // If we found a piece belonging to the OPPONENT
            if(g[i][j] && g[i][j]->color() != col)
                // Check if this enemy piece has a valid move targeting our King
                if(g[i][j]->valid(i, j, k.first, k.second, g))
                    return true; // The King is in check!
                    
    // No enemy piece can attack the King, so we are safe
    return false;
}

// A high-speed function used heavily by the AI to simulate a move without deep-copying the board
Piece* Board::applyMove(int sx, int sy, int ex, int ey) {
    // Save any piece that might be captured on the destination square
    Piece* captured = g[ex][ey];
    // Move the piece from start to destination
    g[ex][ey] = g[sx][sy];
    // Empty the starting square
    g[sx][sy] = nullptr;
    
    // Return the captured piece so we can restore it later if we want to undo
    return captured;
}

// Quickly reverses a simulated move, putting pieces back exactly as they were
void Board::undoMove(int sx, int sy, int ex, int ey, Piece* captured) {
    // Move the piece back to its starting square
    g[sx][sy] = g[ex][ey];
    // Restore the piece that was captured (if any) back onto the destination square
    g[ex][ey] = captured;
}

// Destructor: Cleans up the dynamically allocated pieces from memory when the game ends
Board::~Board(){
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            // If a piece exists here, delete it to prevent memory leaks
            if(g[i][j]) delete g[i][j];
}
