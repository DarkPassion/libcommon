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
