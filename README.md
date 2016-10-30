


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

block queue

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
AutoFreeClassA(char, tmp);

// auto free tmp1
char* tmp1 = new char;
AutoFreeClass(char, tmp1);

// auto delete obj0
obj0_t * obj0 = new obj0_t;
AutoFreeClass(obj0_t, obj0);

// auto delete[] obja
obj0_t* obja = new obj0_t[10];
AutoDeleteScopePtr(obj0_t[], obja);

// auto free aa1
char* aa1 = (char*)malloc(100);
AutoFreeScopePtr(char, aa1);

```

