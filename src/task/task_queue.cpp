
#include <unistd.h>
#include <algorithm>    // std::find
#include "task/task_queue.h"
#include "task/task.h"
#include "util/log.h"

TaskQueueThread::TaskQueueThread() : _pth(NULL), _cond(_mutex)
{
    _pth = new ThreadImp("TaskQueue", this, 1000, true);
}

TaskQueueThread::~TaskQueueThread()
{
    if (true) {
        AutoLock __lock(_mutex);
        _cond.notify_all();
        for (TypedeQueue::iterator iter = _queue.begin(); iter != _queue.end(); iter++) {
            task_t* task = *iter;
            delete task;
        }
        
        TypedeQueue tmp;
        tmp.swap(_queue);
        
    }
    _pth->stop_loop();
    delete _pth;
}

void TaskQueueThread::start_work()
{
    _pth->start();
}

void TaskQueueThread::add_task(task_t* task)
{
    LOGD("func [%s] add task", __FUNCTION__);
    
    if (task == NULL) {
        LOGE("TaskQueueThread add task == null");
        return ;
    }
    
    AutoLock __lock(_mutex);

    _queue.push_back(task);
    
    _cond.notify_all();

}



int TaskQueueThread::cycle()
{
    while (_pth->can_loop()) {
    
        // get task
        TypedeQueue vect;
        get_tasks(vect);
    
        if (vect.size() > 0) {
            for (TypedeQueue::iterator iter = vect.begin(); iter != vect.end(); iter++) {
                task_t * task = *iter;

                if (task != NULL) {
                    if (task->is_cancle()) {
                        remove_task(task);
                        continue;
                    }
                    
                    if (task->can_exec()) {
                        task->exec();
                        remove_task(task);
                        usleep(100 * 1000);
                    } else {
                        usleep(100 * 1000);
                    }
                }
            }
            
        } else {
            usleep(100 * 1000);
            continue;
        }
    }
    
    return 0;
}

void TaskQueueThread::on_thread_start()
{
    LOGD(" TaskQueueThread thread start ! func [%s] ", __FUNCTION__);
}


void TaskQueueThread::on_thread_stop()
{
    LOGD(" TaskQueueThread thread stop ! func [%s] ", __FUNCTION__);
}


void TaskQueueThread::get_tasks(TypedeQueue& vect)
{
    LOGD("TaskQueueThread func [%s] ", __FUNCTION__);

    vect.clear();
    
    AutoLock __lock(_mutex);

    if (_queue.size() > 0) {
        
        for (TypedeQueue::iterator iter = _queue.begin(); iter != _queue.end(); iter++) {
            task_t* task = *iter;
            vect.push_back(task);
        }
        
    } else {
        _cond.wait();
    }
}

void TaskQueueThread::remove_task(task_t* task)
{

    if (task == NULL) {
        LOGE("TaskQueueThread remove task == null");
        return ;
    }

    AutoLock __lock(_mutex);
    TypedeQueue::iterator iter = std::find(_queue.begin(), _queue.end(), task);
    if (iter != _queue.end()) {
        _queue.erase(iter);
        delete task;
    }
    
}



