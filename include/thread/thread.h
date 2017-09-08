#ifndef _THREAD_THREAD_H_
#define _THREAD_THREAD_H_

#include <pthread.h>

namespace libcommon
{
    
    class ThreadHandle
    {
    public:
        ThreadHandle();
        
        virtual ~ThreadHandle();
        
        virtual void on_thread_start();
        
        virtual int on_before_cycle();
        
        virtual int cycle() = 0;
        
        virtual int on_end_cycle();
        
        virtual void on_thread_stop();
    };
    
    
    
    class ThreadImp
    {
        
    public:
        ThreadImp(const char* name, ThreadHandle* handle, int interval_ms, int joinable);
        
        ~ThreadImp();
        
        
    public:
        int start();
        
        int stop();
        
        int can_loop();
        
        void stop_loop();
        
        
    private:
        void thread_cycle();
        
        static void* thread_func(void* param);
        
    private:
        ThreadHandle*   _handle;
        const char*     _name;
        int             _interval_ms;
        pthread_t       _pth;
        int             _loop;
        int             _joinable;
        int             _status;
    };
    
}



#endif
