#include <catch2/catch_test_macros.hpp>
#include "board.h"

TEST_CASE("Board initialization and side to move", "[board]") {
    constexpr Board board;

    REQUIRE(board.sideToMove == Board::white);

    REQUIRE(board.Occupancy(Board::white) != 0);
    REQUIRE(board.Occupancy(Board::black) != 0);
}