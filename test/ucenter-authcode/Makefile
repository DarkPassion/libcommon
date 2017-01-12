

CC=g++
AR=ar
GCC=gcc

RM=rm -f

# "-Wno-xxxx" remove gcc warning
CFLAGS="-Wno-local-type-template-args"

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
        CFLAGS += -l pthread
endif


default: all
auth_code:
	@($(CC) -I$(INC_PATH) $(CFLAGS)  *.cpp $(LIB_PATH) -o auth_code.out)

all:  auth_code

clean:
	@($(RM) *.a *.so *.o *.out)


