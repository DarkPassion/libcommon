


## README

### Cross platform common library
support android
support ios
support linux


### thread
```C++
class Thread1 : public ThreadHandle {
public:
	Thread1()
    {
        _pth = new ThreadImp("Thread1", this, 1000, true);
    }

	~Thread1()
    {
        delete _pth;
    }

	virtual int cycle()
	{
		printf("cycle !\n");
        return 0;
	}

    virtual void on_thread_start()
    {
        printf("on thread start! \n");
    }

    virtual void on_thread_stop()
    {
        printf("on thread stop! \n");
    }

    void start()
    {
        _pth->start();
    }
private:
    ThreadImp* _pth;
}
Thread1 t = new Thread1();
t->start();

```

### ThreadQueue
```C++

using namespace libcommon;

void test_thread_queue();

class obj3
{
public:
    void invoke(int cc1, int cc2, int cc3)
    {
        printf("obj3 == > invoke [%d %d %d]\n", cc1, cc2, cc3);
    }
};

void test_thread_queue()
{
    ThreadQueue _queue("task.run.queue");
    obj3 _obj;

    typedef void (obj3::*PFN)(int, int, int);
    PFN _pf = &obj3::invoke;
    task_t* val1 = task_t::bind_memfunc_delay<obj3*, PFN, int, int, int>(&_obj, _pf, 11, 11, 11, 3000);
    task_t* val2 = task_t::bind_memfunc_delay<obj3*, PFN, int, int, int>(&_obj, _pf, 22, 22, 22, 2000);
    task_t* val3 = task_t::bind_memfunc_delay<obj3*, PFN, int, int, int>(&_obj, _pf, 33, 33, 33, 1000);


    _queue.enqueue(val1);
    _queue.enqueue(val2);
    _queue.enqueue(val3);

    usleep(8000*1000);
}

int main()
{

    test_thread_queue();
    return 0;
}

```

### queue

```C++
struct qmessage
{
    int seq;
    int id;
};
typedef Queue<qmessage> TypeQueue;

TypeQueue* _q  = new TypeQueue;

// thread safe
qmessage* m = _q->pop_front();
if (m) delete m;

qmessage* m = new qmessage();
m->seq = num++;
m->id = 1122;
// thread safe
_q->push_back(m);

```

### block queue

```C++

struct qmessage
{
    int seq;
    int id;
};
typedef BlockQueue<qmessage> TypeQueue;

TypeQueue * _q = new TypeQueue;

// thread safe
_q->push_back(new qmessage());


// will hug when queue empty
qmessage* m = _q->pop_front();
if(m) delete m;

```


### util
```C++

struct obj0_t
{
	int id;
	int inc;
	int seq;
};

// auto free tmp
char* tmp = new char[100];
scoped_sample_array<char> tmp_char_arr(tmp);

// auto free tmp1
char* tmp1 = new char;
scoped_sample_ptr<char> tmp_char(tmp);

// auto delete obj0
obj0_t * obj0 = new obj0_t;
scoped_sample_ptr<obj0_t> tmp_obj_t(obj0);

// auto delete[] obja
obj0_t* obja = new obj0_t[10];
scoped_sample_array<obj0_t> tmp_obj_arr(obja);

// auto free aa1
char* aa1 = (char*)malloc(100);
scoped_ptr_malloc<char> tmp_char_ma(aa1);

```

### Task
```C++
using namespace libcommon;

// g++ task_test.cpp -I../include/ -L../libs/ -lcommon -o task_test.out



void test_bind_function0();
void test_bind_function1();
void test_bind_function2();
void test_bind_function3();
void test_bind_function4();

void test_bind_object0();
void test_bind_func_obj0();
void test_bind_object1();
void test_bind_object2();
void test_bind_object3();
void test_bind_object4();


void test_ff()
{
    printf("test ff \n");
}

void test_bind_function0()
{
    typedef void (* pf)(void);
    task_t* t = task_t::bind_func_delay<pf>(test_ff, 0);
    t->exec();
}

void test_ff1(int cc)
{
    printf("test ff1 [%d]\n", cc);
}
void test_bind_function1()
{
    typedef void (*pf)(int);
    task_t* t = task_t::bind_func_delay<pf>(test_ff1, 11, 0);
    t->exec();
}

void test_ff2(int cc1, int cc2)
{
    printf("test ff2 [%d %d]\n", cc1, cc2);
}
void test_bind_function2()
{
    typedef void (*pf)(int, int);
    task_t* t = task_t::bind_func_delay<pf>(test_ff2, 11, 22, 0);
    t->exec();
}

void test_ff3(int cc1, int cc2, int cc3)
{
    printf("test ff3 [%d %d %d]\n", cc1, cc2, cc3);
}

void test_bind_function3()
{
    typedef void (*pf)(int, int, int);
    task_t* t = task_t::bind_func_delay<pf>(test_ff3, 11, 22, 33, 0);
    t->exec();
}


void test_ff4(int cc1, int cc2, int cc3, int cc4)
{
    printf("test ff4 [%d %d %d %d]\n", cc1, cc2, cc3, cc4);
}

void test_bind_function4()
{
    typedef void (*pf)(int, int, int, int);
    task_t* t = task_t::bind_func_delay<pf>(test_ff4, 11, 22, 33, 44, 0);
    t->exec();
}


void test_bind_func_obj0()
{
    struct obj0
    {
        void invoke()
        {
            printf("func obj0 invoke == \n");
        }
    };

    typedef void(obj0::*pf)();
    obj0 _obj;
    pf _pf = &obj0::invoke;

#if 0
    task_t* t = task_t::bind_memfunc_delay<obj0*, pf>(&_obj, _pf, 0);
    t->exec();
#endif
}

class obj0
{
public:
    void invoke()
    {
        printf("obj0 invoke === \n");
    }
};
void test_bind_object0()
{
    typedef void (obj0::*pf)();
    obj0 _obj;
    pf _pf = &obj0::invoke;
    task_t* t = task_t::bind_memfunc_delay<obj0*, pf>(&_obj, _pf, 0);
    t->exec();
}

class obj1
{
public:

    void invoke(int cc)
    {
        printf("obj1 == > invoke [%d]\n", cc);
    }

};
void test_bind_object1()
{
    typedef void (obj1::*pf)(int);
    obj1 _obj;
    pf _pf = &obj1::invoke;
    task_t* t = task_t::bind_memfunc_delay<obj1*, pf, int>(&_obj, _pf, 11, 0);
    t->exec();
}

class obj2
{

public:
    void invoke(int cc1, int cc2)
    {
    printf("obj2 == > invoke [%d %d]\n", cc1, cc2);
    }
};
void test_bind_object2()
{
    typedef void (obj2::*pf)(int, int);
    obj2 _obj;
    pf _pf = &obj2::invoke;
    task_t* t = task_t::bind_memfunc_delay<obj2*, pf, int, int>(&_obj, _pf, 11, 22, 0);
    t->exec();
}

class obj3
{
public:
    void invoke(int cc1, int cc2, int cc3)
    {
        printf("obj3 == > invoke [%d %d %d]\n", cc1, cc2, cc3);
    }
};

void test_bind_object3()
{
    typedef void (obj3::*pf)(int, int, int);
    obj3 _obj;
    pf _pf = &obj3::invoke;
    task_t* t = task_t::bind_memfunc_delay<obj3*, pf, int, int, int>(&_obj, _pf, 11, 22, 33, 0);
    t->exec();
}


class obj4
{
public:
    void invoke(int cc1, int cc2, int cc3, int cc4)
    {
    printf("obj4 == > invoke [%d %d %d %d]\n", cc1, cc2, cc3, cc4);
    }
};

void test_bind_object4()
{
    typedef void (obj4::*pf)(int, int, int, int);
    obj4 _obj;
    pf _pf = &obj4::invoke;
    task_t* t = task_t::bind_memfunc_delay<obj4*, pf, int, int, int, int>(&_obj, _pf, 11, 22, 33, 44, 0);
    t->exec();
}

int main()
{

    test_bind_function0();
    test_bind_function1();
    test_bind_function2();
    test_bind_function3();
    test_bind_function4();


    test_bind_object0();
    test_bind_object1();
    test_bind_object2();
    test_bind_object3();
    test_bind_object4();

    test_bind_func_obj0();
    return 0;
}
```

### Smart Pointer
```C++

#include <iostream>
#include "smart_ptr/smart_ptr.h"
using namespace std;
// g++ smart_ptr_test.cpp -I../include/ -L../libs/ -lcommon -o smart_ptr_test.out


class CData
{
    public:
        CData()
        {
            p1 = 10;
            p2 = 20;
        }

        ~CData()
        {
            cout << "~CData()" << endl;
        }

    public:

        void func()
        {
            cout << "func -- " << endl;
            cout << " p1 = " << p1 << " p2 = " << p2 << endl;
        }

        void set(int p1, int p2)
        {
            this->p1 = p1;
            this->p2 = p2;
        }

    private:
        int p1;
        int p2;
};


class CData2
{
    public:
        CData2(libcommon::shared_ptr<CData> sh)
        {
            _ptr = sh;
        }

        ~CData2()
        {
            cout << "~CData2() " << endl;
        }

        void invoke()
        {
            cout << "CData2 - invoke" << endl;
            if (_ptr.expired()) {
                cout << "shared_ptr has expired " << endl;
            } else {
                libcommon::shared_ptr<CData> sh = _ptr.lock();
                sh->func();
            }
        }


    private:
        libcommon::weak_ptr<CData> _ptr;
};

class CData3 : public libcommon::enable_shared_from_this<CData3>
{
    public:
    CData3() : p1 ( 10 )
    {

    }

    ~CData3()
    {
        cout << "~CData3() -" << endl;
    }

    private:
    int p1;
};


void test_func_data0()
{
    libcommon::shared_ptr<CData> sc(new CData());
    sc->func();

    libcommon::shared_ptr<CData2> sc2(new CData2(sc));
    sc2->invoke();

    sc.reset();

    sc2->invoke();

    libcommon::shared_ptr<CData3> sc3(new CData3());
    libcommon::weak_ptr<CData3> wc3 = sc3->shared_from_this();
    sc3.reset();
    if (wc3.expired())
    {
        cout << "wc3->expired() true" << endl;
    }
}



int main()
{
    test_func_data0();
    return 0;
}


```


### TEST
[Test Source Code] (https://github.com/DarkPassion/libcommon/tree/master/test)



