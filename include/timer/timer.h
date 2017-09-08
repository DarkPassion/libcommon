#ifndef _TIMER_TIMER_H_
#define _TIMER_TIMER_H_


#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>


#include "thread/thread.h"
#include "thread/lock.h"

namespace libcommon
{
    
    
    class ITimerHandle
    {
    public:
        ITimerHandle();
        
        virtual ~ITimerHandle();
        
        virtual void on_time(int id) = 0;
    };
    
    
    
    struct stimer_t_0;
    
    class CTimer : public ThreadHandle
    {
        
    public:
        ~CTimer();
        
        CTimer(ITimerHandle* handle, int precision);
        
        void set_timer(int id, int elapse);
        
        void kill_timer(int id);
        
        void start();
        
    public:
        // ThreadHandle
        virtual int cycle();
        
        virtual void on_thread_stop();
        
        
    private:
        uint64_t get_tick_count();
        
        void tick();
        
        void clean_queue();
        
    private:
        typedef std::vector<stimer_t_0*> TypeQueue;
        
        TypeQueue       _queue;
        CMutex          _mutex;
        
        ThreadImp*      _pth;
        ITimerHandle*   _handle;
        int             _precision;
        
    };
}


#endif
