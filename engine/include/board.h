#pragma once
#include <iostream>
#include <bitset>
#include <string>
#include <cstddef>
#include <sstream>

// board get/set/pop helpers
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

/*
    Move encoding (32 bits):
    bits  0- 5 : source square (0-63)
    bits  6-11 : target square (0-63)
    bits 12-15 : piece type   (0-11, see enum below)
    bits 16-19 : promoted to  (0-11)
    bit  20    : capture flag
    bit  21    : double pawn push
    bit  22    : en passant
    bit  23    : castling
*/
#define encode_move(src, tgt, piece, promo, capture, dbl, ep, castle) \
    (src) | (tgt << 6) | (piece << 12) | (promo << 16) |              \
        (capture << 20) | (dbl << 21) | (ep << 22) | (castle << 23)

#define get_move_source(m) (m & 0x3f)
#define get_move_target(m) ((m >> 6) & 0x3f)
#define get_move_piece(m) ((m >> 12) & 0xf)
#define get_move_promo(m) ((m >> 16) & 0xf)
#define get_move_capture(m) ((m >> 20) & 0x1)
#define get_move_double(m) ((m >> 21) & 0x1)
#define get_move_ep(m) ((m >> 22) & 0x1)
#define get_move_castle(m) ((m >> 23) & 0x1)

enum
{
    P,
    N,
    B,
    R,
    Q,
    K,
    p,
    n,
    b,
    r,
    q,
    k
};

typedef unsigned long long U64;

struct MoveList
{
    int moves[256];
    int count = 0;
    void add(unsigned int move) { moves[count++] = move; }
};

struct SMagic
{
    U64 mask;
    U64 magic;
};

// clang-format off
static constexpr U64 bishopMagics[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL,
    0x4042004000000ULL,    0x100822020200011ULL,  0xc00444222012000aULL, 0x28808801216001ULL,
    0x400492088408100ULL,  0x201c401040c0084ULL,  0x840800910a0010ULL,   0x82080240060ULL,
    0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL, 0x8144042209100900ULL,
    0x208081020014400ULL,  0x4800201208ca00ULL,   0xf18140408012008ULL,  0x1004002802102001ULL,
    0x841000820080811ULL,  0x40200200a42008ULL,   0x800054042000ULL,     0x88010400410c9000ULL,
    0x520040470104290ULL,  0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL,
    0x941408200c002000ULL, 0x658810000806011ULL,  0x188071040440a00ULL,  0x4800404002011c00ULL,
    0x104442040404200ULL,  0x511080202091021ULL,  0x4022401120400ULL,    0x80c0040400080120ULL,
    0x8040010040820802ULL, 0x480810700020090ULL,  0x102008e00040242ULL,  0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL,  0x19001802081400ULL,   0x200014208040080ULL,
    0x3308082008200100ULL, 0x41010500040c020ULL,  0x4012020c04210308ULL, 0x208220a202004080ULL,
    0x111040120082000ULL,  0x6803040141280a00ULL, 0x2101004202410000ULL, 0x8200000041108022ULL,
    0x21082088000ULL,      0x2410204010040ULL,    0x40100400809000ULL,   0x822088220820214ULL,
    0x40808090012004ULL,   0x910224040218c9ULL,   0x402814422015008ULL,  0x90014004842410ULL,
    0x1000042304105ULL,    0x10008830412a00ULL,   0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

static constexpr U64 rookMagics[64] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,  0x100081001000420ULL,
    0x200020010080420ULL,  0x3001c0002010008ULL,  0x8480008002000100ULL, 0x2080088004402900ULL,
    0x800098204000ULL,     0x2024401000200040ULL, 0x100802000801000ULL,  0x120800800801000ULL,
    0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL, 0x801000060821100ULL,
    0x80044006422000ULL,   0x100808020004000ULL,  0x12108a0010204200ULL, 0x140848010000802ULL,
    0x481828014002800ULL,  0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
    0x100400080208000ULL,  0x2040002120081000ULL, 0x21200680100081ULL,   0x20100080080080ULL,
    0x2000a00200410ULL,    0x20080800400ULL,      0x80088400100102ULL,   0x80004600042881ULL,
    0x4040008040800020ULL, 0x440003000200801ULL,  0x4200011004500ULL,    0x188020010100100ULL,
    0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,  0x200046502000484ULL,
    0x480400080088020ULL,  0x1000422010034000ULL, 0x30200100110040ULL,   0x100021010009ULL,
    0x2002080100110004ULL, 0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL,
    0x101002200408200ULL,  0x40802000401080ULL,   0x4008142004410100ULL, 0x2060820c0120200ULL,
    0x1001004080100ULL,    0x20c020080040080ULL,  0x2935610830022400ULL, 0x44440041009200ULL,
    0x280001040802101ULL,  0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,  0x1004081002402ULL,
};
//clang-format on

static inline int CountBits(U64 bitboard)
{
    int bitCount = 0;

     while (bitboard != 0)
    {
        bitCount++;

        bitboard &= bitboard - 1;
    }

    return bitCount;
}

static inline int GetLSBIndex(U64 bitboard)
{
    return __builtin_ctzll(bitboard);
}

class Board
{
public:
    // functions
    void PrintBitboard(U64 bb);
    void InitJumperAttacks();
    void GenerateMoves(MoveList& list);
    void MakeMove(int move);
    void UnmakeMove(int move);
    U64 Occupany(int side);
    U64 AllOccupancy();
    U64 SetOccupancy(int index, int bitsInMask, U64 attackMask);
    bool IsSquareAttacked(int square);

    // attack masks
    U64 InitRookAttacks(int square, U64 blocker);
    U64 InitBishopAttacks(int square, U64 blocker);

    U64 MaskPawnAttacks(int square, int side);
    U64 MaskRookAttacks(int square);
    U64 MaskKnightAttacks(int square);
    U64 MaskBishopAttacks(int square);
    U64 MaskQueenAttacks(int square);
    U64 MaskKingAttacks(int square);

    // magic
    U64 bishopAttacks(U64 occ, int square);
    U64 rookAttacks(U64 occ, int square);
    void InitMagics();

    // enums

    // clang-format off

    enum {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1
    };

    enum {
        white, black
    };

    /*
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
    */

    // clang-format on

    // not-file masks
    static constexpr U64 notAFile = 18374403900871474942ULL;
    static constexpr U64 notHFile = 9187201950435737471ULL;
    static constexpr U64 notABFile = 18229723555195321596ULL;
    static constexpr U64 notHGFile = 4557430888798830399ULL;

    // attack look up tables
    static U64 pawnAttacks[2][64];
    static U64 knightAttacks[64];
    static U64 queenAttacks[64];
    static U64 kingAttacks[64];

    // Plain magic bitboard
    static U64 mBishopAttacks[64][512];
    static U64 mRookAttacks[64][4096];

    static SMagic mBishopTbl[64];
    static SMagic mRookTbl[64];

    // init bitboards

    U64 whitePawns = 0x00FF000000000000ULL;
    U64 whiteRooks = 0x8100000000000000ULL;
    U64 whiteKnights = 0x4200000000000000ULL;
    U64 whiteBishops = 0x2400000000000000ULL;
    U64 whiteQueens = 0x800000000000000ULL;
    U64 whiteKing = 0x1000000000000000ULL;

    U64 blackPawns = 0xff00ULL;
    U64 blackRooks = 0x81ULL;
    U64 blackKnights = 0x42ULL;
    U64 blackBishops = 0x24ULL;
    U64 blackQueens = 0x8ULL;
    U64 blackKing = 0x10ULL;

    int sideToMove = white;
};