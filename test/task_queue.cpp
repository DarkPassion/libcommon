

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "task/task.h"
#include "task/task_queue.h"


// g++ task_queue.cpp -I../include/ -L../libs/ -lcommon -o task_queue.out

class ClassA
{
public:
    ClassA()
    {
        _pth.start_work();
    }
    
    ~ClassA()
    {
    
    }
    
    
    void asynrun(std::string url)
    {
        printf("func [%s] url [%s] \n", __FUNCTION__, url.c_str());
    }
    
    
    void test_run()
    {
        typedef void (ClassA::*PFUNC)(std::string);
        PFUNC _pf = &ClassA::asynrun;
        
        for (int i = 0; i < 10; i++) {
            char buff[64] = {0};
            sprintf(buff, "hellworld %d", i);
            task_t* task = task_t::bind_memfunc_delay<ClassA*, PFUNC, std::string>(this, _pf, std::string(buff), i * 1000);
            _pth.add_task(task);
        }
        
    }
private:
    TaskQueueThread _pth;
    
};







int main()
{

    ClassA* clsa = new ClassA();
    clsa->test_run();
    
    while (1) {
        usleep(100 * 1000);
    }
    
    return 0;
}

