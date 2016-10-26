
#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_

#include <stdlib.h>  // NULL


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

template <class T>
class scope_ptr
{
private:
    T** _ptr;

    DISALLOW_COPY_AND_ASSIGN(scope_ptr);
public:
    scope_ptr(T** ptr)
    {
        _ptr = ptr;
    }

    ~scope_ptr()
    {
        if (_ptr && *_ptr)
        {
            delete *_ptr;
            *_ptr = NULL;
        }
    }
};

template <class T>
class scope_ptr<T[]>
{
private:
    T** _ptr;
    DISALLOW_COPY_AND_ASSIGN(scope_ptr);

public:
    scope_ptr(T** ptr)
    {
        _ptr = ptr;
    }

    ~scope_ptr()
    {
        if (_ptr && *_ptr)
        {
            delete[] *_ptr;
            *_ptr = NULL;
        }
    }
};

#define AutoDeleteScopePtr(cls, val) scope_ptr<cls> scope_ptr##val(&val)


template <class T>
class scope_ptr_c
{
private:
    void**  _ptr;
    DISALLOW_COPY_AND_ASSIGN(scope_ptr_c);

public:
    scope_ptr_c(T** ptr)
    {
        _ptr = (void**)ptr;
    }

    ~scope_ptr_c()
    {
        if (_ptr && *_ptr)
        {
            free(*_ptr);
            *_ptr = NULL;
        }
    }
};

#define AutoFreeScopePtr(cls, val) scope_ptr_c<cls> scope_ptr_c##val(&val)


#endif

