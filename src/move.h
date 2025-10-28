#pragma once

#include "types.h"
#include <string>

// 4.5 Moves
enum MoveFlags : uint8_t {
    QUIET            = 0,
    CAPTURE          = 1 << 0,
    DOUBLE_PAWN_PUSH = 1 << 1,
    KING_CASTLE      = 1 << 2,
    QUEEN_CASTLE     = 1 << 3,
    EN_PASSANT       = 1 << 4,
    PROMOTION        = 1 << 5
};

struct Move {
    uint8_t from;
    uint8_t to;
    uint8_t flags;
    PieceType promo;
};

// Utility functions
std::string moveToString(const Move& m);
inline bool isCapture(const Move& m) { return m.flags & CAPTURE; }
inline bool isCastle(const Move& m) { return m.flags & (KING_CASTLE | QUEEN_CASTLE); }
