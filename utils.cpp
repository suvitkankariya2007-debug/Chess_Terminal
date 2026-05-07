// Include our custom utility header which declares this clearPath function
#include "utils.h"
// Include piece.h so we know what a Piece object is
#include "piece.h"

// This function checks if the straight path between a start square (sx, sy) 
// and an end square (ex, ey) is clear of any pieces. It's used by Rooks, Bishops, and Queens.
bool clearPath(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
    
    // Calculate the horizontal direction (dx): 
    // -1 if moving up, 1 if moving down, 0 if staying on same row
    int dx = (ex-sx)==0 ? 0 : (ex-sx)/abs(ex-sx);
    
    // Calculate the vertical direction (dy): 
    // -1 if moving left, 1 if moving right, 0 if staying on same column
    int dy = (ey-sy)==0 ? 0 : (ey-sy)/abs(ey-sy);
    
    // Start checking from the square immediately *after* the starting square
    int x = sx + dx;
    int y = sy + dy;
    
    // Continue moving along the path until we reach the destination square
    while(x != ex || y != ey){
        // If we find ANY piece on our path before reaching the destination, the path is blocked
        if(b[x][y]) return false;
        
        // Move one step further in the calculated direction
        x += dx; 
        y += dy;
    }
    
    // If the loop finished without finding any pieces, the path is perfectly clear!
    return true;
}
