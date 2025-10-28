#include "movegen.h"
#include "board.h"
#include "move.h"
#include "make_move.h"
#include <algorithm>

namespace {
    // Helper to add a move to the list
    void addMove(std::vector<Move>& moves, int from, int to, MoveFlags flags = QUIET, PieceType promo = NO_PIECE_TYPE) {
        moves.push_back({(uint8_t)from, (uint8_t)to, (uint8_t)flags, promo});
    }

    // Generate pawn moves
    void generatePawnMoves(const Board& b, std::vector<Move>& moves) {
        Color us = b.sideToMove;
        Piece ourPawn = (us == WHITE) ? W_PAWN : B_PAWN;
        int forward = (us == WHITE) ? 8 : -8;
        int startRank = (us == WHITE) ? 1 : 6;
        int promotionRank = (us == WHITE) ? 7 : 0;

        for (int from = 0; from < 64; ++from) {
            if (b.squares[from] == ourPawn) {
                // 1. Single Push
                int to = from + forward;
                if (to >= 0 && to < 64 && b.squares[to] == EMPTY) {
                    if (rank_of(to) == promotionRank) {
                        addMove(moves, from, to, PROMOTION, QUEEN);
                        addMove(moves, from, to, PROMOTION, ROOK);
                        addMove(moves, from, to, PROMOTION, BISHOP);
                        addMove(moves, from, to, PROMOTION, KNIGHT);
                    } else {
                        addMove(moves, from, to);
                    }
                }

                // 2. Double Push
                if (rank_of(from) == startRank) {
                    int to_double = from + 2 * forward;
                    if (b.squares[to] == EMPTY && b.squares[to_double] == EMPTY) {
                        addMove(moves, from, to_double, DOUBLE_PAWN_PUSH);
                    }
                }

                // 3. Captures
                int captureOffsets[] = {-1, 1};
                for (int offset : captureOffsets) {
                    int to_file = file_of(from) + offset;
                    if (to_file >= 0 && to_file < 8) {
                        int to_capture = from + forward + offset;
                        if (to_capture >= 0 && to_capture < 64) {
                            Piece target = b.squares[to_capture];
                            // Normal capture
                            if (target != EMPTY && pieceColor(target) != us) {
                                if (rank_of(to_capture) == promotionRank) {
                                    addMove(moves, from, to_capture, CAPTURE, QUEEN);
                                    addMove(moves, from, to_capture, CAPTURE, ROOK);
                                    addMove(moves, from, to_capture, CAPTURE, BISHOP);
                                    addMove(moves, from, to_capture, CAPTURE, KNIGHT);
                                } else {
                                    addMove(moves, from, to_capture, CAPTURE);
                                }
                            }
                            // En passant
                            if (to_capture == b.enPassantSquare) {
                                addMove(moves, from, to_capture, (MoveFlags)(EN_PASSANT | CAPTURE));
                            }
                        }
                    }
                }
            }
        }
    }

    // Generate knight moves
    void generateKnightMoves(const Board& b, std::vector<Move>& moves) {
        Color us = b.sideToMove;
        Piece ourKnight = (us == WHITE) ? W_KNIGHT : B_KNIGHT;

        int knightOffsets[] = { -17, -15, -10, -6, 6, 10, 15, 17 };

        for (int from = 0; from < 64; ++from) {
            if (b.squares[from] == ourKnight) {
                for (int offset : knightOffsets) {
                    int to = from + offset;

                    if (to >= 0 && to < 64) {
                        // Check for wrap-around
                        int fromFile = file_of(from);
                        int toFile = file_of(to);
                        if (abs(fromFile - toFile) > 2) continue;

                        Piece target = b.squares[to];
                        if (target == EMPTY) {
                            addMove(moves, from, to);
                        } else if (pieceColor(target) != us) {
                            addMove(moves, from, to, CAPTURE);
                        }
                    }
                }
            }
        }
    }
    
    // Generate sliding moves (Bishop, Rook, Queen)
    void generateSlidingMoves(const Board& b, std::vector<Move>& moves) {
        Color us = b.sideToMove;
        Piece ourBishop = (us == WHITE) ? W_BISHOP : B_BISHOP;
        Piece ourRook = (us == WHITE) ? W_ROOK : B_ROOK;
        Piece ourQueen = (us == WHITE) ? W_QUEEN : B_QUEEN;

        int bishopDirections[] = { -9, -7, 7, 9 };
        int rookDirections[] = { -8, -1, 1, 8 };

        for (int from = 0; from < 64; ++from) {
            Piece p = b.squares[from];
            if (p == EMPTY || pieceColor(p) != us) continue;

            if (pieceType(p) == BISHOP || pieceType(p) == QUEEN) {
                for (int dir : bishopDirections) {
                    for (int to = from + dir; to >= 0 && to < 64 && abs(file_of(to) - file_of(to - dir)) <= 1; to += dir) {
                        Piece target = b.squares[to];
                        if (target == EMPTY) {
                            addMove(moves, from, to);
                        } else {
                            if (pieceColor(target) != us) {
                                addMove(moves, from, to, CAPTURE);
                            }
                            break;
                        }
                    }
                }
            }
            if (pieceType(p) == ROOK || pieceType(p) == QUEEN) {
                for (int dir : rookDirections) {
                    for (int to = from + dir; to >= 0 && to < 64 && abs(file_of(to) - file_of(to - dir)) <= 1; to += dir) {
                        if (dir == 1 || dir == -1) { // Horizontal
                           if(rank_of(to) != rank_of(from)) break;
                        }

                        Piece target = b.squares[to];
                        if (target == EMPTY) {
                            addMove(moves, from, to);
                        } else {
                            if (pieceColor(target) != us) {
                                addMove(moves, from, to, CAPTURE);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    // Generate king moves
    void generateKingMoves(const Board& b, std::vector<Move>& moves) {
        Color us = b.sideToMove;
        Piece ourKing = (us == WHITE) ? W_KING : B_KING;
        int kingOffsets[] = { -9, -8, -7, -1, 1, 7, 8, 9 };
        
        int kingPos = -1;
        for (int i = 0; i < 64; ++i) {
            if (b.squares[i] == ourKing) {
                kingPos = i;
                break;
            }
        }

        // Normal moves
        for (int offset : kingOffsets) {
            int to = kingPos + offset;
            if (to >= 0 && to < 64) {
                if (abs(file_of(kingPos) - file_of(to)) > 1) continue;

                Piece target = b.squares[to];
                if (target == EMPTY) {
                    addMove(moves, kingPos, to);
                } else if (pieceColor(target) != us) {
                    addMove(moves, kingPos, to, CAPTURE);
                }
            }
        }
        
        // Castling
        if (!MoveGen::isSquareAttacked(b, kingPos, opposite(us))) {
            if (us == WHITE) {
                // Kingside
                if ((b.castlingRights & 1) && b.squares[5] == EMPTY && b.squares[6] == EMPTY &&
                    !MoveGen::isSquareAttacked(b, 5, BLACK) && !MoveGen::isSquareAttacked(b, 6, BLACK)) {
                    addMove(moves, 4, 6, KING_CASTLE);
                }
                // Queenside
                if ((b.castlingRights & 2) && b.squares[1] == EMPTY && b.squares[2] == EMPTY && b.squares[3] == EMPTY &&
                    !MoveGen::isSquareAttacked(b, 2, BLACK) && !MoveGen::isSquareAttacked(b, 3, BLACK)) {
                     addMove(moves, 4, 2, QUEEN_CASTLE);
                }
            } else { // BLACK
                // Kingside
                if ((b.castlingRights & 4) && b.squares[61] == EMPTY && b.squares[62] == EMPTY &&
                    !MoveGen::isSquareAttacked(b, 61, WHITE) && !MoveGen::isSquareAttacked(b, 62, WHITE)) {
                    addMove(moves, 60, 62, KING_CASTLE);
                }
                // Queenside
                if ((b.castlingRights & 8) && b.squares[57] == EMPTY && b.squares[58] == EMPTY && b.squares[59] == EMPTY &&
                    !MoveGen::isSquareAttacked(b, 58, WHITE) && !MoveGen::isSquareAttacked(b, 59, WHITE)) {
                    addMove(moves, 60, 58, QUEEN_CASTLE);
                }
            }
        }
    }
}

void MoveGen::generatePseudoLegalMoves(const Board& b, std::vector<Move>& movesOut) {
    movesOut.clear();

    generatePawnMoves(b, movesOut);
    generateKnightMoves(b, movesOut);
    generateSlidingMoves(b, movesOut);
    generateKingMoves(b, movesOut);
}

void MoveGen::filterLegalMoves(Board& b, std::vector<Move>& movesInOut) {
    Color us = b.sideToMove;
    int kingPos = -1;
    Piece ourKing = (us == WHITE) ? W_KING : B_KING;
    for(int i = 0; i < 64; ++i) {
        if (b.squares[i] == ourKing) {
            kingPos = i;
            break;
        }
    }

    movesInOut.erase(std::remove_if(movesInOut.begin(), movesInOut.end(), 
        [&](const Move& m) {
            UndoInfo undo;
            makeMove(b, m, undo);
            bool is_illegal = isSquareAttacked(b, kingPos, opposite(us));
            unmakeMove(b, m, undo);
            return is_illegal;
        }), movesInOut.end());
}

bool MoveGen::isSquareAttacked(const Board& b, int square, Color attackerColor) {
    // 1. Pawn attacks
    int forward = (attackerColor == WHITE) ? -8 : 8; // From the victim's perspective
    Piece attackerPawn = (attackerColor == WHITE) ? W_PAWN : B_PAWN;
    int pawnCaptureOffsets[] = {-1, 1};
    for (int offset : pawnCaptureOffsets) {
        int from_file = file_of(square) + offset;
        if (from_file >= 0 && from_file < 8) {
            int from_sq = square + forward + offset;
            if (from_sq >=0 && from_sq < 64 && b.squares[from_sq] == attackerPawn) {
                return true;
            }
        }
    }

    // 2. Knight attacks
    Piece attackerKnight = (attackerColor == WHITE) ? W_KNIGHT : B_KNIGHT;
    int knightOffsets[] = { -17, -15, -10, -6, 6, 10, 15, 17 };
    for (int offset : knightOffsets) {
        int from_sq = square + offset;
        if (from_sq >= 0 && from_sq < 64) {
            if (abs(file_of(from_sq) - file_of(square)) > 2) continue;
            if (b.squares[from_sq] == attackerKnight) return true;
        }
    }
    
    // 3. Sliding attacks
    Piece attackerBishop = (attackerColor == WHITE) ? W_BISHOP : B_BISHOP;
    Piece attackerRook = (attackerColor == WHITE) ? W_ROOK : B_ROOK;
    Piece attackerQueen = (attackerColor == WHITE) ? W_QUEEN : B_QUEEN;
    int bishopDirections[] = { -9, -7, 7, 9 };
    int rookDirections[] = { -8, -1, 1, 8 };

    for (int dir : bishopDirections) {
        for (int from_sq = square + dir; from_sq >= 0 && from_sq < 64 && abs(file_of(from_sq) - file_of(from_sq - dir)) <= 1; from_sq += dir) {
            Piece p = b.squares[from_sq];
            if (p != EMPTY) {
                if (p == attackerBishop || p == attackerQueen) return true;
                break;
            }
        }
    }
    for (int dir : rookDirections) {
        for (int from_sq = square + dir; from_sq >= 0 && from_sq < 64 && abs(file_of(from_sq) - file_of(from_sq - dir)) <= 1; from_sq += dir) {
             if (dir == 1 || dir == -1) { // Horizontal
                if(rank_of(from_sq) != rank_of(square)) break;
            }
            Piece p = b.squares[from_sq];
            if (p != EMPTY) {
                if (p == attackerRook || p == attackerQueen) return true;
                break;
            }
        }
    }

    // 4. King attacks
    Piece attackerKing = (attackerColor == WHITE) ? W_KING : B_KING;
    int kingOffsets[] = { -9, -8, -7, -1, 1, 7, 8, 9 };
    for (int offset : kingOffsets) {
        int from_sq = square + offset;
        if (from_sq >= 0 && from_sq < 64) {
             if (abs(file_of(from_sq) - file_of(square)) > 1) continue;
             if (b.squares[from_sq] == attackerKing) return true;
        }
    }

    return false;
}
