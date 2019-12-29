#include <iostream>
#include <thread>
#include <vector>

class ThreadRAII {
    std::thread &m_thread;

public:
    ThreadRAII(std::thread &threadObj) : m_thread(threadObj) {}
    ~ThreadRAII() {
        // 假如 joinable 则调用 detach
        if (m_thread.joinable()) {
            m_thread.detach();
        }
    }
};

void thread_function() {
    for (int i = 0; i < 10000; i++) {
        std::cout << "thread_function executing" << std::endl;
    }
}

int main(void) {
    std::thread threadObj(thread_function);

    // 注释掉这行，程序会 crash
    // ThreadRAII wrapperObj(threadObj);
    return 0;
}
