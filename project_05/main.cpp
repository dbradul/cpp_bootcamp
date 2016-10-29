#include <iostream>
#include <cstring>

using namespace std;

struct person_t;

typedef void (*init_ptr_t)(person_t*);
typedef void (*print_ptr_t)(person_t*);
typedef void (*print2_ptr_t)(person_t*);
typedef void (*print3_ptr_t)(person_t*);

enum PersonType
{
    STUDENT,
    PROFESSOR,
    INSTRUCTOR
};

struct vtable_t
{
    print_ptr_t print_func;
//    print2_ptr_t print2_func;
//    print3_ptr_t print3_func;
};

struct person_t
{
    char name[30];
    unsigned short age;
    init_ptr_t init_func;
    vtable_t* vtable;
};

struct student_t
{
    person_t base;
    vtable_t* vtable;
    unsigned short gpa;
};

struct professor2_t
{
    person_t base;
    vtable_t* vtable;
    unsigned short salary;
};

struct professor_t
{
    professor2_t base;
    vtable_t* vtable;
    unsigned short quote_rank;
};

struct instructor_t
{
    professor2_t base;
    vtable_t* vtable;
    unsigned short schedule_hours;
};

void student_print(person_t* entity)
{
    student_t* student = (student_t*)entity;
    cout << "I'm student. GPA=" << student->gpa << endl;
}


void professor_print(person_t* entity)
{
    professor2_t* professor = (professor2_t*)entity;
    cout << "I'm professor. salary=" << professor->salary << endl;
}

void person_init(person_t* thiz)
{
    /* >>> VTable construction START >>> */
    vtable_t* vptr = new vtable_t;
    thiz->vtable = vptr;
    thiz->vtable->print_func = NULL; // pure function
    /* <<< VTable construction END <<< */

    thiz->age = 0;
    strcpy(thiz->name, "<Unknown name>");
    cout << "I'm pure person" << endl;
}

void student_init(student_t* thiz)
{
    person_init((person_t*) thiz);

    cout << "Student init" << endl;

    /* >>> VTable construction START >>> */
    vtable_t* vptr = new vtable_t;
    thiz->vtable = vptr;
    thiz->vtable->print_func = student_print;
    /* <<< VTable construction END <<< */

    thiz->base.age = 19;
    strcpy(thiz->base.name, "Vasyl");
    thiz->gpa = 4.0;
}

void professor_init(professor2_t* thiz)
{
    person_init((person_t*) thiz);

    cout << "Professor init" << endl;

    /* >>> VTable construction START >>> */
    vtable_t* vptr = new vtable_t;
    thiz->vtable = vptr;
    thiz->vtable->print_func = professor_print;
    /* <<< VTable construction END <<< */

    thiz->base.age = 42;
    strcpy(thiz->base.name, "Vasyl Vasylevich");
    thiz->salary = 100.0;
}

person_t* create(PersonType type)
{
    person_t* pEntity = NULL;

    switch (type)
    {
        case STUDENT:
        {
            student_t* student = new student_t;
            student_init(student);
            pEntity = (person_t*)(void*) student;
            break;
        }

        case PROFESSOR:
        {
            professor2_t* professor = new professor2_t;
            professor_init(professor);
            pEntity = (person_t*)(void*) professor;
            break;
        }
        default:
            cout << "Undefined entity type" << endl;
    }

//    pEntity->init_func(pEntity);

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

    person1->vtable->print_func(person1);
    person2->vtable->print_func(person2);

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
