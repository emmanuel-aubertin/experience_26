#include "Board.hpp"

Board::Board(int size)
{
    initialize_board(size);
}

Board::Board()
{
    initialize_board(10);
}

void Board::initialize_board(int size)
{
    std::vector<std::vector<char>> board;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);

    for (int i = 0; i < size; i++)
    {
        std::vector<char> temp;
        for (int j = 0; j < size; j++)
        {
            int type = distrib(gen);
            if (type == 0)
            {
                temp.push_back(static_cast<char>(std::uniform_int_distribution<>(48, 57)(gen))); // digits [0-9]
            }
            else if (type == 1)
            {
                temp.push_back(static_cast<char>(std::uniform_int_distribution<>(65, 90)(gen))); // uppercase [A-Z]
            }
            else
            {
                temp.push_back(static_cast<char>(std::uniform_int_distribution<>(97, 122)(gen))); // lowercase [a-z]
            }
        }
        board.push_back(temp);
    }

    board[0][0] = 'S';
    board[size - 1][size - 1] = 'E';
    this->gameBoard = board;
}

int Board::addPlayer(Player player) {
    for (auto &entry : playersCoordonates) {
        if (std::get<0>(entry).getName() == player.getName()) {
            std::cerr << "Player with nickname " << player.getName() << " already exists." << std::endl;
            return 0;
        }
    }
    playersCoordonates.push_back(std::make_tuple(player, std::make_tuple(0, 0)));
    return 1;
}

void Board::printBoard()
{
    for (const auto &row : gameBoard)
    {
        for (char cell : row)
        {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}