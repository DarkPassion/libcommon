
#ifndef _UTIL_SINGLETON_H_
#define _UTIL_SINGLETON_H_


#include "util/ref_count.h"

namespace libcommon {
    
    enum {
        ONCE_STATE_UNINITIALIZED = 0,
        ONCE_STATE_DONE = 1,
    };

    void GoogleOnceInit(int* once, void(*init_func)(void))
    {

        // __sync_bool_compare_and_swap(ptr, oval, nval)
        if (ATOM_CAS(once, ONCE_STATE_UNINITIALIZED, ONCE_STATE_DONE)) {
            if (init_func) {
                init_func();
            }
        }
    }
    
    template<typename T>
    class Singleton {
    public:
        static T* get() {
            GoogleOnceInit(&once_, &Singleton<T>::Init);
            return instance_;
        }
        static void ShutDown() {
            delete instance_;
            instance_ = NULL;
        }
    private:
        static void Init() {
            instance_ = new T();
        }
        static int once_;
        static T* instance_;
    };
    
    template<typename T>
    int Singleton<T>::once_ = ONCE_STATE_UNINITIALIZED;
    
    template<typename T>
    T* Singleton<T>::instance_ = NULL;
}


#endif
