#include "../include/board.h"
#include <iostream>

U64 Board::mBishopAttacks[64][512];
U64 Board::mRookAttacks[64][4096];
U64 Board::pawnAttacks[2][64];
U64 Board::knightAttacks[64];
U64 Board::queenAttacks[64];
U64 Board::kingAttacks[64];
SMagic Board::mBishopTbl[64];
SMagic Board::mRookTbl[64];

void Board::PrintBitboard(const U64 bb) {
    std::cout << '\n';
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (!file) {
                std::cout << 8 - rank << "  ";
            }
            const uint64_t square = rank * 8 + file;
            std::cout << (get_bit(bb, square) ? "1 " : "0 ");
        }
        std::cout << '\n';
    }
    std::cout << "\n   a b c d e f g h";
    std::cout << "\n\nBitboard: " << bb << '\n';
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
    const U64 myPieces = Occupancy(sideToMove);
    const U64 oppPieces = Occupancy(sideToMove ^ 1);
    const U64 allPieces = AllOccupancy();

    const int PAWN = (sideToMove == white) ? P : p;
    const int KNIGHT = (sideToMove == white) ? N : n;
    const int BISHOP = (sideToMove == white) ? B : b;
    const int ROOK = (sideToMove == white) ? R : r;
    const int QUEEN = (sideToMove == white) ? Q : q;
    const int KING = (sideToMove == white) ? K : k;

    // pawns

    if (sideToMove == white) {
        U64 pawns = whitePawns;
        while (pawns) {
            // get the source square(0-63)
            const uint64_t src = GetLSBIndex(pawns);
            pop_bit(pawns, src);

            // target square (0-63)
            const uint64_t target = src - 8;

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
                        const int target2 = src - 16;
                        if (!get_bit(allPieces, target2)) {
                            list.add(encode_move(src, target2, PAWN, 0, 0, 1, 0, 0));
                        }
                    }
                }
            }

            if (enPassantSquare != -1 && (pawnAttacks[white][src] & (1ULL << enPassantSquare))) {
                list.add(encode_move(src, enPassantSquare, PAWN, 0, 1, 0, 1, 0));
            }

            // attacks
            U64 attacks = pawnAttacks[white][src] & oppPieces;
            while (attacks) {
                const int cap = GetLSBIndex(attacks);
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
            const uint64_t src = GetLSBIndex(pawns);
            pop_bit(pawns, src);

            // target square (0-63)
            const uint64_t target = src + 8;

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
                        const uint64_t target2 = src + 16;
                        if (!get_bit(allPieces, target2)) {
                            list.add(encode_move(src, target2, PAWN, 0, 0, 1, 0, 0));
                        }
                    }
                }
            }

            if (enPassantSquare != -1 && (pawnAttacks[black][src] & (1ULL << enPassantSquare))) {
                list.add(encode_move(src, enPassantSquare, PAWN, 0, 1, 0, 1, 0));
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
        const uint64_t src = GetLSBIndex(rooks);
        pop_bit(rooks, src);
        U64 attacks = rookAttacks(allPieces, src) & ~myPieces;
        while (attacks) {
            const uint64_t cap = GetLSBIndex(attacks);
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

        if (sideToMove == white) {
            // white kingside
            if ((castleRights & 1) &&
                !get_bit(allPieces, f1) && !get_bit(allPieces, g1) &&
                !IsAttackedByOpponent(e1) && !IsAttackedByOpponent(f1) && !IsAttackedByOpponent(g1)) {
                list.add(encode_move(e1, g1, K, 0, 0, 0, 0, 1));
            }
            // white queenside
            if ((castleRights & 2) &&
                !get_bit(allPieces, b1) && !get_bit(allPieces, c1) && !get_bit(allPieces, d1) &&
                !IsAttackedByOpponent(e1) && !IsAttackedByOpponent(d1) && !IsAttackedByOpponent(c1)) {
                list.add(encode_move(e1, c1, K, 0, 0, 0, 0, 1));
            }
        } else {
            // black kingside
            if ((castleRights & 4) &&
                !get_bit(allPieces, f8) && !get_bit(allPieces, g8) &&
                !IsAttackedByOpponent(e8) && !IsAttackedByOpponent(f8) && !IsAttackedByOpponent(g8)) {
                list.add(encode_move(e8, g8, k, 0, 0, 0, 0, 1));
            }
            // black queenside
            if ((castleRights & 8) &&
                !get_bit(allPieces, b8) && !get_bit(allPieces, c8) && !get_bit(allPieces, d8) &&
                !IsAttackedByOpponent(e8) && !IsAttackedByOpponent(d8) && !IsAttackedByOpponent(c8)) {
                list.add(encode_move(e8, c8, k, 0, 0, 0, 0, 1));
            }
        }

        U64 attacks = kingAttacks[src] & ~myPieces;
        while (attacks) {
            int cap = GetLSBIndex(attacks);
            pop_bit(attacks, cap);
            list.add(encode_move(src, cap, KING, 0, get_bit(oppPieces, cap) ? 1 : 0, 0, 0, 0));
        }
    }
}

void Board::MakeMove(const int move) {
    const int src = get_move_source(move);
    const int tgt = get_move_target(move);
    const int piece = get_move_piece(move);
    const int promo = get_move_promo(move);
    const int capture = get_move_capture(move);
    const int dbl = get_move_double(move);
    const int ep = get_move_ep(move);
    const int castle = get_move_castle(move);

    U64 *bb[12] = {
        &whitePawns, &whiteKnights, &whiteBishops, &whiteRooks, &whiteQueens, &whiteKing,
        &blackPawns, &blackKnights, &blackBishops, &blackRooks, &blackQueens, &blackKing
    };

    // move
    pop_bit(*bb[piece], src);
    set_bit(*bb[piece], tgt);

    // clear from opponent
    if (capture) {
        const int start = (sideToMove == white) ? 6 : 0;
        for (int i = start; i < start + 6; i++) {
            if (get_bit(*bb[i], tgt)) {
                capturedPieceStack[ply] = i;
                pop_bit(*bb[i], tgt);
                break;
            }
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
            default: ;
        }
    }

    epSquareStack[ply] = enPassantSquare;
    enPassantSquare = dbl ? (sideToMove == white ? tgt + 8 : tgt - 8) : -1;


    castleRightsStack[ply] = castleRights;

    // king moves lose both rights for that side
    if (piece == K) castleRights &= ~(1 | 2);
    if (piece == k) castleRights &= ~(4 | 8);

    // rook moves from home square lose that side's right
    if (src == h1) castleRights &= ~1;
    if (src == a1) castleRights &= ~2;
    if (src == h8) castleRights &= ~4;
    if (src == a8) castleRights &= ~8;

    // rook captured on its home square also loses that right
    if (tgt == h1) castleRights &= ~1;
    if (tgt == a1) castleRights &= ~2;
    if (tgt == h8) castleRights &= ~4;
    if (tgt == a8) castleRights &= ~8;

    sideToMove ^= 1;
    ply++;
}

void Board::UnmakeMove(int move) {
    const int src = get_move_source(move);
    const int tgt = get_move_target(move);
    const int piece = get_move_piece(move);
    const int promo = get_move_promo(move);
    const int capture = get_move_capture(move);
    const int dbl = get_move_double(move);
    const int ep = get_move_ep(move);
    const int castle = get_move_castle(move);

    U64 *bb[12] = {
        &whitePawns, &whiteKnights, &whiteBishops, &whiteRooks, &whiteQueens, &whiteKing,
        &blackPawns, &blackKnights, &blackBishops, &blackRooks, &blackQueens, &blackKing
    };

    ply--;
    castleRights = castleRightsStack[ply];
    enPassantSquare = epSquareStack[ply];
    sideToMove ^= 1;

    if (promo) {
        pop_bit(*bb[promo], tgt);
    } else {
        pop_bit(*bb[piece], tgt);
    }
    set_bit(*bb[piece], src);

    // restore captured piece
    if (capture && !ep) {
        set_bit(*bb[capturedPieceStack[ply]], tgt);
    }

    // restore en passant captured pawn
    if (ep) {
        if (sideToMove == white)
            set_bit(blackPawns, tgt + 8);
        else
            set_bit(whitePawns, tgt - 8);
    }

    // undo castling rook move
    if (castle) {
        switch (tgt) {
            case g1: pop_bit(whiteRooks, f1);
                set_bit(whiteRooks, h1);
                break;
            case c1: pop_bit(whiteRooks, d1);
                set_bit(whiteRooks, a1);
                break;
            case g8: pop_bit(blackRooks, f8);
                set_bit(blackRooks, h8);
                break;
            case c8: pop_bit(blackRooks, d8);
                set_bit(blackRooks, a8);
                break;
            default: ;
        }
    }
}

U64 Board::Occupancy(int side) const {
    if (!side) {
        return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
    }
    return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
}

U64 Board::AllOccupancy() {
    return Occupancy(white) | Occupancy(black);
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
        const int src = GetLSBIndex(queens);
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

U64 Board::Perft(int depth) {
    MoveList moves;
    U64 nodes = 0;

    if (depth == 0) {
        return 1ULL;
    }

    GenerateMoves(moves);


    for (int i = 0; i < moves.count; i++) {
        int mover = sideToMove;
        MakeMove(moves.moves[i]);

        int kingSquare = GetLSBIndex(mover == white ? whiteKing : blackKing);
        if (!IsSquareAttacked(kingSquare)) {
            nodes += Perft(depth - 1);
        }
        UnmakeMove(moves.moves[i]);
    }

    return nodes;
}

void Board::Divide(int depth) {
    MoveList moves;
    GenerateMoves(moves);

    U64 total = 0;

    for (int i = 0; i < moves.count; i++) {
        int mover = sideToMove;
        MakeMove(moves.moves[i]);

        int kingSquare = GetLSBIndex(mover == white ? whiteKing : blackKing);
        if (!IsSquareAttacked(kingSquare)) {
            U64 nodes = (depth == 1) ? 1ULL : Perft(depth - 1);
            printf("%s: %llu\n", MoveToString(moves.moves[i]).c_str(), nodes);
            total += nodes;
        }

        UnmakeMove(moves.moves[i]);
    }

    printf("\nMoves: %d\n", moves.count);
    printf("Total: %llu\n", total);
}

bool Board::IsAttackedByOpponent(int square) {
    sideToMove ^= 1;
    bool attacked = IsSquareAttacked(square);
    sideToMove ^= 1;
    return attacked;
}



void Board::LoadFEN(const std::string &fen) {
    int index = 0;

    ClearBoard();
    ply = 0;

    U64 *bb[12] = {
        &whitePawns, &whiteKnights, &whiteBishops,
        &whiteRooks, &whiteQueens, &whiteKing,

        &blackPawns, &blackKnights, &blackBishops,
        &blackRooks, &blackQueens, &blackKing
    };

    size_t iter = 0;

    for (; iter < fen.size() && fen[iter] != ' '; iter++) {
        if (fen[iter] == '/')
            continue;

        if (isdigit(fen[iter])) {
            index += fen[iter] - '0';
        } else {
            int piece = pieceIndex(fen[iter]);

            if (piece != -1)
                set_bit(*bb[piece], index);

            ++index;
        }
    }

    while (iter < fen.size() && fen[iter] == ' ')
        ++iter;

    if (fen[iter] == 'w')
        sideToMove = white;
    else if (fen[iter] == 'b')
        sideToMove = black;

    ++iter;

    // Skip space
    while (iter < fen.size() && fen[iter] == ' ')
        ++iter;

    castleRights = 0;

    while (iter < fen.size() && fen[iter] != ' ') {

        switch (fen[iter]) {

            case 'K':
                castleRights |= 0b0001;
                break;

            case 'Q':
                castleRights |= 0b0010;
                break;

            case 'k':
                castleRights |= 0b0100;
                break;

            case 'q':
                castleRights |= 0b1000;
                break;
        }

        ++iter;
    }

    // Skip space
    while (iter < fen.size() && fen[iter] == ' ')
        ++iter;

    if (iter < fen.size()) {

        if (fen[iter] == '-') {
            enPassantSquare = -1;
        }
        else {
            int file = fen[iter] - 'a';
            int rank = fen[iter + 1] - '1';

            enPassantSquare =
                (7 - rank) * 8 + file;
        }
    }
}

void Board::ClearBoard() {
    whitePawns = 0ULL;
    whiteRooks = 0ULL;
    whiteKnights = 0ULL;
    whiteBishops = 0ULL;
    whiteKing = 0ULL;
    whiteQueens = 0ULL;

    blackPawns = 0ULL;
    blackRooks = 0ULL;
    blackKnights = 0ULL;
    blackBishops = 0ULL;
    blackKing = 0ULL;
    blackQueens = 0ULL;
}
