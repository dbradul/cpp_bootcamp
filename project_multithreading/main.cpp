#include <iostream>
#include <thread>
#include <future>
#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <algorithm>
#include <chrono>
#include <ostream>
#include <sstream>

#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <unistd.h>


using namespace std;
using namespace std::chrono;

struct profiler
{
    steady_clock::time_point start = steady_clock::now();

    ~profiler()
    {
        cout << duration_cast<milliseconds>
                (steady_clock::now() - start).count() << " ms" << endl;
    }
};

namespace pthreads
{
    typedef struct {
        char c;
        unsigned long n;
        unsigned long ret_value;
    } param;

    pthread_mutex_t lock;
    string out_result;

    static void wait_thread( void )
    {
        usleep(500);
    }

    static long long sum_N( unsigned long n )
    {
        long long result = 0;

        for (unsigned long i = 0; i < n; ++i )
        {
            if (i%2==0)
            {
                result += i;
            }

        }

        //cout << "Thread -1" << ": " << result << endl;

        return result;
    }

    static void* thread_func( void* vptr_args )
    {
        param* p = static_cast<param*>(vptr_args);

        p->ret_value = sum_N(p->n);

        pthread_mutex_lock(&lock);
        //wait_thread();

//        ostringstream out_result;
//        out_result << "Thread ";
//        out_result << pthread_self();
//        out_result << ": " << p->ret_value;
//        out_result << "\n";
        cout << "Thread ";
        cout << pthread_self();
        cout << ": " << p->ret_value;
        cout << endl;

//        cout << out_result.str();

        pthread_mutex_unlock(&lock);

        return p;
    }

    static void* thread_func_2( void* vptr_args )
    {
        //wait_thread();

        int* p = static_cast<int*>(vptr_args);

//        pthread_mutex_lock(&lock);



        if (*p == 0)
        {
            out_result = "Hillary";
            out_result += " ";
            out_result += "Clinton";
            out_result += "\n";
        }
        else
        {
            out_result = "Donald";
            out_result += " ";
            out_result += "Trump";
            out_result += "\n";
        }
        cout << out_result << endl;


//        pthread_mutex_unlock(&lock);

        return nullptr;
    }


    int run()
    {
        const int NUM_THREADS = 20;
        pthread_t threads[NUM_THREADS];
        param params[NUM_THREADS];


        if (pthread_mutex_init(&lock, NULL) != 0)
        {
            printf("\n mutex init failed\n");
            return 1;
        }

        {
            profiler p;

            for (size_t i=0; i < NUM_THREADS; ++i)
            {
                params[i] = {1, 1000*1000*10*i, 0};

                ////sum_N(1000*1000*10*i);

                if ( pthread_create( &threads[i], nullptr, thread_func, &params[i] ) )
                {
                    break;
                }
            }

            for (size_t i=0; i < NUM_THREADS; ++i)
            {
                param* p_out;
                if ( pthread_join( threads[i], (void**)(&p_out)) )
                {
                    break;
                }
            }
        }


//        // race conditions
//        {
////            if (pthread_mutex_init(&lock, NULL) != 0)
////            {
////                printf("\n mutex init failed\n");
////                return 1;
////            }

//            for (size_t i=0; i < NUM_THREADS; ++i)
//            {
//                int* param = new int(i%2);
//                if ( pthread_create( &threads[i], nullptr, thread_func_2, param ) )
//                {
//                    break;
//                }
//            }

//            for (size_t i=0; i < NUM_THREADS; ++i)
//            {
//                if ( pthread_join( threads[i], nullptr) )
//                {
//                    break;
//                }
//            }

//        }

        pthread_mutex_destroy(&lock);

        return 0;
    }

}


namespace cpp_threads
{

    class Bar
    {
    public:
        void operator()(int a)
        {
            std::cout << a << '\n';
        }

        void foo(int a)
        {
            std::cout << a << '\n';
        }
    };

    void foo(int a)
    {
        std::cout << a << '\n';
    }

    void foo2(int& b)
    {
        b = 123;
    }

    int run()
    {
        int pInt = 42;
        double d = 42.;
//        std::thread thread(foo, 10, &pInt);
//        thread.join();

        std::thread thread2(Bar(), 10);
        thread2.join();

        Bar bar;
        std::thread thread3(&Bar::foo, &bar, 10);
        thread3.join();

        auto lambda = [](int a) { std::cout << a << '\n'; };
        std::thread thread4(lambda, 42);
        thread4.join();

        int a = 1;
        std::thread thread5{ foo2, std::ref(a) }; //'a' is now really passed as reference
        thread5.join();
        std::cout << a << '\n'; //Outputs 10

        // Create and execute the thread
        vector<std::thread> v_th;

        for(int i = 0; i<100; ++i)
        {
            v_th.emplace_back(foo, i ); // Pass 10 to free function
            //thread.join();
        }

        for_each(v_th.begin(), v_th.end(), [](std::thread& th)
        {
            if (th.joinable())
            {
                th.join();
            }
        });

        return 0;
    }
}


namespace async
{
    unsigned int square(unsigned int i)
    {
        return i*i;
    }

    long long sum_N(unsigned long n)
    {
        long long result = 0;

        for (unsigned long i = 0; i < n; ++i )
        {
            if (i%2==0)
            {
                result += i;
            }
        }

        return result;
    }

    int run()
    {
        auto f = std::async(std::launch::async, square, 8);
        std::cout << "square currently running\n"; //do something while square is running
        std::cout << "result is " << f.get() << '\n'; //getting the result from square

        profiler p;

        vector<future<long long>> futures;
        for (size_t i=0; i < 20; ++i)
        {
            future<long long> f = std::async(std::launch::async, sum_N, 1000*1000*10*i);
            futures.push_back(move(f));
        }

        while (futures.size()>0)
        {
            cout << "Result: " << futures.back().get() << endl;
            futures.pop_back();
        }
    }
}

namespace consumer_producer {

    void run()
    {
        std::mutex mtx;
        std::queue<int> intQueue;
        bool stopped = false;

        std::thread producer{[&]()
        {
            // Prepare a random number generator.
            std::default_random_engine gen{};
            std::uniform_int_distribution<int> dist{1, 100};

            std::size_t count = 4096;
            while(count--)
            {
                ////std::lock_guard<std::mutex> L{mtx};
                int val = dist(gen);
                intQueue.push(val);
                std::cout << "Producer pushed: " << val << std::endl;
            }

            std::cout << "Producer is done!" << std::endl;
            stopped = true;
        }};

        std::thread consumer{[&]()
        {
            while(true)
            {
                ///std::unique_lock<std::mutex> L{mtx};

                const auto val = intQueue.front();
                intQueue.pop();

                std::cout << "Consumer popped: " << val << std::endl;

                if(stopped){
                    // producer has signaled a stop
                    std::cout << "Consumer is done!" << std::endl;
                    break;
                }
            }
        }};

        consumer.join();
        producer.join();

        std::cout << "Completed!" << std::endl;
    }
}


namespace consumer_producer_blocked {

void run()
{
    std::condition_variable cond;
    std::mutex mtx;
    std::queue<int> intQueue;
    bool stopped = false;

    std::thread producer{[&]()
    {
        std::default_random_engine gen{};
        std::uniform_int_distribution<int> dist{1, 100};

        std::size_t count = 4096;
        while(count--)
        {
            std::lock_guard<std::mutex> L{mtx};
            intQueue.push(dist(gen));
            cond.notify_all();
        }

        std::cout << "Producer is done!" << std::endl;
        stopped = true;
    }};

    std::thread consumer([&]()
    {
        while(!stopped)
        {
            std::unique_lock<std::mutex> lock(mtx);

            while (intQueue.empty())
            {
                cond.wait(lock);
            }

            const auto val = intQueue.front();
            intQueue.pop();
            std::cout << "Consumer popped: " << val << std::endl;
        }

        std::cout << "Consumer is done!" << std::endl;
    });

    consumer.join();
    producer.join();

    std::cout << "Example Completed!" << std::endl;
}

}



namespace deadlocks {


std::mutex mutex_printer, mutex_HDD;

void prepare()
{
    std::chrono::milliseconds timeout(100);
    std::this_thread::sleep_for(timeout);
}

void task_a ()
{
    prepare();

    mutex_printer.lock();
    mutex_HDD.lock();
    // replaced by:
//    std::lock (mutex_printer, mutex_HDD);

    // print all files checksums to a printer
    std::cout << "task a\n";

    mutex_printer.unlock();
    mutex_HDD.unlock();
}

void task_b ()
{
    prepare();

    mutex_printer.lock();
    mutex_HDD.lock();
    // replaced by:
//    std::lock (mutex_HDD, mutex_printer);

    // print all files checksums to a printer
    std::cout << "task b\n";

    mutex_printer.unlock();
    mutex_HDD.unlock();
}

class Buffer
{
    std::mutex buff_lock;

public:
//    void write()
//    {
//        std::lock_guard<std::mutex> lock(buff_lock);

//        //...
////        write_unsafe();
//        is_full();
//    }

    int read()
    {
        std::lock_guard<std::mutex> lock(buff_lock);

        return read_unsafe();
//        if (is_empty())
//        {
//            throw out_of_range("Empty");
//        }

//        return 42;
    }

    int read_unsafe()
    {
        if (is_empty_unsafe())
        {
            throw out_of_range("Empty");
        }

        //.....

        return 42;
    }



    bool is_empty()
    {
        std::lock_guard<std::mutex> lock(buff_lock);

        return is_empty_unsafe();
    }


    bool is_empty_unsafe()
    {

        return false;
        //...
    }





};

void run()
{
    // First job
    // DVD/wifi/modem drivers from DVD/internet
    // OS popularity depends on apps available, which depends on OS popularity
    // Dining philosofers

    // recursive_lock
    // example with cycles
    // how to solve

//    {
//        Buffer buff;

//        buff.read();
//    }

//    for(size_t i = 0; i<100; ++i)
//    {
//        std::thread th1 (task_a);
//        std::thread th2 (task_b);

//        th1.join();
//        th2.join();
//    }
}

}

int main ()
{

//    unsigned int n = std::thread::hardware_concurrency();
//    std::cout << n << " concurrent threads are supported.\n";


//    {
//        using namespace pthreads;
//        run();
//    }


//    {
//        using namespace cpp_threads;
//        run();
//    }

//    {
//        using namespace async;
//        run();
//    }

    {
        using namespace consumer_producer;
        run();
    }

//    {
//        using namespace consumer_producer_blocked;
//        run();
//    }

    // Notes:
    // https://bugreports.qt.io/browse/QTCREATORBUG-13791

    {
        using namespace deadlocks;
        run();
    }
}
