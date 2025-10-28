#include "make_move.h"
#include "board.h"
#include "move.h"
#include "types.h"

void makeMove(Board& b, const Move& m, UndoInfo& undoOut) {
    // Save current state for undo
    undoOut.capturedPiece = b.squares[m.to];
    undoOut.oldCastlingRights = b.castlingRights;
    undoOut.oldEnPassantSquare = b.enPassantSquare;
    undoOut.oldHalfmoveClock = b.halfmoveClock;
    undoOut.oldFullmoveNumber = b.fullmoveNumber;
    undoOut.oldZobristKey = b.zobristKey;

    // Get the piece that's moving
    Piece movingPiece = b.squares[m.from];
    
    // Clear the source square
    b.squares[m.from] = EMPTY;
    
    // Place the piece on the destination square (handle promotion later)
    b.squares[m.to] = movingPiece;
    
    // Handle special moves
    if (m.flags & PROMOTION) {
        // Replace the pawn with the promoted piece
        if (b.sideToMove == WHITE) {
            b.squares[m.to] = (Piece)(W_PAWN + m.promo);
        } else {
            b.squares[m.to] = (Piece)(B_PAWN + m.promo);
        }
    }
    
    if (m.flags & EN_PASSANT) {
        // Remove the captured pawn
        int capturedPawnSquare = (b.sideToMove == WHITE) ? m.to - 8 : m.to + 8;
        b.squares[capturedPawnSquare] = EMPTY;
    }
    
    if (m.flags & KING_CASTLE) {
        // Move the rook for kingside castling
        if (b.sideToMove == WHITE) {
            b.squares[5] = b.squares[7]; // Move rook from h1 to f1
            b.squares[7] = EMPTY;
        } else {
            b.squares[61] = b.squares[63]; // Move rook from h8 to f8
            b.squares[63] = EMPTY;
        }
    }
    
    if (m.flags & QUEEN_CASTLE) {
        // Move the rook for queenside castling
        if (b.sideToMove == WHITE) {
            b.squares[3] = b.squares[0]; // Move rook from a1 to d1
            b.squares[0] = EMPTY;
        } else {
            b.squares[59] = b.squares[56]; // Move rook from a8 to d8
            b.squares[56] = EMPTY;
        }
    }
    
    // Update castling rights
    if (movingPiece == W_KING) {
        b.castlingRights &= ~(1 | 2); // Clear both white castling rights
    } else if (movingPiece == B_KING) {
        b.castlingRights &= ~(4 | 8); // Clear both black castling rights
    }
    
    // Rook moves affect castling rights
    if (m.from == 0 || m.to == 0) b.castlingRights &= ~2; // White queenside
    if (m.from == 7 || m.to == 7) b.castlingRights &= ~1; // White kingside
    if (m.from == 56 || m.to == 56) b.castlingRights &= ~8; // Black queenside
    if (m.from == 63 || m.to == 63) b.castlingRights &= ~4; // Black kingside
    
    // Update en passant square
    b.enPassantSquare = -1;
    if (m.flags & DOUBLE_PAWN_PUSH) {
        b.enPassantSquare = (b.sideToMove == WHITE) ? m.to - 8 : m.to + 8;
    }
    
    // Update halfmove clock
    if (pieceType(movingPiece) == PAWN || (m.flags & CAPTURE)) {
        b.halfmoveClock = 0;
    } else {
        b.halfmoveClock++;
    }
    
    // Update fullmove number (increment after black moves)
    if (b.sideToMove == BLACK) {
        b.fullmoveNumber++;
    }
    
    // Switch sides
    b.sideToMove = opposite(b.sideToMove);
}

void unmakeMove(Board& b, const Move& m, const UndoInfo& undoIn) {
    // Switch sides back
    b.sideToMove = opposite(b.sideToMove);
    
    // Get the piece that's currently on the destination square
    Piece pieceOnDestination = b.squares[m.to];
    
    // If this was a promotion, restore the original pawn
    if (m.flags & PROMOTION) {
        pieceOnDestination = (b.sideToMove == WHITE) ? W_PAWN : B_PAWN;
    }
    
    // Move the piece back to its original square
    b.squares[m.from] = pieceOnDestination;
    
    // Restore what was on the destination square (captured piece or empty)
    b.squares[m.to] = undoIn.capturedPiece;
    
    // Handle special moves
    if (m.flags & EN_PASSANT) {
        // Restore the captured pawn
        int capturedPawnSquare = (b.sideToMove == WHITE) ? m.to - 8 : m.to + 8;
        Piece capturedPawn = (b.sideToMove == WHITE) ? B_PAWN : W_PAWN;
        b.squares[capturedPawnSquare] = capturedPawn;
    }
    
    if (m.flags & KING_CASTLE) {
        // Move the rook back for kingside castling
        if (b.sideToMove == WHITE) {
            b.squares[7] = b.squares[5]; // Move rook back from f1 to h1
            b.squares[5] = EMPTY;
        } else {
            b.squares[63] = b.squares[61]; // Move rook back from f8 to h8
            b.squares[61] = EMPTY;
        }
    }
    
    if (m.flags & QUEEN_CASTLE) {
        // Move the rook back for queenside castling
        if (b.sideToMove == WHITE) {
            b.squares[0] = b.squares[3]; // Move rook back from d1 to a1
            b.squares[3] = EMPTY;
        } else {
            b.squares[56] = b.squares[59]; // Move rook back from d8 to a8
            b.squares[59] = EMPTY;
        }
    }
    
    // Restore all saved state
    b.castlingRights = undoIn.oldCastlingRights;
    b.enPassantSquare = undoIn.oldEnPassantSquare;
    b.halfmoveClock = undoIn.oldHalfmoveClock;
    b.fullmoveNumber = undoIn.oldFullmoveNumber;
    b.zobristKey = undoIn.oldZobristKey;
}
