


## README


### thread
```C++
class Thread1 : public ThreadHandle {
public:
	Thread1(){}

	~Thread1(){}

	virtual int cycle()
	{
		printf("cycle !\n");
	}

}

Thread1 t1 = new Thread1;
t1.start();
```

### queue

```C++
struct sitem
{
    int seq;
    int id;
};
typedef Queue<sitem> TypeQueue;

TypeQueue* _q  = new TypeQueue;

// thread safe
sitem* i = _q->pop_front();
if (i) delete i;

sitem* i = new sitem();
i->seq = num++;
i->id = 1122;
// thread safe
_q->push_back(i);

```

block queue

```C++

struct sitem
{
    int seq;
    int id;
};
typedef BlockQueue<sitem> TypeQueue;

TypeQueue * _q = new TypeQueue;

// thread safe
_q->push_back(new sitem());


// will hug when queue empty
sitem* i = _q->pop_front();
if(i) delete i;

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

