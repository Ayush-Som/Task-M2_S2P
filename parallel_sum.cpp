#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#define NUM_THREADS 8
#define MAX 1000000

int data[MAX];
long global_sum = 0;

void sum(int tid, int start, int end)
{
    long local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += data[i];
    }
    global_sum += local_sum;
}

int main(int argc, char *argv[])
{ 
    int cores = std::thread::hardware_concurrency();
    std::cout << "The number of cores on this machine = " << cores << std::endl;

    // Initialize data array
    for (int i = 0; i < MAX; i++) {
        data[i] = rand() % 20;
    }

    // Create threads and partition work
    std::vector<std::thread> threads;
    int range = MAX / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        int start = i * range;
        int end = (i == NUM_THREADS - 1) ? MAX : start + range;
        threads.emplace_back(sum, i, start, end);
    }

    // Join threads
    for (std::thread& t : threads) {
        t.join();
    }

    std::cout << "The final sum = " << global_sum << std::endl;

    return 0;
}
