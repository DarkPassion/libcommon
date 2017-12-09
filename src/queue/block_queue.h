#ifndef _QUEUE_BLOCK_QUEUE_
#define _QUEUE_BLOCK_QUEUE_

#include <stdlib.h>
#include <vector>
#include <pthread.h>
#include "util/util.h"
#include "thread/lock.h"
#include "smart_ptr/ref_count.h"

namespace libcommon
{

    template <class T>
    class BlockQueue
    {
    public:
        BlockQueue() : _cond(_mutex), _no_block(0)
        {

        }

        ~BlockQueue()
        {
            clear();
        }

        void push_back(T* val)
        {
            AutoLock __lock(_mutex);

            _q.push_back(val);

            _cond.notify_all();
        }

        T* pop_front()
        {
            AutoLock __lock(_mutex);
            T* val = NULL;

            // __sync_bool_compare_and_swap(ptr, oval, nval)
            while (_q.size() == 0 && ATOM_CAS(&_no_block, 0, _no_block))
            {
                _cond.wait();
            }

            if (_q.size())
            {
                val = _q.front();
                _q.erase(_q.begin());
            }

            return val;
        }

        void set_noblock()
        {
            ATOM_CAS(&_no_block, 0, 1);
            _cond.notify_all();
        }

        void clear()
        {
            AutoLock __lock(_mutex);
            while (_q.size())
            {

                T* val = _q.front();
                _q.erase(_q.begin());
                freep(val);

            }
        }

        size_t size()
        {
            size_t s = 0;
            AutoLock __lock(_mutex);

            s = _q.size();

            return s;
        }

    private:

        typedef std::vector<T*> TypeQueue;
        TypeQueue       _q;

        CMutex          _mutex;
        CCond           _cond;
        volatile long   _no_block;

    };

}

#endif



