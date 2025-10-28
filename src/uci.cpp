#include "uci.h"
#include "board.h"
#include "fen.h"
#include "search.h"
#include "move.h"
#include "movegen.h"
#include "make_move.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace {
    Move parse_move(Board& b, std::string move_str) {
        std::vector<Move> moves;
        MoveGen::generatePseudoLegalMoves(b, moves);
        MoveGen::filterLegalMoves(b, moves);

        for (const auto& m : moves) {
            if (moveToString(m) == move_str) {
                return m;
            }
        }
        return Move{};
    }
}

void UCI::uciLoop(Board& b) {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "uci") {
            std::cout << "id name MyChessEngine" << std::endl;
            std::cout << "id author YourName" << std::endl;
            std::cout << "uciok" << std::endl;
        } else if (command == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (command == "ucinewgame") {
            FEN::loadFEN(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        } else if (command == "position") {
            std::string subcmd;
            ss >> subcmd;
            if (subcmd == "startpos") {
                FEN::loadFEN(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                ss >> subcmd; // moves
            } else if (subcmd == "fen") {
                // ...
            }
            
            std::string move_str;
            while(ss >> move_str) {
                Move m = parse_move(b, move_str);
                UndoInfo undo;
                makeMove(b, m, undo);
            }
        } else if (command == "go") {
            int depth = 4; // default
            std::string arg;
            while(ss >> arg) {
                if (arg == "depth") {
                    ss >> depth;
                }
            }
            Search::SearchResult result = Search::findBestMove(b, depth);
            std::cout << "bestmove " << moveToString(result.bestMove) << std::endl;
        } else if (command == "quit") {
            break;
        }
    }
}
