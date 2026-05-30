#include "../include/board.h"
#include <iostream>
#include <string>

int main()
{
    Board board;

    board.whitePawns >> 1;

    U64 bitboard = 0ULL;
    set_bit(bitboard, board.e2);
    board.PrintBitboard(bitboard);

    pop_bit(bitboard, board.e2);
    board.PrintBitboard(bitboard);

    pop_bit(bitboard, board.e2);
    board.PrintBitboard(bitboard);

    bool doInterface = false;
    std::string input;

    if (doInterface)
    {
        while (std::getline(std::cin, input))
        {
            if (input == "hello")
            {
                std::cout << "Hey" << std::endl;
            }
        }
    }

    return 0;
}
