#ifndef _TASK_BIND_OBJECT_H_
#define _TASK_BIND_OBJECT_H_

#include <stdlib.h>
#include "bind_base.h"

namespace libcommon
{
    template<typename T, typename FUNC>
    class bind_obj_0_t:public bind_base_t
    {
    public:
        bind_obj_0_t():m_func(NULL)
        {
        }
        
        bind_obj_0_t(T instance_, FUNC func_):
        m_instance(instance_),
        m_func(func_)
        {
        }
        
        ~bind_obj_0_t()
        {
        }
        
        virtual void exec()
        {
            (m_instance->*m_func)();
        }
    private:
        T           m_instance;
        FUNC        m_func;
    };
    
    
    template<typename T, typename FUNC, typename ARG0>
    class bind_obj_1_t:public bind_base_t
    {
    public:
        bind_obj_1_t():m_func(NULL)
        {
        }
        
        bind_obj_1_t(T instance_, FUNC func_, ARG0 arg0_):
        m_instance(instance_),
        m_func(func_),
        m_arg0(arg0_)
        {
        }
        
        ~bind_obj_1_t()
        {
        }
        
        virtual void exec()
        {
            (m_instance->*m_func)(m_arg0);
        }
    private:
        T           m_instance;
        FUNC        m_func;
        ARG0        m_arg0;
    };
    
    
    template<typename T, typename FUNC, typename ARG0, typename ARG1>
    class bind_obj_2_t:public bind_base_t
    {
    public:
        bind_obj_2_t():m_func(NULL)
        {
        }
        
        bind_obj_2_t(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_):
        m_instance(instance_),
        m_func(func_),
        m_arg0(arg0_),
        m_arg1(arg1_)
        {
        }
        
        ~bind_obj_2_t()
        {
        }
        
        virtual void exec()
        {
            (m_instance->*m_func)(m_arg0, m_arg1);
        }
    private:
        T           m_instance;
        FUNC        m_func;
        ARG0        m_arg0;
        ARG1        m_arg1;
    };
    
    
    template<typename T, typename FUNC, typename ARG0, typename ARG1, typename ARG2>
    class bind_obj_3_t:public bind_base_t
    {
    public:
        bind_obj_3_t():m_func(NULL)
        {
        }
        
        bind_obj_3_t(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_):
        m_instance(instance_),
        m_func(func_),
        m_arg0(arg0_),
        m_arg1(arg1_),
        m_arg2(arg2_)
        {
        }
        
        ~bind_obj_3_t()
        {
        }
        
        virtual void exec()
        {
            (m_instance->*m_func)(m_arg0, m_arg1, m_arg2);
        }
    private:
        T           m_instance;
        FUNC        m_func;
        ARG0        m_arg0;
        ARG1        m_arg1;
        ARG2        m_arg2;
    };
    
    
    template<typename T, typename FUNC, typename ARG0, typename ARG1, typename ARG2, typename ARG3>
    class bind_obj_4_t:public bind_base_t
    {
    public:
        bind_obj_4_t():m_func(NULL)
        {
        }
        
        bind_obj_4_t(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, ARG3 arg3_):
        m_instance(instance_),
        m_func(func_),
        m_arg0(arg0_),
        m_arg1(arg1_),
        m_arg2(arg2_),
        m_arg3(arg3_)
        {
        }
        
        ~bind_obj_4_t()
        {
        }
        
        virtual void exec()
        {
            (m_instance->*m_func)(m_arg0, m_arg1, m_arg2, m_arg3);
        }
    private:
        T           m_instance;
        FUNC        m_func;
        ARG0        m_arg0;
        ARG1        m_arg1;
        ARG2        m_arg2;
        ARG3        m_arg3;
    };
}


#endif


