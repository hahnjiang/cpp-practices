#include <iostream>
#include <thread>

using namespace std;

int main() {
    int threads = thread::hardware_concurrency();
    cout << threads << endl;

    return 0;
}
