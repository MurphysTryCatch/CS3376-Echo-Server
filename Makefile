echo_server: client server udpClient

client: client.o client_functions.o
	g++ -o client.out client.c client_functions.c

udpClient: udpClient.o client_functions.o
	g++ -o udpClient.out udpClient.c client_functions.c

server: server.o server_functions.o
	g++ -o server.out server.c server_functions.c

clean:
	rm *.o *.out
