#pragma once

#include "types.h"
#include "move.h"

// Forward declarations
struct Board;

namespace Search {
    constexpr int Infinity = 1000000;
    constexpr int Checkmate = Infinity - 1;

    struct SearchResult {
        Move bestMove;
        int score;
    };

    int alphaBeta(Board& b, int depth, int alpha, int beta);
    SearchResult findBestMove(Board& b, int depth);
}
