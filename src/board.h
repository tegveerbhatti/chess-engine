#pragma once

#include "types.h"

// 4.4 Board state
struct Board {
    Piece squares[64];
    Color sideToMove;

    // Castling rights as bit flags:
    // white king-side, white queen-side, black king-side, black queen-side
    uint8_t castlingRights;

    int enPassantSquare;        // -1 if none; else square index
    uint8_t halfmoveClock;      // for 50-move rule
    uint16_t fullmoveNumber;    // increments after Black moves

    uint64_t zobristKey;        // Zobrist hash
};

// 10.2 Board printing
void printBoard(const Board& b);
