#include "../src/board.h"
#include "../src/fen.h"
#include "../src/movegen.h"
#include "../src/search.h"
#include <chrono>
#include <iostream>
#include <vector>

void benchmark_move_generation() {
    std::cout << "=== Move Generation Benchmark ===" << std::endl;
    
    std::vector<std::string> test_positions = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", // Starting position
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", // Complex middle game
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", // Endgame position
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", // Tactical position
    };
    
    int total_moves = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    const int iterations = 10000; // Run each position 10k times
    
    for (int i = 0; i < iterations; ++i) {
        for (const auto& fen : test_positions) {
            Board b;
            FEN::loadFEN(b, fen);
            
            std::vector<Move> moves;
            MoveGen::generatePseudoLegalMoves(b, moves);
            MoveGen::filterLegalMoves(b, moves);
            
            total_moves += moves.size();
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    double seconds = duration.count() / 1000000.0;
    double moves_per_second = total_moves / seconds;
    
    std::cout << "Total moves generated: " << total_moves << std::endl;
    std::cout << "Time taken: " << seconds << " seconds" << std::endl;
    std::cout << "Moves per second: " << static_cast<int>(moves_per_second) << std::endl;
    std::cout << "Average moves per position: " << total_moves / (iterations * test_positions.size()) << std::endl;
}

void benchmark_search_performance() {
    std::cout << "\n=== Search Performance Benchmark ===" << std::endl;
    
    Board b;
    FEN::loadFEN(b, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    
    const int search_iterations = 100;
    int total_positions_evaluated = 0;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < search_iterations; ++i) {
        Search::SearchResult result = Search::findBestMove(b, 3); // Depth 3 for faster benchmark
        // Note: We'd need to modify search to count positions, but this gives us timing
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    double seconds = duration.count() / 1000.0;
    double searches_per_second = search_iterations / seconds;
    
    std::cout << "Search iterations: " << search_iterations << std::endl;
    std::cout << "Time taken: " << seconds << " seconds" << std::endl;
    std::cout << "Searches per second: " << static_cast<int>(searches_per_second) << std::endl;
    std::cout << "Average time per search: " << (seconds * 1000 / search_iterations) << " ms" << std::endl;
}

int main() {
    std::cout << "Chess Engine Performance Benchmark\n" << std::endl;
    
    benchmark_move_generation();
    benchmark_search_performance();
    
    return 0;
}
