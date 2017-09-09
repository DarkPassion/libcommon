
#include <iostream>
#include "util/singleton.h"

// g++ singleton_test.cpp -I../include/ -L../libs/ -lcommon -o singleton_test.out

using namespace libcommon;

class TestType
{
    public:
        TestType()
        {


        }

        ~TestType()
        {


        }

    private:
        char buf[64];
};




int main()
{
    TestType* pt1 = Singleton<TestType>::get();
    TestType* pt2 = Singleton<TestType>::get();
    TestType* pt3 = Singleton<TestType>::get();

    if (pt1 == pt2 && pt2 == pt3) {
        std::cout << " Singleton<TestType>::get() == equal" << std::endl;
    }

    Singleton<TestType>::ShutDown();

    return 0;
}



