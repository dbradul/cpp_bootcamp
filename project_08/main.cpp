#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <bitset>


// operators overload
// prefix/postfix
// members or free functions
// functors
// friend classes and functions

using namespace std;

class A
{
    int m_value;

public:
    A()
        : m_value(0)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    A(const A& rhs)
        : m_value(rhs.m_value)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    ~A()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    A& operator=(const A& rhs)
    {
        if (&rhs != this)
        {
            A tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(A& from)
    {
        std::swap(from.m_value,  m_value);
    }

    A& operator=(const char* value)
    {
        m_value = atoi(value);
        return *this;
    }

    A& operator++()
    {
        ++m_value;
        return *this;
    }

    A operator++(int)
    {
        A tmp(*this);
        ++m_value;
        return tmp;
    }

    explicit operator int() const
    {
        return m_value;
    }

    friend ostream& operator<<(ostream& os, const A& obj)
    {
        os << "A::value = " << obj.m_value;
        return os;
    }
};

class big_int
{
private:
   string m_value;

   void divide(const big_int& limit, big_int& sum, big_int& k)
   {
      if( sum+sum <= limit )
      {
         big_int curr_sum  = sum;
         big_int curr_k    = k;
         sum += sum;
         k += k;

        //continue doubling until we reach or exceed the limit and then go back
         divide(limit, sum, k);

         // if possible increase the result with the current value
         if( ( curr_sum + sum ) <= limit )
         {
            sum   += curr_sum;
            k     += curr_k;
         }
      }
   }

   void mult(const big_int& limit, big_int& sum, big_int& k)
   {
      if( k+k <= limit )
      {
         big_int curr_sum  = sum;
         big_int curr_k    = k;
         sum += sum;
         k += k;

         //continue doubling until we reach or exceed the limit and then go back
         mult(limit, sum, k);

         // if possible increase the result with the current value
         if( ( curr_k + k ) <= limit )
         {
            sum   += curr_sum;
            k     += curr_k;
         }
      }
   }

public:
   big_int()
   {
   }

   big_int(const char* str)
       :m_value(str)
   {
   }

   big_int(const big_int& rhs)
       :m_value(rhs.m_value)
   {
   }

   ~big_int()
   {
   }

   big_int& operator =(const big_int& rhs)
   {
      if (this != & rhs)
      {
         m_value = rhs.m_value;
      }
      return *this;
   }

   bool operator <(const big_int& rhs)
   {
      bool bResult = ( m_value.length()<rhs.m_value.length() );
      if (m_value.length()==rhs.m_value.length())
      {
         string::const_iterator iter1 = m_value.begin();
         string::const_iterator iter2 = rhs.m_value.begin();

         for ( ; iter1 != m_value.end() && *iter1 == *iter2; ++iter1, ++iter2 )
         {
         }

         bResult = ( m_value.end()==iter1 ) ? false : ( *iter1 < *iter2 );
      }
      return bResult;
   }

   bool operator >(const big_int& rhs)
   {
      return !(*this<rhs);
   }

   bool operator ==(const big_int& rhs)
   {
      return m_value == rhs.m_value;
   }

   bool operator !=(const big_int& rhs)
   {
      return !(*this == rhs);
   }

   bool operator <=(const big_int& rhs)
   {
      return ( *this<rhs || *this==rhs );
   }

   bool operator >=(const big_int& rhs)
   {
      return ( *this>rhs || *this==rhs );
   }

   big_int& operator +=(const big_int& rhs)
   {
      if ( rhs.m_value.length() > m_value.length() )
      {
         m_value.insert(m_value.begin(), rhs.m_value.length() - m_value.length(), '0');
      }

      reverse_iterator<string::iterator> riter1 = m_value.rbegin();
      reverse_iterator<string::const_iterator> riter2 = rhs.m_value.rbegin();
      char next_byte_addition = 0;
      for(; riter1 != m_value.rend(); ++riter1 )
      {
         char val1 = *(riter1) - '0';
         char val2 = ( riter2 != rhs.m_value.rend() ) ? *(riter2++) - '0' : 0;
         char res_sum = ( val1 + val2 + next_byte_addition );

         next_byte_addition = 0;
         if ( res_sum > 9 )
         {
            next_byte_addition = 1;
            res_sum %= 10;
         }
         *riter1 = res_sum + '0';
      }
      if ( next_byte_addition )
      {
         m_value.insert(0,1,'1');
      }
      return *this;
   }

   big_int& operator /(const big_int& rhs)
   {
      //check the special case when the dividend is less then the divisor => quotient=0, remainder=dividend
      if (*this < rhs)
      {
         *this = "0";
      }
      else
      {
         big_int sum = rhs;
         big_int k = "1";

         divide(*this, sum, k);

         //so far, k is the quotient: sum=rhs*k+remainder
         *this = k;
      }
      return *this;
   }

   big_int& operator *(const big_int& rhs)
   {
      //check the special case
      if (big_int("1") != rhs)
      {
         big_int sum = rhs;
         big_int k = "1";

         mult(*this, sum, k);

         //so far, sum is the production: sum=rhs*k+remainder
         *this = sum;
      }
      return *this;
   }

   big_int operator+(const big_int& rhs)
   {
      return big_int(*this) += rhs;
   }

   friend ostream& operator<<(ostream& stream, const big_int& rhs)
   {
      return stream << rhs.m_value;
   }


};

class Limiter
{
    int m_limit;

public:
    Limiter(int limit):
        m_limit(limit)
    {

    }

    bool operator()(int val)
    {
        return val < m_limit;
    }
};

bool IsEven(int val)
{
    return val % 2 == 0;
}


class Node
{
private:
    int key;

public:
    int data;
    Node* left;
    Node* right;

    friend class BinaryTree;
};

class BinaryTree
{
    private:
    Node *root;

    int rebalance(Node *root)
    {
        if (root->left)
        {
            // rebalance using key
            if (root->left->key == 1)
            {
                // ...
            }
        }
    }
};

int main(int argc, char *argv[])
{

    {
        #define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
        A a;
        a = "42";

        A b = a++;

        cout << b << endl;
        cout << a << endl;

        int c = static_cast<int>(a);

        cout << "c = " << c << endl;
    }


    {
        assert( big_int("12300000000000000000009990000000000000000000")
             == big_int("12300000000000000000009990000000000000000000") );
        assert( big_int("123") <= big_int("124") );
        assert( big_int("124") >  big_int("123") );
        assert( big_int("0")   <  big_int("1") );
        assert( big_int("100") <  big_int("1100") );
        assert( big_int("1000")>  big_int("110") );

        big_int num1 = "111111111111111111111111111111111111111111111111111111123";
        big_int num2 = "111111111111111111111111111111111111111111111111111111456";
        big_int res  = "222222222222222222222222222222222222222222222222222222579";
        assert( res == ( num1+num2 ) );

        num1 = "0";
        num2 = "111111111111111111111111111111111111111111111111111111456";
        res  = "111111111111111111111111111111111111111111111111111111456";
        assert( res == ( num1+num2 ) );

        num1 = "42";
        num2 = "111111111111111111111111111111111111111111111111111111456";
        res  = "111111111111111111111111111111111111111111111111111111498";
        assert( res == ( num1+num2 ) );

        num1 = "42";
        num2 = "0";
        res  = "42";
        assert( res == ( num1+num2 ) );

        num1 =  "999999999999999999999999999999999999999999999999999999999";
        num2 =  "999999999999999999999999999999999999999999999999999999999";
        res  = "1999999999999999999999999999999999999999999999999999999998";
        assert( res == ( num1+num2 ) );

        big_int num3;
        num1 =  "909090909090909090909090909090909090909090909090909090909";
        num2 =   "90909090909090909090909090909090909090909090909090909090";
        num3 =  "999999999999999999999999999999999999999999999999999999999";
        res  = "1999999999999999999999999999999999999999999999999999999998";
        assert( res == ( num1+num2+num3 ) );

        num1 = "11111111111111111111111111111";
        res  = "22222222222222222222222222222";
        num1 += num1;
        assert( res == num1 );

        num1 = "32";
        num2 = "16";
        res  = "2";
        assert( res == ( num1/num2 ) );

        num1 = "32";
        num2 = "17";
        res  = "1";
        assert( res == ( num1/num2 ) );

        num1 = "41";
        num2 = "42";
        res  = "0";
        assert( res == ( num1/num2 ) );

        num1 = "420000000000000000000000000000000000000000000000000000000";
        num2 = "42";
        res  =  "10000000000000000000000000000000000000000000000000000000";
        assert( num1/num2 == res );

        num1 = "10000000000000000000000000050000000000000000000000000000";
        num2 = "1";
        res  = "10000000000000000000000000050000000000000000000000000000";
        assert( res == ( num1/num2 ) );

        num1 = "10000000000000000000000000000000000000000000000000000000";
        num2 = "10000000000000000000000000000000000000000000000000000000";
        res  = "1";
        assert( res == ( num1/num2 ) );

        num1 = "10000000000000000000000000000000000000000000000000000000";
        num2 = "6";
        res  = "1666666666666666666666666666666666666666666666666666666";
        assert( res == ( num1/num2 ) );

        num1 = "123456790123456790123456790120987654320987654320987654321";
        num2 = "11111111111111111111111111111";
        res  = "11111111111111111111111111111";
        assert( res == ( num1/num2 ) );

        num1 = "45283475823745298719283652734523457238457230945782093480193987";
        num2 = "23354603453495809834504503495803945830495723842734092348209234";
        assert( num1 == (num1*num2)/num2 );

        cout << big_int("11111111111111111111111111111") * big_int("11111111111111111111111111111") << endl;
    }

    {
        vector<int> v = {1,2,3,4,5,6,7,8,9,10};
        cout << count_if(v.begin(), v.end(), IsEven) << endl;

        Limiter upper_limit(5);
        cout << count_if(v.begin(), v.end(), upper_limit) << endl;
    }


    return 0;
}
