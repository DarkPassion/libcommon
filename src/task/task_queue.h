#ifndef _TASK_TASK_QUEUE_H_
#define _TASK_TASK_QUEUE_H_

#include <stdlib.h>
#include <vector>
#include "thread/thread.h"
#include "thread/lock.h"



// 绑定全局函数或者类静态函数
// void async_run(const char* url);
// typedef void (*PFUNC)(const char* url);
// PFUNC _pf = async_run;
// task_t * task = task_t::bind_func_delay<PFUNC, const char*>(_pf, "helloworld", 0);
// BBWorkerThread::getInst()->add_task(task);

// 绑定类成员函数
//  class AsynTask{
//  public:
//      void aysnc_run(const char* url);
//  };
//
//  AsynTask atask;
//  typedef void (AsynTask::*PFUNC)(const char* url);
//  PFUNC _pf = &AsynTask::aysnc_run;
//  task_t * task = task_t::bind_memfunc_delay<AsynTask*, PFUNC, const char*>(&atsk, _pf, "helloworld", 0);
//  BBWorkerThread::getInst()->add_task(task);

// 绑定 C++11 Lamdba表达式
//  typedef void (*PFUNC)(const char* url);
//  PFUNC _pf = [](const char* url){
//      printf("url [%s] \n", url);
//  };
//  task_t* task = task_t::bind_func_delay<PFUNC>(_pf, "helloworld", 0);
//  BBWorkerThread::getInst()->add_task(task);




class task_t;

class TaskQueueThread : public ThreadHandle
{
public:
    TaskQueueThread();
    
    ~TaskQueueThread();
    
    void add_task(task_t* task);
    
    void start_work();
    
public:
    // ThreadHandle interface
    virtual int cycle();
    
    virtual void on_thread_start();

    virtual void on_thread_stop();

private:
    typedef std::vector<task_t*> TypedeQueue;

    void get_tasks(TypedeQueue& vect);
    
    void remove_task(task_t* task);
    
private:
    ThreadImp*      _pth;
    TypedeQueue     _queue;
    CMutex          _mutex;
    CCond           _cond;
};

#endif
