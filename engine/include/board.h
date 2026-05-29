#pragma once
#include <iostream>
#include <bitset>

typedef unsigned long long U64;

class Board
{
public:
    void PrintBitboard(U64 bb);

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