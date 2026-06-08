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

    search.rootDepth = 5;

    std::string input;
    bool run = true;
    while (run && std::getline(std::cin, input))
    {
        if (input == "uci")
        {
            std::cout << "id name KailenEngine\nid author kailen\nuciok\n";
            std::cout.flush();
        }
        else if (input == "isready")
        {
            std::cout << "readyok\n";
            std::cout.flush();
        }
        else if (input.substr(0, 8) == "position")
        {
            // position startpos moves e2e4 e7e5
            board = Board();
            std::string inp = input;
            inp.erase(0, 24);
            while (inp.size() != 0)
            {
                // std::cout << inp << std::endl;
                int src = (inp[0] - 'a') + (8 - (inp[1] - '0')) * 8;
                int tgt = (inp[2] - 'a') + (8 - (inp[3] - '0')) * 8;
                // std::cout << src << " " << tgt << std::endl;
                inp.erase(0, 5);
                // std::cout << inp << std::endl;

                MoveList list;
                board.GenerateMoves(list);

                for (int i = 0; i < list.count; i++)
                {
                    if (get_move_source(list.moves[i]) == src && get_move_target(list.moves[i]) == tgt)
                    {
                        // board.PrintBitboard(board.AllOccupancy());
                        board.MakeMove(list.moves[i]);
                        // board.PrintBitboard(board.AllOccupancy());
                        break;
                    }
                }
            }
            // board.PrintBitboard(board.AllOccupancy());
        }
        else if (input.substr(0, 2) == "go")
        {
            search.NegaMax(board, eval, 5, -10000, 10000);
            int tgt = get_move_target(search.bestMove);
            int src = get_move_source(search.bestMove);
            std::cout << "bestmove ";
            std::cout << (char)('a' + src % 8) << (8 - src / 8);
            std::cout << (char)('a' + tgt % 8) << (8 - tgt / 8) << "\n";
        }
        else if (input == "quit")
        {
            break;
        }
        else if (input == "ucinewgame")
        {
            board = Board();
        }
    }

    /*
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
    */
    return 0;
}
