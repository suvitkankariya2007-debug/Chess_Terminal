// Include standard input/output stream library for printing to the console
#include <iostream>

// Check if the code is being compiled on a Windows operating system
#ifdef _WIN32
// Include the Windows API header, which provides functions like SetConsoleOutputCP
#include <windows.h>
#endif

// Include the Game header file, which contains the main Game class logic
#include "game.h"

// Use the standard namespace so we don't have to type std:: before everything
using namespace std;

// The main function where the program execution begins
int main(){
    
    // If running on Windows, we need to configure the console to support Unicode
    #ifdef _WIN32
        // Set the input code page to UTF-8 (65001) so it can read special characters
        SetConsoleCP(65001);
        // Set the output code page to UTF-8 (65001) so it can print the chess pieces correctly
        SetConsoleOutputCP(65001);
    #endif

    // Instantiate a new Game object. This sets up the board and pieces automatically.
    Game g;
    
    // Start the main game loop, which will handle turns, AI, and user input
    g.play();
    
    // Return 0 to indicate that the program finished executing successfully
    return 0;
}
