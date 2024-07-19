#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <nlohmann/json.hpp>
#include "Player/Player.hpp"
#include "Board/Board.hpp"

using json = nlohmann::json;
const int BOARD_SIZE = 5;
bool GAME_OVER = false;
int TICKS = 2;

void handleIncomingMessages(int sockfd, Board &board)
{
    char buffer[1024];
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    while (!GAME_OVER)
    {
        Player *winner = board.getWinner();
        if (winner != nullptr)
        {
            std::cout << std::endl
                      << "WINNER: " << winner->getName() << std::endl;
            GAME_OVER = true;
        }

        int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr *)&clientAddr, &clientAddrSize);
        if (bytesReceived > 0)
        {
            std::cout << "Received message from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << ": " << std::string(buffer, bytesReceived) << std::endl;
            json json_message = json::parse(std::string(buffer, bytesReceived));

            if (json_message.contains("action"))
            {
                std::string action = json_message["action"];

                if (action == "register")
                {
                    std::cout << "Registering new player" << std::endl;
                    if (json_message.contains("nickname"))
                    {
                        Player tempPlayer(json_message["nickname"].get<std::string>(), inet_ntoa(clientAddr.sin_addr), json_message["port"].get<int>());
                        if (!board.addPlayer(tempPlayer))
                        {
                            tempPlayer.sendMessage("{\"action\": \"error\", \"message\": \"Nickname already taken!\"}");
                            continue;
                        }
                        std::cout << "Registered player: " << tempPlayer.getName() << std::endl;
                        tempPlayer.sendMessage(R"({
                            "action": "success",
                            "message": "Player registered waiting for next game 😎"
                        })");
                    }
                    else
                    {
                        // In theory this should never happen xD
                        std::cerr << "Unable to parse player info!" << std::endl;
                    }
                }
                else if (action == "input")
                {
                    if (json_message.contains("nickname") && json_message.contains("value"))
                    {
                        std::string nickname = json_message["nickname"];
                        char value = json_message["value"].get<std::string>()[0];

                        int result = board.play(nickname, value);
                        Player player = board.getPlayer(nickname);
                        std::string response;

                        // See later if response is useful for the player
                        if (result == 0)
                        {
                            player.sendMessage("{\"action\": \"play\", \"status\": \"ok\"}");
                        }
                        else
                        {
                            player.sendMessage("{\"action\": \"play\", \"status\": \"error\", \"message\": \"Invalid move\"}");
                        }

                        sendto(sockfd, response.c_str(), response.size(), 0, (sockaddr *)&clientAddr, clientAddrSize);
                    }
                }
            }
        }
    }
}

void broadcastStatus(Board &board)
{
    while (!GAME_OVER)
    {
        board.broadcastStatus();
        std::this_thread::sleep_for(std::chrono::seconds(60/TICKS));
    }
}


int main()
{
    int port = 8000;

    Board board(BOARD_SIZE);

    board.printBoard();

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    std::cout << "UDP server listening on port " << port << std::endl;

    std::thread messageHandler(handleIncomingMessages, sockfd, std::ref(board));
    std::thread broadcaster(broadcastStatus, std::ref(board));

    messageHandler.join();
    broadcaster.join();

    

    close(sockfd);
    return 0;
}
