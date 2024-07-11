all: compiler

clean:
	@printf "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f *.bin *.gch udp_client udp_server
	@printf "\e[32m\tDONE\e[0m\


server:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING SERVER\e[0m\e[32m |--------\e[0m\n\n"
	g++ server.cpp -o udp_server
	@printf "\e[32m\tDONE\e[0m\

client:
	@printf "\n\e[32m--------| \e[1;32mCOMPILING SERVER\e[0m\e[32m |--------\e[0m\n\n"
	g++ client.cpp -o udp_client
	@printf "\e[32m\tDONE\e[0m\

