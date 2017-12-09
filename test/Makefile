

CC=g++
AR=ar
GCC=gcc

RM=rm -f
INC_PATH="../include"
JSON_CPP_PATH="../3rd/jsoncpp/include"
SOCKET_SERVER_PATH="../3rd/socket_server/"
LIB_PATH="../libs/libcommon.a"

# "-Wno-xxxx" remove gcc warning
CFLAGS="-Wno-local-type-template-args"

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
        CFLAGS += -l pthread
endif


default: all
block_queue:
	@$(echo $(CFLAGS))
	@($(CC) -I$(INC_PATH) $(CFLAGS)  block_queue.cpp $(LIB_PATH) -o block_queue.out)

byte_buffer:
	@($(CC) -I$(INC_PATH) $(CFLAGS) byte_buffer.cpp $(LIB_PATH) -o byte_buffer.out)

digest:
	@($(CC) -I$(INC_PATH) $(CFLAGS) digest.cpp $(LIB_PATH) -o digest.out)

queue:
	@($(CC) -I$(INC_PATH) $(CFLAGS) queue.cpp  $(LIB_PATH) -o queue.out)

scoped_ptr:
	@($(CC) -I$(INC_PATH) $(CFLAGS) scoped_ptr.cpp $(LIB_PATH) -o scoped_ptr.out)

task_test:
	@($(CC) -I$(INC_PATH) $(CFLAGS) task_test.cpp $(LIB_PATH) -o task_test.out)

thread:
	@($(CC) -I$(INC_PATH) $(CFLAGS) thread.cpp $(LIB_PATH) -o thread.out)

timer:
	@($(CC) -I$(INC_PATH) $(CFLAGS) timer.cpp $(LIB_PATH) -o timer.out)

util:
	@($(CC) -I$(INC_PATH) $(CFLAGS) util.cpp $(LIB_PATH) -o util.out)

json:
	@($(CC) -I$(INC_PATH) -I$(JSON_CPP_PATH) $(CFLAGS) json.cpp $(LIB_PATH) -o json.out)

dns_detect:
	@($(CC) -I$(INC_PATH) $(CFLAGS) dns_detect.cpp $(LIB_PATH) -o dns_detect.out)

gfw_dns_resolver:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  gfw_dns_resolver.cpp $(LIB_PATH) -o gfw_dns_resolver.out)

ip_addr:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  ip_addr.cpp $(LIB_PATH) -o ip_addr.out)

ip_server:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  ip_server.cpp $(LIB_PATH) -o ip_server.out)

signal_test:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  test_signal.cpp $(LIB_PATH) -o signal_test.out)

thread_queue:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  thread_queue.cpp $(LIB_PATH) -o thread_queue.out)

http_server:
	@($(GCC) -I$(INC_PATH) -I$(SOCKET_SERVER_PATH) $(CFLAGS)  http_server.c $(LIB_PATH) -o http_server.out)

http_client:
	@($(GCC) -I$(INC_PATH) -I$(SOCKET_SERVER_PATH) $(CFLAGS)  http_client.c $(LIB_PATH) -o http_client.out)

smart_ptr:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  smart_ptr_test.cpp $(LIB_PATH) -o smart_ptr_test.out)

weak_ptr:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  weak_ptr_test.cpp $(LIB_PATH) -o weak_ptr_test.out)




all:  block_queue byte_buffer digest queue scoped_ptr task_test \
	thread timer util json dns_detect gfw_dns_resolver ip_addr ip_server signal_test thread_queue \
	http_server http_client smart_ptr weak_ptr

clean:
	@($(RM) *.a *.so *.o *.out)



