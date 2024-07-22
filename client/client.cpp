#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <vector>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <nlohmann/json.hpp>

#include "Board/Board.hpp"

using json = nlohmann::json;
using namespace ftxui;

int main()
{
    int SERVER_PORT = 8000;
    int port = 9091;

    std::string SERVER_ADDR;
    std::string playerName;
    std::string playerInput;
    std::vector<std::string> inputHistory;

    auto screen = ScreenInteractive::FullscreenPrimaryScreen();

    auto server_addr_input = Input(&SERVER_ADDR, "Server IP");
    auto player_name_input = Input(&playerName, "Nickname");
    auto player_input = Input(&playerInput, "Your Move");

    auto enter_button = Button("Enter", screen.ExitLoopClosure());

    auto container = Container::Vertical({
                         server_addr_input,
                         player_name_input,
                         enter_button,
                     }) |
                     flex;

    auto renderer = Renderer(container, [&]
                             { return vbox({
                                          text("   Registration   ") | bold | hcenter,
                                          server_addr_input->Render() | hcenter,
                                          player_name_input->Render() | hcenter,
                                          enter_button->Render() | hcenter,
                                      }) |
                                      border | center | flex; });

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

    int sockfd_recv = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_recv < 0)
    {
        std::cerr << "Error creating receive socket" << std::endl;
        return 1;
    }

    sockaddr_in recvAddr;
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_addr.s_addr = INADDR_ANY;
    recvAddr.sin_port = htons(port);

    if (bind(sockfd_recv, (sockaddr *)&recvAddr, sizeof(recvAddr)) < 0)
    {
        std::cerr << "Error binding receive socket" << std::endl;
        close(sockfd_recv);
        return 1;
    }

    Board* board = new Board(playerName);
    bool game_over = false;
    std::string winner_name;

    std::thread receive_thread([&]
                               {
        char buffer[1024];
        while (true)
        {
            socklen_t addrLen = sizeof(recvAddr);
            int recvLen = recvfrom(sockfd_recv, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&recvAddr, &addrLen);
            if (recvLen > 0)
            {
                buffer[recvLen] = '\0'; // Null-terminate
                json json_message = json::parse(buffer);
                if (json_message.contains("action")) {
                    std::string action = json_message["action"];

                    if (action == "error") {
                        std::cout << "\033[1;31mServer error: " << json_message["message"] << "\033[0m" << std::endl;
                        std::terminate();
                    }

                    if (action == "status") {
                        board->updateBoard(json_message);
                        screen.PostEvent(Event::Custom);
                    }

                    if (action == "winner") {
                        winner_name = json_message["nickname"];
                        game_over = true;
                        screen.PostEvent(Event::Custom);
                        break;
                    }
                }
            }
        } });

    auto send_input = [&](std::string value)
    {
        std::string inputMessage = "{\"action\": \"input\", \"nickname\": \"" + playerName + "\", \"value\": \"" + value + "\"}";
        sendto(sockfd_send, inputMessage.c_str(), inputMessage.size(), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
        inputHistory.push_back(value);
    };

    std::thread input_thread([&]
                             {
        while (true) {
            if (!playerInput.empty() && !game_over) {
                send_input(playerInput);
                playerInput.clear();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            screen.PostEvent(Event::Custom); // Custom event to force redraw
        } });

    auto input_container = Container::Vertical({player_input}) | flex;
    auto input_renderer = Renderer(input_container, [&]
                                   {
        std::vector<Element> history_elements;
        for (auto it = inputHistory.rbegin(); it != inputHistory.rend(); ++it) {
            history_elements.push_back(text(*it) | hcenter);
        }

        auto winner_message = game_over ? text("Winner: " + winner_name) | bold | hcenter | color(Color::Green) : text("");
        
        return hbox({
            board->renderBoard() | flex,
            vbox({
                text("Game Input") | bold | hcenter,
                player_input->Render() | hcenter,
                vbox(history_elements) | hcenter, 
                winner_message,
            }) | border | flex,
        }) | flex; });

    screen.Loop(input_renderer);

    receive_thread.join();
    input_thread.join();

    close(sockfd_send);
    close(sockfd_recv);
    return 0;
}
