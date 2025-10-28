#include "board.h"
#include "types.h"
#include "fen.h"
#include <iostream>

namespace {
    char pieceToChar(Piece p) {
        switch (p) {
            case W_PAWN:   return 'P';
            case W_KNIGHT: return 'N';
            case W_BISHOP: return 'B';
            case W_ROOK:   return 'R';
            case W_QUEEN:  return 'Q';
            case W_KING:   return 'K';
            case B_PAWN:   return 'p';
            case B_KNIGHT: return 'n';
            case B_BISHOP: return 'b';
            case B_ROOK:   return 'r';
            case B_QUEEN:  return 'q';
            case B_KING:   return 'k';
            default:       return '.';
        }
    }
}

void printBoard(const Board& b) {
    std::cout << "\n  +-----------------+" << std::endl;
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " | ";
        for (int file = 0; file < 8; ++file) {
            std::cout << pieceToChar(b.squares[toSquare(file, rank)]) << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "  +-----------------+" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;

    std::cout << "\nSide to move: " << ((b.sideToMove == WHITE) ? "White" : "Black") << std::endl;
    std::cout << "FEN: " << FEN::boardToFEN(b) << std::endl;
}
