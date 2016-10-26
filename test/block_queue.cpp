#include <stdlib.h>
#include <unistd.h>
#include <queue/block_queue.h>
#include <thread/lock.h>
#include <thread/thread.h>


// g++ block_queue.cpp -I../include/ -L../libs/ -lcommon -o block_queue.out

struct sitem
{
    int seq;
    int id;
};
typedef BlockQueue<sitem> TypeQueue;


class ThreadConsumer : public ThreadHandle
{
public:
    ThreadConsumer(TypeQueue* q)
    {
        printf("ThreadConsumer construct ! \n");
        
        _q = q;
        _pth = new ThreadImp("Thread1", this, 100, true);
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




int main()
{
    TypeQueue _q;
    ThreadConsumer c(&_q);
    
    int num = 0;
    while (1) {
        sitem* i = new sitem;
        i->seq = num++;
        i->id = 1122;
        _q.push_back(i);
        
        usleep(1000* 1000);
        
        printf("producer add sitem [%d %d ]\n", i->seq, i->id);
    }
    return 0;
}
