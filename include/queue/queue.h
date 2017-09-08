#ifndef _QUEUE_QUEUE_H_
#define _QUEUE_QUEUE_H_


#include <stdlib.h>
#include <vector>
#include "thread/lock.h"

namespace libcommon
{
    template <class T>
    class Queue
    {
    public:
        Queue()
        {
            
        }
        
        ~Queue()
        {
            clear();
        }
        
        
        void push_back(T* val)
        {
            AutoLock __lock(_mutex);
            _q.push_back(val);
        }
        
        T* pop_front()
        {
            AutoLock __lock(_mutex);
            T* val = NULL;
            if (_q.size())
            {
                val = _q.front();
                _q.erase(_q.begin());
            }
            return val;
        }
        
        size_t size()
        {
            AutoLock __lock(_mutex);
            return _q.size();
        }
        
        void clear()
        {
            AutoLock __lock(_mutex);
            
            while (_q.size())
            {
                
                T* val = _q.front();
                freep(val);
                _q.erase(_q.begin());
            }
        }
        
    private:
        typedef std::vector<T*> TypeQueue;
        
        TypeQueue   _q;
        
        CMutex  _mutex;
        
    };

}



#endif
