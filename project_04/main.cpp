#include <iostream>

using namespace std;


struct student_t
{
    char name[30];
    unsigned short gpa;
};

void print_student(student_t* student)
{
    cout << "name: " << student->name
         << ", gpa:" << student->gpa << endl;
}

void process_student(student_t& student)
{
    cout << "BEFORE: " << endl;
    print_student(&student);

    student.gpa *= 1.1;

    cout << "AFTER: " << endl;
    print_student(&student);
}

void init_students(student_t* students, size_t num_students)
{
    srand(time(0));
    for (int i = 0; i<num_students; i++)
    {
        sprintf(students[i].name, "Aaaa%d", i);
        students[i].gpa = rand() % 100;
    }
}

void print_students(student_t* students, size_t num_students)
{
    for (int i =0; i<num_students; i++)
    {
        print_student(&students[i]);
    }
}


void func(int** ppointer)
{
    if (ppointer)
    {
        (*ppointer)++;
    }
}

void func_ref(int*& pointer)
{
    pointer++;
}

void func_2_ref(int& pointer)
{
    pointer++;
}

int& func_return_ref()
{
    int value = 42;
    return value;
}


int* func_return_ptr()
{
    int value = 42;
    return &value;
}


void func_2_ref(int* pointer)
{
    (*pointer)++;
}

int func1(float value)
{
    return ++value;
}

void func2(float& value)
{
    ++value;
}

int add(int first, int second)
{
    cout << "first: " << first << ", second: " << second << endl;
    return first + second;
}

int mult(int first, int second)
{
    return first * second;
}

typedef int (*op)(int, int);

int do_action(op operation, int a, int b)
{
    return operation(a, b);
}

//-----------------------------------------------------------
//
int main(int argc, char* argv[])
{
    int n = 10;
    student_t* student_array = new student_t[n];
    init_students(student_array, n);

    print_students(student_array, n);

    for (int i=0; i<n; i++)
    {
        process_student(student_array[i]);
    }

    print_students(student_array, n);

    {
        cout << "==============================" << endl;
        int value = 42;
        int * foo = &value;

        cout << "foo: " << foo << endl;
        func(&foo);
        cout << "foo: " << foo << endl;
        func_ref(foo);
        cout << "foo: " << foo << endl;
    }

    {
        cout << "==============================" << endl;
        int value = 42;
        const int& value_ref = 5;
        //value_ref = 43;

        cout << "value: " << value << endl;
        cout << "value_ref: " << value_ref << endl;
        //value_ref++;
        cout << "value: " << value << endl;
        cout << "value_ref: " << value_ref << endl;
    }

    {
        cout << "==============================" << endl;
        int value = 4445;

        cout << "value: " << value << endl;
        func_2_ref(value);
        cout << "value: " << value << endl;
        func_2_ref(&value);
        cout << "value: " << value << endl;
    }

    {
        cout << "==============================" << endl;
//        int value = func_return_ref();
//        cout << "value (return ref): " << value << endl;
//        int* pvalue = func_return_ptr();
//        cout << "value (return ptr): " << *pvalue << endl;
    }


    {
        cout << "==============================" << endl;
        union S
        {
            int n;      // 4 bytes
            float f;    // 4 bytes
            char c;     // 1 byte
        };              //  = total 4 bytes

        S s;
        s.n = 42;
        cout << "s.f = " << s.f << endl;
        s.n = 0xffffffff;
        cout << "s.f = " << s.f << endl;
        s.f = 42.0;
        cout << "s.n = " << s.n << endl;
    }

    {
        struct S
        {
            enum{CHAR, INT, DOUBLE} tag;
            union
            {
                char c;
                int i;
                double d;
            };
        };

        S s = {S::CHAR, 'a'};

        switch(s.tag)
        {
            case S::CHAR: std::cout << s.c << '\n'; break;
            case S::INT: std::cout << s.i << '\n'; break;
            case S::DOUBLE: std::cout << s.d << '\n'; break;
        }

        s.tag = S::INT;
        s.i = 123;

        switch(s.tag)
        {
            case S::CHAR: std::cout << s.c << '\n'; break;
            case S::INT: std::cout << s.i << '\n'; break;
            case S::DOUBLE: std::cout << s.d << '\n'; break;
        }
    }

    {
        int (*pt1Function)(float);

        pt1Function = func1;
        //pt1Function = func2;
        cout << pt1Function(42.0) << endl;

        void (*pt2Function)(float&);
//        int (*pt2Function)(float);
        pt2Function = &func2;
        float value = 42.0;
        pt2Function(value);
        cout << "from pt2Function: " << value << endl;
        (*pt2Function)(value);
        cout << "from pt2Function: " << value << endl;
    }

    {
        typedef int (*op)(int, int);
        int (*paction)(op, int, int);
        paction = do_action;

        cout << "add: " << paction(add, 2, 4) << endl;
        cout << "mult: " << paction(mult, 2, 4) << endl;
    }

    return 0;
}
