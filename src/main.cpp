#include "board.h"
#include "fen.h"
#include "movegen.h"
#include "make_move.h"
#include "search.h"
#include "uci.h"
#include <iostream>
#include <string>
#include <vector>

const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(int argc, char* argv[]) {
    Board board;
    FEN::loadFEN(board, START_FEN);

    if (argc > 1 && std::string(argv[1]) == "--uci") {
        UCI::uciLoop(board);
        return 0;
    }

    int search_depth = 4;

    while (true) {
        printBoard(board);

        std::vector<Move> legal_moves;
        MoveGen::generatePseudoLegalMoves(board, legal_moves);
        MoveGen::filterLegalMoves(board, legal_moves);

        if (legal_moves.empty()) {
            // Need to check if king is in check to determine stalemate vs checkmate
            int king_pos = -1;
            Piece ourKing = (board.sideToMove == WHITE) ? W_KING : B_KING;
             for(int i = 0; i < 64; ++i) {
                if (board.squares[i] == ourKing) {
                    king_pos = i;
                    break;
                }
            }
            if (MoveGen::isSquareAttacked(board, king_pos, opposite(board.sideToMove))) {
                std::cout << "Checkmate!" << std::endl;
            } else {
                std::cout << "Stalemate!" << std::endl;
            }
            break;
        }

        if (board.sideToMove == WHITE) { // Human's turn
            std::cout << "Enter move (e.g. e2e4): ";
            std::string move_str;
            std::cin >> move_str;

            Move user_move;
            bool move_found = false;
            for (const auto& m : legal_moves) {
                if (moveToString(m) == move_str) {
                    user_move = m;
                    move_found = true;
                    break;
                }
            }

            if (move_found) {
                UndoInfo undo;
                makeMove(board, user_move, undo);
            } else {
                std::cout << "Invalid move!" << std::endl;
            }
        } else { // Engine's turn
            std::cout << "Engine is thinking..." << std::endl;
            Search::SearchResult result = Search::findBestMove(board, search_depth);
            std::cout << "Engine plays: " << moveToString(result.bestMove) << " (Score: " << result.score << ")" << std::endl;
            UndoInfo undo;
            makeMove(board, result.bestMove, undo);
        }
    }

    return 0;
}
