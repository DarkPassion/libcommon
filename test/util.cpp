#include <stdlib.h>
#include <stdio.h>
#include "util/util.h"

using namespace libcommon;

// g++ util.cpp -I../include/ -L../libs/ -lcommon -o util.out
// valgrind --tool=memcheck --leak-check=full ./util.out

void test_util();



struct obj0_t
{
	int id;
	int inc;
	int seq;
};

void test_util()
{
	char* tmp = new char[100];
	AutoDeleteScopePtr(char, tmp);

	char* tmp1 = new char;
	AutoDeleteScopePtr(char, tmp1);

	obj0_t * obj0 = new obj0_t;
	AutoFreeScopePtr(obj0_t, obj0);

	if (obj0)
	{
		/* code */
		obj0->id = 100;
		obj0->inc = 101;
		obj0->seq = 11;
	}

	printf("[%d %d %d]\n",obj0->id, obj0->inc, obj0->seq);
	/*
		some process with obj0
	 */

	obj0_t* obja = new obj0_t[10];
	AutoDeleteScopePtr(obj0_t[], obja);


	char* aa1 = (char*)malloc(100);
	AutoFreeScopePtr(char, aa1);

}


int main()
{
	test_util();

	return 0;
}



