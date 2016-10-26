#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "thread/thread.h"
#include "thread/lock.h"
#include "util/util.h"
#include "queue/queue.h"

// g++ queue.cpp -I../include/ -L../libs/ -lcommon -o queue.out


struct sitem
{
    int seq;
    int id;
};
typedef Queue<sitem> TypeQueue;


class ThreadConsumer : public ThreadHandle
{
public:
    ThreadConsumer(TypeQueue* q)
    {
        printf("ThreadConsumer construct ! \n");
        
        _q = q;
        _pth = new ThreadImp("Thread1", this, 1000, true);
        _pth->start();
    }
    
    ~ThreadConsumer()
    {
        printf("ThreadConsumer deconstruct! \n");
        _pth->stop_loop();
        _pth->stop();
        freep(_pth);
    }
    
    
    virtual int cycle()
    {
        //        while (_pth->can_loop()) {
        //
        //            printf("lopp === \n");
        //        }
        
        sitem* i = _q->pop_front();
        if (i) {
            printf("Counsumer cycle [%p %d %d %zu]\n", i, i->seq, i->id, _q->size());
        }
        freep(i);
        
        return 0;
    }
    
    virtual void on_thread_start()
    {
        printf("on thread start ! \n");
    }
    
    virtual void on_thread_stop()
    {
        printf("on thread stop ! \n");
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
    TypeQueue*   _q;
    
};



class ThreadProducer : public ThreadHandle
{
public:
    ThreadProducer(TypeQueue * q)
    {
        num = 0;
        printf("ThreadProducer construct ! \n");
        _q = q;
        
        _pth = new ThreadImp("Thread1", this, 1000, true);
        _pth->start();
    }
    
    ~ThreadProducer()
    {
        printf("ThreadProducer deconstruct! \n");
        _pth->stop_loop();
        _pth->stop();
        freep(_pth);
    }
    
    
    virtual int cycle()
    {
        //        while (_pth->can_loop()) {
        //
        //            printf("lopp === \n");
        //        }
        
        printf("ThreadProducer cycle end ! \n");
        sitem* i = new sitem();
        i->seq = num++;
        i->id = 1122;
        _q->push_back(i);
        
        return 0;
    }
    
    virtual void on_thread_start()
    {
        printf("on thread start ! \n");
    }
    
    virtual void on_thread_stop()
    {
        printf("on thread stop ! \n");
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
    
    TypeQueue*   _q;
    int         num;
    
};


int main()
{

    printf("queue test ! \n");
    
    TypeQueue myqueue;
    
    ThreadProducer p(&myqueue);
    
    ThreadConsumer c(&myqueue);
    
    while (1) {
        usleep(200 * 1000);
    }
    
    return 0;
}
