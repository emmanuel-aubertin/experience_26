#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


int main() {
    int SERVER_PORT = 8000;
    const char* SERVER_ADDR = "127.0.0.1";

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serverAddr.sin_port = htons(SERVER_PORT);

    const char* message = "Hello world!";
    sendto(sockfd, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

    close(sockfd);
    return 0;
}