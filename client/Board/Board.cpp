#include "Board.hpp"

Board::Board(const json &data)
{
    if (data.contains("board") && data["board"].is_array())
    {
        for (const auto &row : data["board"])
        {
            std::vector<char> boardRow;
            for (const auto &cell : row)
            {
                boardRow.push_back(cell.get<char>());
            }
            gameBoard.push_back(boardRow);
        }
    }

    if (data.contains("players") && data["players"].is_array())
    {
        for (const auto &player : data["players"])
        {
            std::string nickname = player["nickname"];
            int x = player["coordinates"][0];
            int y = player["coordinates"][1];
            playersCoordinates.push_back(std::make_tuple(nickname, std::make_tuple(x, y)));
        }
    }
}

void Board::updateBoard(const json &data)
{
    if (data.contains("board") && data["board"].is_array())
    {
        gameBoard.clear();
        for (const auto &row : data["board"])
        {
            std::vector<char> boardRow;
            for (const auto &cell : row)
            {
                boardRow.push_back(cell.get<char>());
            }
            gameBoard.push_back(boardRow);
        }
    }

    if (data.contains("players") && data["players"].is_array())
    {
        playersCoordinates.clear();
        for (const auto &player : data["players"])
        {
            std::string nickname = player["nickname"];
            int x = player["coordinates"][0];
            int y = player["coordinates"][1];
            playersCoordinates.push_back(std::make_tuple(nickname, std::make_tuple(x, y)));
        }
    }
}
