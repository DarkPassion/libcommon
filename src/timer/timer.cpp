#include <algorithm>
#include <assert.h>
#include <sys/time.h>
#include "timer/timer.h"
#include "util/log.h"

namespace libcommon
{


    
    struct stimer_t_0
    {
        int id;
        int elapse;
        uint64_t last_timestamp;
    };
    
    // ITimerHandle
    //
    
    ITimerHandle::ITimerHandle()
    {
        
    }
    
    ITimerHandle::~ITimerHandle()
    {
        
    }
    
    
    
    
    // CTimer
    //
    
    CTimer::~CTimer()
    {
        _pth->stop();
        clean_queue();
    }
    
    
    CTimer::CTimer(ITimerHandle* handle, int precision)
    {
        _handle = handle;
        _precision = precision;
        
        _pth = new ThreadImp("CTimer", this, 0, true);
    }
    
    void CTimer::start()
    {
        assert(_pth);
        _pth->start();
        
    }
    
    
    int CTimer::cycle()
    {
        while (_pth->can_loop())
        {
            tick();
            
            if (_precision)
            {
                usleep(_precision);
            }
        }
        return 0;
    }
    
    void CTimer::on_thread_stop()
    {
        freep(_pth);
        LOGD("CTimer on_thread_stop ");
    }
    
    void CTimer::tick()
    {
        AutoLock __lock(_mutex);
        uint64_t curr_timestamp = gettickcount();
        for (TypeQueue::iterator iter = _queue.begin(); iter != _queue.end(); iter++)
        {
            stimer_t_0* t = *iter;
            
            if (curr_timestamp - t->last_timestamp > (uint64_t)t->elapse)
            {
                _handle->on_time(t->id);
                t->last_timestamp = curr_timestamp;
            }
        }
        
    }
    
   
    
    
    void CTimer::clean_queue()
    {
        AutoLock __lock(_mutex);
        while (_queue.size())
        {
            stimer_t_0* t = _queue.at(0);
            freep(t);
            _queue.erase(_queue.begin());
        }
    }
    
    
    void CTimer::set_timer(int id, int elapse)
    {
        AutoLock __lock(_mutex);
        
        LOGD("CTimer set_timer [%d %d]", id, elapse);
        int bfind = 0;
        for (TypeQueue::iterator iter = _queue.begin(); iter != _queue.end(); iter++)
        {
            stimer_t_0* t = *iter;
            
            if (t->id == id)
            {
                // find old timer
                t->elapse = elapse;
                t->last_timestamp = gettickcount();
                bfind = 1;
                break;
            }
        }
        
        // new timer
        if (!bfind)
        {
            
            stimer_t_0* t = new stimer_t_0;
            t->id = id;
            t->elapse = elapse;
            t->last_timestamp = gettickcount();
            _queue.push_back(t);
        }
        
    }
    
    
    void CTimer::kill_timer(int id)
    {
        AutoLock __lock(_mutex);
        
        for (TypeQueue::iterator iter = _queue.begin(); iter != _queue.end(); iter++)
        {
            stimer_t_0* t = *iter;
            if (t->id == id)
            {
                _queue.erase(iter);
                freep(t);
                break;
            }
        }
    }

    
    uint64_t gettickcount()
    {
        struct timeval val;
        gettimeofday(&val, NULL);
        
        uint64_t t = val.tv_sec * 1000 + val.tv_usec / 1000;
        return t;
    }
    
}
