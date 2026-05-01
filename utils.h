#ifndef UTILS_H
#define UTILS_H

#include <vector>
using namespace std;

class Piece;

bool clearPath(int sx,int sy,int ex,int ey, vector<vector<Piece*>> &b);

#endif
