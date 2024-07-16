#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int SERVER_PORT = 8000;
    int port = 9092;

    // Create a socket for sending the registration message
    int sockfd_send = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_send < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }



    std::string SERVER_ADDR;
    std::cout << "Server IP: ";
    std::cin >> SERVER_ADDR;


    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR.c_str());
    serverAddr.sin_port = htons(SERVER_PORT);

    std::string playerName;
    std::cout << "Nickname: ";
    std::cin >> playerName;

    std::string message = "{\"action\": \"register\", \"nickname\": \"" + playerName + "\", \"port\": " + std::to_string(port) + "}";
    std::cout << "Sending message: " << message << std::endl;
    sendto(sockfd_send, message.c_str(), message.size(), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));

    while (true)
    {
        std::string input;
        std::cin >> input;

        std::string inputMessage = "{\"action\": \"input\", \"nickname\": \"" + playerName + "\", \"value\": \"" + input + "\"}";
        sendto(sockfd_send, inputMessage.c_str(), inputMessage.size(), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
    }

    close(sockfd_send);
    return 0;
}
