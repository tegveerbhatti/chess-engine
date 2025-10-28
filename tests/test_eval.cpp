#include "../src/board.h"
#include "../src/fen.h"
#include "../src/eval.h"
#include <cassert>
#include <iostream>

void test_eval_sanity() {
    std::cout << "Testing evaluation sanity..." << std::endl;
    
    Board b;

    // Startpos should be close to 0
    FEN::loadFEN(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int score1 = Eval::evaluate(b);
    assert(abs(score1) < 50); // Should be balanced

    // White up a queen
    FEN::loadFEN(b, "rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int score2 = Eval::evaluate(b);
    assert(score2 > 800 && score2 < 1000);
    
    // Black up a rook (White missing a rook)
    FEN::loadFEN(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/1NBQKBNR b KQkq - 0 1");
    int score3 = Eval::evaluate(b);
    assert(score3 > 400 && score3 < 600);


    std::cout << "Evaluation sanity test passed." << std::endl;
}

int main() {
    test_eval_sanity();
    return 0;
}
