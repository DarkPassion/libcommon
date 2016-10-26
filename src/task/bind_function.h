
#ifndef _TASK_BIND_FUNCTION_H_
#define _TASK_BIND_FUNCTION_H_

#include <stdlib.h>
#include "bind_base.h"



template<typename FUNC>
class bind_func_0_t : public bind_base_t
{

public:
    bind_func_0_t():m_func(NULL)
    {
    }

    bind_func_0_t(FUNC func_):m_func(func_)
    {
    }

    ~bind_func_0_t()
    {

    }

    virtual void exec()
    {
        (*m_func)();
    }
private:
    FUNC        m_func;
};


template<typename FUNC, typename ARG0>
class bind_func_1_t:public bind_base_t
{
public:
    bind_func_1_t():m_func(NULL)
    {
    }

    bind_func_1_t(FUNC func_, ARG0 arg0_):
        m_func(func_),
        m_arg0(arg0_)
    {
    }

    ~bind_func_1_t()
    {
    }

    virtual void exec()
    {
        (*m_func)(m_arg0);
    }
private:
    FUNC        m_func;
    ARG0        m_arg0;
};

template<typename FUNC, typename ARG0, typename ARG1>
class bind_func_2_t:public bind_base_t
{
public:
    bind_func_2_t():m_func(NULL)
    {
    }

    bind_func_2_t(FUNC func_, ARG0 arg0_, ARG1 arg1_):
        m_func(func_),
        m_arg0(arg0_),
        m_arg1(arg1_)
    {
    }

    ~bind_func_2_t()
    {
    }

    virtual void exec()
    {
        (*m_func)(m_arg0, m_arg1);
    }
private:
    FUNC        m_func;
    ARG0        m_arg0;
    ARG1        m_arg1;
};


template<typename FUNC, typename ARG0, typename ARG1, typename ARG2>
class bind_func_3_t:public bind_base_t
{
public:
    bind_func_3_t():m_func(NULL)
    {
    }

    bind_func_3_t(FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_):
        m_func(func_),
        m_arg0(arg0_),
        m_arg1(arg1_),
        m_arg2(arg2_)
    {
    }

    ~bind_func_3_t()
    {
    }

    virtual void exec()
    {
        (*m_func)(m_arg0, m_arg1, m_arg2);
    }
private:
    FUNC        m_func;
    ARG0        m_arg0;
    ARG1        m_arg1;
    ARG2        m_arg2;
};


template<typename FUNC, typename ARG0, typename ARG1, typename ARG2, typename ARG3>
class bind_func_4_t:public bind_base_t
{
public:
    bind_func_4_t():m_func(NULL)
    {
    }

    bind_func_4_t(FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, ARG3 arg3_):
        m_func(func_),
        m_arg0(arg0_),
        m_arg1(arg1_),
        m_arg2(arg2_),
        m_arg3(arg3_)
    {
    }

    ~bind_func_4_t()
    {
    }

    virtual void exec()
    {
        (*m_func)(m_arg0, m_arg1, m_arg2, m_arg3);
    }
private:
    FUNC        m_func;
    ARG0        m_arg0;
    ARG1        m_arg1;
    ARG2        m_arg2;
    ARG3        m_arg3;
};


#endif

