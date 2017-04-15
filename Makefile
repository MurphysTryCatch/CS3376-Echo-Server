echo_server: client server

client: client.o client_functions.o
	g++ -o client.out client.c client_functions.c

server: server.o server_functions.o
	g++ -o server.out server.c server_functions.c

clean:
	rm *.o *.out
