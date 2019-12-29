#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

class WorkerThread {
public:
    void operator()() { std::cout << "Worker Thread " << std::endl; }
};

int main(void) {
    std::thread threadObj((WorkerThread()));
    // 程序会中断，因为既没有调用 join() 也没有调用 detach()
    return 0;
}
