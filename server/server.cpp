#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <array>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include <vector>

#include "Player/Player.hpp"
#include "Board/Board.hpp"

using json = nlohmann::json;
const int BOARD_SIZE = 10;

int main() {
    int port = 8000;

    Board board;

    board.printBoard();

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    std::cout << "UDP server listening on port " << port << std::endl;

    char buffer[1024];
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    while (true) {
        int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr *)&clientAddr, &clientAddrSize);
        if (bytesReceived > 0) {
            std::cout << "Received message from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << ": " << std::string(buffer, bytesReceived) << std::endl;
            json json_message = json::parse(std::string(buffer, bytesReceived));
            if (json_message.contains("action") && json_message["action"] == "register") {
                std::cout << "Registering new player " << std::endl;
                if (json_message.contains("nickname")) {
                    Player *tempPlayer = new Player(json_message["nickname"].get<std::string>(), inet_ntoa(clientAddr.sin_addr), json_message["port"].get<int>());
                    if (!board.addPlayer(*tempPlayer)) {
                        tempPlayer->sendMessage("Nickname already taken !");
                        delete tempPlayer;
                        continue;
                    }
                    std::cout << "Registered player: " << tempPlayer->getName() << std::endl;
                } else {
                    std::cerr << "Unable to parse player info! " << std::endl;
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
