#include <vector>
#include "thread/thread.h"
#include "thread/lock.h"
#include "task/task.h"

namespace libcommon {
    
    
    class ThreadQueue : public ThreadHandle
    {
    public:
        ThreadQueue(const char* name);
        
        ~ThreadQueue();
        
    public:
        // virtual interface
        virtual void on_thread_start();
        
        virtual int on_before_cycle();
        
        virtual int cycle();
        
        virtual int on_end_cycle();
        
        virtual void on_thread_stop();
        
    public:
        // queue
        void enqueue(task_t* param);
        
        void stop_loop();
        
    
    private:
        
        task_t* pop_back();
        
        void clean_queue();
        
        
        
    private:
        typedef std::vector<task_t*> TypedQueue;
        ThreadImp*      _pth;
        TypedQueue      _queue;
        CMutex          _queue_mutex;
        CCond           _queue_push_cond;

        
    };
    
}




