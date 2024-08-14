#include <MyLib/simpleContainerOperator.h>
#include <MyLib/myDelay.h>

#include <bits/stl_algobase.h>
#include <vector>

/**
 * @brief std::mismatch() 寻找两个序列的第一个不匹配点。
*/

using namespace MyLib::SimpleContainerOperator;
using namespace MyLib::MyLoger;

void show(const std::vector<int> & __v, const char __tag, std::vector<int>::iterator it);

int main(int argc, char const *argv[])
{
    system("cls");

    std::vector<int> numberList_A = {0, 1, 2, 3, 4, 5};
    std::vector<int> numberList_B = {0, 1, 2, 3, 3, 4, 6, 7, 8, 9, 10};

    auto missmatchPair = 
    std::mismatch(numberList_A.begin(), numberList_A.end(), numberList_B.begin());

    showContainerToStream(std::cout, numberList_A, numberList_A.size());
    showContainerToStream(std::cout, numberList_B, numberList_B.size());

    loger(
        std::cout, CORRECT,
        "Missmatch position = [",
        missmatchPair.first.operator->(), "] [",
        missmatchPair.second.operator->(), "]\n"
    );

    show(numberList_A, 'A', missmatchPair.first);
    show(numberList_B, 'B', missmatchPair.second);

    DONE;

    return EXIT_SUCCESS;
}

void show(const std::vector<int> & __v, const char __tag, std::vector<int>::iterator it)
{
    printSplitLine(30, '-');
    NOTIFY_LOG(std::string("Number List ") + __tag + " =\n");
    printSplitLine(30, '-');
    for (const int & n : __v)
    {
        if (&n == it.base())
        {
            loger(
                std::cout, CORRECT,
                n, '\t', &n, '\n'
            );
        }
        loger(
            std::cout, ORIGINAL,
            n, '\t', &n, '\n'
        );

        MyLib::MyDelay::delay(30);
    }
    printSplitLine(30, '-');
}