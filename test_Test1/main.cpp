#include <iostream>
#include <cassert>


using namespace std;


template<typename T>
class ImmutableBuffer
{
public:
    ImmutableBuffer(T* data, size_t length)
        : m_data(new T[length]())
        , m_size(length)
    {
        for (size_t i = 0; i<length; ++i)
        {
            m_data[i] = data[i];
        }
    }

    ImmutableBuffer(const ImmutableBuffer& rhs)
        : m_data(new T[rhs.m_size]())
        , m_size(rhs.m_size)
    {
        for (size_t i = 0; i<rhs.m_size; ++i)
        {
            m_data[i] = rhs.m_data[i];
        }
    }

    ~ImmutableBuffer()
    {
        if (m_data)
        {
            delete[] m_data;
        }
    }

    bool operator==(const ImmutableBuffer& rhs)
    {
        bool bResult = true;

        if(m_size!=rhs.m_size)
        {
            bResult = false;
        }

        for (size_t i = 0; i<m_size; ++i)
        {
            bResult &= (m_data[i] == rhs.m_data[i]);
        }

        return bResult;
    }

    bool operator!=(const ImmutableBuffer& rhs)
    {
        return !operator==(rhs);
    }

    const T& operator[](size_t idx)
    {
        return m_data[idx];
    }

    template <typename U>
    friend ostream& operator <<(ostream& os, const ImmutableBuffer<U>& rhs);
//    {
//        os << "{";
//        for (size_t i = 0; i<rhs.m_size; ++i)
//        {
//            os << rhs.m_data[i] << ", ";
//        }
//        os << "}";

//        return os;
//    }

private:
    ImmutableBuffer& operator=(const ImmutableBuffer& rhs);

    T* m_data;
    size_t m_size;
};

template<typename T>
ostream& operator << (ostream& os, const ImmutableBuffer<T>& rhs)
{
    os << "{";
    for (size_t i = 0; i<rhs.m_size; ++i)
    {
        os << rhs.m_data[i] << ", ";
    }
    os << "}";

    return os;
}



class HTMLElement
{
public:

    HTMLElement(unsigned width, unsigned height, bool hidden=false)
        : m_width(width)
        , m_height(height)
        , m_hidden(hidden)
    {
    }

    virtual ~HTMLElement()
    {
    }


    unsigned Width() const
    {
        return m_width;
    }
    unsigned Height() const
    {
        return m_height;
    }

    bool Hidden() const
    {
        return m_hidden;
    }

    virtual string Render()=0;

    virtual HTMLElement* Duplicate()=0;

private:

    HTMLElement& operator=(const HTMLElement& rhs);
    HTMLElement(const HTMLElement& rhs);

    unsigned m_width;
    unsigned m_height;
    bool m_hidden;
};


class HTMLButtonElement : public HTMLElement
{
public:

    HTMLButtonElement(unsigned width, unsigned height, bool hidden, const string& title)
        : HTMLElement(width, height, hidden)
        , m_title(title)
    {
    }

    HTMLButtonElement(const HTMLButtonElement& rhs)
        : HTMLButtonElement(rhs.Width(), rhs.Height(), rhs.Hidden(), rhs.m_title)
    {
    }

    string Render() override
    {
        return "<button>" + m_title + "</button>";
    }

    HTMLElement* Duplicate() override
    {
        return new HTMLButtonElement(*this);
    }

private:
    string m_title;
};

class HTMLImageElement : public HTMLElement
{
public:

    HTMLImageElement(unsigned width, unsigned height, bool hidden, const string& url)
        : HTMLElement(width, height, hidden)
        , m_url(url)
    {
    }

    HTMLImageElement(const HTMLImageElement& rhs)
        : HTMLImageElement(rhs.Width(), rhs.Height(), rhs.Hidden(), rhs.m_url)
    {
    }

    string Render() override
    {
        return "<img src=\"" + m_url + "\" height=" + to_string(Height()) + \
                                         "  width=" + to_string(Width()) + ">";
    }

    HTMLElement* Duplicate() override
    {
        return new HTMLImageElement(*this);
    }

private:
    string m_url;
};

class HTMLTextAreaElement : public HTMLElement
{
public:

    HTMLTextAreaElement(unsigned width, unsigned height, bool hidden, const string& content)
        : HTMLElement(width, height, hidden)
        , m_content(content)
    {
    }

    HTMLTextAreaElement(const HTMLTextAreaElement& rhs)
        : HTMLTextAreaElement(rhs.Width(), rhs.Height(), rhs.Hidden(), rhs.m_content)
    {
    }

    string Render() override
    {
        return "<textarea>\n" + m_content + "\n</textarea>";
    }

    HTMLElement* Duplicate() override
    {
        return new HTMLTextAreaElement(*this);
    }

private:
    string m_content;
};


template<typename T>
bool all_of(T* arr, size_t length, bool(*p)(const T& elem))
{
    for(size_t i=0; i<length; ++i)
    {
        if (!p(arr[i]))
        {
            return false;
        }
    }

    return true;
}

template<typename T>
bool any_of(T* arr, size_t length, bool(*p)(const T& elem))
{
    for(size_t i=0; i<length; ++i)
    {
        if (p(arr[i]))
        {
            return true;
        }
    }

    return false;
}

//typedef HTMLElement* HTMLElementPtr;
using HTMLElementPtr = HTMLElement*;
//typedef HTMLElement HTMLElementPtr;
//#define HTMLElementPtr HTMLElement*

bool IsHidden(const HTMLElementPtr& elem)
{
    return elem->Hidden();
}

#define ARR_SIZE(a) sizeof(a)/sizeof(a[0])

int main(int argc, char *argv[])
{
    // TEST 1
    // #################################
    {
        double arr[] = {1.0, 2.0, 3.0};
        ImmutableBuffer<double> buff(arr, ARR_SIZE(arr));
        ImmutableBuffer<double> buff2 = buff;
        arr[0] = 0;
        assert(buff[0] == 1.0);

        // equal
        assert(buff == buff2);

        // should fail
    //    buff[0] = 42.0;
    //    buff    = buff2;

        cout << buff << endl;
    }


    // TEST 2+3
    // #################################
    {
        HTMLElementPtr arr[] = {
            new HTMLButtonElement   (0, 0, false, "Click me"),
            new HTMLImageElement    (0, 0, false, "http://imgur.com/cat.gif"),
            new HTMLTextAreaElement (0, 0, true,  "This is long multiline text")
        };
        for(size_t i = 0; i<ARR_SIZE(arr); ++i)
        {
            cout << arr[i]->Render() << endl;
        }

        HTMLElement* arr2[3] = {};
        for(size_t i = 0; i<ARR_SIZE(arr); ++i)
        {
            arr2[i] = arr[i]->Duplicate();
        }

        for(size_t i = 0; i<ARR_SIZE(arr2); ++i)
        {
            assert (arr[i]->Render() == arr2[i]->Render());
            cout << arr2[i]->Render() << endl;
        }

        // TEST4
        assert (any_of(arr, ARR_SIZE(arr), IsHidden)==true);
        assert (all_of(arr, ARR_SIZE(arr), IsHidden)==false);
    }


    return 0;
}
