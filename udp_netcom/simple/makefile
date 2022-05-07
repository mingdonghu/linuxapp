all:
	@gcc server.c udp_socket.c -o server
	@gcc client.c udp_socket.c -o client

clean:
	@rm server client
	@echo "clean binary file"