#include <iostream>
#include <cassert>

using namespace std;


bool isEven(int number)
{
    return !(number & 1);
}

int sumDigits(int number)
{
    return (number / 100 + (number % 100) / 10  + number % 10);
}


int main(int argc, char *argv[])
{
    assert(isEven(0) == 1);
    assert(isEven(1) == 0);
    assert(isEven(2) == 1);
    assert(isEven(99) == 0);

    assert(sumDigits(123) == 6);
    assert(sumDigits(111) == 3);
    assert(sumDigits(999) == 27);
    assert(sumDigits(0) == 0);

    return 0;
}
