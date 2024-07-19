#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Board
{
public:
    explicit Board(const json &data);
    virtual ~Board() = default;

    void updateBoard(const json &data);

protected:
    std::vector<std::vector<char>> gameBoard;
    std::vector<std::tuple<std::string, std::tuple<int, int>>> playersCoordinates;
};

#endif
