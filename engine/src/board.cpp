#include "../include/board.h"

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