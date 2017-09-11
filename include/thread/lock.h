#ifndef _THREAD_LOCK_H_
#define _THREAD_LOCK_H_

#include <pthread.h>
#include <semaphore.h>
#include "util/util.h"

namespace libcommon
{
    class ILock
    {
    public:
        virtual ~ILock() {};
        
        virtual void lock() = 0;
        
        virtual void unlock() = 0;
    };
    
    class CMutex;
    class CCond
    {
    public:
        CCond(CMutex& mutex);
        
        ~CCond();
        
        void notify();
        
        void notify_all();
        
        void wait();
        
    private:
        DISALLOW_COPY_AND_ASSIGN(CCond);
        CMutex&     _lock;
        pthread_cond_t  _cond;
    };
    
    class CMutex : public ILock
    {
        
    public:
        CMutex();
        
        virtual ~CMutex();
        
        virtual void lock();
        
        virtual void unlock();
        
        friend class CCond;
    private:
        DISALLOW_COPY_AND_ASSIGN(CMutex);
        pthread_mutex_t _mutex;
    };
    
    
    class AutoLock
    {
        
    public:
        explicit AutoLock(ILock& mutex);

        
        ~AutoLock();
        
    private:
        DISALLOW_COPY_AND_ASSIGN(AutoLock);
        ILock&     _lock;
        
    };
    
    
    class CSem
    {
    public:
        
        CSem(int num);
        
        ~CSem();
        
        void signal_up();
        
        void signal_down();
        
    private:
        DISALLOW_COPY_AND_ASSIGN(CSem);
        sem_t   _sem;
        
    };
}

#endif
