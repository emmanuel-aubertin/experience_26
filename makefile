all: compiler

clean:
	@printf "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f *.bin *.gch udp_client udp_server
	@printf "\e[32m\tDONE\e[0m\n"

server:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING SERVER\e[0m\e[32m |--------\e[0m\n\n"
	g++ -std=c++20 server/server.cpp server/Player/*.cpp -o udp_server
	@printf "\e[32m\tDONE\e[0m\n"

client:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING CLIENT\e[0m\e[32m |--------\e[0m\n\n"
	g++ -std=c++20 client.cpp -o udp_client
	@printf "\e[32m\tDONE\e[0m\n"

.PHONY: all clean server client
