#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>
#include "util/json.h"

// g++ json.cpp -I../include/ -I../3rd/jsoncpp/include -L../libs/ -L../3rd/jsoncpp/lib -lcommon -ljsoncpp -o json.out


using namespace libcommon;
using namespace std;

void test_json_func();


static Json::Value in_s("foo");
static Json::Value in_sn("99");
static Json::Value in_si("-99");
static Json::Value in_sb("true");
static Json::Value in_sd("1.2");
static Json::Value in_n(12);
static Json::Value in_i(-12);
static Json::Value in_u(34U);
static Json::Value in_b(true);
static Json::Value in_d(1.2);
static Json::Value big_sn("12345678901234567890");
static Json::Value big_si("-12345678901234567890");
static Json::Value big_u(0xFFFFFFFF);
static Json::Value bad_a(Json::arrayValue);
static Json::Value bad_o(Json::objectValue);


void test_json_func()
{

    Json::Value o, out;
    o["string"] = in_s;
    o["int"] = in_i;
    o["uint"] = in_u;
    o["bool"] = in_b;
    string s = JsonValueToString(o);
    printf("jsonvalueToString [%s]\n", s.c_str());

    assert(GetValueFromJsonObject(o, "int", &out));
    assert(GetValueFromJsonObject(o, "bool", &out));
    assert(!GetValueFromJsonObject(o, "foo", &out));
    assert(!GetValueFromJsonObject(o, "", &out));


    Json::Reader reader;
    Json::Value root;
    if (reader.parse(s, root))
    {
        printf("reader parse succ! \n");
        assert(GetValueFromJsonObject(root, "int", &out));
        string jval;
        GetStringFromJson(out, &jval);
        int ival = 0;
        GetIntFromJson(out, &ival);
        printf("jsoncpp GetStringFromJson %s %d\n", jval.c_str(), ival);


    }



}

int main()
{

    test_json_func();
    return 0;
}




