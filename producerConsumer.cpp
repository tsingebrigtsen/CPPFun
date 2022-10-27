
#include <thread>
#include <deque>

#include <chrono>
using namespace std::chrono_literals;

#include <iostream>
using std::cout;
using std::endl;

std::mutex mtx;
std::condition_variable cond;

// Buffer with up to 10 elements.
std::deque<int> dec;

// Produce 10 items one by one.
void producer() {
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for( 50ms );
        std::lock_guard lock( mtx );
        dec.push_back( i );
        cout << "Notifying consumer" << endl;
        cond.notify_all();
    }
}

// Consume when ready.
void consumer() {
    for( int i = 0; i < 10; i++ ) {
        std::unique_lock uni(mtx );
        // Waits for mutex, predicate and notify.
        cond.wait( uni, [](){ return !dec.empty(); } );
        cout << "Consuming data: " << dec.front() << endl;
        dec.pop_front();
    }
}

int main(){
    std::thread thread1( producer );
    std::thread thread2( consumer );

    thread1.join();
    thread2.join();

    return 0;
}
