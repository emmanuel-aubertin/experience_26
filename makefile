all: client

clean:
	@printf "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f *.bin *.gch udp_client udp_server
	@printf "\e[32m\tDONE\e[0m\n"

install_client:
	git clone https://github.com/ArthurSonzogni/FTXUI.git client/FTXUI
	cd client/FTXUI && mkdir build && cd build && \
	if [ `uname -m` = "arm64" ]; then \
		cmake .. -DCMAKE_OSX_ARCHITECTURES=arm64; \
	else \
		cmake ..; \
	fi && \
	make && sudo make install

server:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING SERVER\e[0m\e[32m |--------\e[0m\n\n"
	clang++ -std=c++20 -stdlib=libc++ server/server.cpp server/Player/*.cpp server/Board/*.cpp  -I/opt/homebrew/opt/nlohmann-json/include -o udp_server
	@printf "\e[32m\tDONE\e[0m\n"

client:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING CLIENT\e[0m\e[32m |--------\e[0m\n\n"
	clang++ -std=c++20 -stdlib=libc++ client/client.cpp client/Board/*.cpp -o udp_client -L/usr/local/lib -lftxui-component -lftxui-dom -lftxui-screen -I/opt/homebrew/opt/nlohmann-json/include
	@printf "\e[32m\tDONE\e[0m\n"

.PHONY: all clean server client install_client
