#include "../src/board.h"
#include "../src/fen.h"
#include "../src/movegen.h"
#include "../src/move.h"
#include "../src/make_move.h"
#include <cassert>
#include <iostream>
#include <vector>

bool boards_equal(const Board& b1, const Board& b2) {
    for (int i = 0; i < 64; ++i) {
        if (b1.squares[i] != b2.squares[i]) return false;
    }
    return b1.sideToMove == b2.sideToMove &&
           b1.castlingRights == b2.castlingRights &&
           b1.enPassantSquare == b2.enPassantSquare &&
           b1.halfmoveClock == b2.halfmoveClock &&
           b1.fullmoveNumber == b2.fullmoveNumber;
}

void test_make_unmake() {
    std::cout << "Testing make/unmake..." << std::endl;
    
    Board b;
    FEN::loadFEN(b, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    
    Board copy = b;
    std::vector<Move> moves;
    MoveGen::generatePseudoLegalMoves(b, moves);
    MoveGen::filterLegalMoves(b, moves);

    for (const auto& move : moves) {
        UndoInfo undo;
        makeMove(b, move, undo);
        unmakeMove(b, move, undo);
        assert(boards_equal(b, copy));
    }
    
    std::cout << "Make/unmake test passed." << std::endl;
}

int main() {
    test_make_unmake();
    return 0;
}
