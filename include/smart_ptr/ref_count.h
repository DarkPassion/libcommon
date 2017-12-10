
#ifndef _UTIL_REF_COUNT_H_
#define _UTIL_REF_COUNT_H_

#include <stdint.h>
#include "util/util.h"

namespace libcommon {
    
#define ATOM_CAS(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_CAS_POINTER(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_INC(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOM_FINC(ptr) __sync_fetch_and_add(ptr, 1)
#define ATOM_DEC(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOM_FDEC(ptr) __sync_fetch_and_sub(ptr, 1)
#define ATOM_ADD(ptr,n) __sync_add_and_fetch(ptr, n)
#define ATOM_SUB(ptr,n) __sync_sub_and_fetch(ptr, n)
#define ATOM_AND(ptr,n) __sync_and_and_fetch(ptr, n)

    
    
    // This class can be used for instantiating
    // reference counted objects.
    // int32_t AddRef() and int32_t Release().
    // Usage:
    // RefCountImpl<T>* implementation = new RefCountImpl<T>(p);
    //
    // Example:
    // class MyInterface {
    //  public:
    //   virtual void DoSomething() = 0;
    //   virtual int32_t AddRef() = 0;
    //   virtual int32_t Release() = 0:
    //  private:
    //   virtual ~MyInterface(){};
    // }
    // class MyImplementation : public MyInterface {
    //  public:
    //   virtual DoSomething() { printf("hello"); };
    // };
    // MyImplementation* CreateMyImplementation() {
    //   RefCountImpl<MyImplementation>* implementation =
    //       new RefCountImpl<MyImplementation>();
    //   return implementation;
    // }
    
    template <class T>
    class RefCountImpl : public T {
    public:
        RefCountImpl() : ref_count_(0) {}
        
        template<typename P>
        explicit RefCountImpl(P p) : T(p), ref_count_(0) {}
        
        template<typename P1, typename P2>
        RefCountImpl(P1 p1, P2 p2) : T(p1, p2), ref_count_(0) {}
        
        template<typename P1, typename P2, typename P3>
        RefCountImpl(P1 p1, P2 p2, P3 p3) : T(p1, p2, p3), ref_count_(0) {}
        
        template<typename P1, typename P2, typename P3, typename P4>
        RefCountImpl(P1 p1, P2 p2, P3 p3, P4 p4) : T(p1, p2, p3, p4), ref_count_(0) {}
        
        template<typename P1, typename P2, typename P3, typename P4, typename P5>
        RefCountImpl(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        : T(p1, p2, p3, p4, p5), ref_count_(0) {}
        
        virtual int32_t AddRef() {
            return ATOM_INC(&ref_count_);
        }
        
        virtual int32_t Release() {
            int32_t ref_count;
            ref_count = ATOM_DEC(&ref_count_);
            if (ref_count == 0)
                delete this;
            return ref_count;
        }
        
    protected:
        volatile long ref_count_;
    };
    
    
    
    
    namespace subtle {
        
        class  RefCountedBase {
        public:
            bool HasOneRef() const { return ref_count_ == 1; }
            
        protected:
            RefCountedBase();
            ~RefCountedBase();
            
            void AddRef() const;
            
            // Returns true if the object should self-delete.
            bool Release() const;
            
        private:
            mutable int ref_count_;
#ifndef NDEBUG
            mutable bool in_dtor_;
#endif
            
            
            DISALLOW_COPY_AND_ASSIGN(RefCountedBase);
        };
        
        class RefCountedThreadSafeBase {
        public:
            bool HasOneRef() const;
            
        protected:
            RefCountedThreadSafeBase();
            ~RefCountedThreadSafeBase();
            
            void AddRef() const;
            
            // Returns true if the object should self-delete.
            bool Release() const;
            
        private:
            mutable long ref_count_;
#ifndef NDEBUG
            mutable bool in_dtor_;
#endif
            
            DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafeBase);
        };
        
    }  // namespace subtle
    
    //
    // A base class for reference counted classes.  Otherwise, known as a cheap
    // knock-off of WebKit's RefCounted<T> class.  To use this guy just extend your
    // class from it like so:
    //
    //   class MyFoo : public base::RefCounted<MyFoo> {
    //    ...
    //    private:
    //     friend class base::RefCounted<MyFoo>;
    //     ~MyFoo();
    //   };
    //
    // You should always make your destructor private, to avoid any code deleting
    // the object accidently while there are references to it.
    template <class T>
    class RefCounted : public subtle::RefCountedBase {
    public:
        RefCounted() {}
        
        void AddRef() const {
            subtle::RefCountedBase::AddRef();
        }
        
        void Release() const {
            if (subtle::RefCountedBase::Release()) {
                delete static_cast<const T*>(this);
            }
        }
        
    protected:
        ~RefCounted() {}
        
    private:
        DISALLOW_COPY_AND_ASSIGN(RefCounted<T>);
    };
    
    // Forward declaration.
    template <class T, typename Traits> class RefCountedThreadSafe;
    
    // Default traits for RefCountedThreadSafe<T>.  Deletes the object when its ref
    // count reaches 0.  Overload to delete it on a different thread etc.
    template<typename T>
    struct DefaultRefCountedThreadSafeTraits {
        static void Destruct(const T* x) {
            // Delete through RefCountedThreadSafe to make child classes only need to be
            // friend with RefCountedThreadSafe instead of this struct, which is an
            // implementation detail.
            RefCountedThreadSafe<T,
            DefaultRefCountedThreadSafeTraits>::DeleteInternal(x);
        }
    };
    
    //
    // A thread-safe variant of RefCounted<T>
    //
    //   class MyFoo : public base::RefCountedThreadSafe<MyFoo> {
    //    ...
    //   };
    //
    // If you're using the default trait, then you should add compile time
    // asserts that no one else is deleting your object.  i.e.
    //    private:
    //     friend class base::RefCountedThreadSafe<MyFoo>;
    //     ~MyFoo();
    template <class T, typename Traits = DefaultRefCountedThreadSafeTraits<T> >
    class RefCountedThreadSafe : public subtle::RefCountedThreadSafeBase {
    public:
        RefCountedThreadSafe() {}
        
        void AddRef() const {
            subtle::RefCountedThreadSafeBase::AddRef();
        }
        
        void Release() const {
            if (subtle::RefCountedThreadSafeBase::Release()) {
                Traits::Destruct(static_cast<const T*>(this));
            }
        }
        
    protected:
        ~RefCountedThreadSafe() {}
        
    private:
        friend struct DefaultRefCountedThreadSafeTraits<T>;
        static void DeleteInternal(const T* x) { delete x; }
        
        DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafe);
    };
    
    //
    // A wrapper for some piece of data so we can place other things in
    // scoped_refptrs<>.
    //
    template<typename T>
    class RefCountedData : public libcommon::RefCounted< libcommon::RefCountedData<T> > {
    public:
        RefCountedData() : data() {}
        RefCountedData(const T& in_value) : data(in_value) {}
        
        T data;
        
    private:
        friend class libcommon::RefCounted<libcommon::RefCountedData<T> >;
        ~RefCountedData() {}
    };
    
    
    
    
    //
    // A smart pointer class for reference counted objects.  Use this class instead
    // of calling AddRef and Release manually on a reference counted object to
    // avoid common memory leaks caused by forgetting to Release an object
    // reference.  Sample usage:
    //
    //   class MyFoo : public RefCounted<MyFoo> {
    //    ...
    //   };
    //
    //   void some_function() {
    //     scoped_refptr<MyFoo> foo = new MyFoo();
    //     foo->Method(param);
    //     // |foo| is released when this function returns
    //   }
    //
    //   void some_other_function() {
    //     scoped_refptr<MyFoo> foo = new MyFoo();
    //     ...
    //     foo = NULL;  // explicitly releases |foo|
    //     ...
    //     if (foo)
    //       foo->Method(param);
    //   }
    //
    // The above examples show how scoped_refptr<T> acts like a pointer to T.
    // Given two scoped_refptr<T> classes, it is also possible to exchange
    // references between the two objects, like so:
    //
    //   {
    //     scoped_refptr<MyFoo> a = new MyFoo();
    //     scoped_refptr<MyFoo> b;
    //
    //     b.swap(a);
    //     // now, |b| references the MyFoo object, and |a| references NULL.
    //   }
    //
    // To make both |a| and |b| in the above example reference the same MyFoo
    // object, simply use the assignment operator:
    //
    //   {
    //     scoped_refptr<MyFoo> a = new MyFoo();
    //     scoped_refptr<MyFoo> b;
    //
    //     b = a;
    //     // now, |a| and |b| each own a reference to the same MyFoo object.
    //   }
    //
    template <class T>
    class scoped_refptr {
    public:
        typedef T element_type;
        
        scoped_refptr() : ptr_(NULL) {
        }
        
        scoped_refptr(T* p) : ptr_(p) {
            if (ptr_)
                ptr_->AddRef();
        }
        
        scoped_refptr(const scoped_refptr<T>& r) : ptr_(r.ptr_) {
            if (ptr_)
                ptr_->AddRef();
        }
        
        template <typename U>
        scoped_refptr(const scoped_refptr<U>& r) : ptr_(r.get()) {
            if (ptr_)
                ptr_->AddRef();
        }
        
        ~scoped_refptr() {
            if (ptr_)
                ptr_->Release();
        }
        
        T* get() const { return ptr_; }
        operator T*() const { return ptr_; }
        T* operator->() const {
            assert(ptr_ != NULL);
            return ptr_;
        }
        
        // Release a pointer.
        // The return value is the current pointer held by this object.
        // If this object holds a NULL pointer, the return value is NULL.
        // After this operation, this object will hold a NULL pointer,
        // and will not own the object any more.
        T* release() {
            T* retVal = ptr_;
            ptr_ = NULL;
            return retVal;
        }
        
        scoped_refptr<T>& operator=(T* p) {
            // AddRef first so that self assignment should work
            if (p)
                p->AddRef();
            T* old_ptr = ptr_;
            ptr_ = p;
            if (old_ptr)
                old_ptr->Release();
            return *this;
        }
        
        scoped_refptr<T>& operator=(const scoped_refptr<T>& r) {
            return *this = r.ptr_;
        }
        
        template <typename U>
        scoped_refptr<T>& operator=(const scoped_refptr<U>& r) {
            return *this = r.get();
        }
        
        void swap(T** pp) {
            T* p = ptr_;
            ptr_ = *pp;
            *pp = p;
        }
        
        void swap(scoped_refptr<T>& r) {
            swap(&r.ptr_);
        }
        
    protected:
        T* ptr_;
    };
    
    // Handy utility for creating a scoped_refptr<T> out of a T* explicitly without
    // having to retype all the template arguments
    template <typename T>
    scoped_refptr<T> make_scoped_refptr(T* t) {
        return scoped_refptr<T>(t);
    }
}








#endif





