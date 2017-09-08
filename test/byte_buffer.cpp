
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "util/bytebuffer.h"

// g++ byte_buffer.cpp -I../include/ -L../libs/ -lcommon -o byte_buffer.out

using namespace libcommon;



void test_bytebuff();


void test_bytebuff()
{
    ByteBuffer b1;

    printf("byteBuffer size [%zu %zu] \n", b1.Length(), b1.Capacity());

    if (b1.Order() == ByteBuffer::ORDER_NETWORK) {
        printf("bytebuffer order ORDER_NETWORK! \n");
    } else if (b1.Order() == ByteBuffer::ORDER_HOST) {
        printf("bytebuffer order ORDER_HOST! \n" );
    }

    int st = 0;
    for (int i = 0; i < 100; i++) {
        b1.WriteUInt8(i);
    }

    while (1) {

        uint8 c;
        bool ret = b1.ReadUInt8(&c);

        if (ret == false) {

            break;
        }
        if (st++ == c) {
            // eq
        } else {
            assert(false);
        }
    }
    printf("byteBuffer size [%zu %zu] \n", b1.Length(), b1.Capacity());


    st = 0;
    for (int i = 0; i < 1000; i++) {
        b1.WriteUInt16(i);
    }

    while (1) {

        uint16 s;

        bool ret = b1.ReadUInt16(&s);

        if (ret == false) {

            break;
        }

        if (st++ == s) {
            // eq
        } else {
            assert(false);
        }
    }

    st = 0;
    for (int i = 0; i < 1000; i++) {
        b1.WriteUInt24(i);
    }

    while (1) {
        uint32 s;

        bool ret = b1.ReadUInt24(&s);

        if (ret == false) {

            break;
        }

        if (st++ == s) {
            // eq
        } else {
            assert(false);
        }
    }

    st = 0;
    for (int i = 0; i < 100000; i++) {

        b1.WriteUInt32(i);
    }

    while (1) {
        uint32 s;

        bool ret = b1.ReadUInt32(&s);

        if (ret == false) {
            break;
        }

        if (st++ == s) {
            // eq
        } else {
            assert(false);
        }
    }

    uint64 s64 = 0;

    for (uint64 i = 0; i < 1234567890; i++) {
        b1.WriteUInt64(i);
    }

    while (1) {

        uint64 s;

        bool ret = b1.ReadUInt64(&s);

        if (ret == false) {
            break;
        }

        if (s64++ == s) {
            // eq
        } else {
            assert(false);
        }
    }


}


int main()
{

    test_bytebuff();
    return 0;
}
