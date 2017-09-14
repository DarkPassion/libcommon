
#include <iostream>
#include <ctime>
#include <string>
#include "task/sigslot.h"
#include "util/util.h"


//using namespace libcommon;
using namespace std;
// g++ sigslot_test.cpp -DSIGSLOT_USE_POSIX_THREADS -I../include/ -L../libs/ -lcommon -o sigslot_test.out

void test_sigslot_fun1();
void test_sigslot_fun2();
void test_sigslot_fun3();



class Sender  {
public:
    sigslot::signal2<std::string, std::time_t> SignalDanger;
    
    sigslot::signal0<> SignalCallv;
    
    sigslot::signal1<string> SignalCalls;
    
    void Panic(){
        SignalDanger("Help!", std::time(0));
    }
    
    void TCallv() {
        SignalCallv();
    }
    
    void TCalls() {
        SignalCalls("helloworld");
    }
};


class Receiver : public sigslot::has_slots<> {
public:
    Receiver(Sender& sender){
        sender.SignalDanger.connect(this, &Receiver::OnDanger);
        sender.SignalCallv.connect(this, &Receiver::OnCall);
        sender.SignalCalls.connect(this, &Receiver::onCall2);
    }
    
    void OnDanger(std::string message, std::time_t time){
        if(message == "Help!")
        {
            std::cout << "Call the police" << std::endl;
        }
    }
    
    void OnCall() {
        
        cout << "on call !"<< endl;
    }
    
    void onCall2(string s) {
        cout << "on call 2!" << s.c_str() <<endl;
    }
    
};

void test_sigslot_fun1()
{
    Sender sender;
    Receiver receiver(sender);
    sender.Panic();
    sender.TCallv();
    sender.TCalls();
}



class SignalCls1 : public sigslot::has_slots<>
{
private:
    sigslot::signal1<string> sig1;
    sigslot::signal2<string, string> sig2;
    sigslot::signal3<string, string, string> sig3;
    sigslot::signal4<string, string, string, string> sig4;
    sigslot::signal5<string, string, string, string, string> sig5;
    
    void  _test_sig1(string a1)
    {
        cout << "FUNC " << __FUNCTION__ << " " << a1 << endl;
    }
    
    void  _test_sig2(string a1, string a2)
    {
        cout << "FUNC " << __FUNCTION__ << " " << a1 << endl;
    }
    
    void  _test_sig3(string a1, string a2, string a3)
    {
        cout << "FUNC " << __FUNCTION__ << " " << a1 << endl;
    }
    
    void  _test_sig4(string a1, string a2, string a3, string a4)
    {
        cout << "FUNC " << __FUNCTION__ << " " << a1 << endl;
    }
    
    void  _test_sig5(string a1, string a2, string a3, string a4, string a5)
    {
        cout << "FUNC " << __FUNCTION__ << " " << a1 << endl;
    }
    
    void _signal()
    {
        cout << "FUNC " << __FUNCTION__ << endl;

        sig1("test sig1");
        sig2("test sig2", "1");
        sig3("test sig3", "1", "2");
        sig4("test sig4", "1", "2", "3");
        sig5("test sig5", "1", "2", "3", "4");

    }
    
    void  _connect()
    {
        cout << "FUNC " << __FUNCTION__ << endl;
        
        sig1.connect(this, &SignalCls1::_test_sig1);
        sig2.connect(this, &SignalCls1::_test_sig2);
        sig3.connect(this, &SignalCls1::_test_sig3);
        sig4.connect(this, &SignalCls1::_test_sig4);
        sig5.connect(this, &SignalCls1::_test_sig5);

    }
    
    void _disconnect()
    {
        cout << "FUNC " << __FUNCTION__ << endl;
        this->disconnect_all();
    }
    
    void _disconnect_one()
    {
        cout << "FUNC " << __FUNCTION__ << endl;
        this->sig1.disconnect(this);
    }
    
public:
    SignalCls1()
    {
        this->_connect();
        this->_signal();
        this->_disconnect();
        
        this->_signal();
        this->_connect();
        this->_disconnect_one();
        this->_signal();
    }
};


void test_sigslot_fun2()
{
    SignalCls1 cls;
    
}


int main()
{
    test_sigslot_fun1();
    test_sigslot_fun2();
    
    return 0;
}




