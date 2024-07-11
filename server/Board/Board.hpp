#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "../Player/Player.hpp"

class Board
{
public:
    explicit Board(int size);
    explicit Board();
    virtual ~Board() = default;

    int addPlayer(Player player);

    std::vector<std::vector<char>> getGameBoard();

    void printBoard();

protected:
    std::vector<std::vector<char>> gameBoard;
    std::vector<std::tuple<Player, std::tuple<int, int>>> playersCoordonates;

    void initialize_board(int size);
};

#endif