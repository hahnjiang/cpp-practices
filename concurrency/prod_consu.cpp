#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class BoundedBuffer {
public:
    BoundedBuffer(const BoundedBuffer& rhs) = delete;
    BoundedBuffer& operator=(const BoundedBuffer& rhs) = delete;

    BoundedBuffer(std::size_t size)
        : begin_(0), end_(0), buffered_(0), circular_buffer_(size) {}

    void Produce(int n) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            // 等待缓冲不为满。
            not_full_cv_.wait(
                lock, [=] { return buffered_ < circular_buffer_.size(); });

            // 插入新的元素，更新下标。
            circular_buffer_[end_] = n;
            end_ = (end_ + 1) % circular_buffer_.size();

            ++buffered_;
        }  // 通知前，自动解锁。

        // 通知消费者。
        not_empty_cv_.notify_one();
    }

    int Consume() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 等待缓冲不为空。
        not_empty_cv_.wait(lock, [=] { return buffered_ > 0; });

        // 移除一个元素。
        int n = circular_buffer_[begin_];
        begin_ = (begin_ + 1) % circular_buffer_.size();

        --buffered_;

        // 通知前，手动解锁。
        lock.unlock();
        // 通知生产者。
        not_full_cv_.notify_one();
        return n;
    }

private:
    std::size_t begin_;
    std::size_t end_;
    std::size_t buffered_;
    std::vector<int> circular_buffer_;
    std::condition_variable not_full_cv_;
    std::condition_variable not_empty_cv_;
    std::mutex mutex_;
};

BoundedBuffer g_buffer(2);
boost::mutex g_io_mutex;

void Producer() {
    int n = 0;
    while (n < 100000) {
        g_buffer.Produce(n);
        if ((n % 10000) == 0) {
            std::unique_lock<std::mutex> lock(g_io_mutex);
            std::cout << "Produce: " << n << std::endl;
        }
        ++n;
    }

    g_buffer.Produce(-1);
}

void Consumer() {
    std::thread::id thread_id = std::this_thread::get_id();

    int n = 0;
    do {
        n = g_buffer.Consume();
        if ((n % 10000) == 0) {
            std::unique_lock<std::mutex> lock(g_io_mutex);
            std::cout << "Consume: " << n << " (" << thread_id << ")"
                      << std::endl;
        }
    } while (n != -1);  // -1 表示缓冲已达末尾。

    // 往缓冲里再放一个 -1，这样其他消费者才能结束。
    g_buffer.Produce(-1);
}

int main() {
    std::vector<std::thread> threads;

    threads.push_back(std::thread(&Producer));
    threads.push_back(std::thread(&Consumer));
    threads.push_back(std::thread(&Consumer));
    threads.push_back(std::thread(&Consumer));

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
