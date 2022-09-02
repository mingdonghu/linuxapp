.PHONY:clean
all:
	@g++ udp_server_node.cpp network_socket_interface_linux.cpp -o server_udp -std=c++11 -lpthread
	@g++ udp_client_node.cpp network_socket_interface_linux.cpp -o client_udp -std=c++11 -lpthread
	@g++ tcp_server_node.cpp network_socket_interface_linux.cpp -o server_tcp -std=c++11 -lpthread
	@g++ tcp_client_node.cpp network_socket_interface_linux.cpp -o client_tcp -std=c++11 -lpthread
clean:
	@rm server_udp client_udp server_tcp client_tcp
	@echo "clean binary file"