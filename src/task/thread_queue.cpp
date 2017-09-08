
#include <unistd.h>
#include <algorithm>
#include "task/thread_queue.h"
#include "util/log.h"

namespace libcommon {
    
    
    ThreadQueue::ThreadQueue(const char* name) : _queue_push_cond(_queue_mutex)
    {
        _pth = new ThreadImp(name, this, 1000, true);
        _pth->start();
        LOGD("ThreadQueue -------- ");

    }
    
    
    ThreadQueue::~ThreadQueue()
    {
        LOGD("ThreadQueue -------- deconstruct!");

        _pth->stop_loop();
        _queue_push_cond.notify_all();

        _pth->stop();
        delete _pth;
    }
    
    
    
    void ThreadQueue::on_thread_start()
    {
    
    }

    void ThreadQueue::on_thread_stop()
    {
    
    }
    
    
    int ThreadQueue::on_before_cycle()
    {
        return 0;
    }
    
    int ThreadQueue::on_end_cycle()
    {
        return 0;
    }
    
    int ThreadQueue::cycle()
    {
        while(_pth->can_loop())
        {
            task_t* val = this->pop_back();
            
            if (!_pth->can_loop()) {
                break;
            }

            if (!val) {
                break;
            }
            
            if (val) {
            
                if (val->can_exec()) {
                    val->exec();
                    delete val;
                } else {
                    this->enqueue(val);
                    usleep(10*1000);
                }
            }
        }
        
        LOGD("ThreadQueue -------- cycle end !");

        this->clean_queue();
        
        return 0;
    }
    
    bool _ThreadQueueCompare(const task_t* lhs, const task_t* rhs)
    {
        return lhs->get_timestamp() < rhs->get_timestamp();
    }
    
    void ThreadQueue::enqueue(libcommon::task_t *param)
    {
    
        AutoLock __lock(_queue_mutex);
        _queue.push_back(param);
        
        if (_queue.size() > 1) {
            std::sort(_queue.begin(), _queue.end(), _ThreadQueueCompare);
        }
        _queue_push_cond.notify_all();

    }
    
    task_t* ThreadQueue::pop_back()
    {
        AutoLock __lock(_queue_mutex);
        task_t* val = NULL;
        
        if (_queue.size() == 0) {
            _queue_push_cond.wait();
        }
        
        if (_queue.size())
        {
            val = _queue.front();
            _queue.erase(_queue.begin());
        }
        
        return val;
    }
    
    
    void ThreadQueue::stop_loop()
    {
        _pth->stop_loop();
        
        _queue_push_cond.notify_all();
    }
    
    
    void ThreadQueue::clean_queue()
    {
        AutoLock __lock(_queue_mutex);
        
        while (_queue.size() > 0) {
            task_t* val = _queue.front();
            _queue.erase(_queue.begin());
            freep(val);
        }
    
    }
    
}





