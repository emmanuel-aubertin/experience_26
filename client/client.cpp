#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int SERVER_PORT = 8000;
    int port = 9091;
    const char *SERVER_ADDR = "127.0.0.1";

    // Create a socket for sending the registration message
    int sockfd_send = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_send < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    std::string playerName;
    std::cout << "Nickname: ";
    std::cin >> playerName;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serverAddr.sin_port = htons(SERVER_PORT);

    std::string message = "{\"action\": \"register\", \"nickname\": \"" + playerName + "\", \"port\": " + std::to_string(port) + "}";
    std::cout << "Sending message: " << message << std::endl;
    sendto(sockfd_send, message.c_str(), message.size(), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));

    close(sockfd_send);

    // Create a socket for receiving messages
    int sockfd_recv = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_recv < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(port);

    if (bind(sockfd_recv, (sockaddr *)&clientAddr, sizeof(clientAddr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    std::cout << "UDP client listening on port " << port << std::endl;

    char buffer[1024];
    sockaddr_in serverResponseAddr;
    socklen_t serverResponseAddrSize = sizeof(serverResponseAddr);
    while (true)
    {
        int bytesReceived = recvfrom(sockfd_recv, buffer, sizeof(buffer), 0, (sockaddr *)&serverResponseAddr, &serverResponseAddrSize);
        if (bytesReceived > 0)
        {
            std::cout << "Received message from server: " << std::string(buffer, bytesReceived) << std::endl;
        }
    }

    close(sockfd_recv);
    return 0;
}
