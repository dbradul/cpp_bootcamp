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

#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <unistd.h>


using namespace std;

namespace pthreads
{
    typedef struct {
        char c;
        long n;
        long ret_value;
    } param;

    static void wait_thread( void )
    {
        usleep(500*1000);
    }

    static void* thread_func( void* vptr_args )
    {
        unsigned i;

        param* p = static_cast<param*>(vptr_args);

        for( i = 0; i < 10; ++i )
        {
            cout << p->n;
            wait_thread();
        }

        p->ret_value = p->c * p->n;

        return p;
    }


    int run()
    {
        const int NUM_THREADS = 10;
        pthread_t threads[NUM_THREADS];
        param params[NUM_THREADS];

        unsigned i;

        for (i=0; i < NUM_THREADS; ++i)
        {
            params[i] = {'a' + i, i};

            if ( pthread_create( &threads[i], nullptr, thread_func, &params[i] ) )
            {
                break;
            }
        }

        for (i=0; i < NUM_THREADS; ++i)
        {
            param* p_out;
            if ( pthread_join( threads[i], (void**)(&p_out)) )
            {
                break;
            }

            cout << "result: " << p_out->ret_value << endl;
        }

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

    int run()
    {
        auto f = std::async(std::launch::async, square, 8);
        std::cout << "square currently running\n"; //do something while square is running
        std::cout << "result is " << f.get() << '\n'; //getting the result from square
    }
}

namespace consumer_producer {

    void run()
    {
        std::condition_variable cond;
        std::mutex mtx;
        std::queue<int> intq;
        bool stopped = false;

        std::thread producer{[&]()
        {
            // Prepare a random number generator.
            // Our producer will simply push random numbers to intq.
            //
            std::default_random_engine gen{};
            std::uniform_int_distribution<int> dist{};

            std::size_t count = 4006;
            while(count--)
            {
                // Always lock before changing
                // state guarded by a mutex and
                // condition_variable (a.k.a. "condvar").
                std::lock_guard<std::mutex> L{mtx};

                // Push a random int into the queue
                intq.push(dist(gen));

                // Tell the consumer it has an int
                cond.notify_one();
            }

            // All done.
            // Acquire the lock, set the stopped flag,
            // then inform the consumer.
            std::lock_guard<std::mutex> L{mtx};

            std::cout << "Producer is done!" << std::endl;

            stopped = true;
            cond.notify_one();
        }};

        std::thread consumer{[&]()
        {
            do{
                std::unique_lock<std::mutex> L{mtx};
                cond.wait(L,[&]()
                {
                    // Acquire the lock only if
                    // we've stopped or the queue
                    // isn't empty
                    return stopped || ! intq.empty();
                });

                // We own the mutex here; pop the queue
                // until it empties out.

                while( ! intq.empty())
                {
                    const auto val = intq.front();
                    intq.pop();

                    std::cout << "Consumer popped: " << val << std::endl;
                }

                if(stopped){
                    // producer has signaled a stop
                    std::cout << "Consumer is done!" << std::endl;
                    break;
                }

            }while(true);
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
}
