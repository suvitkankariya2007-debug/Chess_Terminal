#include "utils.h"
#include "piece.h"

bool clearPath(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b){
    int dx = (ex-sx)==0?0:(ex-sx)/abs(ex-sx);
    int dy = (ey-sy)==0?0:(ey-sy)/abs(ey-sy);
    int x=sx+dx,y=sy+dy;
    while(x!=ex || y!=ey){
        if(b[x][y]) return false;
        x+=dx; y+=dy;
    }
    return true;
}
