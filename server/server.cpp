#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <array>
#include <random>

const int BOARD_SIZE = 10;

std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> init_game_board()
{
    std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> board;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);

    for (auto &row : board)
    {
        for (auto &cell : row)
        {
            int type = distrib(gen);
            if (type == 0)
            {
                cell = static_cast<char>(std::uniform_int_distribution<>(48, 57)(gen)); // digits [0-9]
            }
            else if (type == 1)
            {
                cell = static_cast<char>(std::uniform_int_distribution<>(65, 90)(gen)); // uppercase [A-Z]
            }
            else
            {
                cell = static_cast<char>(std::uniform_int_distribution<>(97, 122)(gen)); // lowercase [a-z]
            }
        }
    }

    board[0][0] = 'S';
    board[BOARD_SIZE - 1][BOARD_SIZE - 1] = 'E';
    return board;
}
void print_board(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> &board)
{
    for (const auto &row : board)
    {
        for (char cell : row)
        {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

int main()
{
    int port = 8000;

    std::vector<std::string> players_registred;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> board = init_game_board();

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    print_board(board);

    std::cout << "UDP server listening on port " << port << std::endl;

    char buffer[1024];
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    while (true)
    {
        int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr *)&clientAddr, &clientAddrSize);
        if (bytesReceived > 0)
        {
            std::cout << "Received message from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << ": " << std::string(buffer, bytesReceived) << std::endl;
        }
    }

    close(sockfd);
    return 0;
}