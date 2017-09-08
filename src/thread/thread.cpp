#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "thread/thread.h"
#include "util/log.h"

namespace libcommon
{
enum ThreadStatus
{
    THREAD_INIT = 1,
    THREAD_RUNING,
    THREAD_STOP
};

ThreadHandle::ThreadHandle()
{

}

ThreadHandle::~ThreadHandle()
{

}

void ThreadHandle::on_thread_start()
{

}


void ThreadHandle::on_thread_stop()
{

}


int ThreadHandle::on_before_cycle()
{
    return 0;
}


int ThreadHandle::on_end_cycle()
{
    return 0;
}

ThreadImp::ThreadImp(const char* name, ThreadHandle* handle, int interval_ms, int joinable)
{
    _name = name;
    _handle = handle;
    _interval_ms = interval_ms;

    _loop = 0;
    _pth = 0;
    _joinable = joinable;
    _status = THREAD_INIT;

}

ThreadImp::~ThreadImp()
{
    stop();
}

int ThreadImp::start()
{
    if (_pth)
    {
        LOGW("thread already run !");
        return 0;
    }

    _loop = 1;
    pthread_create(&_pth, NULL, thread_func, this);
    return 0;
}

int ThreadImp::stop()
{
    LOGD("ThreadImp [%s %d %d] stop !", _name, _interval_ms, _joinable);
    if (_status == THREAD_INIT)
    {
        return 0;
    }
    _loop = 0;


    if (_joinable && _pth)
    {
        pthread_join(_pth, NULL);
        LOGD("ThreadImp [%s] pthread_detach", _name);
    }

    if (!_joinable && _pth)
    {
        pthread_detach(_pth);
        LOGD("ThreadImp [%s] pthread_detach", _name);
    }

    _status = THREAD_INIT;
    _pth = NULL;

    return 0;
}

void ThreadImp::stop_loop()
{
    _loop = 0;
}


void* ThreadImp::thread_func(void* param)
{
    ThreadImp* pthis = (ThreadImp*)param;
    pthis->thread_cycle();
    return NULL;
}


void ThreadImp::thread_cycle()
{
    LOGD("ThreadImp thread cycle [%s]", _name);

    assert(_handle);
    _handle->on_thread_start();

    _status = THREAD_RUNING;
    while (_loop)
    {

        if (_handle->on_before_cycle() != 0)
        {
            break;
        }

        if (_handle->cycle() != 0)
        {
            break;
        }

        if (_handle->on_end_cycle() != 0)
        {
            break;
        }

        if (_interval_ms)
        {
            usleep(_interval_ms * 1000);
        }

    }

    _status = THREAD_STOP;
    _handle->on_thread_stop();
}

int ThreadImp::can_loop()
{
    return _loop;
}

}







