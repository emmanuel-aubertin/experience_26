#include "Player.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

Player::Player(const std::string &name, const std::string &ipAdresse, const int &port)
{
    this->name = name;
    this->ipAdresse = ipAdresse;
    this->port = port;
}

std::string Player::getName() { return name; }

void Player::sendMessage(const std::string &message)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ipAdresse.c_str());
    serverAddr.sin_port = htons(port);

    const char *to_send = message.c_str();

    sendto(sockfd, to_send, strlen(to_send), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
}