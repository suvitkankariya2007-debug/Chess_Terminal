# Terminal Chess

**A two-player terminal chess application built to practice C++ object-oriented design.**

The project separates piece movement rules, board representation, game flow, and terminal utilities. It includes move history, undo, save/load, promotion, and check indication without an external chess engine or GUI framework.

[Game loop](game.cpp) · [Board](board.cpp) · [Piece interface](piece.h) · [Piece rules](pieces.h)

## Build and play

Use a C++17-capable compiler and a terminal that displays UTF-8 chess symbols and ANSI colors.

```bash
git clone https://github.com/suvitkankariya2007-debug/Chess_Terminal.git
cd Chess_Terminal
g++ chess.cpp board.cpp game.cpp utils.cpp -std=c++17 -o chess
./chess
```

On Windows with a configured C++ compiler:

```powershell
g++ chess.cpp board.cpp game.cpp utils.cpp -std=c++17 -o chess.exe
.\chess.exe
```

Build from the source files above. The executables already tracked in this repository are legacy build artifacts and may not match your platform or the current source.

## Interaction

White moves first. Enter source and destination squares using lowercase algebraic coordinates:

```text
e2 e4
e7 e5
```

| Input | Action |
| --- | --- |
| `e2 e4` | Request a move between two squares |
| `undo` | Restore the most recent move recorded in the history stack |
| `save` | Write move history to `save.txt` in the current working directory |
| `load` | Read saved move history and replay it onto the board |
| `exit` | Leave the game |
| `Q`, `R`, `B`, `N` | Promotion choice when prompted |

These examples show input syntax, not an automated test transcript. Save/load and undo have edge cases that need further validation, particularly after promotion and when loading during a game.

## Implemented behavior

- An 8x8 board and alternating White/Black turns.
- Piece-specific movement validation using inheritance and virtual dispatch.
- Path checks for sliding pieces and protection against capturing your own pieces.
- Move history and stack-based undo.
- File-based save/load of move notation.
- Pawn promotion and checked-king indication.
- Square-format validation and exception-based move error reporting.

## Design guide

| Component | Responsibility |
| --- | --- |
| [chess.cpp](chess.cpp) | Program entry point and Windows UTF-8 setup |
| [game.h](game.h) / [game.cpp](game.cpp) | Turn handling, input, move history, undo, persistence, promotion |
| [board.h](board.h) / [board.cpp](board.cpp) | Board setup, display, and check detection |
| [piece.h](piece.h) | Shared piece abstraction |
| [pieces.h](pieces.h) | Pawn, Rook, Knight, Bishop, Queen, and King movement rules |
| [utils.h](utils.h) / [utils.cpp](utils.cpp) | Shared utility code |

## What this project demonstrates

C++ inheritance and polymorphism, a 2D board-state model, stack-based state restoration, file I/O, exception handling, and separation of responsibilities in a small application.

## Rules and implementation boundaries

This is a learning project rather than a complete chess-rules engine. Castling, en passant, checkmate/stalemate detection, and complete prevention of moves that leave your own king in check are not implemented. A check indicator should not be interpreted as full legal-move enforcement.

Further work includes rule-completeness tests, promotion/undo and load-state fixes, safer ownership of piece objects, and a separate computer-opponent extension.
