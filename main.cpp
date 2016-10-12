//#include <cstdlib>
#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

void foo(int)
{
    cout << __PRETTY_FUNCTION__ << endl;
}


void foo(unsigned int)
{
    cout << __PRETTY_FUNCTION__ << endl;
}

void foo(float)
{
    cout << __PRETTY_FUNCTION__ << endl;
}


void foo(double)
{
    cout << __PRETTY_FUNCTION__ << endl;
}


bool double_equals(double a, double b, double epsilon = 0.001)
{
    return std::abs(a-b) < epsilon;
}

bool double_equals_sys(double a, double b, double epsilon = 0.001)
{
    return std::abs(a-b) < std::numeric_limits<double>::epsilon() * max(abs(a), abs(b));
}


int main()
{
    std::cout << "Hello world!" << std::endl;

    cout << "==========================" << endl;

    std::cout << sizeof(char) << endl;
    std::cout << sizeof(short) << endl;
    std::cout << sizeof(int) << endl;
    std::cout << sizeof(unsigned int) << endl;

    cout << numeric_limits<int>::max()+1 << endl;
    cout << numeric_limits<int>::min() << endl;
    cout << numeric_limits<unsigned int>::max() << endl;

    cout << 0u-1 << endl;
    cout << numeric_limits<unsigned int>::min()-1 << endl;
    cout << -1u << endl;
    cout << numeric_limits<unsigned int>::max()+1 << endl;
    foo(1);
    foo(1u);

    std::cout << sizeof(long) << endl;
    std::cout << sizeof(long long) << endl;
    std::cout << sizeof(float) << endl;
    std::cout << sizeof(double) << endl;

    std::cout << sizeof(char*) << endl;
    std::cout << sizeof(short*) << endl;
    std::cout << sizeof(int*) << endl;
    std::cout << sizeof(long*) << endl;
    std::cout << sizeof(float*) << endl;
    std::cout << sizeof(double*) << endl;

    cout << "==========================" << endl;

    cout << std::numeric_limits<double>::epsilon() << endl;
    cout << std::numeric_limits<double>::radix << endl;
    cout << std::numeric_limits<double>::digits << endl;
    cout << std::numeric_limits<double>::max_exponent << endl;
    cout << std::numeric_limits<double>::min_exponent << endl;

    cout << ((double_equals(0.9, 1.4-0.2-0.3)) ? "TRUE" : "FALSE") << endl;
    ////for(double d = 0; d != 0.3; d += 0.1);
    cout << ((0.9==1.4-0.3-0.2) ? "TRUE" : "FALSE") << endl;

    cout << "==========================" << endl;
    cout << 14/3 << endl;
    cout << 14./3 << endl;
    cout << 14%3 << endl;
    cout << (2<<2) << endl;
    cout << (20 & 1) << endl;
    cout << (21 & 1) << endl;


    cout << "==========================" << endl;
    char c = 20;

    int size_in_bits=sizeof(char)*8;

    while (size_in_bits-- > 0)
    {
        cout << ((c >> size_in_bits) & 1);
    }

    cout << endl;

    cout << 0x123abc << endl;

    cout << "==========================" << endl;
    int val = 534534;

    int pow_2 = log2(val);
    int max_num_of_bits = pow_2;

    cout << pow_2 << endl;

    while(max_num_of_bits >= 0)
    {
        cout << ((pow_2==max_num_of_bits) ? "1" : "0");

        if (pow_2>0 && (pow_2==max_num_of_bits))
        {
            val -= pow(2, pow_2);
            pow_2 = log2(val);
        }

        max_num_of_bits--;
    }
    cout << endl;

    cout << "==========================" << endl;



//    int i;
//    std::cin >> i;
//    if (std::cin.fail())
//    {
//        std::cout << "Error" << std::endl;
//    }

//    if (!std::cin)
//    {
//        std::cout << "Error2" << std::endl;
//    }

//    //cin.ignore(100);

//    long l;
//    std::cin >> l;

//    float f;
//    std::cin >> f;

//    double d;
//    std::cin >> d;

//    std::cout << i << ", " << l << ", " << f << ", " << d << std::endl;
}

