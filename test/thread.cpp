#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "thread/thread.h"
#include "thread/lock.h"

using namespace libcommon;


// g++ thread.cpp -I../include/ -L../libs/ -lcommon -o thread.out

int start_threads = 0;
int stop_threads = 0;

class Thread1 : public ThreadHandle
{
public:
    Thread1()
    {
        printf("thread1 construct ! \n");

        _pth = new ThreadImp("Thread1", this, 1000, true);
        _pth->start();
        start_threads++;
    }

    ~Thread1()
    {
        printf("thread1 deconstruct! \n");
    }


    virtual int cycle()
    {
//        while (_pth->can_loop()) {
//
//            printf("lopp === \n");
//        }

        AutoLock __lock(_mutex);
        printf("thread cycle  ! \n");

        return -1;
    }

    virtual void on_thread_start()
    {
        printf("on thread start ! \n");
    }

    virtual void on_thread_stop()
    {
        freep(_pth);
        stop_threads++;
        printf("on thread stop ! [%d %d]\n", start_threads, stop_threads);
        delete this;
    }

    virtual int on_before_cycle()
    {
        printf("on before cycle ! \n");
        return 0;
    }

    virtual int on_end_cycle()
    {
        printf("on end cycle ! \n");
        return 0;
    }


private:

    ThreadImp*  _pth;
    CMutex _mutex;

};




int main()
{
    while(1) {
        for (int i = 0; i < 10; ++i)
        {
            /* code */

            Thread1 * t = new Thread1;

        }

        usleep(500 * 1000);
    }

}
