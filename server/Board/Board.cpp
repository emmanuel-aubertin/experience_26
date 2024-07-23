#include "Board.hpp"
#include <nlohmann/json.hpp>



Board::Board(int size)
{
    this->isFinnish = false;
    this->winner = nullptr;
    initialize_board(size);
}

Board::Board()
{
    this->isFinnish = false;
    this->winner = nullptr;
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

std::optional<Player> Board::getPlayer(const std::string &nickname)
{
    for (auto &player : this->playersCoordinates)
    {
        if (std::get<0>(player).getName() == nickname)
        {
            return std::get<0>(player);
        }
    }
    return std::nullopt;
}

std::tuple<int, int> Board::getPlayersCoordinates(const std::string &nickname)
{
    for (auto &player : this->playersCoordinates)
    {
        if (std::get<0>(player).getName() == nickname)
        {
            return std::get<1>(player);
        }
    }
    return std::make_tuple(-1, -1);
}

int Board::addPlayer(Player player)
{
    for (auto &entry : playersCoordinates)
    {
        if (std::get<0>(entry).getName() == player.getName())
        {
            std::cerr << "Player with nickname " << player.getName() << " already exists." << std::endl;
            return 0;
        }
    }
    playersCoordinates.push_back(std::make_tuple(player, std::make_tuple(0, 0)));
    this->broadcastStatus();
    return 1;
}

int Board::play(const std::string &nickname, const char &input)
{
    std::tuple<int, int> coords = getPlayersCoordinates(nickname);
    int x = std::get<0>(coords);
    int y = std::get<1>(coords);

    if (x == -1 && y == -1)
    {
        std::cerr << "Player with nickname " << nickname << " not found." << std::endl;
        return 0;
    }

    if (x > 0 && this->gameBoard[x - 1][y] == input)
    {
        setCoordinates(nickname, std::make_tuple(x - 1, y));
        return 1;
    }

    if (y < gameBoard.size() - 1 && this->gameBoard[x][y + 1] == input)
    {
        setCoordinates(nickname, std::make_tuple(x, y + 1));
        return 1;
    }

    if (x < gameBoard.size() - 1 && this->gameBoard[x + 1][y] == input)
    {
        setCoordinates(nickname, std::make_tuple(x + 1, y));
        return 1;
    }

    if (y > 0 && this->gameBoard[x][y - 1] == input)
    {
        setCoordinates(nickname, std::make_tuple(x, y - 1));
        return 1;
    }

    std::cerr << "Not able to move that way." << std::endl;

    return 0;
}

Player *Board::getWinner()
{
    if (this->isFinnish)
    {
        return this->winner;
    }
    return nullptr;
}

void Board::broadcastWinner()
{
    if (this->winner != nullptr)
    {
        nlohmann::json winnerMessage;
        winnerMessage["action"] = "winner";
        winnerMessage["nickname"] = this->winner->getName();
        std::string message = winnerMessage.dump();

        for (auto &player : playersCoordinates)
        {
            std::get<0>(player).sendMessage(message);
        }
    }
}

void Board::setCoordinates(const std::string &nickname, const std::tuple<int, int> &coords)
{
    for (auto &player : playersCoordinates)
    {
        if (std::get<0>(player).getName() == nickname)
        {
            std::get<1>(player) = coords;
            if (std::get<0>(coords) == this->gameBoard.size() - 1 && std::get<1>(coords) == this->gameBoard.size() - 1)
            {
                this->winner = &std::get<0>(player);
                isFinnish = true;
            }
            break;
        }
    }
    this->broadcastStatus();
}

void Board::broadcastStatus()
{

    if (gameBoard.empty() || playersCoordinates.empty())
    {
        return;
    }

    nlohmann::json status;
    status["action"] = "status";

    for (const auto &row : gameBoard)
    {
        nlohmann::json jsonRow;
        for (char cell : row)
        {
            jsonRow.push_back(std::string(1, cell));
        }
        status["board"].push_back(jsonRow);
    }

    for (auto &entry : playersCoordinates)
    {
        nlohmann::json playerJson;
        playerJson["nickname"] = std::get<0>(entry).getName();
        playerJson["coordinates"] = {std::get<0>(std::get<1>(entry)), std::get<1>(std::get<1>(entry))};
        status["players"].push_back(playerJson);
    }

    std::string message = status.dump();
    for (auto &player : playersCoordinates)
    {
        std::get<0>(player).sendMessage(message);
    }
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