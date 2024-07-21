#include "Board.hpp"

using namespace ftxui;

Board::Board(const std::string &nickname) : nickname(nickname) {}

Board::Board(const json &data)
{
    updateBoard(data);
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
                boardRow.push_back(cell.get<std::string>()[0]);
            }
            std::cout << std::endl;
            gameBoard.push_back(boardRow);
        }
    }

    if (data.contains("players") && data["players"].is_array())
    {
        playersCoordinates.clear();
        for (const auto &player : data["players"])
        {
            std::string nickname = player["nickname"];
            int x = player["coordinates"][0].get<int>();
            int y = player["coordinates"][1].get<int>();
            playersCoordinates.push_back(std::make_tuple(nickname, std::make_tuple(x, y)));
        }
    }
}

Element Board::renderBoard()
{
    if (gameBoard.empty())
    {
        return text("No data available") | hcenter | vcenter;
    }

    std::vector<Element> rows;
    for (size_t y = 0; y < gameBoard.size(); ++y)
    {
        std::vector<Element> cells;
        for (size_t x = 0; x < gameBoard[y].size(); ++x)
        {
            char cell = gameBoard[y][x];
            auto cellElement = text(std::string(1, cell)) | border;

            // Check if there is a player at this position
            for (const auto &player : playersCoordinates)
            {
                std::string playerName = std::get<0>(player);
                int playerX = std::get<1>(std::get<1>(player));
                int playerY = std::get<0>(std::get<1>(player));

                if (playerX == x && playerY == y)
                {
                    if (playerName == this->nickname)
                    {
                        cellElement = cellElement | color(Color::Green);
                    }
                    else
                    {
                        cellElement = cellElement | color(Color::Red);
                    }
                }
            }
            cells.push_back(cellElement);
        }
        rows.push_back(hbox(std::move(cells)));
    }
    return vbox(std::move(rows));
}