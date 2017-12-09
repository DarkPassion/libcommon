#include "util/util.h"
#include "smart_ptr/ref_count.h"

namespace libcommon {


    void GoogleOnceInit(int* once, void(*init_func)(void))
    {

        // __sync_bool_compare_and_swap(ptr, oval, nval)
        if (ATOM_CAS(once, ONCE_STATE_UNINITIALIZED, ONCE_STATE_DONE)) {
            if (init_func) {
                init_func();
            }
        }
    }

}
