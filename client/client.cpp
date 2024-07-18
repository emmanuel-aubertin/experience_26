#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/captured_mouse.hpp>

void clear_terminal() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape code to clear the terminal
}

int main()
{
    using namespace ftxui;

    int SERVER_PORT = 8000;
    int port = 9092;

    std::string SERVER_ADDR;
    std::string playerName;

    clear_terminal();

    auto screen = ScreenInteractive::TerminalOutput();

    auto server_addr_input = Input(&SERVER_ADDR, "Server IP");
    auto player_name_input = Input(&playerName, "Nickname");

    auto enter_button = Button("Enter", screen.ExitLoopClosure());

    auto container = Container::Vertical({
        server_addr_input,
        player_name_input,
        enter_button,
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Registration") | bold | hcenter,
            server_addr_input->Render() | hcenter,
            player_name_input->Render() | hcenter,
            enter_button->Render() | hcenter,
        }) | border | center;
    });

    screen.Loop(renderer);

    int sockfd_send = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_send < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR.c_str());
    serverAddr.sin_port = htons(SERVER_PORT);

    std::string message = "{\"action\": \"register\", \"nickname\": \"" + playerName + "\", \"port\": " + std::to_string(port) + "}";
    sendto(sockfd_send, message.c_str(), message.size(), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));

    clear_terminal();

    std::thread input_thread([&] {
        while (true)
        {
            std::string input;
            std::getline(std::cin, input);

            std::string inputMessage = "{\"action\": \"input\", \"nickname\": \"" + playerName + "\", \"value\": \"" + input + "\"}";
            sendto(sockfd_send, inputMessage.c_str(), inputMessage.size(), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
        }
    });

    input_thread.join();
    close(sockfd_send);
    return 0;
}
