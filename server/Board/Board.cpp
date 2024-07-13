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

std::tuple<int, int> Board::getPlayersCoordinates(const std::string &nickname){
    for(auto &player : this->playersCoordinates){
        if(std::get<0>(player).getName() == nickname){
            return std::get<1>(player);
        }
    }
    return std::make_tuple(-1, -1);
}


int Board::addPlayer(Player player) {
    for (auto &entry : playersCoordinates) {
        if (std::get<0>(entry).getName() == player.getName()) {
            std::cerr << "Player with nickname " << player.getName() << " already exists." << std::endl;
            return 0;
        }
    }
    playersCoordinates.push_back(std::make_tuple(player, std::make_tuple(0, 0)));
    return 1;
}

int Board::play(const std::string &nickname, const char &input)
{
    std::tuple<int, int> coords = getPlayersCoordinates(nickname);
    int x = std::get<0>(coords);
    int y = std::get<1>(coords);

    if (x == -1 && y == -1) {
        std::cerr << "Player with nickname " << nickname << " not found." << std::endl;
        return 0;
    }

    // Check each possible direction for a valid move
    if (x > 0 && this->gameBoard[x-1][y] == input) {
        setCoordinates(nickname, std::make_tuple(x-1, y));
        return 1;
    }

    if (y < gameBoard.size() - 1 && this->gameBoard[x][y+1] == input) {
        setCoordinates(nickname, std::make_tuple(x, y+1));
        return 1;
    }

    if (x < gameBoard.size() - 1 && this->gameBoard[x+1][y] == input) {
        setCoordinates(nickname, std::make_tuple(x+1, y));
        return 1;
    }

    if (y > 0 && this->gameBoard[x][y-1] == input) {
        setCoordinates(nickname, std::make_tuple(x, y-1));
        return 1;
    }

    std::cerr << "Not able to move that way." << std::endl;
    return 0;
}


void Board::setCoordinates(const std::string &nickname, const std::tuple<int, int> &coords) 
{
    for (auto &player : playersCoordinates) {
        if (std::get<0>(player).getName() == nickname) {
            std::get<1>(player) = coords;
            return;
        }
    }
    std::cerr << "Player with nickname " << nickname << " not found." << std::endl;
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