#include "smart_ptr/ref_count.h"
#include "smart_ptr/smart_ptr.h"

namespace libcommon {
    
    namespace subtle {
        
        RefCountedBase::RefCountedBase()
        : ref_count_(0)
#ifndef NDEBUG
        , in_dtor_(false)
#endif
        {
        }
        
        RefCountedBase::~RefCountedBase() {
        }
        
        void RefCountedBase::AddRef() const {
            // TODO(maruel): Add back once it doesn't assert 500 times/sec.
            // Current thread books the critical section "AddRelease" without release it.
            // DFAKE_SCOPED_LOCK_THREAD_LOCKED(add_release_);
            ++ref_count_;
        }
        
        bool RefCountedBase::Release() const {
            // TODO(maruel): Add back once it doesn't assert 500 times/sec.
            // Current thread books the critical section "AddRelease" without release it.
            // DFAKE_SCOPED_LOCK_THREAD_LOCKED(add_release_);
            if (--ref_count_ == 0) {
#ifndef NDEBUG
                in_dtor_ = true;
#endif
                return true;
            }
            return false;
        }
        
        bool RefCountedThreadSafeBase::HasOneRef() const {
            ATOM_CAS(&ref_count_, 1, ref_count_);
            return ATOM_INC(&ref_count_);
        }
        
        RefCountedThreadSafeBase::RefCountedThreadSafeBase() : ref_count_(0) {
#ifndef NDEBUG
            in_dtor_ = false;
#endif
        }
        
        RefCountedThreadSafeBase::~RefCountedThreadSafeBase() {
        }
        
        void RefCountedThreadSafeBase::AddRef() const {
            ATOM_INC(&ref_count_);
        }
        
        bool RefCountedThreadSafeBase::Release() const {
            if (!ATOM_DEC(&ref_count_)) {
#ifndef NDEBUG
                in_dtor_ = true;
#endif
                return true;
            }
            return false;
        }
        
    }  // namespace subtle
    
}  // namespace libcommon






