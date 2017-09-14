
#include <unistd.h>
#include <iostream>

#include "task/thread_notify.h"
#include "util/singleton.h"

// g++ thread_notify.cpp -I../include/ -L../libs/ -lcommon -o thread_notify.out

using namespace libcommon;
using namespace std;

void test_thread_notify();

void test_signal1_com_xx1(void* param)
{
    cout << "test_signal1_com_xx1 " << endl;
}

void test_signal1_com_xxx1(void* param)
{
    cout << "test_signal1_com_xxx1 " << endl;
}

struct notify_signal1
{
    int p;
    
    void test(void* param)
    {
        cout << " notify_signal1 --" << endl;
    }
};

void test_thread_notify()
{
    ThreadNotify* notify = Singleton<ThreadNotify>::get();
    char name1[] = "com.xx.xx1";
    char name2[] = "com.xx.xx1";
    notify_signal1 _sig;
    
    //typedef void (X::*func)( Param1 p1 );
    typedef void (notify_signal1::*PFN)(void*);
    PFN fn = &notify_signal1::test;

    notify->register_signal1(name1);
    notify->register_signal1(name2);

    notify->connect_signal1(name1, test_signal1_com_xx1);
    notify->connect_signal1(name1, test_signal1_com_xxx1);
    
    notify->connect_signal1(name1, MakeDelegate<notify_signal1, notify_signal1>(&_sig, fn));
    
    cout << "post connect signal----" << endl;
    notify->post_signal1(name1, NULL);
    
    usleep(800*1000);

}

int main()
{
    test_thread_notify();
    usleep(8000*1000);

    return 0;
}




