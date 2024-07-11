#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>

class Player
{
public:
    explicit Player(const std::string &name, const std::string &ipAdresses, const int &port);
    virtual ~Player() = default;
    std::string getName();
    void sendMessage(const std::string &message);

protected:
    std::string ipAdresse;
    std::string name;
    int port;
};
#endif