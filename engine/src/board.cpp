#include "../include/board.h"

void Board::PrintBitboard(U64 bb)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            U64 mask = 1ULL << square;

            std::cout << ((bb & mask) ? "1 " : ". ");
        }
        std::cout << std::endl;
    }
}