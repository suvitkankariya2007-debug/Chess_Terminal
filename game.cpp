#include "game.h"
#include "pieces.h"
#include "ai.h"
#include <iostream>
#include <fstream>
#include <exception>
using namespace std;

// ANSI escape codes for bold text and resetting styles
#define BOLD "\033[1m"
#define RESET "\033[0m"

// A custom exception class used to cleanly handle illegal move attempts.
// Whenever a user tries something invalid, we throw this, catch it, and print the message.
class ChessException : public exception {//generic
    string msg;
public:
    // Constructor takes the error message
    ChessException(string m):msg(m){}
    // Overriding what() to return our custom error message as a C-string
    const char* what() const noexcept override { return msg.c_str(); }
};

// Constructor for the Game class. We always start with White's turn (true).
Game::Game() : turn(true) { }

// Converts a standard chess notation string (like "e2") into a 2D array coordinate pair.
// For example: 'a' becomes column 0. '8' becomes row 0.
pair<int,int> Game::conv(string s){
    // Row math: 8 - ('2' - '0') = 8 - 2 = 6 (Row index 6)
    // Col math: 'e' - 'a' = 4 (Column index 4)
    return {8-(s[1]-'0'), s[0]-'a'};
}

// Undoes the last move played in the game
void Game::undo(){
    // If the history stack is empty, there is nothing to undo
    if(history.empty()){ cout<<"No undo\n"; return; }
    
    // Pop the most recent move off the stack
    Move m = history.top(); history.pop();

    // Move the piece from its destination back to its start
    b.g[m.sx][m.sy] = b.g[m.ex][m.ey];
    // Put whatever piece was captured (if any) back onto the destination square
    b.g[m.ex][m.ey] = m.captured;

    // Remove the string notation of the move from our history list
    moves.pop_back();
    
    // Switch the turn back to the previous player
    turn = !turn;
}

// Checks if a Pawn has reached the end of the board and asks what to promote it to
void Game::promote(int x,int y){
    // First, check if the piece at (x,y) is actually a Pawn
    if(dynamic_cast<Pawn*>(b.g[x][y])){
        // Row 0 is the top (White promotion), Row 7 is the bottom (Black promotion)
        if(x == 0 || x == 7){
            cout<<"Promote to (Q/R/B/N): ";
            char ch; cin>>ch;
            
            // Delete the old pawn from memory to prevent memory leaks
            delete b.g[x][y];
            
            // Create a new piece of the selected type in its place, keeping the same color
            if(ch=='Q') b.g[x][y] = new Queen(turn);
            else if(ch=='R') b.g[x][y] = new Rook(turn);
            else if(ch=='B') b.g[x][y] = new Bishop(turn);
            else b.g[x][y] = new Knight(turn);
        }
    }
}

// Validates and executes a move on the board
bool Game::move(int sx,int sy,int ex,int ey,string a,string c){
    // ERROR: The starting square is empty, or the piece belongs to the opponent
    if(!b.g[sx][sy] || b.g[sx][sy]->color()!=turn)
        throw ChessException("Invalid selection");//opponent piece touvched

    // ERROR: Trying to move on top of your own piece
    if(b.g[ex][ey] && b.g[ex][ey]->color()==turn)
        throw ChessException("Destination occupied by own piece");

    // ERROR: The specific piece logic (e.g. Knight L-shape) says this move is mathematically illegal
    if(!b.g[sx][sy]->valid(sx,sy,ex,ey,b.g))
        throw ChessException("Invalid move for this piece");

    // Simulate the move to see if it leaves the player's own King in check
    Piece* captured = b.applyMove(sx, sy, ex, ey);
    bool inCheckAfterMove = b.check(turn);
    b.undoMove(sx, sy, ex, ey, captured);

    // ERROR: The move exposes the King to capture, which is strictly illegal
    if (inCheckAfterMove) {
        throw ChessException("Move leaves your King in check! You must protect the King.");
    }

    // Record this move in a Move struct so we can undo it later if needed
    // The notation format is e.g., "e2->e4"
    Move m = {sx, sy, ex, ey, b.g[ex][ey], a + "->" + c};

    // Execute the move by shifting pointers in the 2D array
    b.g[ex][ey] = b.g[sx][sy];
    b.g[sx][sy] = nullptr;

    // After moving, check if we triggered a pawn promotion
    promote(ex,ey);

    // Push the move object to the stack (for undo functionality)
    history.push(m);
    // Add the move notation to the string list (for display and saving)
    moves.push_back(m.notation);

    return true;
}

// Saves the current game by writing the list of move strings to a text file
void Game::save(){
    ofstream f("save.txt"); // Open file for writing
    for(auto &m : moves) f << m << "\n"; // Write each move separated by a newline
    cout<<"Game saved\n";
}

// Loads a saved game by replaying all moves from a text file
void Game::load(){
    ifstream f("save.txt"); // Open file for reading
    if(!f){ cout<<"No save\n"; return; } // If file doesn't exist, abort
    
    // Reset the board to the starting state
    b.init();
    moves.clear();
    history = stack<Move>();

    string mv;
    // Read the file line by line
    while(getline(f,mv)){
        // Extract start coordinate (e.g. "e2") and end coordinate (e.g. "e4") from "e2->e4"
        string a = mv.substr(0,2), c = mv.substr(3,2);
        
        // Convert strings to array coordinates
        auto s = conv(a), e = conv(c);
        
        // Execute the move without validation (assuming the save file is valid)
        move(s.first, s.second, e.first, e.second, a, c);
        
        // Alternate turns for every move loaded
        turn = !turn;
    }
    cout<<"Game loaded\n";
}

// Prints the formatted history of all moves played so far
void Game::showHistory(){
    cout<<"\n== Moves:\n";
    // We iterate by 2 so we can print White and Black moves side-by-side on the same line
    for(int i = 0; i < moves.size(); i += 2){
        // Print White's move
        cout << (i/2)+1 << ". White: " << moves[i];
        // If Black has responded, print Black's move
        if (i+1 < moves.size()) cout << "  |  Black: " << moves[i+1];
        cout << endl;
    }
    
    cout<<"\n== Board Status:\n";
    // If either king is in check, print a warning in the history log
    if(b.whiteInCheck) cout<<BOLD<<"-> White King is in CHECK!"<<RESET<<"\n";
    if(b.blackInCheck) cout<<BOLD<<"-> Black King is in CHECK!"<<RESET<<"\n";
}

// The core interactive game loop
void Game::play(){
    // Set up the pieces
    b.init();
    
    int mode = 0;
    // Mode Selection Loop: Force the user to pick PvP or PvE
    while (true) {
        cout << "Select Mode:\n1) Player vs Player\n2) Player vs AI (You are White)\nChoice: ";
        // Only break the loop if the input is an integer and is exactly 1 or 2
        if(cin >> mode && (mode == 1 || mode == 2)) {
            break;
        }
        // If they type a letter or an invalid number, print an error and clear the input buffer
        cout << "\n\033[48;5;196m\033[97;1m >>> ERROR: Invalid mode! Please enter 1 or 2. <<< \033[0m\n\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    // Main Game Loop
    while(true){
        // 1. Draw the board and history
        b.show();
        showHistory();
        
        // 1.5. Check for Checkmate or Stalemate
        // We use the AI's move generator to see if the current player has ANY legal moves left
        vector<Move> validMoves = AI::generateAllMoves(b, turn);
        if (validMoves.empty()) {
            // If there are no legal moves AND the king is in check, it's Checkmate
            if (b.check(turn)) {
                string winner = turn ? "BLACK" : "WHITE";
                cout << "\n\033[48;5;196m\033[97;1m     >>> CHECKMATE! " << winner << " WINS! <<<     \033[0m\n\n";
            } 
            // If there are no legal moves but the king is NOT in check, it's Stalemate (Draw)
            else {
                cout << "\n\033[48;5;208m\033[97;1m     >>> STALEMATE! THE GAME IS A DRAW! <<<     \033[0m\n\n";
            }
            // End the game
            break;
        }
        
        // 2. Handle AI's turn if playing PvE and it is Black's turn
        if (mode == 2 && !turn) {
            cout << "\nAI is thinking...\n";
            // Ask the AI to calculate the best move looking ahead 4 turns (Depth 4)
            Move aiMove = AI::getBestMove(b, false, 4); 
            
            // If the AI found a move, execute it
            if (aiMove.sx != -1) {
                // Extract coordinates from the AI's move notation
                string a = aiMove.notation.substr(0, 2);
                string c = aiMove.notation.substr(4, 2);
                cout << "AI plays " << a << " to " << c << "\n";
                
                try {
                    // Force the move onto the board
                    move(aiMove.sx, aiMove.sy, aiMove.ex, aiMove.ey, a, c);
                    turn = !turn; // Switch turn back to player
                } catch(exception &e) {
                    // Should theoretically never happen, but catches AI bugs
                    cout << "AI ERROR: " << e.what() << endl;
                    break;
                }
            } else {
                // If the AI has no valid moves, it's either checkmate or stalemate
                cout << "AI has no valid moves.\n";
                break;
            }
            // Skip the human input section and redraw the board
            continue; 
        }

        // 3. Handle Human Player's turn
        string player = turn ? "White" : "Black";
        cout<<"\n"<<player<<" move (format: e2 e4) or (undo/save/load/exit): ";
        
        string a,c;
        // Read the first input word. If reading fails (e.g., EOF), break out of the game.
        if(!(cin >> a)) break;

        // Check for special text commands
        if(a=="undo"){ undo(); continue; }
        if(a=="save"){ save(); continue; }
        if(a=="load"){ load(); continue; }
        if(a=="exit") break;

        // If it wasn't a command, we expect a second coordinate (the destination)
        if(!(cin >> c)){
            cout<<"ERROR: Invalid input format. Use: e2 e4\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // Validate that both coordinates are exactly 2 characters long and fall within a1-h8
        if(a.length() != 2 || c.length() != 2 || 
           a[0] < 'a' || a[0] > 'h' || a[1] < '1' || a[1] > '8' ||
           c[0] < 'a' || c[0] > 'h' || c[1] < '1' || c[1] > '8'){
            cout<<"ERROR: Invalid square notation (use a1-h8)\n";
            continue;
        }

        // Try to execute the move
        try{
            // Convert string coordinates (e.g. "e2") to numeric array indices (e.g. [6][4])
            auto s=conv(a), e=conv(c);
            
            // Perform the move logic. This will throw an exception if the move is illegal!
            move(s.first, s.second, e.first, e.second, a, c);

            // Switch the turn if the move was successful
            turn=!turn;
        }
        // If the move was illegal, catch the specific error
        catch(exception &e){
            // Print a giant red error message explaining WHY the move failed
            cout<<"\n\033[48;5;196m\033[97;1m >>> WRONG MOVE: "<<e.what()<<" <<< "<<RESET<<"\n";
            
            // Pause the game until the user presses Enter, otherwise the screen 
            // clears instantly, and they won't see the error message.
            cout<<"Press Enter to continue...";
            cin.ignore(10000, '\n'); // Clear the buffer (in case they mashed keys)
            cin.get();               // Wait for an Enter key press
        }
    }
}
