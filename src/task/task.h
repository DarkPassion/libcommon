#ifndef _TASK_TASK_H_
#define _TASK_TASK_H_

#include <stdlib.h>
#include <stdio.h>
#include "bind_base.h"
#include "bind_function.h"
#include "bind_object.h"


class task_t
{
public:
    // bind function
    //
    template<typename FUNC>
    static task_t bind_func(FUNC func_)
    {
        bind_base_t* base = new bind_func_0_t<FUNC>(func_);
        return task_t(base);
    }

    template<typename FUNC, typename ARG0>
    static task_t bind_func(FUNC func_, ARG0 arg0_)
    {
        bind_base_t* bind = new bind_func_1_t<FUNC, ARG0>(func_, arg0_);
        return task_t(bind);
    }

    template<typename FUNC, typename ARG0, typename ARG1>
    static task_t bind_func(FUNC func_, ARG0 arg0_, ARG1 arg1_)
    {
        bind_base_t* bind = new bind_func_2_t<FUNC, ARG0, ARG1>(func_, arg0_, arg1_);
        return task_t(bind);
    }

    template<typename FUNC, typename ARG0, typename ARG1, typename ARG2>
    static task_t bind_func(FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_)
    {
        bind_base_t * bind = new bind_func_3_t<FUNC, ARG0, ARG1, ARG2>(func_, arg0_, arg1_, arg2_);
        return task_t(bind);
    }

    template<typename FUNC, typename ARG0, typename ARG1, typename ARG2, typename ARG3>
    static task_t bind_func(FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, ARG3 arg3_)
    {
        bind_base_t* base = new bind_func_4_t<FUNC, ARG0, ARG1, ARG2, ARG3>(func_, arg0_, arg1_, arg2_, arg3_);
        return task_t(base);
    }


    // bind class object member function
    //

    template<typename T, typename FUNC>
    static task_t bind_memfunc(T instance_, FUNC func_)
    {
        bind_base_t* bind = new bind_obj_0_t<T, FUNC>(instance_, func_);
        return task_t(bind);
    }

    template<typename T, typename FUNC, typename ARG0>
    static task_t bind_memfunc(T instance_, FUNC func_, ARG0 arg0_)
    {
        bind_base_t* base = new bind_obj_1_t<T, FUNC, ARG0>(instance_, func_, arg0_);
        return task_t(base);
    }

    template<typename T, typename FUNC, typename ARG0, typename ARG1>
    static task_t bind_memfunc(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_)
    {
        bind_base_t* base  = new bind_obj_2_t<T, FUNC, ARG0, ARG1>(instance_, func_, arg0_, arg1_);
        return task_t(base);
    }

    template<typename T, typename FUNC, typename ARG0, typename ARG1, typename ARG2>
    static task_t bind_memfunc(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_)
    {
        bind_base_t* base = new bind_obj_3_t<T, FUNC, ARG0, ARG1, ARG2>(instance_, func_, arg0_, arg1_, arg2_);
        return task_t(base);
    }

    template<typename T, typename FUNC, typename ARG0, typename ARG1, typename ARG2, typename ARG3>
    static task_t bind_memfunc(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, ARG3 arg3_)
    {
        bind_base_t * base = new bind_obj_4_t<T, FUNC, ARG0, ARG1, ARG2, ARG3>(instance_, func_, arg0_, arg1_, arg2_, arg3_);
        return task_t(base);
    }


public:
    task_t(bind_base_t* bind_base):m_bind(bind_base)
    {
    }

    task_t(const task_t& bind_)
    {
        release();
        m_bind = bind_.m_bind;
    }

    void operator=(const task_t& bind_)
    {
        release();
        m_bind = bind_.m_bind;
    }

    ~task_t()
    {
        release();
    }

    void release()
    {
        if (m_bind)
        {
            delete m_bind;
            m_bind = NULL;
        }
    }

    void operator()()
    {
        if (m_bind)
        {
            m_bind->exec();
        }
    }
private:
    bind_base_t*    m_bind;
};



#endif


