#include "fen.h"
#include "board.h"
#include "types.h"
#include <vector>
#include <sstream>
#include <iostream>

namespace {
    Piece charToPiece(char c) {
        switch (c) {
            case 'P': return W_PAWN;
            case 'N': return W_KNIGHT;
            case 'B': return W_BISHOP;
            case 'R': return W_ROOK;
            case 'Q': return W_QUEEN;
            case 'K': return W_KING;
            case 'p': return B_PAWN;
            case 'n': return B_KNIGHT;
            case 'b': return B_BISHOP;
            case 'r': return B_ROOK;
            case 'q': return B_QUEEN;
            case 'k': return B_KING;
            default:  return EMPTY;
        }
    }

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
            default:       return ' ';
        }
    }
}

bool FEN::loadFEN(Board& b, const std::string& fenStr) {
    std::fill(b.squares, b.squares + 64, EMPTY);
    b.castlingRights = 0;
    b.enPassantSquare = -1;
    b.halfmoveClock = 0;
    b.fullmoveNumber = 1;

    std::stringstream ss(fenStr);
    std::string part;
    ss >> part;

    int rank = 7, file = 0;
    for (char c : part) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            file += c - '0';
        } else {
            b.squares[toSquare(file, rank)] = charToPiece(c);
            file++;
        }
    }

    if (!(ss >> part)) return false;
    b.sideToMove = (part[0] == 'w') ? WHITE : BLACK;

    if (!(ss >> part)) return false;
    for (char c : part) {
        switch(c) {
            case 'K': b.castlingRights |= 1; break;
            case 'Q': b.castlingRights |= 2; break;
            case 'k': b.castlingRights |= 4; break;
            case 'q': b.castlingRights |= 8; break;
        }
    }

    if (!(ss >> part)) return false;
    if (part[0] != '-') {
        int epFile = part[0] - 'a';
        int epRank = part[1] - '1';
        b.enPassantSquare = toSquare(epFile, epRank);
    }

    if (!(ss >> part)) return true; // FEN can end here
    b.halfmoveClock = std::stoi(part);

    if (!(ss >> part)) return true; // FEN can end here
    b.fullmoveNumber = std::stoi(part);

    return true;
}

std::string FEN::boardToFEN(const Board& b) {
    std::string fen;
    for (int rank = 7; rank >= 0; --rank) {
        int emptyCount = 0;
        for (int file = 0; file < 8; ++file) {
            Piece p = b.squares[toSquare(file, rank)];
            if (p == EMPTY) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += pieceToChar(p);
            }
        }
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        if (rank > 0) {
            fen += '/';
        }
    }

    fen += (b.sideToMove == WHITE) ? " w " : " b ";

    std::string castlingStr;
    if (b.castlingRights & 1) castlingStr += 'K';
    if (b.castlingRights & 2) castlingStr += 'Q';
    if (b.castlingRights & 4) castlingStr += 'k';
    if (b.castlingRights & 8) castlingStr += 'q';
    fen += (castlingStr.empty()) ? "-" : castlingStr;

    fen += " ";

    if (b.enPassantSquare != -1) {
        fen += (char)('a' + file_of(b.enPassantSquare));
        fen += (char)('1' + rank_of(b.enPassantSquare));
    } else {
        fen += "-";
    }

    fen += " " + std::to_string(b.halfmoveClock);
    fen += " " + std::to_string(b.fullmoveNumber);

    return fen;
}
