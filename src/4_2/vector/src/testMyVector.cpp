#include "../include/myVector.h"
#include <MyLib/myLogerDef.h>

#include <iostream>
#include <algorithm>

template <typename Type>
void showVector(const My_Vector<Type> & __vec, const char * __outputMsg);

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");

    My_Vector<int> newVec(5, 1);

    showVector(newVec, "Init My_Vector<int> container:\n");

    newVec.push_back(2);
    newVec.push_back(3);
    newVec.push_back(4);

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

    DONE

    return EXIT_SUCCESS;
}

template <typename Type>
void showVector(const My_Vector<Type> & __vec, const char * __outputMsg)
{
    using namespace MyLib::MyLoger;

    auto outputRules = [](const Type & __n) { std::cout << __n << ' '; };

    printSplitLine(30, '-');

    if (__outputMsg || *__outputMsg != '\0') { NOTIFY_LOG(__outputMsg); }

    CORRECT_LOG("vector size     = " + std::to_string(__vec.size()) + '\n');
    CORRECT_LOG("vector capacity = " + std::to_string(__vec.capacity()) + '\n');

    std::for_each(__vec.begin(), __vec.end(), outputRules); std::cout.put('\n');

    printSplitLine(30, '-');
}