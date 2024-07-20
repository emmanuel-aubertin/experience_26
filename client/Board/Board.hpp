#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <nlohmann/json.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>


using json = nlohmann::json;
using namespace ftxui;

class Board
{
public:
    explicit Board(const json &data);
    explicit Board();
    virtual ~Board() = default;

    void updateBoard(const json &data);
    Element renderBoard();

protected:
    std::vector<std::vector<char>> gameBoard;
    std::vector<std::tuple<std::string, std::tuple<int, int>>> playersCoordinates;
};

#endif
