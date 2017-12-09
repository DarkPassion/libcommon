
#include <iostream>
#include <unistd.h>
#include "smart_ptr/weak_ptr.h"

using namespace std;
using namespace libcommon;

//  class Controller : public SupportsWeakPtr<Controller> {
//   public:
//    void SpawnWorker() { Worker::StartNew(AsWeakPtr()); }
//    void WorkComplete(const Result& result) { ... }
//  };
//
//  class Worker {
//   public:
//    static void StartNew(const WeakPtr<Controller>& controller) {
//      Worker* worker = new Worker(controller);
//      // Kick off asynchronous processing...
//    }
//   private:
//    Worker(const WeakPtr<Controller>& controller)
//        : controller_(controller) {}
//    void DidCompleteAsynchronousProcessing(const Result& result) {
//      if (controller_)
//        controller_->WorkComplete(result);
//    }
//    WeakPtr<Controller> controller_;
//  };


class Controller : public SupportsWeakPtr<Controller> {
public:
    void test()
    {

    }
    void WorkComplete(int index)
    {
        cout << "WorkComplete " << " index = " << index << endl;
    }

    ~Controller()
    {
        cout << "~Controller -" << endl;
    }
};


class Worker
{
public:
    Worker(const WeakPtr<Controller>& controller, int index) : controller_(controller), index_(index)
    {

    }

    void DidCompleteAsynchronousProcessing()
    {
        if (controller_) {
            controller_->WorkComplete(index_);
        } else {
            cout << "controller_ == null" << endl;
        }

    }

private:
    WeakPtr<Controller> controller_;
    int index_;
};


int main()
{
    Controller* ctrl = new Controller();

    Worker* ws[10] = {NULL};
    for (int i = 0; i < 10; ++i)
    {
        ws[i] = new Worker(ctrl->AsWeakPtr(), i + 1);
    }

    for (int i = 0; i < 10; ++i)
    {
        ws[i]->DidCompleteAsynchronousProcessing();

        if (i == 8) {
            delete ctrl;
            ctrl = NULL;
        }
        usleep(10 * 1000);

    }



    return 0;
}


