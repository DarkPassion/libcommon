
#ifndef _TASK_THREAD_NOTIFY_H_
#define _TASK_THREAD_NOTIFY_H_

#include <string>
#include <map>
#include "util/signal.h"
#include "util/delegate.h"
#include "thread/lock.h"


namespace libcommon {
    
    
    class ThreadQueue;
    
    class ThreadNotify
    {
    public:
        ThreadNotify();
        
        ~ThreadNotify();
        
        
        void register_signal1(const char* name);
        

        void unregister_signal1(const char* name);


        void connect_signal1(const char* name, Delegate1< void* > delegate);


        void disconnect_signal1(const char* name, Delegate1< void* > delegate);

        
        void post_signal1(const char* name, void* param);

        
    private:
        void _do_register_signal1(const char* name);
        void _do_unregister_signal1(const char* name);
        void _do_connect_signal1(const char* name, Delegate1< void* > delegate);
        void _do_disconnect_signal1(const char* name, Delegate1< void* > delegate);
        void _do_post_signal1(const char* name, void* param);

        
    private:
        typedef std::map<std::string, Signal1<void*>*>                     TypedeSignal1;


        ThreadQueue*    _pth;
        TypedeSignal1   _signal1;
    };
    
    
}


#endif


