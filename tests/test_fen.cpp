#include "../src/board.h"
#include "../src/fen.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_fen_roundtrip() {
    std::cout << "Testing FEN round-trip..." << std::endl;

    std::vector<std::string> fens = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    };

    for (const auto& fen_in : fens) {
        Board b;
        assert(FEN::loadFEN(b, fen_in));
        std::string fen_out = FEN::boardToFEN(b);
        assert(fen_in == fen_out);
    }
    std::cout << "FEN round-trip test passed." << std::endl;
}

int main() {
    test_fen_roundtrip();
    return 0;
}
