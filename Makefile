echo_server: echo_s echo_c log_s

echo_c: echo_c.o client_functions.o
	gcc -o echo_c.out echo_c.c client_functions.c

echo_s: echo_s.o server_functions.o
	gcc -o echo_s.out echo_s.c server_functions.c

log_s: log_s.o server_functions.o
	gcc -o log_s.out log_s.c server_functions.c

clean:
	rm *.o *.out
