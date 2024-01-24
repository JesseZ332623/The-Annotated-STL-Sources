/*
    1.9.4 increment（前进），decrement（后退），dereference（解引用）操作符。

    increment（前进），decrement（后退）操作符在迭代器的实现上占有非常重要的地位，
    任何一个迭代器都必须实现出前进（operator++）和解引用（operator*）的功能，
    其中前进和后退还分为前置式和后置式两种。
*/

#include <iostream>

class INT
{
    private:
        int data;
    
    public:
        INT(int __dat) : data(__dat) {}

        /*先自增后赋值*/
        INT & operator++() { ++(this->data); return *this; }

        /*先赋值后自增*/
        const INT operator++(int) 
        { 
            INT temp = *this; 
            ++(*this);

            return temp;
        }

        /*先自减后赋值*/
        INT & operator--() { --(this->data); return *this; }

        /*先赋值后自减*/
        const INT operator--(int) 
        { 
            INT temp = *this; 
            --(*this);

            return temp;
        }

        int & operator*() const { return (int &)data; }

        friend std::ostream & operator<<(std::ostream & __os, const INT & __int);
};

std::ostream & operator<<(std::ostream & __os, const INT & __int)
{
    __os << '[' << __int.data << ']';

    return __os;
}

int main(int argc, char const *argv[])
{
    INT newData(5);

    std::cout << newData++ << '\n';
    std::cout << ++newData << '\n';
    std::cout << newData-- << '\n';
    std::cout << --newData << '\n';
    std::cout << *newData << '\n';
    return EXIT_SUCCESS;
}
