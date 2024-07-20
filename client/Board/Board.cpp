#include "Board.hpp"

using namespace ftxui;

Board::Board(){

}

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




#include "Board.hpp"

Element Board::renderBoard() {
    if (gameBoard.empty()) {
        return text("No data available") | hcenter | vcenter;
    }

    std::vector<Element> rows;
    for (const auto &row : gameBoard) {
        std::vector<Element> cells;
        for (const auto &cell : row) {
            cells.push_back(text(std::string(1, cell)) | border);
        }
        rows.push_back(hbox(std::move(cells)));
    }
    return vbox(std::move(rows));
}
