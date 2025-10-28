#pragma once

#include <cstdint>
#include <string>

// All core types should live in types.h. This header MUST NOT depend on
// any other project header to avoid circular includes.

// 4.1 Squares
constexpr int file_of(int square) { return square % 8; }
constexpr int rank_of(int square) { return square / 8; }
constexpr int toSquare(int file, int rank) { return rank * 8 + file; }

// 4.2 Colors
enum Color : uint8_t {
    WHITE = 0,
    BLACK = 1
};

inline Color opposite(Color c) { return (c == WHITE) ? BLACK : WHITE; }

// 4.3 Piece types
enum PieceType : uint8_t {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    NO_PIECE_TYPE = 6
};

enum Piece : uint8_t {
    EMPTY = 0,
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
};

// Utility functions
inline bool isEmpty(Piece p) { return p == EMPTY; }
inline Color pieceColor(Piece p) {
    // assumes p != EMPTY
    return (p >= W_PAWN && p <= W_KING) ? WHITE : BLACK;
}
inline PieceType pieceType(Piece p) {
    // assumes p != EMPTY
    return PieceType((p - 1) % 6);
}
