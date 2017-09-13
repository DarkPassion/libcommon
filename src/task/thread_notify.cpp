
#include "task/thread_notify.h"
#include "task/thread_queue.h"

namespace libcommon {
    
    
    ThreadNotify::ThreadNotify() : _pth(new ThreadQueue("thread_notify"))
    {
    
    }
    
    ThreadNotify::~ThreadNotify()
    {
        delete _pth;
    }
    
    
    void ThreadNotify::register_signal1(const char* name)
    {
        typedef void (ThreadNotify::*PFN)(const char*);
        PFN pf = &ThreadNotify::_do_register_signal1;
        task_t* t = task_t::bind_memfunc_delay<ThreadNotify*, PFN>(this, pf, name, 0);
        _pth->enqueue(t);
    }
    
    void ThreadNotify::_do_register_signal1(const char *name)
    {
        TypedeSignal1::iterator iter = _signal1.find(name);
        if (iter == _signal1.end()) {
            _signal1[name] = new Signal1<void*>;
        }
    }


    void ThreadNotify::unregister_signal1(const char* name)
    {
        typedef void (ThreadNotify::*PFN)(const char*);
        PFN pf = &ThreadNotify::_do_unregister_signal1;
        task_t* t = task_t::bind_memfunc_delay<ThreadNotify*, PFN>(this, pf, name, 0);
        _pth->enqueue(t);
    }
    
    void ThreadNotify::_do_unregister_signal1(const char* name)
    {
        TypedeSignal1::iterator iter = _signal1.find(name);
        if (iter != _signal1.end()) {
            delete iter->second;
            _signal1.erase(iter);
        }
    }
  
    
    void ThreadNotify::connect_signal1(const char* name, Delegate1< void* > delegate)
    {
        typedef void (ThreadNotify::*PFN)(const char*, Delegate1< void* >);
        PFN pf = &ThreadNotify::_do_connect_signal1;
        task_t* t = task_t::bind_memfunc_delay<ThreadNotify*, PFN>(this, pf, name, delegate, 0);
        _pth->enqueue(t);
    }

  
    void ThreadNotify::_do_connect_signal1(const char* name, Delegate1< void* > delegate)
    {
        TypedeSignal1::iterator iter = _signal1.find(name);
        if (iter != _signal1.end()) {
            iter->second->Connect(delegate);
        }
    }
    

    void ThreadNotify::disconnect_signal1(const char* name, Delegate1< void* > delegate)
    {
        typedef void (ThreadNotify::*PFN)(const char*, Delegate1< void* >);
        PFN pf = &ThreadNotify::_do_disconnect_signal1;
        task_t* t = task_t::bind_memfunc_delay<ThreadNotify*, PFN>(this, pf, name, delegate, 0);
        _pth->enqueue(t);
    }

    void ThreadNotify::_do_disconnect_signal1(const char* name, Delegate1< void* > delegate)
    {
        TypedeSignal1::iterator iter = _signal1.find(name);
        if (iter != _signal1.end()) {
            iter->second->Disconnect(delegate);
        }
    }
 
    
    void ThreadNotify::post_signal1(const char* name, void* param)
    {
        typedef void (ThreadNotify::*PFN)(const char*, void*);
        PFN pf = &ThreadNotify::_do_post_signal1;
        task_t* t = task_t::bind_memfunc_delay<ThreadNotify*, PFN>(this, pf, name, param, 0);
        _pth->enqueue(t);
    }
    
    void ThreadNotify::_do_post_signal1(const char *name, void *param)
    {
        TypedeSignal1::iterator iter = _signal1.find(name);
        if (iter != _signal1.end()) {
            iter->second->Emit(param);
        }
    }

}




