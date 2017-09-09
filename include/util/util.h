
#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_

#include <stdlib.h>  // NULL
#include <cstddef>             // for std::ptrdiff_t
#include <assert.h>            // for assert

namespace libcommon
{
    
// Undefine macros first, just in case. Some third-party includes have their own
// version.

#undef DISALLOW_ASSIGN
#define DISALLOW_ASSIGN(TypeName) \
void operator=(const TypeName&)

// A macro to disallow the evil copy constructor and operator= functions
// This should be used in the private: declarations for a class.
#undef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
private:                                      \
TypeName(const TypeName&);                    \
DISALLOW_ASSIGN(TypeName)

// Alternative, less-accurate legacy name.
#undef DISALLOW_EVIL_CONSTRUCTORS
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) \
DISALLOW_COPY_AND_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#undef DISALLOW_IMPLICIT_CONSTRUCTORS
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
TypeName();                                    \
DISALLOW_EVIL_CONSTRUCTORS(TypeName)


#define  freep(p) \
    if(p) { \
        delete p; \
        p = NULL; \
    }

#define freepa (pa) \
    if (pa) { \
        delete[] pa; \
        pa = NULL; \
    }

    
    // ----------------------------------
    // class scoped_ptr2
    // ----------------------------------
    
    template <typename T>
    class scoped_sample_ptr {
    private:
        
        T* ptr;
        
        DISALLOW_COPY_AND_ASSIGN(scoped_sample_ptr);
        
    public:
        
        typedef T element_type;
        
        explicit scoped_sample_ptr(T* p = NULL): ptr(p) {}
        
        ~scoped_sample_ptr() {
            typedef char type_must_be_complete[sizeof(T)];
            delete ptr;
        }
        
        void reset(T* p = NULL) {
            typedef char type_must_be_complete[sizeof(T)];
            
            if (ptr != p) {
                T* obj = ptr;
                ptr = p;
                // Delete last, in case obj destructor indirectly results in ~scoped_ptr
                delete obj;
            }
        }
        
        T& operator*() const {
            assert(ptr != NULL);
            return *ptr;
        }
        
        T* operator->() const  {
            assert(ptr != NULL);
            return ptr;
        }
        
        T* get() const  {
            return ptr;
        }
        
        void swap(scoped_sample_ptr & b) {
            T* tmp = b.ptr;
            b.ptr = ptr;
            ptr = tmp;
        }
        
        T* release() {
            T* tmp = ptr;
            ptr = NULL;
            return tmp;
        }
        
        T** accept() {
            if (ptr) {
                delete ptr;
                ptr = NULL;
            }
            return &ptr;
        }
        
        T** use() {
            return &ptr;
        }
    };
    
    template<typename T> inline
    void swap(scoped_sample_ptr<T>& a, scoped_sample_ptr<T>& b) {
        a.swap(b);
    }

    
    
    // ----------------------------------
    // class scoped_array
    // ----------------------------------
    
    //  scoped_array extends scoped_ptr to arrays. Deletion of the array pointed to
    //  is guaranteed, either on destruction of the scoped_array or via an explicit
    //  reset(). Use shared_array or std::vector if your needs are more complex.
    
    template<typename T>
    class scoped_sample_array {
    private:
        
        T* ptr;
        
        scoped_sample_array(scoped_sample_array const &);
        scoped_sample_array & operator=(scoped_sample_array const &);
        
    public:
        
        typedef T element_type;
        
        explicit scoped_sample_array(T* p = NULL) : ptr(p) {}
        
        ~scoped_sample_array() {
            typedef char type_must_be_complete[sizeof(T)];
            delete[] ptr;
        }
        
        void reset(T* p = NULL) {
            typedef char type_must_be_complete[sizeof(T)];
            
            if (ptr != p) {
                T* arr = ptr;
                ptr = p;
                // Delete last, in case arr destructor indirectly results in ~scoped_array
                delete [] arr;
            }
        }
        
        T& operator[](std::ptrdiff_t i) const {
            assert(ptr != NULL);
            assert(i >= 0);
            return ptr[i];
        }
        
        T* get() const {
            return ptr;
        }
        
        void swap(scoped_sample_array & b) {
            T* tmp = b.ptr;
            b.ptr = ptr;
            ptr = tmp;
        }
        
        T* release() {
            T* tmp = ptr;
            ptr = NULL;
            return tmp;
        }
        
        T** accept() {
            if (ptr) {
                delete [] ptr;
                ptr = NULL;
            }
            return &ptr;
        }
    };
    
    template<class T> inline
    void swap(scoped_sample_array<T>& a, scoped_sample_array<T>& b) {
        a.swap(b);
    }
    
    
    
    
    // ----------------------------------
    // class scoped_ptr_malloc
    // ----------------------------------

    // scoped_ptr_malloc<> is similar to scoped_ptr<>, but it accepts a
    // second template argument, the function used to free the object.
    
    template<typename T, void (*FF)(void*) = free> class scoped_ptr_malloc {
    private:
        
        T* ptr;
        
        scoped_ptr_malloc(scoped_ptr_malloc const &);
        scoped_ptr_malloc & operator=(scoped_ptr_malloc const &);
        
    public:
        
        typedef T element_type;
        
        explicit scoped_ptr_malloc(T* p = 0): ptr(p) {}
        
        ~scoped_ptr_malloc() {
            FF(static_cast<void*>(ptr));
        }
        
        void reset(T* p = 0) {
            if (ptr != p) {
                FF(static_cast<void*>(ptr));
                ptr = p;
            }
        }
        
        T& operator*() const {
            assert(ptr != 0);
            return *ptr;
        }
        
        T* operator->() const {
            assert(ptr != 0);
            return ptr;
        }
        
        T* get() const {
            return ptr;
        }
        
        void swap(scoped_ptr_malloc & b) {
            T* tmp = b.ptr;
            b.ptr = ptr;
            ptr = tmp;
        }
        
        T* release() {
            T* tmp = ptr;
            ptr = 0;
            return tmp;
        }
        
        T** accept() {
            if (ptr) {
                FF(static_cast<void*>(ptr));
                ptr = 0;
            }
            return &ptr;
        }
    };
    
    template<typename T, void (*FF)(void*)> inline
    void swap(scoped_ptr_malloc<T,FF>& a, scoped_ptr_malloc<T,FF>& b) {
        a.swap(b);
    }
    
    

}
#endif

