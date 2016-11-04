#include <cstdlib>
#include <climits>
#include <cmath>
#include <iostream>
#include <limits>
#include <iomanip>
#include <bitset>

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


int main(int argc, char* argv[])
{
    std::cout << "Hello world!" << std::endl;

    //    return 0;

    cout << "==========================" << endl;

    std::cout << sizeof(char) << endl;
    std::cout << sizeof(short) << endl;

    std::cout << sizeof(int) << endl;
    std::cout << sizeof(unsigned int) << endl;

    cout << "==========================" << endl;

    cout << numeric_limits<int>::max() << endl;
    cout << numeric_limits<int>::min() << endl;
    cout << "INT_MAX: " << INT_MAX << endl;
    cout << "INT_MIN: " << INT_MIN << endl;


    cout << numeric_limits<unsigned int>::max()/*+1*/ << endl;
    cout << numeric_limits<unsigned int>::min()/*-1*/ << endl;

    cout << "0u-1:  " << 0u-1 << endl;
    cout << "-1u:   " << -1u << endl;
    printf("-1u: %u\n", -1);

    foo(1);
    foo(1u);
    foo(1.);
    foo(1.f);


    std::cout << sizeof(int32_t) << endl;
    std::cout << sizeof(long) << endl;
    std::cout << sizeof(long long) << endl;
    std::cout << sizeof(float) << endl;
    std::cout << sizeof(double) << endl;

    cout << "==[FLOAT]========================" << endl;

    cout << std::numeric_limits<float>::epsilon() << endl;
    cout << std::numeric_limits<float>::radix << endl;
    cout << std::numeric_limits<float>::digits << endl;
    cout << std::numeric_limits<float>::max_exponent << endl;
    cout << std::numeric_limits<float>::min_exponent << endl;

    cout << "==[DOUBLE]========================" << endl;

    cout << std::numeric_limits<double>::epsilon() << endl;
    cout << std::numeric_limits<double>::radix << endl;
    cout << std::numeric_limits<double>::digits << endl;
    cout << std::numeric_limits<double>::digits10 << endl;
    cout << std::numeric_limits<double>::max_exponent << endl;
    cout << std::numeric_limits<double>::min_exponent << endl;

    cout << ((0.9==1.4-0.2-0.3) ? "TRUE" : "FALSE") << endl;
    float res = 1.4-0.2-0.3;
    cout << setprecision(15) << res << endl;

    ////for(double d = 0; d != 0.3; d += 0.1);

    cout << ((double_equals(0.9, 1.4-0.2-0.3)) ? "TRUE" : "FALSE") << endl;


    {
        cout << "==============================" << endl;
        union S
        {
            unsigned int n;      // 4 bytes
            float f;    // 4 bytes
            char c;     // 1 byte
        };              //  = total 4 bytes

        S s;
        s.n = 42;
        cout << "s.f = " << s.f << endl;
        s.n = 0xffffffff;
        cout << "s.f = " << s.f << endl;
        s.n = 4286578688;///2
        cout << "s.f = " << s.f << endl;
        s.n = 4286578688/2;///2
        cout << "s.f = " << s.f << endl;
        s.n = 2139095040;
        cout << "s.f = " << s.f << endl;
        s.f = -5;
        cout << "s.n = " << s.n << endl;
        s.f = 0.1;
        cout << "s.n (0.1) = " << s.n << endl;
        cout << "bin (0.1) = " << bitset<32>(s.n) << endl;

        s.f = 0.15625;
        cout << "s.n (0.1) = " << s.n << endl;

        cout << "float 1./0 = " << 1.0/0 << endl;
        cout << "float -1./0 = " << -1./0 << endl;
        cout << "float 1/-1./0 = " << 1/(-1./0) << endl;
        cout << "float -1/-1./0 = " << -1/(-1./0) << endl;
        ////return 0;
    }


    cout << "=============bitwise=============" << endl;
    cout << 14/3 << endl;
    cout << 14./3 << endl;
    cout << 14%3 << endl;
    cout << (2<<2) << endl;
    cout << (20 & 1) << endl;
    cout << (21 & 1) << endl;
    cout << (22 & 1) << endl;


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

//    long l;
//    std::cin >> l;

//    float f;
//    std::cin >> f;

//    double d;
//    std::cin >> d;

//    std::cout << l << ", " << f << ", " << d << std::endl;

//    std::cout << std::endl;

//    int i;
//    while (!(std::cin >> i))
//    {
//        cin.clear();
//        std::string ignoreLine; //read the invalid input into it
//        std::getline(cin, ignoreLine); //read the line till next space

//        std::cout << "Error: " << ignoreLine << std::endl;
//    }
//    cout << "Valid: " << i << endl;
}

