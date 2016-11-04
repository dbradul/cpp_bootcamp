#include <iostream>
#include <cstring>

using namespace std;

const int ROWS = 2;
const int COLUMNS = 3;


void print_array(int _2darray[ROWS][COLUMNS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            cout << _2darray[i][j] << " ";
        }
        cout << endl;
    }
}


template<int N, int M>
void print_array(int _2darray[N][M])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            cout << _2darray[i][j] << "\t";
        }
        cout << endl;
    }
}


void print_array_arithmetics(int _2darray[ROWS][COLUMNS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            cout << *((int*)_2darray + (COLUMNS*i+j)) << " ";
        }
        cout << endl;
    }
}


void print_array(int** _2darray, int num_rows, int num_columns)
{
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_columns; j++)
        {
            cout << _2darray[i][j] << " ";
        }
        cout << endl;
    }
}

template<int N>
void rotate(int _2darray[N][N])
{
    for (int i = 0; i < N/2; i++)
    {
        for (int j = N-i-1; j > i; j--)
        {
            swap(_2darray[i][j],         _2darray[N-j-1][i]);
            swap(_2darray[N-j-1][i],     _2darray[N-i-1][N-j-1]);
            swap(_2darray[N-i-1][N-j-1], _2darray[j][N-i-1]);
        }
    }
}

const int ALPHABET_LENGTH = 'z'-'a';

int char2idx(char c)
{
    int idx = 0;
    if (c>='a' && c<='z')
    {
        idx = c-'a';
    }
    else if (c>='A' && c<='Z')
    {
        idx = ALPHABET_LENGTH + (c-'A');
    }

    return idx;
}

void count(const char* str)
{
    size_t n = strlen(str);
    const int NUM_OF_SYMBOLS = ALPHABET_LENGTH*2;

    int count_array[NUM_OF_SYMBOLS] = {0};

    for (int i = 0; i<n; i++)
    {
        count_array[char2idx(str[i])] += 1;
    }

    for (int i = 0; i<n; i++)
    {
        cout << str[i] << ": " << count_array[char2idx(str[i])] << endl;
    }
}


#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

int main(int argc, char *argv[])
{
    {
        const int dim1 = 5;
        const int dim2 = 5;
        int x[][dim2] = {  {1, 2, 3, 4, 5},
                           {6, 7, 8, 9, 10},
                           {11,12,13,14,15},
                           {16,17,18,19,20},
                           {21,22,23,24,25},
                            };

        print_array<5,5>(x);
        cout << endl;
        rotate(x);
        print_array<5,5>(x);
    }

    return 0;

    {
        int x[ROWS][COLUMNS] = {{1,2,3},{4,5,6}};

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                cout << x[i][j] << " ";
            }
            cout << endl;
        }

        print_array(x);
        //print_array_arithmetics(x);
    }

    {
        int** x = new int*[ROWS];

        for (int i = 0; i < ROWS; i++)
        {
            x[i] = new int[COLUMNS];
        }

        print_array(x, ROWS, COLUMNS);

        for (int i = 0; i < ROWS; i++)
        {
            delete [] x[i];
        }
        delete[] x;
    }


    struct timespec start, finish;
    double elapsed;

    {
        clock_gettime(CLOCK_MONOTONIC, &start);

        int i,j;
        static int x[4000][4000];
        for (i = 0; i < 4000; i++) {
          for (j = 0; j < 4000; j++) {
            x[i][j] = i + j; }
        }

        clock_gettime(CLOCK_MONOTONIC, &finish);

        elapsed = (finish.tv_sec - start.tv_sec) +
                  (finish.tv_nsec - start.tv_nsec) / 1.e9;

        cout << "elapsed[i,j]: " << elapsed << endl;
    }

    {
        clock_gettime(CLOCK_MONOTONIC, &start);

        int i,j;
        static int x[4000][4000];
        for (i = 0; i < 4000; i++) {
          for (j = 0; j < 4000; j++) {
            x[j][i] = i + j; }
        }

        clock_gettime(CLOCK_MONOTONIC, &finish);

        elapsed = (finish.tv_sec - start.tv_sec) +
                  (finish.tv_nsec - start.tv_nsec) / 1.e9;

        cout << "elapsed[j,i]: " << elapsed << endl;
    }

    {
        // padding
        struct student
        {
            char name[20];
            unsigned short age;//+2
            float gpa;
        };

        struct str1
        {
            char f1;
            char f2; //+6
            char* f4;
            char f3; //+7
            char* f5;
        };

        struct str2
        {
            char* f4;
            char* f5;
            char f1;
            char f2;
            char f3;//+5
        };

        cout << "size1: " << sizeof(str1) << endl;
        cout << "size2: " << sizeof(str2) << endl;

        student st1 = {"Jhon", 19, 4.0};
        student st2 = {0};

        cout << "size: " << sizeof(student) << endl;
//        cout << "size2: " << sizeof(student2) << endl;
        cout << st1.name << ", " << st1.age << ", " << st1.gpa << endl;
        cout << st2.name << ", " << st2.age << ", " << st2.gpa << endl;

        student group[10] = {{"Alice", 18, 4.0},
                             {"Bob", 22, 3.5}};

        for (int i=0; i<ARRAY_SIZE(group); i++)
        {
            student st = group[i];
            cout << st.name << ", " << st.age << ", " << st.gpa << endl;
        }
    }

    {
        struct address
        {
            char city[64];
            char street[64];
            unsigned short number;
        };

        struct student
        {
            address addr;
            char name[20];
            unsigned short age;
            float gpa;
        };

        student st1 = {{"Odessa", "Deribasovskaya", 1}, "Jhon", 19, 4.0};

        cout << "size (addr): " << sizeof(address) << endl;
        cout << "size: "        << sizeof(student) << endl;
        cout << st1.addr.city << ", " << st1.addr.street << ", " << st1.addr.number << endl;
        cout << st1.name << ", " << st1.age << ", " << st1.gpa << endl;
    }

    {
        struct student
        {
            char name[20];
            unsigned short age;
            float gpa;
        };

        student st1 = {"Jhon", 19, 4.0};
        student* pStudent = &st1;

        cout << "size: "        << sizeof(student) << endl;
        cout << pStudent->name << ", " << pStudent->age << ", " << pStudent->gpa << endl;
    }

    {
        struct address
        {
            char city[64];
            char street[64];
            unsigned short number;
        };

        struct student
        {
            address* addr;
            char name[20];
            unsigned short age;
            float gpa;
        };

        address addr = {"Odessa", "Deribasovskaya", 1};
        student st1 = {&addr, "Jhon", 19, 4.0};

        cout << st1.addr->city << ", " << st1.addr->street << ", " << st1.addr->number << endl;
        cout << st1.name << ", " << st1.age << ", " << st1.gpa << endl;

        address* addr2 = new address;
        strcpy(addr2->city, "Kyiv");
        strcpy(addr2->street, "Khreschatik");
        addr2->number = 1;

        cout << addr2->city << ", " << addr2->street << ", " << addr2->number << endl;
        delete addr2;
    }

    return 0;
}
