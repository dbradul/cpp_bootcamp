#include <iostream>
#include <type_traits>
#include <vector>
#include <map>
#include <cstring>
#include <memory>

//#define NDEBUG
#include <cassert>

using namespace std;

// asserts best practices:
//  -- Used for problems that should be fixed, not handled
//  -- No side effects: shouldn't use real code as asserts can be disabled. Only vars
//  -- shouldn't handle expected run-time errors as asserts could be disabled
//  -- Run both debug and release builds

// exceptions best practices:
//  -- throw by value, catch by ref (avoid copying and memory allocation)
//  -- don't throw basic types (not informative)
//  -- rethrow with `throw;`
//  -- absorb in dtors
//  -- swap and delete operators shoudn't throw exceptions
//  -- ctor can (and sometimes must) throw in constructor. Has to do cleanup on its own.
//  -- exception safety levels
//  -- don't use exception specification (throw()), use noexcept judishiously (move)

// Links:
// http://ideone.com/I7irGR
// http://www.gotw.ca/publications/mill22.htm
// http://www.acodersjourney.com/2016/08/top-15-c-exception-handling-mistakes-avoid/
// http://www.artima.com/intv/handcuffsP.html
// http://www.stroustrup.com/except.pdf
// http://bit.ly/2jUtE8N

void printTerminated()
{
    cout << "Terminated!" << endl;
    exit(0);
}

void printUnexpected()
{
    cout << "Unexpected!" << endl;
}

void execute_static() noexcept
{
    //if (noexcept(execute_static()))
        throw "Ha!";
}

void execute_dynamic() throw()
{
    throw "Ha!";
}

namespace asserts
{
    template <typename T1, typename T2>
    T1 add(T1 t1, T2 t2)
    {
       static_assert(std::is_integral<T1>::value, "Type T1 must be integral");
       static_assert(std::is_integral<T2>::value, "Type T2 must be integral");

       return t1 + t2;
    }
}

namespace exception_handling_implicit_casting
{
    void foo()
    {
        throw "Error!";
    }

    void foo_int()
    {
        throw 42;
    }

    void foo_double()
    {
        throw 42.;
    }

    void foo_string()
    {
        throw "42";
    }
}

namespace exception_handling_polymorphism
{
    class My_Exception : public std::exception
    {
        public:

        My_Exception() : exception()
            {cout << __PRETTY_FUNCTION__ << endl;}
        My_Exception(const My_Exception& d) : exception(d)
            {cout << __PRETTY_FUNCTION__ << endl;}

        char const * what() const throw () override
        {
            cout << __PRETTY_FUNCTION__ << endl;
            return "Something bad happend (base).";
        }
    };

    class My_Exception_derived : public My_Exception
    {
        public:
        My_Exception_derived() : My_Exception()
            {cout << __PRETTY_FUNCTION__ << endl;}
        My_Exception_derived(const My_Exception_derived& d) : My_Exception(d)
            {cout << __PRETTY_FUNCTION__ << endl;}

        char const * what() const throw () override
        {
            cout << __PRETTY_FUNCTION__ << endl;
            return "Something bad happend (derived).";
        }
    };

    void foo()
    {
        throw My_Exception_derived();
    }
}


namespace status_code
{
    enum ErrCode
    {
        OK = 0,
        ERR_OUT_OF_MEMORY,
        ERR_DEVICE_IS_BUSY
    };

    // W/o exceptions
    //----------------------------------------
    ErrCode foo()
    {
        return ERR_OUT_OF_MEMORY;
    }

    ErrCode foo_2()
    {
        return ERR_DEVICE_IS_BUSY;
    }

    ErrCode bar()
    {
        ErrCode result = foo();

        if (OK != result)
        {
            return result;
        }
        else if (OK != (result = foo_2()))
        {
            return result;
        }
        else
        {
            //...
        }

        return result;
    }

    ErrCode baz()
    {
        ErrCode result = bar();

        if (OK != result)
        {
            return result;
        }
        {
            //...
        }

        return result;
    }

    void func()
    {
        ErrCode result = bar();

        if (ERR_OUT_OF_MEMORY == result)
        {
            cout << "error: out of memory" << endl;
        }
        else if (ERR_DEVICE_IS_BUSY == result)
        {
            cout << "error: device is busy" << endl;
        }
        else
        {
            //...
        }
    }
}


namespace exception_handling
{
    // With exceptions
    //-----------------------------------------
    int foo()
    {
        throw bad_alloc();
    }

    int foo_2()
    {
        throw runtime_error("Device is busy");
    }

    int bar()
    {
        foo();
        foo_2();
        //...
    }

    int baz()
    {
        bar();
        //...
    }

    void func()
    {
        try
        {
            baz();
            // ...
        }
        catch (bad_alloc& ex)
        {
            cout << ex.what() << endl;
        }
        catch (runtime_error& ex)
        {
            cout << ex.what() << endl;
        }
        catch(...)
        {
            cout << "Unknown error: something is not quite right." << endl;
        }
    }
}


namespace exception_handling_stack_unwinding
{

    struct A
    {
         A() { cout << __PRETTY_FUNCTION__ << endl; }
        ~A() { cout << __PRETTY_FUNCTION__ << endl; }
    };

    struct B
    {
         B() { cout << __PRETTY_FUNCTION__ << endl; }
        ~B() { cout << __PRETTY_FUNCTION__ << endl; }
    };

    struct C
    {
         C() { cout << __PRETTY_FUNCTION__ << endl; }
        ~C() { cout << __PRETTY_FUNCTION__ << endl; }
    };

    struct D
    {
         D() { cout << __PRETTY_FUNCTION__ << endl; }
        ~D() { cout << __PRETTY_FUNCTION__ << endl; }
    };

    struct E
    {
         E() { cout << __PRETTY_FUNCTION__ << endl; }
        ~E() { cout << __PRETTY_FUNCTION__ << endl; }
    };

    struct F
    {
         F() { cout << __PRETTY_FUNCTION__ << endl; }
        ~F() { cout << __PRETTY_FUNCTION__ << endl; }
    };


    void foo () {
        D d;
        E e;
        throw 42;
        F f;
    }

    void bar ()
    {
        A a;
        try
        {
            B b;
            foo ();
            C c;
        }
        catch (const int& i)
        {
            throw i;
        }
    }
}

namespace exception_handling_ctor_dtor
{

    class String
    {
    private:
        char* m_buffer          = nullptr;
        size_t* m_refCounter    = nullptr;

        void cleanup()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            delete [] m_buffer;
            delete m_refCounter;
            m_buffer = nullptr;
            m_buffer = nullptr;
        }

        void attach()
        {
            (*m_refCounter)++;
        }

        void detach()
        {
            if ((*m_refCounter) > 0)
            {
                if (--(*m_refCounter) == 0)
                {
                    cleanup();
                }
            }

            assert((*m_refCounter) >= 0);
        }

    public:

        String()
            : m_buffer(nullptr)
            , m_refCounter(new size_t(0))
        {
            cout << __PRETTY_FUNCTION__ << endl;
            attach();
        }

        String(const char* value)
        try
            : m_buffer(new char[strlen(value)+1])
            , m_refCounter(new size_t(0))
        {
            cout << __PRETTY_FUNCTION__ << endl;
            strcpy(m_buffer, value);
            attach();
        }
        catch(const bad_alloc& ex)
        {
            cout << "Out of memory exception" << endl;
            cleanup();
        }

        String(const String& rhs)
            : m_buffer(rhs.m_buffer)
            , m_refCounter(rhs.m_refCounter)
        {
            attach();
        }

        String& operator= (const String& rhs)
        {
            if (&rhs != this)
            {
                String tmp(rhs);
                swap(tmp);
            }
            return *this;
        }

        String& operator= (const char* value)
        {
            String(value).swap(*this);
            return *this;
        }

        ~String()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            detach();
        }

        void swap(String& other)
        {
            std::swap(m_buffer,     other.m_buffer);
            std::swap(m_refCounter, other.m_refCounter);
        }

        char& operator[](size_t index)
        {
            return m_buffer[index];
        }

        const char& operator[](size_t index) const
        {
            return m_buffer[index];
        }

        size_t count() const
        {
            return *m_refCounter;
        }

        void set_elem(size_t index, char value)
        {
            if (count()>1)
            {
                String tmp(m_buffer);
                std::swap(*this, tmp);
            }
            m_buffer[index] = value;
        }

        const char *data()const
        {
            return m_buffer;
        }
    };


    template <class T>
    class COWPtr
    {
    private:
        shared_ptr<T> m_sp;

        void copy()
        {
            T* tmp = m_sp.get();

            if( !( tmp == 0 || m_sp.use_count() == 1 ) )
            {
                m_sp = make_shared<T>(*tmp);
            }
        }

    public:
        COWPtr(T* t) : m_sp(t) {}

        COWPtr(const shared_ptr<T>& refptr) : m_sp(refptr) {}

        COWPtr(const COWPtr& rhs) : m_sp(rhs.m_sp) {}

        COWPtr(COWPtr&& rhs) : m_sp(move(rhs.m_sp)) {}

        COWPtr& operator=(const COWPtr& rhs)
        {
            COWPtr(rhs).swap(*this);
            return *this;
        }

        COWPtr& operator=(COWPtr&& rhs)
        {
            COWPtr(move(rhs)).swap(*this);
            return *this;
        }

        void swap(COWPtr& lhs, COWPtr& rhs)
        {
            ::swap(lhs.m_sp, rhs.m_sp);
        }

        T& operator*()
        {
            copy();
            return *m_sp;
        }

        T* operator->()
        {
            copy();
            return m_sp.operator->();
        }

        const T& operator*() const
        {
            return *m_sp;
        }

        const T* operator->() const
        {
            return m_sp.operator->();
        }
    };


    struct DB
    {
        DB()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~DB()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        void open(const string& url)
        {

        }

        void close()
        {
            throw runtime_error("DB file is corrupted");
        }
    };


    class DBConnection
    {
        DB* m_pDB;

        public:

        DBConnection()
        {
        }

        DBConnection(const string& url)
            : m_pDB(new DB())
        {
            cout << __PRETTY_FUNCTION__ << endl;

            if (url.empty())
            {
                //delete m_pDB;
                throw runtime_error("URL is empty");
            }
        }

        ~DBConnection()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            try
            {
                m_pDB->close();
                delete m_pDB;
            }
            catch(const exception & ex)
            {
                // Log
                cout << "Fatal error: " << ex.what() << endl;
            }
            catch(...)
            {
                // Log
                cout << "Fatal error: " << ": " << "..." << endl;
            }
        }
    };
}

namespace exception_safety_levels {

    struct Box
    {
        int value;

        Box()
            : value(0)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Box(int value_)
            : value(value_)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Box(const Box& obj)
            : value(obj.value)
        {
            //throw runtime_error("Failed copy");
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Box(Box&& obj) noexcept
            : value(move(obj.value))
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Box& operator=(const Box& obj)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            value = obj.value;
            return *this;
        }

        Box& operator=(Box&& obj)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            value = move(obj.value);
            return *this;
        }
    };

    ostream& operator<< (ostream& os, const Box& box)
    {
        os << "{";
        os << "value: " << box.value;
        os << "}";
    }

    template<typename T>
    ostream& operator<< (ostream& os, const vector<T>& rhs)
    {
        os << rhs.size() << ", " << rhs.capacity() << ": { ";
        for (const T& elem : rhs)
        {
            os << (&elem!=&rhs[0] ? ", " : "");
            os << elem;
        }
        os << " }";
        return os;
    }
}

int main()
{
    {
        using namespace asserts;

        // static
        #define ASSERT_CONCAT_(a, b) a##b
        #define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
        #define my_static_assert(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

        my_static_assert(sizeof(int*) * 8 == 64);

        static_assert(sizeof(int*) * 8 == 64, "Should only compile on 64-bit platform!");
        static_assert(true == 1 && false == 0, "What?");

        add(34, 56);
//        add(34, 0);

        // runtime
//        size_t bytes_to_write = 1024;
//        while (bytes_to_write>0)
//        {
//            int bytes_written = write(fromBuffer, toBuffer, bytes_to_write);
//            assert (bytes_to_write >= bytes_written);
//            bytes_to_write -= bytes_written;
//        }

        // what's wrong?
        FILE * pFile;
        pFile = fopen ("myfile.txt","w");
        assert (pFile!=NULL);
        fputs ("test",pFile);
        fclose (pFile);
    }

    {
        using namespace status_code;

        func();

        // PROS:
        // simple
        // can easily transfer over libs/systems/languages bounaderies

        // CONS:
        // can be ignored
        // mix actual and error handling code
        // chain from source and destinations can be easily broken (<= ignorance)
    }

    {
        using namespace exception_handling;

        func();

        // PROS:
        //  can't be silently ignored
        //  propagate automatically
        //  separate error handling and "useful" code
        //  constructor and operators error

        // CONS:
        //  binary size impact
        //  not easy to reason
    }

    {
        using namespace exception_handling_implicit_casting;


        // exceptions don't support implcit casting, but support inheritance
        try
        {
            foo();
            foo_int();
            foo_double();
            foo_string();
        }
        catch(int i)
        {
            cout << "Exception: " << i << endl;
        }
        catch(int& i)
        {
            cout << "Exception: " << i << endl;
        }
        catch(long i)
        {
            cout << "Exception: " << i << endl;
        }
        catch(double d)
        {
            cout << "Exception: " << d << endl;
        }
        catch(const char* c)
        {
            cout << "Exception: " << c << endl;
        }
        catch(string s)
        {
            cout << "Exception: " << s << endl;
        }
        catch(const string& s)
        {
            cout << "Exception: " << s << endl;
        }
        catch(...)
        {
            cout << "Exception: (default)" << endl;
        }
    }


    {
        using namespace exception_handling_stack_unwinding;

        //set_terminate(&printTerminated);

        ////bar();
    }



    {
        using namespace exception_handling_ctor_dtor;


        cout << "==== exception in ctor ===" << endl;

        try
        {
            string url;// = "127.0.0.1:8080;storage.sch;admin";
            DBConnection db_conn (url);

            cout << "Is about to call ~DBConnetion()\n";
        }
        catch(exception& ex)
        {
            cout << "Exception (DB): " << ex.what() << endl;
        }
        cout << "Passed through\n";

//        try
//        {
//            DBConnection db_conn;

//            cout << "Throwing 42...\n";
//            throw 42;
//            //...
//        }
//        catch(...)
//        {
//            std::cout << "exception_dtor (catch)" << std::endl;
//        }
//        cout << "Passed through\n";

    }

    {
        using namespace exception_handling_polymorphism;

        //foo();

        try
        {
            foo();
        }
        catch(My_Exception& ex)
        {
            cout << "Exception (ref, base): " << ex.what() << endl;
        }
        catch(My_Exception_derived& ex)
        {
            cout << "Exception (ref, derived): " << ex.what() << endl;
        }
        //return 0;


        try
        {
            foo();
        }
        catch(My_Exception ex)
        {
            cout << "Exception (value): " << ex.what() << endl;
        }
        // catch by ref
        catch(My_Exception& ex)
        {
            cout << "Exception (ref): " << ex.what() << endl;
        }

        // rethrow
        cout << "==== rethrow ===" << endl;
        try
        {
            try
            {
                foo();
            }
            catch(My_Exception& ex)
            {
                cout << "Exception: " << ex.what() << endl;
                throw /*ex*/; // don't specify param to avoid slicing
            }
        }
        catch(My_Exception& ex)
        {
            cout << "Exception (rethrown): " << ex.what() << endl;
        }
    }

    {
        cout << "==== terminated ===" << endl;

        set_terminate(&printTerminated);

        //throw runtime_error("xxx");
    }

    try
    {
        cout << "==== unexpected exception (USE) ===" << endl;

        //execute_static();
    }
    catch(...)
    {
        cout << "Cought!" << endl;
    }

//    try
//    {
//        cout << "==== unexpected exception (DON'T USE) ===" << endl;
//        set_unexpected(&printUnexpected);
//        execute_dynamic();
//    }
//    catch(...)
//    {
//        cout << "Cought!" << endl;
//    }

    // exception safety levels
    {
        using namespace  exception_safety_levels;
        // No-trow: swap, dtor, noexcept
        // Strong: copy before modification => copy-n-swap, RAII
        // Basic : keep invariant (changed, but valid) => RAII
        // No-guarantee: avoid!!!

        vector<Box> v;// = {{1}, {2}, {3}};
        v.reserve(4);
        v.emplace_back(1);
        v.emplace_back(2);
        v.emplace_back(3);

        try
        {
            Box box{4};
            v.push_back({4});
//            v.push_back({5});
//            v.push_back({6});
//            v.push_back({7});
//            v.push_back({8});
        }
        catch(exception& ex)
        {
            cout << ex.what() << endl;
        }

        cout << v << endl;

        // what about vector::pop_back and back
    }

    // strategies
    {
        //  Report: Report an error (e.g., write throw) wherever a function detects an error that it cannot resolve itself and that makes it impossible for the function to continue execution.
        //  Handle: Handle an error (e.g., write a catch that doesn't rethrow the same or another exception or emit another kind of error code) in the places that have sufficient knowledge to handle the error.
        //  Absorb: Absorb errors in the bodies of destructors and deallocation operations.
        //  Translate: Translate an error (e.g., write a catch that does rethrow a different exception or emits another kind of error code) on layer boundaries.
        //  Propagate: In the rest of cases.

        //  Fail fast: terminate loudly when don't know how to handle and there is no upper levels to report to.
        //             Otherwise, the system will work into inconsistent state, which may disastrous consequences (corrupted data, incorrect work, etc).
    }

    cout << "PASSED" << endl;
}

