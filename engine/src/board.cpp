#include "../include/board.h"

U64 Board::mBishopAttacks[64][512];
U64 Board::mRookAttacks[64][4096];

void Board::PrintBitboard(U64 bb)
{
    std::cout << std::endl;
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            if (!file)
            {
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

void Board::InitJumperAttacks()
{
    for (int square = 0; square < 64; square++)
    {
        pawnAttacks[white][square] = MaskPawnAttacks(square, white);
        pawnAttacks[black][square] = MaskPawnAttacks(square, black);
        knightAttacks[square] = MaskKnightAttacks(square);
        kingAttacks[square] = MaskKingAttacks(square);

        // Testing
        // PrintBitboard(kingAttacks[square]);
    }
}

U64 Board::Occupany(int side)
{
    if (!side)
    {
        return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
    }
    else
    {
        return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
    }
}

U64 Board::AllOccupancy()
{
    return Occupany(white) | Occupany(black);
}

U64 Board::SetOccupancy(int index, int bitsInMask, U64 attackMask)
{
    U64 occupancy = 0ULL;
    for (int count = 0; count < bitsInMask; count++)
    {
        int square = GetLSBIndex(attackMask);
        pop_bit(attackMask, square);
        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }
    return occupancy;
}

U64 Board::InitRookAttacks(int square, U64 blocker)
{
    U64 attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (1ULL << (r * 8 + tf) & blocker)
            break;
    }
    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (1ULL << (r * 8 + tf) & blocker)
            break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (1ULL << (tr * 8 + f) & blocker)
            break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (1ULL << (tr * 8 + f) & blocker)
            break;
    }

    return attacks;
}

U64 Board::InitBishopAttacks(int square, U64 blocker)
{
    U64 attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & blocker)
            break;
    }

    return attacks;
}

U64 Board::MaskPawnAttacks(int square, int side)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if (!side)
    {
        if (bitboard >> 7 & notAFile)
            attacks |= (bitboard >> 7);

        if (bitboard >> 9 & notHFile)
            attacks |= (bitboard >> 9);
    }
    else
    {
        if (bitboard << 7 & notHFile)
            attacks |= (bitboard << 7);

        if (bitboard << 9 & notAFile)
            attacks |= (bitboard << 9);
    }

    return attacks;
}

U64 Board::MaskRookAttacks(int square)
{
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

U64 Board::MaskKnightAttacks(int square)
{
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

U64 Board::MaskBishopAttacks(int square)
{
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

U64 Board::MaskQueenAttacks(int square)
{
    return MaskRookAttacks(square) | MaskBishopAttacks(square);
}

U64 Board::MaskKingAttacks(int square)
{
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

U64 Board::bishopAttacks(U64 occ, int square)
{
    occ &= mBishopTbl[square].mask;
    occ *= mBishopTbl[square].magic;
    occ >>= 64 - 9;
    return mBishopAttacks[square][occ];
}
U64 Board::rookAttacks(U64 occ, int square)
{
    occ &= mRookTbl[square].mask;
    occ *= mRookTbl[square].magic;
    occ >>= 64 - 12;
    return mRookAttacks[square][occ];
}
void Board::InitMagics()
{
    for (int square = 0; square < 64; square++)
    {
        // bishop
        mBishopTbl[square].mask = MaskBishopAttacks(square);
        mBishopTbl[square].magic = bishopMagics[square];

        int bBits = CountBits(mBishopTbl[square].mask);
        for (int i = 0; i < (1 << bBits); i++)
        {
            U64 occ = SetOccupancy(i, bBits, mBishopTbl[square].mask);
            int index = (occ * mBishopTbl[square].magic) >> (64 - 9);
            mBishopAttacks[square][index] = InitBishopAttacks(square, occ);
        }
        // rook
        mRookTbl[square].mask = MaskRookAttacks(square);
        mRookTbl[square].magic = rookMagics[square];

        int rBits = CountBits(mRookTbl[square].mask);
        for (int i = 0; i < (1 << rBits); i++)
        {
            U64 occ = SetOccupancy(i, rBits, mRookTbl[square].mask);
            int index = (occ * mRookTbl[square].magic) >> (64 - 12);
            mRookAttacks[square][index] = InitRookAttacks(square, occ);
        }
    }
}