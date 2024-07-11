#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <array>
#include <random>

std::array<std::array<char, 100>, 100> init_game_board() {
    std::array<std::array<char, 100>, 100> board;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(97, 106);

    for (auto& row : board) {
        for (auto& cell : row) {
            cell = static_cast<char>(distrib(gen));
        }
    }

    board[0][0] = 'S';
    board[99][99] = 'E';
    return board;
}

void print_board(const std::array<std::array<char, 100>, 100>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

int main() {
    int port = 8000;
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    std::array<std::array<char, 100>, 100> board = init_game_board();


    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }
    
    print_board(board);

    std::cout << "UDP server listening on port " << port << std::endl;

    
    char buffer[1024];
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    while (true) {
        int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived > 0) {
            std::cout << "Received message from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << ": " << std::string(buffer, bytesReceived) << std::endl;
        }
    }

    
    close(sockfd);
    return 0;
}