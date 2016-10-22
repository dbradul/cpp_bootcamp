#include <iostream>
#include <cstring>

using namespace std;

struct person_t;

typedef void (*init_ptr_t)(person_t*);
typedef void (*print_ptr_t)(person_t*);

enum PersonType
{
    STUDENT,
    PROFESSOR
};

struct person_t
{
    char name[30];
    unsigned short age;
    init_ptr_t init_func;
    print_ptr_t print_func;
};

struct student_t
{
    person_t base;
    unsigned short gpa;
};

struct professor_t
{
    person_t base;
    unsigned short salary;
};

void student_print(person_t* entity)
{
    student_t* student = (student_t*)entity;
    cout << "I'm student. GPA=" << student->gpa << endl;
}

void professor_print(person_t* entity)
{
    professor_t* professor = (professor_t*)entity;
    cout << "I'm professor. salary=" << professor->salary << endl;
}

void person_init(person_t* entity)
{
    entity->age = 0;
    strcpy(entity->name, "<Unknown name>");
    cout << "I'm pure person" << endl;
}

void student_init(person_t* entity)
{
    student_t* student = (student_t*)entity;
    cout << "Student init" << endl;
    student->base.age = 19;
    strcpy(student->base.name, "Vasyl");
    student->gpa = 4.0;
}

void professor_init(person_t* entity)
{
    professor_t* professor = (professor_t*)entity;
    cout << "Professor init" << endl;
    professor->base.age = 42;
    strcpy(professor->base.name, "Vasyl Vasylevich");
    professor->salary = 100.0;
}

person_t* create(PersonType type)
{
    person_t* pEntity = NULL;

    switch (type)
    {
        case STUDENT:
            pEntity = (person_t*)(void*) new student_t;
            pEntity->init_func = student_init;
            pEntity->print_func = student_print;
            break;
        case PROFESSOR:
            pEntity = (person_t*)(void*) new professor_t;
            pEntity->init_func = professor_init;
            pEntity->print_func = professor_print;
            break;
        default:
            cout << "Undefined entity type" << endl;
    }

    pEntity->init_func(pEntity);

    return pEntity;
}

void foo (int)
{
    cout << __PRETTY_FUNCTION__ << endl;
}

void foo (double)
{
    cout << __PRETTY_FUNCTION__ << endl;
}


//int foo2 ()
//{
//    cout << __PRETTY_FUNCTION__ << endl;
//}

//double foo2 ()
//{
//    cout << __PRETTY_FUNCTION__ << endl;
//}


int next()
{
//    static int counter = 0;
    static int counter;
    counter++;
    return counter;
}

int a_var = 10;
extern int b_var;
//static int auto c_var = 10;   // CONFLICTING SPECIFIERS!
//static extern int d_var = 10; // CONFLICTING SPECIFIERS!
void project_05_foo_global();
void project_05_foo_local();


int main(int argc, char *argv[])
{
    student_t s = {0};
    student_t* pStudent = &s;
    person_t* pPerson = &s.base;

    cout << "pStudent: " << pStudent << endl;
    cout << "pPerson: " << pPerson << endl;

    person_t* person1 = create(STUDENT);
    person_t* person2 = create(PROFESSOR);

    cout << "age: " << person1->age << " name: " << person1->name << endl;
    cout << "age: " << person2->age << " name: " << person2->name << endl;

    person1->print_func(person1);
    person2->print_func(person2);

    delete person1;
    delete person2;

    {
        foo(1);
        foo(1.);
    }

    {
        {
            int x = 10;
            int y = 20;
//            char auto array[] = "abc";
//            char auto* array2 = "abc";
//            char c = 'z';
//            char * pchar = &c;
//            cout << "array = " << array << endl;
//            cout << "array2 = " << array2 << endl;
//            cout << "pchar = " << pchar << endl;
//            cout << "x: " << x << "y: " << y << endl;
        }
        //cout << "x: " << x << "y: " << y << endl;
    }

    {
        register int x = 10;
        int* pInt = &x;
    }

    {
        cout << next() << endl;
        cout << next() << endl;
        cout << next() << endl;
    }

    {
        cout << "b_var: " << b_var << endl;
    }

    {
        project_05_foo_global();
        //project_05_foo_local();
    }

    return 0;
}
