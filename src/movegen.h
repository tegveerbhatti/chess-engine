#pragma once

#include <vector>
#include "types.h"

// Forward declarations
struct Board;
struct Move;

namespace MoveGen {
    void generatePseudoLegalMoves(const Board& b, std::vector<Move>& movesOut);
    void filterLegalMoves(Board& b, std::vector<Move>& movesInOut);
    bool isSquareAttacked(const Board& b, int square, Color attackerColor);
}
