#include "ai.h"
#include "pieces.h"
#include <algorithm>
#include <iostream>

using namespace std;

// Evaluates the board and returns a score.
// A positive score means White is winning, negative means Black is winning.
int AI::evaluateBoard(Board& b, bool isWhite) {
    int score = 0;
    
    // Loop through every square on the 8x8 board
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            Piece* p = b.g[i][j];
            if(p){ // If there is a piece on this square
                int val = 0;
                
                // Assign a standard value to each piece type
                if(dynamic_cast<Pawn*>(p)) val = 100;
                else if(dynamic_cast<Knight*>(p)) val = 300;
                else if(dynamic_cast<Bishop*>(p)) val = 300;
                else if(dynamic_cast<Rook*>(p)) val = 500;
                else if(dynamic_cast<Queen*>(p)) val = 900;
                else if(dynamic_cast<King*>(p)) val = 9000;
                
                // Positional bonus: Give +10 points if the piece is in the center 4 squares
                if(i >= 3 && i <= 4 && j >= 3 && j <= 4) val += 10;
                
                // If the piece belongs to the player we are evaluating for, add its value.
                // Otherwise, subtract its value.
                if(p->color() == isWhite) score += val;
                else score -= val;
            }
        }
    }
    return score;
}

// Generates a list of all valid, legal moves for the given player color.
std::vector<Move> AI::generateAllMoves(Board& b, bool isWhite) {
    vector<Move> moves;
    
    // Iterate through all squares to find pieces of the given color
    for(int sx = 0; sx < 8; ++sx){
        for(int sy = 0; sy < 8; ++sy){
            Piece* p = b.g[sx][sy];
            
            if(p && p->color() == isWhite){
                // For each piece, check every possible destination square
                for(int ex = 0; ex < 8; ++ex){
                    for(int ey = 0; ey < 8; ++ey){
                        
                        // Check if the move is pseudo-legal (follows piece movement rules)
                        if(p->valid(sx, sy, ex, ey, b.g)){
                            // Make sure we are not capturing our own piece
                            if(!b.g[ex][ey] || b.g[ex][ey]->color() != isWhite) {
                                
                                // Simulate the move on the board
                                Piece* captured = b.applyMove(sx, sy, ex, ey);
                                
                                // A move is strictly legal ONLY IF it doesn't leave our King in check
                                if(!b.check(isWhite)){
                                    moves.push_back({sx, sy, ex, ey, captured, ""});
                                }
                                
                                // Undo the simulation to restore the board
                                b.undoMove(sx, sy, ex, ey, captured);
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}

// The core AI algorithm: Minimax with Alpha-Beta Pruning.
// It looks ahead 'depth' turns to find the best possible outcome.
int AI::minimax(Board& b, int depth, int alpha, int beta, bool isMaximizingPlayer, bool isWhite) {
    // Base case: If we've reached the maximum search depth, return the board's score
    if (depth == 0) return evaluateBoard(b, isWhite);

    bool currentPlayer = isMaximizingPlayer ? isWhite : !isWhite;
    vector<Move> moves = generateAllMoves(b, currentPlayer);
    
    // Check for game over states (Checkmate or Stalemate)
    if (moves.empty()) {
        if (b.check(currentPlayer)) {
            return isMaximizingPlayer ? -99999 : 99999; // Checkmate: huge penalty
        } else {
            return 0; // Stalemate: draw is neutral
        }
    }

    // Set starting best score depending on if we are maximizing or minimizing
    int bestEval = isMaximizingPlayer ? -100000 : 100000;

    // Try every possible move
    for (Move& m : moves) {
        Piece* captured = b.applyMove(m.sx, m.sy, m.ex, m.ey);
        Piece* promotedTo = nullptr;
        
        // Handle Pawn Promotion during simulation (auto-promote to Queen for simplicity)
        if (dynamic_cast<Pawn*>(b.g[m.ex][m.ey]) && (m.ex == 0 || m.ex == 7)) {
            promotedTo = b.g[m.ex][m.ey];
            b.g[m.ex][m.ey] = new Queen(currentPlayer);
        }

        // Recursively call minimax for the next turn (switch maximizing player)
        int eval = minimax(b, depth - 1, alpha, beta, !isMaximizingPlayer, isWhite);
        
        // Undo the promotion and move
        if (promotedTo) {
            delete b.g[m.ex][m.ey];
            b.g[m.ex][m.ey] = promotedTo;
        }
        b.undoMove(m.sx, m.sy, m.ex, m.ey, captured);
        
        // Update the best evaluation and alpha/beta values
        if (isMaximizingPlayer) {
            bestEval = max(bestEval, eval);
            alpha = max(alpha, eval);
        } else {
            bestEval = min(bestEval, eval);
            beta = min(beta, eval);
        }
        
        // Alpha-Beta Pruning: If we found a move that is "too good", the opponent will 
        // avoid this branch anyway, so we can stop searching here.
        if (beta <= alpha) break;
    }
    
    return bestEval;
}

// Entry point for the AI to pick the best move.
Move AI::getBestMove(Board& b, bool isWhite, int depth) {
    int bestVal = -100000;
    Move bestMove = {-1, -1, -1, -1, nullptr, ""};
    
    vector<Move> moves = generateAllMoves(b, isWhite);
    if(moves.empty()) return bestMove;
    
    // Evaluate all root moves and pick the one with the highest minimax score
    for (Move& m : moves) {
        Piece* captured = b.applyMove(m.sx, m.sy, m.ex, m.ey);
        Piece* promotedTo = nullptr;
        
        if (dynamic_cast<Pawn*>(b.g[m.ex][m.ey]) && (m.ex == 0 || m.ex == 7)) {
            promotedTo = b.g[m.ex][m.ey];
            b.g[m.ex][m.ey] = new Queen(isWhite);
        }
        
        // Start the recursive search (opponent's turn, so isMaximizingPlayer = false)
        int moveVal = minimax(b, depth - 1, -100000, 100000, false, isWhite);
        
        if (promotedTo) {
            delete b.g[m.ex][m.ey];
            b.g[m.ex][m.ey] = promotedTo;
        }
        b.undoMove(m.sx, m.sy, m.ex, m.ey, captured);
        
        // If this move leads to a better outcome, remember it
        if (moveVal > bestVal) {
            bestVal = moveVal;
            bestMove = m;
        }
    }
    
    // Format the move into standard string notation (e.g., e2->e4)
    if (bestMove.sx != -1) {
        char colS = 'a' + bestMove.sy;
        char rowS = '8' - bestMove.sx;
        char colE = 'a' + bestMove.ey;
        char rowE = '8' - bestMove.ex;
        bestMove.notation = string(1, colS) + string(1, rowS) + "->" + string(1, colE) + string(1, rowE);
    }
    
    return bestMove;
}
