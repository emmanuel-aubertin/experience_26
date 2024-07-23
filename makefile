# Variables
CXX = g++
CXXFLAGS = -std=c++2b
LIBS = -lftxui-component -lftxui-dom -lftxui-screen
INCLUDES_LINUX = -I/usr/include/nlohmann
INCLUDES_DARWIN = -I/opt/homebrew/opt/nlohmann-json/include
CLIENT_SRCS = client/client.cpp client/Board/*.cpp
SERVER_SRCS = server/server.cpp server/Player/*.cpp server/Board/*.cpp

# Detect platform
UNAME := $(shell uname)
ARCH := $(shell uname -m)

# Targets
all: client

clean:
	@printf "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f *.bin *.gch udp_client udp_server
	@printf "\e[32m\tDONE\e[0m\n"

install-json:
	@printf "\n\e[32m--------| \e[1;32mINSTALLING JSON LIBRARY\e[0m\e[32m |--------\e[0m\n\n"
	@if [ $(UNAME) = "Linux" ]; then \
		sudo apt-get update && sudo apt-get install -y g++ nlohmann-json3-dev; \
	elif [ $(UNAME) = "Darwin" ]; then \
		brew install nlohmann-json; \
	else \
		printf "\e[31mUnsupported OS\e[0m\n"; \
		exit 1; \
	fi
	@printf "\e[32m\tDONE\e[0m\n"

install_client: install-json
	@printf "\n\e[32m--------| \e[1;32mINSTALLING DEPS\e[0m\e[32m |--------\e[0m\n\n"
	git clone https://github.com/ArthurSonzogni/FTXUI.git client/FTXUI
	cd client/FTXUI && mkdir build && cd build && \
	cmake .. $(if $(filter $(ARCH), arm64), -DCMAKE_OSX_ARCHITECTURES=arm64) && \
	make && sudo make install
	@printf "\e[32m\tDONE\e[0m\n"

server:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING SERVER\e[0m\e[32m |--------\e[0m\n\n"
	@if [ $(UNAME) = "Linux" ]; then \
		$(CXX) -std=c++23 $(SERVER_SRCS) $(INCLUDES_LINUX) -o udp_server; \
	elif [ $(UNAME) = "Darwin" ]; then \
		clang++ $(CXXFLAGS) -stdlib=libc++ $(SERVER_SRCS) $(INCLUDES_DARWIN) -o udp_server; \
	else \
		printf "\e[31mUnsupported OS\e[0m\n"; \
		exit 1; \
	fi
	@printf "\e[32m\tDONE\e[0m\n"

client:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING CLIENT\e[0m\e[32m |--------\e[0m\n\n"
	@if [ $(UNAME) = "Linux" ]; then \
		$(CXX) -std=c++23 $(CLIENT_SRCS) -o udp_client -L/usr/local/lib $(LIBS) $(INCLUDES_LINUX); \
	elif [ $(UNAME) = "Darwin" ]; then \
		clang++ $(CXXFLAGS) -stdlib=libc++ $(CLIENT_SRCS) -o udp_client -L/usr/local/lib $(LIBS) $(INCLUDES_DARWIN); \
	else \
		printf "\e[31mUnsupported OS\e[0m\n"; \
		exit 1; \
	fi
	@printf "\e[32m\tDONE\e[0m\n"

.PHONY: all clean server client install_client install-json
