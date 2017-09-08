#include <stdlib.h>
#include <stdio.h>


#include "util/scoped_ptr.h"

// g++ scoped_ptr.cpp -I../include/ -L../libs/ -lcommon -o scoped_ptr.out
// valgrind --tool=memcheck --leak-check=full ./scoped_ptr.out

using namespace libcommon;

void test_share_ptr_func();


struct Foo
{
    int i;

    void func()
    {
        printf("foo func called! [%d]\n", i);
    }

    Foo (int ii)
    {
        i = ii;
    }

    Foo ()
    {
        i = 0;
    }
};

void test_share_ptr_func()
{
    scoped_ptr<Foo> foo(new Foo(1));

    scoped_ptr<Foo> foo1;          // No pointer managed.
    foo1.reset(new Foo(2));        // Foo(2) is managed
    foo1.get()->func();            // Foo(2) func called

    scoped_ptr<Foo[]> foo2(new Foo[100]);
    foo2.get()->func();             // Foo::func on the 0th element.
    foo2[10].i = 100;
    foo2[10].func();                // Foo::Method on the 10th element.


}


int main()
{

    test_share_ptr_func();
    return 0;
}


