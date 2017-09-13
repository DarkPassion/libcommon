
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

void test_thread_notify()
{
    ThreadNotify* notify = Singleton<ThreadNotify>::get();
    notify->register_signal1("com.xx.xx1");
    notify->register_signal1("com.xx.xx2");

    //void connect_signal1(const char* name, Delegate1< void* > delegate);
    notify->connect_signal1("com.xx.xx1", test_signal1_com_xx1);
    
    notify->post_signal1("com.xx.xx1", NULL);
}

int main()
{
    test_thread_notify();
    usleep(8000*1000);

    return 0;
}




