#include "../include/board.h"

U64 Board::mBishopAttacks[64][512];
U64 Board::mRookAttacks[64][4096];
U64 Board::pawnAttacks[2][64];
U64 Board::knightAttacks[64];
U64 Board::queenAttacks[64];
U64 Board::kingAttacks[64];
SMagic Board::mBishopTbl[64];
SMagic Board::mRookTbl[64];

void Board::PrintBitboard(U64 bb) {
    std::cout << std::endl;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (!file) {
                std::cout << 8 - rank << "  ";
            }
            int square = rank * 8 + file;
            std::cout << (get_bit(bb, square) ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
    std::cout << "\n   a b c d e f g h";
    std::cout << "\n\nBitboard: " << bb << std::endl;
}

void Board::InitJumperAttacks() {
    for (int square = 0; square < 64; square++) {
        pawnAttacks[white][square] = MaskPawnAttacks(square, white);
        pawnAttacks[black][square] = MaskPawnAttacks(square, black);
        knightAttacks[square] = MaskKnightAttacks(square);
        kingAttacks[square] = MaskKingAttacks(square);

        // Testing
        // PrintBitboard(kingAttacks[square]);
    }
}

void Board::GenerateMoves(MoveList &list) {
    U64 myPieces = Occupany(sideToMove);
    U64 oppPieces = Occupany(sideToMove ^ 1);
    U64 allPieces = AllOccupancy();

    int PAWN = (sideToMove == white) ? P : p;
    int KNIGHT = (sideToMove == white) ? N : n;
    int BISHOP = (sideToMove == white) ? B : b;
    int ROOK = (sideToMove == white) ? R : r;
    int QUEEN = (sideToMove == white) ? Q : q;
    int KING = (sideToMove == white) ? K : k;

    // pawns

    if (sideToMove == white) {
        U64 pawns = whitePawns;
        while (pawns) {
            // get the source square(0-63)
            int src = GetLSBIndex(pawns);
            pop_bit(pawns, src);

            // target square (0-63)
            int target = src - 8;

            // move forward 1
            if (target >= 0 && !get_bit(allPieces, target)) {
                if (target <= h8) {
                    list.add(encode_move(src, target, PAWN, QUEEN, 0, 0, 0, 0));
                    list.add(encode_move(src, target, PAWN, ROOK, 0, 0, 0, 0));
                    list.add(encode_move(src, target, PAWN, BISHOP, 0, 0, 0, 0));
                    list.add(encode_move(src, target, PAWN, KNIGHT, 0, 0, 0, 0));
                } else {
                    list.add(encode_move(src, target, PAWN, 0, 0, 0, 0, 0));

                    // move forward 2
                    if (src >= a2 && src <= h2) {
                        int target2 = src - 16;
                        if (!get_bit(allPieces, target2)) {
                            list.add(encode_move(src, target2, PAWN, 0, 0, 1, 0, 0));
                        }
                    }
                }
            }

            // attacks
            U64 attacks = pawnAttacks[white][src] & oppPieces;
            while (attacks) {
                int cap = GetLSBIndex(attacks);
                pop_bit(attacks, cap);

                if (cap <= h8) {
                    list.add(encode_move(src, cap, PAWN, QUEEN, 1, 0, 0, 0));
                    list.add(encode_move(src, cap, PAWN, ROOK, 1, 0, 0, 0));
                    list.add(encode_move(src, cap, PAWN, BISHOP, 1, 0, 0, 0));
                    list.add(encode_move(src, cap, PAWN, KNIGHT, 1, 0, 0, 0));
                } else {
                    list.add(encode_move(src, cap, PAWN, 0, 1, 0, 0, 0));
                }
            }
        }
    } else if (sideToMove == black) {
        U64 pawns = blackPawns;
        while (pawns) {
            // get the source square(0-63)
            int src = GetLSBIndex(pawns);
            pop_bit(pawns, src);

            // target square (0-63)
            int target = src + 8;

            // move forward 1
            if (target <= 63 && !get_bit(allPieces, target)) {
                if (target >= a1) {
                    list.add(encode_move(src, target, PAWN, QUEEN, 0, 0, 0, 0));
                    list.add(encode_move(src, target, PAWN, ROOK, 0, 0, 0, 0));
                    list.add(encode_move(src, target, PAWN, BISHOP, 0, 0, 0, 0));
                    list.add(encode_move(src, target, PAWN, KNIGHT, 0, 0, 0, 0));
                } else {
                    list.add(encode_move(src, target, PAWN, 0, 0, 0, 0, 0));

                    // move forward 2
                    if (src >= a7 && src <= h7) {
                        int target2 = src + 16;
                        if (!get_bit(allPieces, target2)) {
                            list.add(encode_move(src, target2, PAWN, 0, 0, 1, 0, 0));
                        }
                    }
                }
            }

            // attacks
            U64 attacks = pawnAttacks[black][src] & oppPieces;
            while (attacks) {
                int cap = GetLSBIndex(attacks);
                pop_bit(attacks, cap);

                if (cap >= a1) {
                    list.add(encode_move(src, cap, PAWN, QUEEN, 1, 0, 0, 0));
                    list.add(encode_move(src, cap, PAWN, ROOK, 1, 0, 0, 0));
                    list.add(encode_move(src, cap, PAWN, BISHOP, 1, 0, 0, 0));
                    list.add(encode_move(src, cap, PAWN, KNIGHT, 1, 0, 0, 0));
                } else {
                    list.add(encode_move(src, cap, PAWN, 0, 1, 0, 0, 0));
                }
            }
        }
    }

    // rooks
    U64 rooks = (sideToMove == white) ? whiteRooks : blackRooks;
    while (rooks) {
        int src = GetLSBIndex(rooks);
        pop_bit(rooks, src);
        U64 attacks = rookAttacks(allPieces, src) & ~myPieces;
        while (attacks) {
            int cap = GetLSBIndex(attacks);
            pop_bit(attacks, cap);
            list.add(encode_move(src, cap, ROOK, 0, get_bit(oppPieces, cap) ? 1 : 0, 0, 0, 0));
        }
    }

    // knights
    U64 knights = (sideToMove == white) ? whiteKnights : blackKnights;
    while (knights) {
        int src = GetLSBIndex(knights);
        pop_bit(knights, src);
        U64 attacks = knightAttacks[src] & ~myPieces;
        while (attacks) {
            int cap = GetLSBIndex(attacks);
            pop_bit(attacks, cap);
            list.add(encode_move(src, cap, KNIGHT, 0, get_bit(oppPieces, cap) ? 1 : 0, 0, 0, 0));
        }
    }

    // bishops
    U64 bishops = (sideToMove == white) ? whiteBishops : blackBishops;
    while (bishops) {
        int src = GetLSBIndex(bishops);
        pop_bit(bishops, src);
        U64 attacks = bishopAttacks(allPieces, src) & ~myPieces;
        while (attacks) {
            int cap = GetLSBIndex(attacks);
            pop_bit(attacks, cap);
            list.add(encode_move(src, cap, BISHOP, 0, get_bit(oppPieces, cap) ? 1 : 0, 0, 0, 0));
        }
    }

    // queens
    U64 queens = (sideToMove == white) ? whiteQueens : blackQueens;
    while (queens) {
        int src = GetLSBIndex(queens);
        pop_bit(queens, src);
        U64 attacks = (bishopAttacks(allPieces, src) | rookAttacks(allPieces, src)) & ~myPieces;
        while (attacks) {
            int cap = GetLSBIndex(attacks);
            pop_bit(attacks, cap);
            list.add(encode_move(src, cap, QUEEN, 0, get_bit(oppPieces, cap) ? 1 : 0, 0, 0, 0));
        }
    }

    // king
    U64 king = (sideToMove == white) ? whiteKing : blackKing;
    while (king) {
        int src = GetLSBIndex(king);
        pop_bit(king, src);
        U64 attacks = kingAttacks[src] & ~myPieces;
        while (attacks) {
            int cap = GetLSBIndex(attacks);
            pop_bit(attacks, cap);
            list.add(encode_move(src, cap, KING, 0, get_bit(oppPieces, cap) ? 1 : 0, 0, 0, 0));
        }
    }
}

void Board::MakeMove(int move) {
    int src = get_move_source(move);
    int tgt = get_move_target(move);
    int piece = get_move_piece(move);
    int promo = get_move_promo(move);
    int capture = get_move_capture(move);
    int dbl = get_move_double(move);
    int ep = get_move_ep(move);
    int castle = get_move_castle(move);

    U64 *bb[12] = {
        &whitePawns, &whiteKnights, &whiteBishops, &whiteRooks, &whiteQueens, &whiteKing,
        &blackPawns, &blackKnights, &blackBishops, &blackRooks, &blackQueens, &blackKing
    };

    // move
    pop_bit(*bb[piece], src);
    set_bit(*bb[piece], tgt);

    // clear from opponent
    if (capture) {
        int start = (sideToMove == white) ? 6 : 0;
        for (int i = start; i < start + 6; i++) {
            pop_bit(*bb[i], tgt);
        }
    }

    // promo
    if (promo) {
        pop_bit(*bb[piece], tgt);
        set_bit(*bb[promo], tgt);
    }

    // en passant
    if (ep) {
        if (sideToMove == white)
            pop_bit(blackPawns, tgt + 8);
        else
            pop_bit(whitePawns, tgt - 8);
    }

    // castle
    if (castle) {
        switch (tgt) {
            case g1:
                pop_bit(whiteRooks, h1);
                set_bit(whiteRooks, f1);
                break;
            case c1:
                pop_bit(whiteRooks, a1);
                set_bit(whiteRooks, d1);
                break;
            case g8:
                pop_bit(blackRooks, h8);
                set_bit(blackRooks, f8);
                break;
            case c8:
                pop_bit(blackRooks, a8);
                set_bit(blackRooks, d8);
                break;
        }
    }

    sideToMove ^= 1;
}

void Board::UnmakeMove(int move) {
}

U64 Board::Occupany(int side) {
    if (!side) {
        return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
    } else {
        return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
    }
}

U64 Board::AllOccupancy() {
    return Occupany(white) | Occupany(black);
}

U64 Board::SetOccupancy(int index, int bitsInMask, U64 attackMask) {
    U64 occupancy = 0ULL;
    for (int count = 0; count < bitsInMask; count++) {
        int square = GetLSBIndex(attackMask);
        pop_bit(attackMask, square);
        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }
    return occupancy;
}

bool Board::IsSquareAttacked(int square) {
    U64 pawns = sideToMove == white ? whitePawns : blackPawns;
    while (pawns) {
        int src = GetLSBIndex(pawns);
        pop_bit(pawns, src);
        if (pawnAttacks[sideToMove][src] & (1ULL << square)) {
            return true;
        }
    }

    U64 rooks = sideToMove == white ? whiteRooks : blackRooks;
    while (rooks) {
        int src = GetLSBIndex(rooks);
        pop_bit(rooks, src);
        if (rookAttacks(AllOccupancy(), src) & (1ULL << square)) {
            return true;
        }
    }

    U64 knights = sideToMove == white ? whiteKnights : blackKnights;
    while (knights) {
        int src = GetLSBIndex(knights);
        pop_bit(knights, src);
        if (knightAttacks[src] & (1ULL << square)) {
            return true;
        }
    }

    U64 bishops = sideToMove == white ? whiteBishops : blackBishops;
    while (bishops) {
        int src = GetLSBIndex(bishops);
        pop_bit(bishops, src);
        if (bishopAttacks(AllOccupancy(), src) & (1ULL << square)) {
            return true;
        }
    }

    U64 queens = sideToMove == white ? whiteQueens : blackQueens;
    while (queens) {
        int src = GetLSBIndex(queens);
        pop_bit(queens, src);
        if ((bishopAttacks(AllOccupancy(), src) | rookAttacks(AllOccupancy(), src)) & (1ULL << square)) {
            return true;
        }
    }

    U64 king = sideToMove == white ? whiteKing : blackKing;
    while (king) {
        int src = GetLSBIndex(king);
        pop_bit(king, src);
        if (kingAttacks[src] & (1ULL << square)) {
            return true;
        }
    }
    return false;
}

U64 Board::InitRookAttacks(int square, U64 blocker) {
    U64 attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 7; r++) {
        attacks |= (1ULL << (r * 8 + tf));
        if (1ULL << (r * 8 + tf) & blocker)
            break;
    }
    for (r = tr - 1; r >= 0; r--) {
        attacks |= (1ULL << (r * 8 + tf));
        if (1ULL << (r * 8 + tf) & blocker)
            break;
    }

    for (f = tf + 1; f <= 7; f++) {
        attacks |= (1ULL << (tr * 8 + f));
        if (1ULL << (tr * 8 + f) & blocker)
            break;
    }

    for (f = tf - 1; f >= 0; f--) {
        attacks |= (1ULL << (tr * 8 + f));
        if (1ULL << (tr * 8 + f) & blocker)
            break;
    }

    return attacks;
}

U64 Board::InitBishopAttacks(int square, U64 blocker) {
    U64 attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }

    return attacks;
}

U64 Board::MaskPawnAttacks(int square, int side) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if (!side) {
        if (bitboard >> 7 & notAFile)
            attacks |= (bitboard >> 7);

        if (bitboard >> 9 & notHFile)
            attacks |= (bitboard >> 9);
    } else {
        if (bitboard << 7 & notHFile)
            attacks |= (bitboard << 7);

        if (bitboard << 9 & notAFile)
            attacks |= (bitboard << 9);
    }

    return attacks;
}

U64 Board::MaskRookAttacks(int square) {
    U64 attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));

    return attacks;
}

U64 Board::MaskKnightAttacks(int square) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    // 17, 15, 10, 6 forward and backward
    attacks |= (bitboard >> 17) & notHFile;
    attacks |= (bitboard >> 15) & notAFile;
    attacks |= (bitboard >> 10) & notHGFile;
    attacks |= (bitboard >> 6) & notABFile;

    attacks |= (bitboard << 17) & notAFile;
    attacks |= (bitboard << 15) & notHFile;
    attacks |= (bitboard << 10) & notABFile;
    attacks |= (bitboard << 6) & notHGFile;

    return attacks;
}

U64 Board::MaskBishopAttacks(int square) {
    U64 attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attacks |= (1ULL << (r * 8 + f));

    return attacks;
}

U64 Board::MaskQueenAttacks(int square) {
    return MaskRookAttacks(square) | MaskBishopAttacks(square);
}

U64 Board::MaskKingAttacks(int square) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if (bitboard << 8)
        attacks |= (bitboard << 8);
    if (bitboard >> 8)
        attacks |= (bitboard >> 8);

    attacks |= (bitboard >> 1) & notHFile;
    attacks |= (bitboard >> 7) & notAFile;
    attacks |= (bitboard >> 9) & notHFile;

    attacks |= (bitboard << 1) & notAFile;
    attacks |= (bitboard << 7) & notHFile;
    attacks |= (bitboard << 9) & notAFile;

    return attacks;
}

U64 Board::bishopAttacks(U64 occ, int square) {
    occ &= mBishopTbl[square].mask;
    occ *= mBishopTbl[square].magic;
    occ >>= 64 - 9;
    return mBishopAttacks[square][occ];
}

U64 Board::rookAttacks(U64 occ, int square) {
    occ &= mRookTbl[square].mask;
    occ *= mRookTbl[square].magic;
    occ >>= 64 - 12;
    return mRookAttacks[square][occ];
}

void Board::InitMagics() {
    for (int square = 0; square < 64; square++) {
        // bishop
        mBishopTbl[square].mask = MaskBishopAttacks(square);
        mBishopTbl[square].magic = bishopMagics[square];

        int bBits = CountBits(mBishopTbl[square].mask);
        for (int i = 0; i < (1 << bBits); i++) {
            U64 occ = SetOccupancy(i, bBits, mBishopTbl[square].mask);
            int index = (occ * mBishopTbl[square].magic) >> (64 - 9);
            mBishopAttacks[square][index] = InitBishopAttacks(square, occ);
        }
        // rook
        mRookTbl[square].mask = MaskRookAttacks(square);
        mRookTbl[square].magic = rookMagics[square];

        int rBits = CountBits(mRookTbl[square].mask);
        for (int i = 0; i < (1 << rBits); i++) {
            U64 occ = SetOccupancy(i, rBits, mRookTbl[square].mask);
            int index = (occ * mRookTbl[square].magic) >> (64 - 12);
            mRookAttacks[square][index] = InitRookAttacks(square, occ);
        }
    }
}
