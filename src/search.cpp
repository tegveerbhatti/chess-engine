#include "search.h"
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "make_move.h"
#include "eval.h"
#include <algorithm>
#include <vector>

namespace {
    // Move ordering helper
    int scoreMove(const Move& m) {
        if (isCapture(m)) return 100;
        if (m.flags & PROMOTION) return 90;
        if (isCastle(m)) return 50;
        return 0;
    }
}

int Search::alphaBeta(Board& b, int depth, int alpha, int beta) {
    if (depth == 0) {
        return Eval::evaluate(b);
    }

    std::vector<Move> moves;
    MoveGen::generatePseudoLegalMoves(b, moves);
    MoveGen::filterLegalMoves(b, moves);

    if (moves.empty()) {
        int king_pos = -1;
        Piece ourKing = (b.sideToMove == WHITE) ? W_KING : B_KING;
        for(int i = 0; i < 64; ++i) {
            if (b.squares[i] == ourKing) {
                king_pos = i;
                break;
            }
        }
        if (MoveGen::isSquareAttacked(b, king_pos, opposite(b.sideToMove))) {
             return -Checkmate; // Checkmate
        } else {
            return 0; // Stalemate
        }
    }

    // Move ordering
    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return scoreMove(a) > scoreMove(b);
    });

    for (const auto& move : moves) {
        UndoInfo undo;
        makeMove(b, move, undo);
        int score = -alphaBeta(b, depth - 1, -beta, -alpha);
        unmakeMove(b, move, undo);

        if (score >= beta) {
            return beta; // Fail hard beta-cutoff
        }
        if (score > alpha) {
            alpha = score; // alpha acts like max in MiniMax
        }
    }

    return alpha;
}

Search::SearchResult Search::findBestMove(Board& b, int depth) {
    SearchResult result;
    int best_score = -Infinity;

    std::vector<Move> moves;
    MoveGen::generatePseudoLegalMoves(b, moves);
    MoveGen::filterLegalMoves(b, moves);
    
    // Move ordering
    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return scoreMove(a) > scoreMove(b);
    });

    for (const auto& move : moves) {
        UndoInfo undo;
        makeMove(b, move, undo);
        int score = -alphaBeta(b, depth - 1, -Infinity, Infinity);
        unmakeMove(b, move, undo);

        if (score > best_score) {
            best_score = score;
            result.bestMove = move;
        }
    }

    result.score = best_score;
    return result;
}
