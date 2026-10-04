# Terminal Chess

A terminal-based chess application written in C++ to practice object-oriented design, move validation, game-state management, and file I/O.

## Overview

Terminal Chess implements a playable two-player chess board inside the command line. Each chess piece owns its movement rules, while the board and game classes handle board state, turn management, move history, undo, saving/loading, promotion, and check detection.

The project was built as a first-year C++ project and is intentionally lightweight: no external chess engine or GUI framework is required.

## Implemented features

- 8x8 terminal chess board with Unicode chess pieces
- separate movement validation for Pawn, Rook, Knight, Bishop, Queen, and King
- path-clearance checks for sliding pieces
- alternating White/Black turns
- same-color destination protection
- captured-piece tracking
- pawn promotion to Queen, Rook, Bishop, or Knight
- undo using a stack of move states
- move-history display
- save game history to `save.txt`
- reload saved move history and reconstruct the board
- king-location lookup and check detection
- highlighted checked king in the terminal
- input validation and custom exception handling
- ANSI terminal styling with Windows UTF-8 handling

## Design

### Piece hierarchy

```text
Piece
├── Pawn
├── Rook
├── Knight
├── Bishop
├── Queen
└── King
```

Each derived piece implements its own `valid(...)` movement rule.

### Main components

```text
Chess_Terminal/
├── chess.cpp       # application entry point
├── board.h/.cpp    # board initialization, rendering and check detection
├── game.h/.cpp     # game loop, moves, undo, save/load and promotion
├── piece.h         # base Piece abstraction
├── pieces.h        # concrete piece movement rules
└── utils.*         # shared movement/path utilities
```

## Example interaction

Moves use standard square coordinates:

```text
White move (format: e2 e4) or (undo/save/load/exit): e2 e4
Black move (format: e2 e4) or (undo/save/load/exit): e7 e5
```

Supported commands:

```text
undo
save
load
exit
```

## Building

With a C++ compiler:

```bash
g++ chess.cpp board.cpp game.cpp utils.cpp -std=c++17 -o chess
./chess
```

On Windows:

```powershell
g++ chess.cpp board.cpp game.cpp utils.cpp -std=c++17 -o chess.exe
.\chess.exe
```

## What this project demonstrates

- object-oriented programming in C++
- inheritance and polymorphism
- dynamic dispatch for piece-specific rules
- 2D board-state representation
- stack-based undo
- file I/O
- exception handling
- terminal rendering
- separation of game, board, piece, and utility responsibilities

## Known limitations

This is a learning project, not a complete tournament-rules chess engine.

The current implementation does **not** fully enforce every chess rule. In particular, features such as castling, en passant, checkmate/stalemate detection, and full prevention of moves that leave the moving side's own king in check would need additional logic.

Documenting these limitations is intentional so the repository accurately reflects the current implementation.

## Possible next steps

- add checkmate and stalemate detection
- prevent moves that leave the player's own king in check
- implement castling and en passant
- add unit tests for each piece's movement rules
- replace raw piece pointers with safer ownership constructs
- remove committed executable binaries from the repository
- add a computer opponent as a separate extension

## Repository

**GitHub:** https://github.com/suvitkankariya2007-debug/Chess_Terminal
