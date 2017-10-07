#include <iostream>
#include <memory>
#include "util/smart_ptr.h"
using namespace libcommon;

// g++ smart_ptr_test.cpp -I../include/ -L../libs/ -lcommon -o smart_ptr_test.out

class CBase
{
public:
    CBase() : data0(0)
    {
        std::cout << "CBase()" << std::endl;
    }

    virtual ~CBase()
    {
        std::cout << "~CBase()" << std::endl;
    }

    int data0;
private:
};

class CDevide : public CBase
{
public:
    CDevide() : data2(3)
    {
        std::cout << "CDevide()" << std::endl;
    }

    ~CDevide()
    {
        std::cout << "~CDevide()" << std::endl;
    }

    int data2;
private:
};


#ifndef CDECL
#if defined(WIN32)
#define CDECL           _cdecl
#else
#define CDECL
#endif // defined(WIN32)
#endif // !CDECL

int test1(void)
{
    strong_ptr<CBase> spBase(new CDevide());

    {
        strong_ptr<CDevide> spDummy(spBase);

        weak_ptr<CBase> spW1(spBase);
        spW1.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;

        weak_ptr<CDevide> spW2(spBase);
        spW2.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;

        weak_ptr<CBase> spW3(spW1);
        spW3.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;

        weak_ptr<CDevide> spW4(spW1);
        spW4.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;

        weak_ptr<CBase> spW5;
        spW5 = spBase;
        spW5.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;

        weak_ptr<CDevide> spW6;
        spW6 = spBase;
        spW6.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;

        weak_ptr<CBase> spW7;
        spW7 = spW1;
        spW7.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;

        weak_ptr<CDevide> spW8;
        spW8 = spW1;
        spW8.lock()->data0++;
        std::cout << spDummy->data0 << std::endl;
    }

    {
        weak_ptr<CBase> wpDummy(spBase);

        strong_ptr<CBase> sp1(spBase);
        sp1->data0++;
        std::cout << sp1->data0 << std::endl;

        strong_ptr<CDevide> sp2(spBase);
        sp2->data0++;
        std::cout << sp1->data0 << std::endl;

        strong_ptr<CBase> sp3(wpDummy);
        sp3->data0++;
        std::cout << sp1->data0 << std::endl;

        strong_ptr<CDevide> sp4(wpDummy);
        sp4->data0++;
        std::cout << sp1->data0 << std::endl;

        strong_ptr<CBase> sp5;
        sp5 = spBase;
        sp5->data0++;
        std::cout << sp1->data0 << std::endl;

        strong_ptr<CDevide> sp6;
        sp6 = spBase;
        sp6->data0++;
        std::cout << sp1->data0 << std::endl;

        strong_ptr<CBase> sp7;
        sp7 = wpDummy;
        sp7->data0++;
        std::cout << sp1->data0 << std::endl;

        strong_ptr<CDevide> sp8;
        sp8 = wpDummy;
        sp8->data0++;
        std::cout << sp1->data0 << std::endl;
    }

    return 0;
}

#include <memory>
void test2(void)
{
    CBase * pObj = new CDevide();
#if 0
    std::shared_ptr<CBase> sp1;
    sp1 = (pObj);
    std::weak_ptr<CBase> wsp;
    wsp = (sp1);
#else
    strong_ptr<CBase> sp1;
    sp1.reset(pObj);
    weak_ptr<CDevide> wsp;
    wsp.reset(sp1);

    if (sp1 == wsp.lock()) {
        std::cout << "Equal" <<std::endl;
    }

    sp1.reset();

    if (sp1.get() == NULL) {
        std::cout << "sp1 == NULL" << std::endl;
    }

    if (wsp.expired()) {
        std::cout << "wsp expired " << std::endl;
    }


    if (sp1 == wsp.lock()) {
        std::cout << "== Equal" <<std::endl;
    }

#endif
}

void test3()
{
    const int CI = 10;
    strong_array<CDevide> sp1;
    CDevide* pobj = new CDevide[CI];
    sp1.reset(pobj);

    for (int i = 0; i < CI; i++) {
        sp1[i].data0 = i + 1;

    }

    for (int i = 0; i < CI; i++) {
        std::cout << " spi " << i << " data0 "<< sp1[i].data0 << std::endl;
    }
}


void test4()
{


}




int CDECL main(void)
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}
