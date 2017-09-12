// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "network/autobuffer.h"

#include "gtest/gtest.h"
#include "iostream"

using namespace std;
using namespace libcommon;
// g++ autobuffer_unittest.cc -I../include/ -I../3rd/gtest/output/include -L../libs/ -lcommon -L../3rd/gtest/output/lib/ -lgtest -lgtest_main -o autobuffer_unitest


TEST(AutoBufferTest, Basic) {
    AutoBuffer ab0;
    EXPECT_EQ(ab0.Length(), 0);
    EXPECT_EQ(ab0.Capacity(), 0);

    char buf[] = "123456";
    ab0.Attach(buf, sizeof(buf));
    EXPECT_EQ(ab0.PosPtr(), buf);
    EXPECT_EQ(ab0.Length(), sizeof(buf));
    char val;
    ab0.Read<char>(val);
    EXPECT_EQ(val, '1');
    ab0.Read<char>(val);
    EXPECT_EQ(val, '2');
    ab0.Read<char>(val);
    EXPECT_EQ(val, '3');
    ab0.Read<char>(val);
    EXPECT_EQ(val, '4');
    ab0.Read<char>(val);
    EXPECT_EQ(val, '5');
    ab0.Read<char>(val);
    EXPECT_EQ(val, '6');
    ab0.Read<char>(val);
    EXPECT_EQ(val, '\0');

    EXPECT_EQ(ab0.PosLength(), 0);
    ab0.Detach();
}




TEST(AutoBufferTest, write) {
    AutoBuffer ab;
    char buf[] = "12345678";
    ab.Write(buf[0]);
    EXPECT_EQ(ab.Pos(), 1);
    ab.Write(buf[1]);
    EXPECT_EQ(ab.Pos(), 2);
    ab.Write(buf[2]);
    EXPECT_EQ(ab.Pos(), 3);
    ab.Write(buf[3]);
    EXPECT_EQ(ab.Pos(), 4);
    ab.Write(buf[4]);
    EXPECT_EQ(ab.Pos(), 5);
    ab.Write(buf[5]);
    EXPECT_EQ(ab.Pos(), 6);
    ab.Write(buf[6]);
    EXPECT_EQ(ab.Pos(), 7);
    ab.Write(buf[7]);
    EXPECT_EQ(ab.Pos(), 8);

    char* pt = (char*) ab.Ptr();
    cout << "ab.Ptr() " << pt << endl;
 
    bool eq = strcmp(pt, buf) == 0 ? true : false;
    EXPECT_EQ(eq, true);
}




