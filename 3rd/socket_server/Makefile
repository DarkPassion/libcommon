
RM = rm -rf

default : all

#all : libsocket socket-test http-server http-client
all : libsocket

libsocket: socket_server.c
	gcc -c $^
	ar -r libsocket.a socket_server.o
	$(RM) socket_server.o


socket-test : test.c
	gcc -g -Wall -o $@ $^ -lpthread -lsocket -L.

http-server : http_server.c
	gcc -g -Wall -o $@ $^ -lpthread -lsocket -L.

http-client : http_client.c
	gcc -g -Wall -o $@ $^ -lpthread -lsocket -L.


clean:
	$(RM) socket-test
	$(RM) http-server
	$(RM) http-client
	$(RM) socket-test.*
	$(RM) http-server.*
	$(RM) http-client.*
	$(RM) libsocket.a
