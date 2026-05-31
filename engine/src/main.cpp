#include "../include/board.h"
#include <iostream>
#include <string>

int main()
{
    Board board;

    board.InitJumperAttacks();
    board.InitMagics();

    board.PrintBitboard(board.AllOccupancy());
    board.PrintBitboard(board.Occupany(board.white));
    board.PrintBitboard(board.Occupany(board.black));

    // python interface
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
