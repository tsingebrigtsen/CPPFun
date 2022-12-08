// Inspired by Bill Weinman's C++20 book.

// std::thread
#include <thread>
// std::queue
#include <queue>
// Simulates work.
#include <chrono>
using namespace std::chrono_literals;
// std::cout, endl
#include <iostream>
// std::condition_variable
#include <condition_variable>

namespace tsi{
    // Parameters.
    bool doneProducing = false;
    constexpr auto timeout = 100ms;
    constexpr int bufferSize = 3;
    int itemsToProduce = 20;

    std::mutex mtxProducerConsumer;
    std::condition_variable producerCondition;
    std::condition_variable consumerCondition;

    // Non-thread-safe buffer with up to bufferSize elements.
    std::queue<int> buffer;

    // Producer thread.
    void producer(const int& id){
        while(itemsToProduce){
            // Simulates work.
            std::this_thread::sleep_for(50ms);
            // Locks the mutex.
            std::unique_lock uni(mtxProducerConsumer);
            // Wait for buffer to have space.
            consumerCondition.wait(uni, [&]() { return buffer.size() < bufferSize; });

            // Add item to buffer.
            if(buffer.size() < bufferSize && itemsToProduce > 0){
                buffer.push(itemsToProduce);
                std::cout << "Thread: " << id << ". Notify consumers with item " << itemsToProduce << std::endl;
                itemsToProduce--;
                producerCondition.notify_all();
            }
        }

        // Wait for all consumers to empty the buffer before setting doneProducing.
        std::unique_lock uni(mtxProducerConsumer);
        consumerCondition.wait_for(uni, timeout, []() { return buffer.empty(); });
        std::cout << "Thread: " << id << ". I am done producing." << std::endl;
        doneProducing = true;
    }

    // Consumer thread.
    void consumer(const int& id){
        while(!doneProducing){
            std::unique_lock uni(mtxProducerConsumer);
            producerCondition.wait_for(uni, timeout, []() { return !buffer.empty(); });

            // Simulates work.
            std::this_thread::sleep_for(250ms);

            if (!buffer.empty()){
                std::cout << "Thread: " << id << ". Consuming data: " << buffer.front() << std::endl;
                buffer.pop();
                consumerCondition.notify_all();
            }
        }
        std::cout << "Thread: " << id << ". I am done consuming." << std::endl;
    }
}

// 2 producers and 3 consumers.
int main(){
    std::thread thread0(tsi::producer, 0);
    std::thread thread1(tsi::producer, 1);
    std::thread thread2(tsi::consumer, 2);
    std::thread thread3(tsi::consumer, 3);
    std::thread thread4(tsi::consumer, 4);

    // Wait for completion.
    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    std::cout << "Program done." << std::endl;
    return 0;
}