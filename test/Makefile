

CC=g++
AR=ar
GCC=gcc

RM=rm -f
INC_PATH="../include"
LIB_PATH="../libs/libcommon.a"

# "-Wno-xxxx" remove gcc warning
CFLAGS="-Wno-local-type-template-args"

default: all
block_queue:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) block_queue.cpp -o block_queue.out)

byte_buffer:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) byte_buffer.cpp  -o byte_buffer.out)

digest:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) digest.cpp -o digest.out)

queue:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) queue.cpp -o queue.out)

scoped_ptr:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) scoped_ptr.cpp -o scoped_ptr.out)

task_test:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) $(CFLAGS) task_test.cpp -o task_test.out)

thread:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) thread.cpp -o thread.out)

timer:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) timer.cpp -o timer.out)

util:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) util.cpp -o util.out)

json:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) json.cpp -o json.out)

dns_detect:
	@($(CC) -I$(INC_PATH) $(LIB_PATH) dns_detect.cpp -o dns_detect.out)

gfw_dns_resolver:
	@($(CC) -I$(INC_PATH) $(LIB_PATH)  gfw_dns_resolver.cpp -o gfw_dns_resolver.out)

ip_addr:
	@($(CC) -I$(INC_PATH) $(LIB_PATH)  ip_addr.cpp -o ip_addr.out)

all:  block_queue byte_buffer digest queue scoped_ptr task_test thread timer util dns_detect gfw_dns_resolver ip_addr


clean:
	@($(RM) *.a *.so *.o *.out)


