#include <iostream>
#include <windows.h>
#include "game.h"
using namespace std;

int main(){
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif
    Game g;
    g.play();
    return 0;
}

