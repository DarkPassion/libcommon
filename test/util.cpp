#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util/util.h"

using namespace libcommon;

// g++ util.cpp -I../include/ -L../libs/ -lcommon -o util.out
// valgrind --tool=memcheck --leak-check=full ./util.out

void test_util_scoped_ptr();
void test_util_scoped_array_ptr();
void test_util_scoped_ptr_malloc();

struct obj0_t
{
	int id;
	int inc;
	int seq;
};



void func_obj0_t(obj0_t* obj)
{
    if (obj) {
        printf("-------- obj->id  = %d \n", obj->id);
        printf("-------- obj->inc  = %d \n", obj->inc);
        printf("-------- obj->seq = %d \n", obj->seq);
    } else {
        printf("-------- obj == null \n");
    }
}

void test_util_scoped_ptr()
{
    scoped_ptr<char> tmp;
    
    scoped_ptr<obj0_t> obj1;
    obj1.reset(new obj0_t());
    if (obj1.get()) {
        obj1->id = 100;
        obj1->inc = 100;
        obj1->seq = 100;
    }
    
    func_obj0_t(obj1.get());
    obj1.reset();
    func_obj0_t(obj1.get());
}

void test_util_scoped_array_ptr()
{
    scoped_array<char> obj1;
    char* pt = new char[100];
    obj1.reset(pt);
    if (obj1.get() != pt) {
        printf("error ------ scoped_array get not equal ! \n");
    }

    char h[] = "hello world";
    memcpy(pt, h, sizeof(h));
    
    printf(" ------ %s \n", h);
    printf(" ------ %s \n", obj1.get());
    if (strcmp(h, obj1.get()) != 0) {
        printf("error ------ scoped_array get not equal ! \n");
    }
    
}


void test_util_scoped_ptr_malloc()
{
    scoped_ptr_malloc<char> obj1;
    if (obj1.get() != NULL) {
        printf("error ----- scoped_ptr_malloc get not null ! \n");
    }
    char* pt = (char*)malloc(100);
    char h[] = "hello world";
    memcpy(pt, h, sizeof(h));
    obj1.reset(pt);
    if (obj1.get() != pt) {
        printf("error ------- scoped_ptr_malloc get ! \n");
    }
    
    if (strcmp(pt, obj1.get()) != 0) {
        printf("error ------ scoped_ptr_malloc strcmp not equal \n");
    }

}


int main()
{
    test_util_scoped_ptr();
    test_util_scoped_array_ptr();
    test_util_scoped_ptr_malloc();
    
	return 0;
}



