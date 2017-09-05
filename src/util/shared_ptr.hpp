/**
 * @file  shared_ptr.hpp
 * @brief shared_ptr is a minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * Copyright (c) 2013-2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <cstddef>      // NULL
#include <algorithm>    // std::swap

// can be replaced by other error mechanism
#include <cassert>
#define SHARED_ASSERT(x)    assert(x)

#define ATOM_CAS(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_CAS_POINTER(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_INC(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOM_FINC(ptr) __sync_fetch_and_add(ptr, 1)
#define ATOM_DEC(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOM_FDEC(ptr) __sync_fetch_and_sub(ptr, 1)
#define ATOM_ADD(ptr,n) __sync_add_and_fetch(ptr, n)
#define ATOM_SUB(ptr,n) __sync_sub_and_fetch(ptr, n)
#define ATOM_AND(ptr,n) __sync_and_and_fetch(ptr, n)


namespace internal
{

// Function object which deletes its parameter, which must be a pointer.
// If C is an array type, invokes 'delete[]' on the parameter; otherwise,
// invokes 'delete'. The default deleter for scoped_ptr<T>.
template <class T>
struct DefaultDeleter
{
    DefaultDeleter() {}
    template <typename U> DefaultDeleter(const DefaultDeleter<U>& other)
    {
        // IMPLEMENTATION NOTE: C++11 20.7.1.1.2p2 only provides this constructor
        // if U* is implicitly convertible to T* and U is not an array type.
        //
        // Correct implementation should use SFINAE to disable this
        // constructor. However, since there are no other 1-argument constructors,
        // using a static_assert based on is_convertible<> and requiring
        // complete types is simpler and will cause compile failures for equivalent
        // misuses.
        //
        // Note, the is_convertible<U*, T*> check also ensures that U is not an
        // array. T is guaranteed to be a non-array, so any U* where U is an array
        // cannot convert to T*.
        enum { T_must_be_complete = sizeof(T) };
        enum { U_must_be_complete = sizeof(U) };
        // "U* must implicitly convert to T*"
        // assert(rtc::is_convertible<U*, T*>::value);
    }
    inline void operator()(T* ptr) const
    {
        enum { type_must_be_complete = sizeof(T) };
        delete ptr;
    }
};

// Specialization of DefaultDeleter for array types.
template <class T>
struct DefaultDeleter<T[]>
{
    inline void operator()(T* ptr) const
    {
        enum { type_must_be_complete = sizeof(T) };
        delete[] ptr;
    }

private:
    // Disable this operator for any U != T because it is undefined to execute
    // an array delete when the static type of the array mismatches the dynamic
    // type.
    //
    // References:
    //   C++98 [expr.delete]p3
    //   http://cplusplus.github.com/LWG/lwg-defects.html#938
    template <typename U> void operator()(U* array) const;
};

template <class T, int n>
struct DefaultDeleter<T[n]>
{
    // Never allow someone to declare something like scoped_ptr<int[10]>.
    // static_assert(sizeof(T) == -1, "do not use array with size as type");
};

// Function object which invokes 'free' on its parameter, which must be
// a pointer. Can be used to store malloc-allocated pointers in scoped_ptr:
//
// scoped_ptr<int, rtc::FreeDeleter> foo_ptr(
//     static_cast<int*>(malloc(sizeof(int))));
struct FreeDeleter
{
    inline void operator()(void* ptr) const
    {
        free(ptr);
    }
};


} // end namesapce


/**
 * @brief implementation of reference counter for the following minimal smart pointer.
 *
 * shared_ptr_count is a container for the allocated pn reference counter.
 */
template<class U>
class shared_ptr_count
{
public:

    shared_ptr_count() :
        pn(NULL)
    {
    }
    shared_ptr_count(const shared_ptr_count& count) :
        pn(count.pn)
    {
    }
    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(shared_ptr_count& lhs) throw() // never throws
    {
        std::swap(pn, lhs.pn);
    }
    /// @brief getter of the underlying reference counter
    long use_count(void) const throw() // never throws
    {
        long count = 0;
        if (NULL != pn)
        {
            count = *pn;
        }
        return count;
    }
    /// @brief acquire/share the ownership of the pointer, initializing the reference counter
    void acquire(U* p) // may throw std::bad_alloc
    {
        if (NULL != p)
        {
            if (NULL == pn)
            {
                try
                {
                    pn = new long(1); // may throw std::bad_alloc
                }
                catch (std::bad_alloc&)
                {
                    delete p;
                    throw; // rethrow the std::bad_alloc
                }
            }
            else
            {
                //++(*pn);
                ATOM_INC(pn);
            }
        }
    }
    /// @brief release the ownership of the px pointer, destroying the object when appropriate
    template<class D>
    void release(D data) throw() // never throws
    {
        if (NULL != pn)
        {
            //--(*pn);
            ATOM_DEC(pn);
            if (0 == *pn)
            {
                //delete p;
                data(data.ptr);
                delete pn;
            }
            pn = NULL;
        }
    }

private:
    long*   pn; //!< Reference counter
};


/**
 * @brief minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * shared_ptr is a smart pointer retaining ownership of an object through a provided pointer,
 * and sharing this ownership with a reference counter.
 * It destroys the object when the last shared pointer pointing to it is destroyed or reset.
 */
template<class T, class D = internal::DefaultDeleter<T> >
class shared_ptr
{
public:
    /// The type of the managed object, aliased as member type
    typedef T element_type;

    /// @brief Default constructor
    shared_ptr(void) throw() : // never throws
        _data(NULL),
        pn()
    {
    }
    /// @brief Constructor with the provided pointer to manage
    explicit shared_ptr(T* p) : // may throw std::bad_alloc
      //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
        pn()
    {
        acquire(p);   // may throw std::bad_alloc
    }
    /// @brief Constructor to share ownership. Warning : to be used for pointer_cast only ! (does not manage two separate <T> and <U> pointers)
    template <class U>
    shared_ptr(const shared_ptr<U>& ptr, T* p) :
     //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
       pn(ptr.pn)
    {
       acquire(p);   // may throw std::bad_alloc
    }
    /// @brief Copy constructor to convert from another pointer type
    template <class U>
    shared_ptr(const shared_ptr<U>& ptr) throw() : // never throws (see comment below)
      //px(ptr.px),
        pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr._data.ptr) || (0 != ptr.pn.use_count())); // must be coherent : no allocation allowed in this path
        acquire(static_cast<typename shared_ptr<T>::element_type*>(ptr._data.ptr));   // will never throw std::bad_alloc
    }
    /// @brief Copy constructor (used by the copy-and-swap idiom)
    shared_ptr(const shared_ptr& ptr) throw() : // never throws (see comment below)
       //px(ptr.px),
        pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr._data.ptr) || (0 != ptr.pn.use_count())); // must be cohérent : no allocation allowed in this path
        acquire(ptr._data.ptr);   // will never throw std::bad_alloc
    }
    /// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
    shared_ptr& operator=(shared_ptr ptr) throw() // never throws
    {
        swap(ptr);
        return *this;
    }
    /// @brief the destructor releases its ownership
    inline ~shared_ptr(void) throw() // never throws
    {
        release();
    }
    /// @brief this reset releases its ownership
    inline void reset(void) throw() // never throws
    {
        release();
    }
    /// @brief this reset release its ownership and re-acquire another one
    void reset(T* p) // may throw std::bad_alloc
    {
        SHARED_ASSERT((NULL == p) || (_data.ptr != p)); // auto-reset not allowed
        release();
        acquire(p); // may throw std::bad_alloc
    }

    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(shared_ptr& lhs) throw() // never throws
    {
        std::swap(_data.ptr, lhs._data.ptr);
        pn.swap(lhs.pn);
    }

    // reference counter operations :
    inline operator bool() const throw() // never throws
    {
        return (0 < pn.use_count());
    }
    inline bool unique(void)  const throw() // never throws
    {
        return (1 == pn.use_count());
    }
    long use_count(void)  const throw() // never throws
    {
        return pn.use_count();
    }

    // underlying pointer operations :
    inline T& operator*()  const throw() // never throws
    {
        SHARED_ASSERT(NULL != _data.ptr);
        return *_data.ptr;
    }
    inline T* operator->() const throw() // never throws
    {
        SHARED_ASSERT(NULL != _data.ptr);
        return _data.ptr;
    }
    inline T* get(void)  const throw() // never throws
    {
        // no assert, can return NULL
        return _data.ptr;
    }

private:
    /// @brief acquire/share the ownership of the px pointer, initializing the reference counter
    inline void acquire(T* p) // may throw std::bad_alloc
    {
        pn.acquire(p); // may throw std::bad_alloc
        _data.ptr = p; // here it is safe to acquire the ownership of the provided raw pointer, where exception cannot be thrown any more
    }

    /// @brief release the ownership of the px pointer, destroying the object when appropriate
    inline void release(void) throw() // never throws
    {
        pn.release(_data);
        _data.ptr = NULL;
    }

private:

    // Use the empty base class optimization to allow us to have a D
    // member, while avoiding any space overhead for it when D is an
    // empty class.  See e.g. http://www.cantrip.org/emptyopt.html for a good
    // discussion of this technique.
    struct DataPtr : public D
    {
        explicit DataPtr(T* ptr_in) : ptr(ptr_in) {}
        explicit DataPtr() : ptr(NULL) {}
        DataPtr(T* ptr_in, const D& other) : D(other), ptr(ptr_in) {}
        T* ptr;
    };

private:
    // This allow pointer_cast functions to share the reference counter between different shared_ptr types
    //template<class U, class D = internal::DefaultDeleter<T> >
    //friend class shared_ptr;

private:
    DataPtr                 _data; //!< Native pointer
    shared_ptr_count<T>    pn; //!< Reference counter
};





// comparaison operators
template<class T, class U> inline bool operator==(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() == r.get());
}
template<class T, class U> inline bool operator!=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() != r.get());
}
template<class T, class U> inline bool operator<=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() <= r.get());
}
template<class T, class U> inline bool operator<(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() < r.get());
}
template<class T, class U> inline bool operator>=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() >= r.get());
}
template<class T, class U> inline bool operator>(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() > r.get());
}



// static cast of shared_ptr
template<class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) // never throws
{
    return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type*>(ptr.get()));
}

// dynamic cast of shared_ptr
template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) // never throws
{
    T* p = dynamic_cast<typename shared_ptr<T>::element_type*>(ptr.get());
    if (NULL != p)
    {
        return shared_ptr<T>(ptr, p);
    }
    else
    {
        return shared_ptr<T>();
    }
}
