#include <iostream>
#include <type_traits>
#include <vector>
#include <cstring>

//#define NDEBUG
#include <cassert>

using namespace std;

// asserts best practices:
//  -- No side effects: shouldn't use real code as asserts can be disabled. Only vars
//  -- shouldn't handle expected run-time errors as asserts could be disabled

// exceptions best practices:
//  -- throw by value, catch by ref
//  -- rethrow with `throw;`
//  -- absorb in dtors
//  -- swap and delete operators shoudn't throw exceptions
//  -- ctor can (and sometimes must) throw in constructor. Has to do cleanup on its own.
//  -- exception safety levels
//  -- don't use exception specification (throw(...))

// Links:
// http://ideone.com/I7irGR
// http://www.gotw.ca/publications/mill22.htm
// http://www.acodersjourney.com/2016/08/top-15-c-exception-handling-mistakes-avoid/
// http://www.artima.com/intv/handcuffsP.html

void printTerminated()
{
    cout << "Terminated!" << endl;
    exit(0);
}

void printUnexpected()
{
    cout << "Unexpected!" << endl;
}

void handleUnexpected()
{
    cout << "Unexpected (h)!" << endl;
    throw;
}

void execute_static() noexcept
{
    if (noexcept(execute_static())) throw "Ha!";
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

    int f()
    {
        return 42;
    }

    int write(int, void*)
    {
        return 1;
    }
}

namespace exception_handling
{
    void foo()
    {
        throw "Error!";
    }

    void foo_int()
    {
        throw 42;
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

        virtual char const * what() const throw ()
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

        virtual char const * what() const throw ()
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

namespace exception_ctor
{
    class MyString
    {
        private:
        char* m_data;

        public:

        MyString()
            : m_data(nullptr)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        MyString(const char* data)
            : m_data(strdup(data))
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~MyString()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            cleanup();
        }

        void cleanup()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            if (m_data)
            {
                delete[] m_data;
            }
        }

        MyString(const MyString& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            // allocate and throw in case of failure
            m_data = new char[strlen(rhs.m_data)+1];

            strcpy(m_data, rhs.m_data);
            cleanup();
            throw runtime_error("Copy failed!");
        }

        MyString& operator=(const MyString& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            if (&rhs!=this)
            {
                MyString tmp(rhs);
                swap(*this, tmp);
            }

            return *this;
        }

        void swap(MyString& from, MyString& to) noexcept
        {
            using std::swap;
            swap(from.m_data, to.m_data);
        }
    };
}


namespace exception_dtor
{
    class DBConnection
    {

        public:

        DBConnection()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~DBConnection()
        {
//            try
//            {
                // close resources
                throw "From dtor!";
//            }
//            catch(const exception & ex)
//            {
//                // Log
//                cout << __PRETTY_FUNCTION__ << ex.what() << endl;
//            }
//            catch(...)
//            {
//                // Log
//                cout << __PRETTY_FUNCTION__ << ": " << "..." << endl;
//            }
        }
    };

}

namespace exception_safety
{
//    class File
//    {
//        private:
//            FILE* m_file;
//        public:
//        ~File()
//        {
//            if (m_file)
//            {
//                fclose (m_file);
//            }
//        }
//    };
}



namespace exception_motivation
{
    // W/o exceptions
    //----------------------------------------
    int foo()
    {
        return -1;
    }

    int foo_2()
    {
        return -2;
    }

    int bar()
    {
        int result = foo();
        if (0 != result)
        {
            return result;
        }
        else
        {
            //...
        }

        result = foo_2();
        if (0 != result)
        {
            return result;
        }
        else
        {
            //...
        }

        return 0;
    }

    int baz()
    {
        int result = bar();
        if (0 != result)
        {
            return result;
        }
        {
            //...
        }

        return 0;
    }

    void func()
    {
        int result = bar();
        if (-1 == result)
        {
            cout << "error: -1" << endl;
        }
        else if (-2 == result)
        {
            cout << "error: -2" << endl;
        }
    }


    // With exceptions
    //-----------------------------------------

    //int foo()
    //{
    //    throw "Error";
    //}

    //int foo_2()
    //{
    //    throw 42;
    //}

    //int bar()
    //{
    //    foo();
    //    foo_2();
    //    //...
    //}

    //int baz()
    //{
    //    bar();
    //    //...
    //}

    //void func()
    //{
    //    try
    //    {
    //        baz();
    //        // ...
    //    }
    //    catch (int value)
    //    {
    //        cout << value << endl;
    //    }
    //    catch (const char* value)
    //    {
    //        cout << value << endl;
    //    }
    //}
}


int main()
{
    {
        using namespace asserts;

        // static
        #define ASSERT_CONCAT_(a, b) a##b
        #define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
        #define ct_assert(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

        ct_assert(sizeof(int*) * 8 == 64);

        static_assert(sizeof(int*) * 8 == 64, "64-bit platform!");
        static_assert(true == 1 && false == 0, "Who is on duty today?");

        add(34, 56);
//        add(34, 0);

        // runtime
        int bytes_to_write = f();
        assert (bytes_to_write>=0 && "Something went wrong!");

        while (bytes_to_write>0)
        {
            int bytes_written = write(bytes_to_write, nullptr);
            assert (bytes_written >= 0);
            //assert ((bytes_written = write(bytes_to_write, nullptr)) >= 0);
            bytes_to_write -= bytes_written;
        }

        FILE * pFile;
        pFile = fopen ("myfile.txt","w");
        assert (pFile!=NULL);
        fputs ("test",pFile);
        fclose (pFile);
    }

    {
        // PROS:
        //  can't be silently ignored
        //  propagate automatically
        //  separate error handling and "useful" code
        //  constructor and operators error

        // CONS:
        //  performance and memory impact
        //  comlicated changes

        using namespace exception_handling;

        set_terminate(&printTerminated);

        try
        {
            foo();
            foo_int();
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
//        catch(const char* c)
//        {
//            cout << "Exception: " << c << endl;
//        }
        catch(...)
        {
            cout << "Exception: (default)" << endl;
        }
    }


    {
        using namespace exception_handling_polymorphism;

        //foo();

        try
        {
            foo();
        }
//        catch(My_Exception ex)
//        {
//            cout << "Exception (value): " << ex.what() << endl;
//        }
        // catch by ref
        catch(My_Exception& ex)
        {
            cout << "Exception (ref): " << ex.what() << endl;
        }

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
        using namespace exception_dtor;

        cout << "==== exception in dtor ===" << endl;
        try
        {
            DBConnection db_conn;
        }
        catch(...)
        {
            std::cout << "exception_dtor (catch)" << std::endl;
        }


        try
        {
            DBConnection db_conn;
            throw 42;
            //...
        }
        catch(...)
        {
            std::cout << "exception_dtor (catch-2)" << std::endl;
        }
    }


    {
        using namespace exception_ctor;

        cout << "==== exception in ctor ===" << endl;

        try
        {
            MyString s1("test string");

            MyString s2 = s1;
        }
        catch(exception& ex)
        {
            cout << "Exception: " << ex.what() << endl;
        }
    }

    try
    {
        cout << "==== unexpected exception ===" << endl;

        set_terminate(&printTerminated);
        set_unexpected(&printUnexpected);
        execute_static();
        //execute_dynamic();
    }
    catch(...)
    {
        cout << "Cought!" << endl;
    }

    cout << "Continued" << endl;
}

