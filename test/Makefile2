

CC=g++
AR=ar
GCC=gcc

RM=rm -f
INC_PATH="../include"
LIB_PATH="../libs/libcommon.a"

# "-Wno-xxxx" remove gcc warning
CFLAGS="-Wno-local-type-template-args"

default: all

src_dirs := .
SRCS=$(foreach dir, $(src_dirs), $(wildcard $(dir)/*.cpp))
EXES=$(patsubst %.cpp,%.out,$(SRCS) )



$(EXES): $(SRCS)
	$(CC) -o $@ $< $(CFLAGS) -I$(INC_PATH) $(LIB_PATH)

all: $(EXES)

clean:
	@($(RM) *.a *.so *.o *.out)


