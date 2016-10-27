#include <stdlib.h>
#include <stdio.h>

#include "task/task.h"

// g++ task_test.cpp -I../include/ -L../libs/ -lcommon -o task_test.out



void test_bind_function0();
void test_bind_function1();
void test_bind_function2();
void test_bind_function3();
void test_bind_function4();

void test_bind_object0();
void test_bind_func_obj0();
void test_bind_object1();
void test_bind_object2();
void test_bind_object3();
void test_bind_object4();


void test_ff()
{
    printf("test ff \n");
}

void test_bind_function0()
{
    typedef void (* pf)(void);
    task_t t = task_t::bind_func<pf>(test_ff);
    t();
}

void test_ff1(int cc)
{
    printf("test ff1 [%d]\n", cc);
}
void test_bind_function1()
{
    typedef void (*pf)(int);
    task_t t = task_t::bind_func<pf>(test_ff1, 11);
    t();
}

void test_ff2(int cc1, int cc2)
{
    printf("test ff2 [%d %d]\n", cc1, cc2);
}
void test_bind_function2()
{
    typedef void (*pf)(int, int);
    task_t t = task_t::bind_func<pf>(test_ff2, 11, 22);
    t();
}

void test_ff3(int cc1, int cc2, int cc3)
{

    printf("test ff3 [%d %d %d]\n", cc1, cc2, cc3);
}

void test_bind_function3()
{
    typedef void (*pf)(int, int, int);
    task_t t = task_t::bind_func<pf>(test_ff3, 11, 22, 33);
    t();
}


void test_ff4(int cc1, int cc2, int cc3, int cc4)
{

    printf("test ff4 [%d %d %d %d]\n", cc1, cc2, cc3, cc4);
}

void test_bind_function4()
{
    typedef void (*pf)(int, int, int, int);
    task_t t = task_t::bind_func<pf>(test_ff4, 11, 22, 33, 44);
    t();
}


void test_bind_func_obj0()
{
    struct obj0
    {
        void invoke()
        {
            printf("func obj0 invoke == \n");
        }
    };

    typedef void(obj0::*pf)();
    obj0 _obj;
    pf _pf = &obj0::invoke;

#if 0
    task_t t = task_t::bind_memfunc<obj0*, pf>(&_obj, _pf);
    t();
#endif
}

class obj0
{
public:
    void invoke()
    {
        printf("obj0 invoke === \n");
    }
};
void test_bind_object0()
{
    typedef void (obj0::*pf)();
    obj0 _obj;
    pf _pf = &obj0::invoke;
    task_t t = task_t::bind_memfunc<obj0*, pf>(&_obj, _pf);
    t();
}

class obj1
{
public:

    void invoke(int cc)
    {
        printf("obj1 == > invoke [%d]\n", cc);
    }

};
void test_bind_object1()
{
    typedef void (obj1::*pf)(int);
    obj1 _obj;
    pf _pf = &obj1::invoke;
    task_t t = task_t::bind_memfunc<obj1*, pf, int>(&_obj, _pf, 11);
    t();
}

class obj2
{

public:
    void invoke(int cc1, int cc2)
    {
        printf("obj2 == > invoke [%d %d]\n", cc1, cc2);
    }
};
void test_bind_object2()
{
    typedef void (obj2::*pf)(int, int);
    obj2 _obj;
    pf _pf = &obj2::invoke;
    task_t t = task_t::bind_memfunc<obj2*, pf, int, int>(&_obj, _pf, 11, 22);
    t();
}

class obj3
{
public:
    void invoke(int cc1, int cc2, int cc3)
    {
        printf("obj3 == > invoke [%d %d %d]\n", cc1, cc2, cc3);
    }
};

void test_bind_object3()
{
    typedef void (obj3::*pf)(int, int, int);
    obj3 _obj;
    pf _pf = &obj3::invoke;
    task_t t = task_t::bind_memfunc<obj3*, pf, int, int, int>(&_obj, _pf, 11, 22, 33);
    t();
}


class obj4
{
public:
    void invoke(int cc1, int cc2, int cc3, int cc4)
    {
        printf("obj4 == > invoke [%d %d %d %d]\n", cc1, cc2, cc3, cc4);
    }
};

void test_bind_object4()
{
    typedef void (obj4::*pf)(int, int, int, int);
    obj4 _obj;
    pf _pf = &obj4::invoke;
    task_t t = task_t::bind_memfunc<obj4*, pf, int, int, int, int>(&_obj, _pf, 11, 22, 33, 44);
    t();
}

int main()
{

    test_bind_function0();
    test_bind_function1();
    test_bind_function2();
    test_bind_function3();
    test_bind_function4();


    test_bind_object0();
    test_bind_object1();
    test_bind_object2();
    test_bind_object3();
    test_bind_object4();

    test_bind_func_obj0();
    return 0;
}



