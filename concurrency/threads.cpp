#include <iostream>
#include <thread>

using namespace std;

void foo() {
    for (int i = 0; i < 10; i++) {
        cout << "foo: " << i << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void bar(int x) {
    for (int i = x; i < x + 10; i++) {
        cout << "bar: " << i << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    std::thread first(foo);
    std::thread second(bar, 100);

    std::cout << "main, foo and bar now execute concurrently...\n";

    // synchronize threads:
    first.join();   // pauses until first finishes
    second.join();  // pauses until second finishes

    cout << "foo and bar completed.\n";

    return 0;
}
