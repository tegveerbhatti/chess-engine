#pragma once

#include <string>

// Forward declaration
struct Board;

namespace FEN {
    bool loadFEN(Board& b, const std::string& fenStr);
    std::string boardToFEN(const Board& b);
}
