#include "../src/board.h"
#include "../src/fen.h"
#include "../src/search.h"
#include <cassert>
#include <iostream>

void test_search_winning_capture() {
    std::cout << "Testing search for winning capture..." << std::endl;
    
    Board b;
    FEN::loadFEN(b, "k7/8/8/8/8/8/8/qK6 w - - 0 1"); // White king at b1, black queen at a1
    
    Search::SearchResult result = Search::findBestMove(b, 2);
    
    assert(result.bestMove.from == 1 && result.bestMove.to == 0); // Kb1xQa1

    std::cout << "Search winning capture test passed." << std::endl;
}

int main() {
    test_search_winning_capture();
    return 0;
}
