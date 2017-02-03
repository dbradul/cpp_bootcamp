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

//    static void wait_thread( void )
//    {
//        usleep(500*1000);
//    }

    static long long sum_N( unsigned long n )
    {
        long long result = 0;

        for (unsigned long i = 0; i < n; ++i )
        {
            if (i%2==0)
            {
                result += i;
            }
            //wait_thread();
        }

        cout << "Thread -1" << ": " << result << endl;

        return result;
    }

    static void* thread_func( void* vptr_args )
    {
        param* p = static_cast<param*>(vptr_args);

        p->ret_value = sum_N(p->n);

        //pthread_mutex_lock(&lock);

        cout << "Thread " << pthread_self() << ": " << p->ret_value << endl;

        //pthread_mutex_unlock(&lock);

        return p;
    }


    int run()
    {
        const int NUM_THREADS = 10;
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
                params[i] = {1, 1000*1000*100*i, 0};

//                func(1000*1000*100*i);

                if ( pthread_create( &threads[i], nullptr, thread_func, &params[i] ) )
                {
                    break;
                }
            }
        }


        for (size_t i=0; i < NUM_THREADS; ++i)
        {
            param* p_out;
            if ( pthread_join( threads[i], (void**)(&p_out)) )
            {
                break;
            }

            //cout << "result: " << p_out->ret_value << endl;
        }

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
        std::thread thread(foo, 10);
        thread.join();

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

        vector<future<long long>> futures;
        for (size_t i=0; i < 10; ++i)
        {
            future<long long> f = std::async(std::launch::async, sum_N, 1000*1000*100*i);
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
                std::lock_guard<std::mutex> L{mtx};
                intQueue.push(dist(gen));
            }

            std::cout << "Producer is done!" << std::endl;
            stopped = true;
        }};

        std::thread consumer{[&]()
        {
            while(true)
            {
                std::unique_lock<std::mutex> L{mtx};

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
            cond.notify_one();
        }

        std::cout << "Producer is done!" << std::endl;
        stopped = true;
    }};

    std::thread consumer{[&]()
    {
        while(!stopped)
        {
            std::unique_lock<std::mutex> lock{mtx};

            while (intQueue.empty())
            {
                cond.wait(lock);
            }

            const auto val = intQueue.front();
            intQueue.pop();
            std::cout << "Consumer popped: " << val << std::endl;
        }

        std::cout << "Consumer is done!" << std::endl;
    }};

    consumer.join();
    producer.join();

    std::cout << "Example Completed!" << std::endl;
}

}

int main ()
{
    {
        using namespace pthreads;
        run();
    }


    {
        using namespace cpp_threads;
        run();
    }

    {
        using namespace async;
        run();
    }

    {
        using namespace consumer_producer;
        run();
    }

    {
        using namespace consumer_producer_blocked;
        run();
    }
}
