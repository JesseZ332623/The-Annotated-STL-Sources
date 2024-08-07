#include "../include/myVector.h"
#include <MyLib/myLogerDef.h>

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <vector>

template <typename Type>
void showVector(const My_Vector<Type> & __vec, const char * __outputMsg);

template <typename Type>
void inputRandomData(My_Vector<Type> & __vec, const std::size_t __dataCount);

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");

    /**
     * 看似 vector 的实现如此冗长，
     * 其实在栈上的只有 3 个 Type 类型的指针，共 24 字节。
    */
    CORRECT_LOG("Default My_Vector<int> size = " + std::to_string(sizeof(My_Vector<int>)) + " Bytes.\n");

    My_Vector<int> newVec(5, 1);

    My_Vector<double> vec_2 = {1, 2, 3, 4, 5, 8, 2456, 25, 0.56445};

    My_Vector<double> vec_3(vec_2);

    showVector(newVec, "Init My_Vector<int> container newVec:\n");
    showVector(vec_2, "Init My_Vector<double> container vec_2:\n");
    showVector(vec_3, "Init My_Vector<double> container vec_3 copy by vec_2:\n");

    My_Vector<int> vec_4 = std::move(newVec);

    showVector(vec_4, "Init My_Vector<int> container moved by vec_4:\n");
    showVector(newVec, "After move(), newVec: \n");

#if true
    inputRandomData(newVec, 15);

    showVector(newVec, "After push_back(const int & __n):\n");

    newVec.pop_back();

    showVector(newVec, "After pop_back():\n");

    newVec.erase(newVec.begin() + 2, newVec.begin() + 5);

    showVector(newVec, "After erase(int *__first, int *__last):\n");

    newVec.erase(newVec.begin());

    showVector(newVec, "After erase(int *__pos):\n");

    newVec.insert(newVec.begin() + 1, 10, 0);

    showVector(newVec, "After insert(int *__pos, std::size_t __n, const int &__x):\n");

    newVec.insert(newVec.begin() + 5, 1, 114514);

    showVector(newVec, "After insert(int *__pos, std::size_t __n, const int &__x):\n");

    newVec[0] = 125;

    showVector(newVec, "After operator[](sizeType __n):\n");

    newVec.at(15) = 99;

    showVector(newVec, "After at(sizeType __n):\n");

    newVec.resize(25, 114);

    showVector(newVec, "After resize(sizeType __newSize, const valueType & __x):\n");

    newVec.resize(5);

    showVector(newVec, "After resize(sizeType __newSize):\n");

    My_Vector<int> vec_5;
    vec_5.reserve(77);

    showVector(vec_5, "vec_5 after reserve(sizeType __newSize):\n");

    int array[] = {7, 6, 5, 4, 3, 2, 1};
    vec_5 = My_Vector<int>(array, array + 7);

    showVector(
                vec_5, 
                "vec_5 After "
                "My_Vector(const valueType * __first, const valueType * __last) and "
                "My_Vector(My_Vector && __vec):\n"
            );

    My_Vector<int> vec_6 = {1, 2, 3};

    vec_6.insert(vec_6.end(), 18, 123);

    showVector(vec_6, "vec_6 After insert():\n");
#endif

    My_Vector<int> vec_7 = {1, 2, 3, 4, 5, 6, 7, 0, 1, -1};

    int * arrayPointer = new int[vec_7.size()];
    std::copy(vec_7.data(), vec_7.data() + vec_7.size(), arrayPointer);

    [](const int * __arr, std::size_t __size) 
    {
        for (std::size_t index = 0; index < __size; ++index)
            NOTIFY_LOG(std::to_string(__arr[index]) + ' ');
    }(arrayPointer, vec_7.size()); puts("");

    delete[] arrayPointer;

    DONE

    return EXIT_SUCCESS;
}

template <typename Type>
void showVector(const My_Vector<Type> & __vec, const char * __outputMsg)
{
    using namespace MyLib::MyLoger;

    auto outputRules = [](const Type & __n) { std::cout << __n << ' '; };

    printSplitLine(30, '-');

    if (__outputMsg != nullptr) { NOTIFY_LOG(__outputMsg); }

    CORRECT_LOG("vector size     = [" + std::to_string(__vec.size()) + "]\n");
    CORRECT_LOG("vector capacity = [" + std::to_string(__vec.capacity()) + "]\n");

    std::for_each(__vec.begin(), __vec.end(), outputRules); std::cout.put('\n');

    printSplitLine(30, '-');
}

template <typename Type>
void inputRandomData(My_Vector<Type> & __vec, const std::size_t __dataCount)
{
    std::srand(std::time(nullptr));

    for (std::size_t index = 0ULL; index < __dataCount; ++index)
    {
        __vec.push_back(std::rand() % 114 + 1);
    }
}