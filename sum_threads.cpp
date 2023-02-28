#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <string>
using namespace std;

#define NUM_THREADS 8
#define MAX 1000000

pthread_mutex_t mutx; // create a mutex variable to protect shared data access

int data[MAX]; // global array of integers
long global_sum = 0; // global sum variable to store the final result

// function that each thread will execute
void *sum(void *threadid)
{
   long tid;
   long sum = 0;
   tid = (long)threadid;
   
   // determine the range of the array that this thread will sum
   int range = MAX/NUM_THREADS;
   int start = tid * range;
   int end = start + range;

   // sum the elements in the range assigned to this thread
   for(int i = start; i < end; i++) {
        sum += data[i];
   }

   // enforce sequential update of the global sum variable using a mutex
   pthread_mutex_lock(&mutx);
   global_sum += sum;
   pthread_mutex_unlock(&mutx);

   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{ 
    int cores = std::thread::hardware_concurrency(); // get the number of available CPU cores
    pthread_mutex_init(&mutx, NULL); // initialize the mutex variable

    cout << "The number of cores on this machine = " << cores << endl;

    // fill the data array with random integers
    for(int i = 0; i < MAX; i++) {
       data[i] = rand() % 20;
    }

    pthread_t threads[NUM_THREADS]; // create an array of thread identifiers
    long tid = 0;

    // create the threads
    for(tid = 0; tid < NUM_THREADS; tid++) {
       pthread_create(&threads[tid], NULL, sum, (void *)tid);
    }

    // wait for the threads to complete
    for(tid = 0; tid < NUM_THREADS; tid++) {
       pthread_join(threads[tid], NULL);
    }

    cout << "The final sum = " << global_sum << endl;
}
