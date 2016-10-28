#include <iostream>

using namespace std;

class A_empty
{
    void foo(){}
};


struct S_empty
{
};


class A
{
public:
    //!char member0;
    int member1;
    int mutable member2;
    static int member_static1;

    enum Color
    {
        RED,
        BLUE,
        GREEN
    };

    void func_member()
    {
        member_static1 = 32;
        cout << __PRETTY_FUNCTION__ << endl;
    }

    static void func_member_static()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    int getSomething() const
    {
        //!member1 = 42;
        member2 = 42;
        const_cast<A*>(this)->member1 = 42;
        return member1;
    }

private:
    int member_private;
};

struct S
{
    //!char member0;
    int member1;
    static int member_static1;

    void func_member()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    static void func_member_static()
    {
        //!member1 = 42;
        cout << __PRETTY_FUNCTION__ << endl;
    }
};


//!int A::member_static1;
//!int S::member_static1;
//!
int A::member_static1 = 0;
int S::member_static1 = 0;


class Shape
{
public:

    Shape()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Shape(float value)
    {
        cout << __PRETTY_FUNCTION__ << value << endl;
    }

    enum Colour
    {
        RED,
        BLUE,
        GREEN
    };

    Colour m_colour;
    bool m_transparency;
    int m_border;

    Shape& setColor(Colour colour)
    {
        m_colour = colour;
        return *this;
    }

    Shape& setBorder(int border)
    {
        m_border = border;
        return *this;
    }

    Shape& setTransparency(bool transparency)
    {
        m_transparency = transparency;
        return *this;
    }
};


class Person
{

public:
    int member1;
    int member2;

    Person()
    {
        cout << __PRETTY_FUNCTION__ << endl;
        member1 = 42;
        member2 = 45;
    }

    ~Person()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    virtual void print()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

protected:
    int member5;
    int member6;

private:
    int member3;
    int member4;
};

class Student : public Person
{
    double gpa;

public:

    Student()
    {
        cout << __PRETTY_FUNCTION__ << endl;
        gpa = -1;
    }

    ~Student()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    /*virtual*/ void print()
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << "I'm student with gpa: " << gpa << endl;
        cout << "member1: " << member1 << endl;
        cout << "member5: " << member5 << endl;
//        cout << "member3: " << member3 << endl;

    }
};

class Professor : public Person
{
    double salary;

    /*virtual*/ void print()
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << "I'm professor with salary: " << salary << endl;
    }
};

int main()
{

    {
        cout << sizeof(A_empty) << endl;
        cout << sizeof(S_empty) << endl;

        cout << sizeof(S) << endl;
        cout << sizeof(A) << endl;
    }

    {
        A a;
        cout << a.member1 << endl;

        A::member_static1 = 33;
        cout << a.member_static1 << endl;
        cout << A::member_static1 << endl;

        A a2;
        cout << a2.member1 << endl;

        cout << a2.member_static1 << endl;
    }

    {
        A a;
        S s;
        cout << a.member1 << endl;
        cout << s.member1 << endl;

        A::member_static1 = 1;
        S::member_static1 = 1;
        cout << a.member_static1 << endl;
        cout << s.member_static1 << endl;

        A a2;
        S s2;
        cout << a2.member1 << endl;
        cout << s2.member1 << endl;

        cout << a2.member_static1 << endl;
        cout << s2.member_static1 << endl;

        cout << A::member_static1 << endl;
        cout << S::member_static1 << endl;
    }

    {
        A a;
        S s;
        a.func_member();
        s.func_member();

        a.func_member_static();
        s.func_member_static();
        A::func_member_static();
        S::func_member_static();
    }


    {
        A a;
        cout << a.member2 << endl;
        a.getSomething();
        cout << a.member2 << endl;
    }


    {
        Shape sh1;
        sh1.setColor(Shape::RED)
           .setBorder(123)
           .setTransparency(true);

        cout << sh1.m_border << endl;
        cout << sh1.m_transparency << endl;
        cout << sh1.m_colour << endl;
    }

    // vptr
    cout << "===[vptr]================================" << endl;
    {
        cout << sizeof(Person) << endl;
        cout << sizeof(Student) << endl;
    }

    // poly with A2 and B2
    {
        #define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

        {
            Person pp;
            cout << "p.member1: " << pp.member1 << endl;

            Student st;
            cout << "st.member1: " << st.member1 << endl;
        }

        cout << sizeof(Person) << endl;

        Student* s   = new Student;
        Professor* p = new Professor;

        cout << "member1: " << s->member1 << endl;

        Person* arr[2] = {s, p};

        for (int i =0; i<ARRAY_SIZE(arr); ++i)
        {
            arr[i]->print();
        }
    }

}
