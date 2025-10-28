#include "move.h"
#include <string>

std::string moveToString(const Move& m) {
    std::string str;
    str += (char)('a' + file_of(m.from));
    str += (char)('1' + rank_of(m.from));
    str += (char)('a' + file_of(m.to));
    str += (char)('1' + rank_of(m.to));

    if (m.promo != NO_PIECE_TYPE) {
        switch (m.promo) {
            case QUEEN:  str += 'q'; break;
            case ROOK:   str += 'r'; break;
            case BISHOP: str += 'b'; break;
            case KNIGHT: str += 'n'; break;
            default: break;
        }
    }
    return str;
}
