#include "../include/board.h"
#include <iostream>
#include <string>

int main()
{
    Board board;
    // board.PrintBitboard(board.whitePawns);

    std::string input;

    while (std::getline(std::cin, input))
    {
        if (input == "hello")
        {
            std::cout << "Hey" << std::endl;
        }
    }

    return 0;
}
