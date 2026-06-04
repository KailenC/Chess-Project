#include "../include/board.h"
#include "../include/evaluation.h"
#include "../include/search.h"
#include <iostream>
#include <string>

int main()
{
    Board board;
    Search search;
    Evaluation eval;

    board.InitJumperAttacks();
    board.InitMagics();

    while (true)
    {
        search.NegaMax(board, eval, 2, -100000, 10000);
        board.PrintBitboard(board.AllOccupancy());
    }

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
