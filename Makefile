echo_server: client server udp_client

client: client.o client_functions.o
	g++ -o client.out client.c client_functions.c

udp_client: udp_client.o client_functions.o
	g++ -o udp_client.out udp_client.c client_functions.c

server: server.o server_functions.o
	g++ -o server.out server.c server_functions.c

clean:
	rm *.o *.out
