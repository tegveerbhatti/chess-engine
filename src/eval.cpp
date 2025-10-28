#include "eval.h"
#include "board.h"
#include "types.h"
#include <array>

namespace {
    constexpr int PawnValue = 100;
    constexpr int KnightValue = 320;
    constexpr int BishopValue = 330;
    constexpr int RookValue = 500;
    constexpr int QueenValue = 900;

    constexpr std::array<int, 6> PieceValues = {
        PawnValue, KnightValue, BishopValue, RookValue, QueenValue, 0
    };
    
    // Piece-Square Tables (midgame)
    // Mirrored for black
    constexpr std::array<int, 64> PST_PAWN = {
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
         5,  5, 10, 25, 25, 10,  5,  5,
         0,  0,  0, 20, 20,  0,  0,  0,
         5, -5,-10,  0,  0,-10, -5,  5,
         5, 10, 10,-20,-20, 10, 10,  5,
         0,  0,  0,  0,  0,  0,  0,  0
    };

    constexpr std::array<int, 64> PST_KNIGHT = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };

    constexpr std::array<int, 64> PST_BISHOP = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };
    
    constexpr std::array<int, 64> PST_ROOK = {
          0,  0,  0,  0,  0,  0,  0,  0,
          5, 10, 10, 10, 10, 10, 10,  5,
         -5,  0,  0,  0,  0,  0,  0, -5,
         -5,  0,  0,  0,  0,  0,  0, -5,
         -5,  0,  0,  0,  0,  0,  0, -5,
         -5,  0,  0,  0,  0,  0,  0, -5,
         -5,  0,  0,  0,  0,  0,  0, -5,
          0,  0,  0,  5,  5,  0,  0,  0
    };

    constexpr std::array<int, 64> PST_QUEEN = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
         -5,  0,  5,  5,  5,  5,  0, -5,
          0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };

    constexpr std::array<int, 64> PST_KING = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
    };

    const std::array<const std::array<int, 64>*, 6> PSTs = {
        &PST_PAWN, &PST_KNIGHT, &PST_BISHOP, &PST_ROOK, &PST_QUEEN, &PST_KING
    };
    
    int get_pst_score(PieceType pt, int sq, Color c) {
        if (c == WHITE) {
            return (*PSTs[pt])[sq];
        } else {
            return (*PSTs[pt])[sq ^ 56]; // Flip board
        }
    }
}

int Eval::evaluate(const Board& b) {
    int score = 0;
    for (int i = 0; i < 64; ++i) {
        Piece p = b.squares[i];
        if (p != EMPTY) {
            Color c = pieceColor(p);
            PieceType pt = pieceType(p);
            int value = PieceValues[pt];
            value += get_pst_score(pt, i, c);
            
            if (c == WHITE) {
                score += value;
            } else {
                score -= value;
            }
        }
    }
    return (b.sideToMove == WHITE) ? score : -score;
}
