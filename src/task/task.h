#ifndef _TASK_TASK_H_
#define _TASK_TASK_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "bind_base.h"
#include "bind_function.h"
#include "bind_object.h"


class task_t
{
public:
    // bind function delay
    // 绑定全局函数api 可以延时执行
    
    template<typename FUNC>
    static task_t* bind_func_delay(FUNC func_, uint64_t ms)
    {
        bind_base_t* base = new bind_func_0_t<FUNC>(func_);
        return new task_t(base, ms);
    }
    
    template<typename FUNC, typename ARG0>
    static task_t* bind_func_delay(FUNC func_, ARG0 arg0_, uint64_t ms)
    {
        bind_base_t* bind = new bind_func_1_t<FUNC, ARG0>(func_, arg0_);
        return new task_t(bind, ms);
    }
    
    template<typename FUNC, typename ARG0, typename ARG1>
    static task_t* bind_func_delay(FUNC func_, ARG0 arg0_, ARG1 arg1_, uint64_t ms)
    {
        bind_base_t* bind = new bind_func_2_t<FUNC, ARG0, ARG1>(func_, arg0_, arg1_);
        return new task_t(bind, ms);
    }
    
    template<typename FUNC, typename ARG0, typename ARG1, typename ARG2>
    static task_t* bind_func_delay(FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, uint64_t ms)
    {
        bind_base_t * bind = new bind_func_3_t<FUNC, ARG0, ARG1, ARG2>(func_, arg0_, arg1_, arg2_);
        return new task_t(bind, ms);
    }
    
    template<typename FUNC, typename ARG0, typename ARG1, typename ARG2, typename ARG3>
    static task_t* bind_func_delay(FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, ARG3 arg3_, uint64_t ms)
    {
        bind_base_t* base = new bind_func_4_t<FUNC, ARG0, ARG1, ARG2, ARG3>(func_, arg0_, arg1_, arg2_, arg3_);
        return new task_t(base, ms);
    }
    
    
    // bind class object member function with delay
    // 可以延时执行的任务api 绑定成员函数
    
    template<typename T, typename FUNC>
    static task_t* bind_memfunc_delay(T instance_, FUNC func_, uint64_t ms)
    {
        bind_base_t* bind = new bind_obj_0_t<T, FUNC>(instance_, func_);
        return new task_t(bind, ms);
    }
    
    template<typename T, typename FUNC, typename ARG0>
    static task_t* bind_memfunc_delay(T instance_, FUNC func_, ARG0 arg0_, uint64_t ms)
    {
        bind_base_t* base = new bind_obj_1_t<T, FUNC, ARG0>(instance_, func_, arg0_);
        return new task_t(base, ms);
    }
    
    template<typename T, typename FUNC, typename ARG0, typename ARG1>
    static task_t* bind_memfunc_delay(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_, uint64_t ms)
    {
        bind_base_t* base  = new bind_obj_2_t<T, FUNC, ARG0, ARG1>(instance_, func_, arg0_, arg1_);
        return new task_t(base, ms);
    }
    
    template<typename T, typename FUNC, typename ARG0, typename ARG1, typename ARG2>
    static task_t* bind_memfunc_delay(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, uint64_t ms)
    {
        bind_base_t* base = new bind_obj_3_t<T, FUNC, ARG0, ARG1, ARG2>(instance_, func_, arg0_, arg1_, arg2_);
        return new task_t(base, ms);
    }
    
    template<typename T, typename FUNC, typename ARG0, typename ARG1, typename ARG2, typename ARG3>
    static task_t* bind_memfunc_delay(T instance_, FUNC func_, ARG0 arg0_, ARG1 arg1_, ARG2 arg2_, ARG3 arg3_, uint64_t ms)
    {
        bind_base_t * base = new bind_obj_4_t<T, FUNC, ARG0, ARG1, ARG2, ARG3>(instance_, func_, arg0_, arg1_, arg2_, arg3_);
        return new task_t(base, ms);
    }
    
public:
    task_t():m_bind(NULL), m_timestamp(0), m_is_cancle(0)
    {
        
    }
    
    task_t(bind_base_t* bind_base):m_bind(bind_base), m_timestamp(0), m_is_cancle(0)
    {
    }
    
    task_t(bind_base_t* bind_base, uint64_t ts):m_bind(bind_base), m_timestamp(ts + get_tickcount()), m_is_cancle(0)
    {
        
    }
    
    bool can_exec()
    {
        // note: 这里时时间戳需要强制转换成int64_t
        if (m_timestamp == 0 || int64_t(get_tickcount() - m_timestamp) > 100) {
            return true;
        }
        return false;
    }
    
    void exec()
    {
        if (m_bind) {
            m_bind->exec();
        }
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
    
    bool is_cancle()
    {
        return m_is_cancle;
    }
    
    void cancle_task()
    {
        m_is_cancle = 1;
    }
    
private:
    
    task_t(const task_t& bind_);
    
    void operator=(const task_t& bind_);
    
    uint64_t get_tickcount()
    {
        uint64_t ts = 0;
        struct timeval val;
        gettimeofday(&val, NULL);
        ts = val.tv_sec * 1000 + val.tv_usec / 1000;
        return ts;
    }
    
private:
    bind_base_t*    m_bind;         // 任务指针
    uint64_t        m_timestamp;    // 时间相关任务 时间戳 非时间相关任务这个字段为空
    int             m_is_cancle;    // 任务是否已经取消
};



#endif


