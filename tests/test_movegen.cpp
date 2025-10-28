#include "../src/board.h"
#include "../src/fen.h"
#include "../src/movegen.h"
#include "../src/move.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_movegen_counts() {
    std::cout << "Testing move generation counts..." << std::endl;
    
    Board b;
    std::vector<Move> moves;

    // Position 1: Start position
    FEN::loadFEN(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MoveGen::generatePseudoLegalMoves(b, moves);
    assert(moves.size() == 20);

    // Position 2: After 1. e4
    FEN::loadFEN(b, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    MoveGen::generatePseudoLegalMoves(b, moves);
    assert(moves.size() == 20);
    
    std::cout << "Move generation count test passed." << std::endl;
}

int main() {
    test_movegen_counts();
    return 0;
}
