#include <algorithm>
#include <iostream>
#include <thread>

using namespace std;

class WorkerThread {
public:
    void operator()() {
        cout << "Worker Thread " << this_thread::get_id() << "is Executing"
             << endl;
    }
};

int main(void) {
    vector<thread> threadList;
    for (int i = 0; i < 10; i++) {
        threadList.push_back(thread(WorkerThread()));
    }
    // 等待所有的工作线程结束，即对每一个 thread 对象调用 join() 函数
    cout << "wait for all the worker thread to finish" << endl;
    for_each(threadList.begin(), threadList.end(), mem_fn(&thread::join));
    // 下面这条语句是最后打印的
    cout << "Exiting from Main Thread" << endl;

    return 0;
}
