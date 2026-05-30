#pragma once
#include <iostream>
#include <bitset>
#include <string>

// board get/set/pop helpers
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

typedef unsigned long long U64;

class Board
{
public:
    void PrintBitboard(U64 bb);

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

    // it might be good to have two position arrays one for black and one for white
    U64 position[12];

    U64 whitePawns = 0x00FF000000000000ULL;
    U64 whiteRooks;
    U64 whiteKnights;
    U64 whiteBishops;
    U64 whiteQueens;
    U64 whiteKing;

    U64 blackPawns;
    U64 blackRooks;
    U64 blackKnights;
    U64 blackBishops;
    U64 blackQueens;
    U64 blackKing;
};