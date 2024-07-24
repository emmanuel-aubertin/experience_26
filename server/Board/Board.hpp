#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <optional>

#include "../Player/Player.hpp"

class Board
{
public:
    explicit Board(int size);
    explicit Board();
    virtual ~Board() = default;

    int addPlayer(Player player);
    int play(const std::string &nickname, const char &input);
    std::optional<Player> getPlayer(const std::string &nickname);
    std::tuple<int, int> getPlayersCoordinates(const std::string &nickname);
    std::vector<std::vector<char>> getGameBoard();
    Player *getWinner();

    void broadcastWinner();
    void broadcastStatus();
    void broadcastMessage(const std::string &message);

    void printBoard();

protected:
    bool isFinnish;
    Player *winner;
    std::vector<std::vector<char>> gameBoard;
    std::vector<std::tuple<Player, std::tuple<int, int>>> playersCoordinates;
    void setCoordinates(const std::string &nickname, const std::tuple<int, int> &coords);
    void initialize_board(int size);
};

#endif