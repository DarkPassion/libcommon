
#include <iostream>
#include <unistd.h>

#include "task/thread_queue.h"

// g++ thread_queue.cpp -I../include/ -L../libs/ -lcommon -o thread_queue.out


using namespace libcommon;

void test_thread_queue();

class obj3
{
public:
    void invoke(int cc1, int cc2, int cc3)
    {
        printf("obj3 == > invoke [%d %d %d]\n", cc1, cc2, cc3);
    }
};

void test_thread_queue()
{
    ThreadQueue _queue("task.run.queue");
    obj3 _obj;

    typedef void (obj3::*PFN)(int, int, int);
    PFN _pf = &obj3::invoke;
    task_t* val1 = task_t::bind_memfunc_delay<obj3*, PFN, int, int, int>(&_obj, _pf, 11, 11, 11, 3000);
    task_t* val2 = task_t::bind_memfunc_delay<obj3*, PFN, int, int, int>(&_obj, _pf, 22, 22, 22, 2000);
    task_t* val3 = task_t::bind_memfunc_delay<obj3*, PFN, int, int, int>(&_obj, _pf, 33, 33, 33, 1000);


    _queue.enqueue(val1);
    _queue.enqueue(val2);
    _queue.enqueue(val3);

    usleep(8000*1000);
}

int main()
{

    test_thread_queue();
    
    
    return 0;
}

