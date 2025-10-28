#pragma once

#include "types.h"

// Forward declarations
struct Board;
struct Move;

// 7.1 Undo stack
struct UndoInfo {
    Piece capturedPiece;
    uint8_t oldCastlingRights;
    int oldEnPassantSquare;
    uint8_t oldHalfmoveClock;
    uint16_t oldFullmoveNumber;
    uint64_t oldZobristKey;
};

void makeMove(Board& b, const Move& m, UndoInfo& undoOut);
void unmakeMove(Board& b, const Move& m, const UndoInfo& undoIn);
