#include "../include/list.h"

#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>
#include <algorithm>
#include <random>

//#include <list>

template <typename Type>
void showList(const MyList<Type> & __myList, const std::string __msg, const char __note);

int main(int argc, char const *argv[])
{
    system("cls");

    using namespace MyLib::SimpleContainerOperator;
    using namespace MyLib::MyLoger;

    std::random_device  randDeveice;
    std::mt19937        randomEngine(randDeveice());
    std::uniform_int_distribution<> distribution(0, 170000);
    
    MyList<int> testList_1(15);
    MyList<std::string> testList_2(15, "Hello");

    for (auto beginIt = testList_1.begin(); beginIt != testList_1.end(); ++beginIt)
    {
        *beginIt = distribution(randomEngine);
    }  

    showList(testList_1, "testLst_1: ", ' ');
    showList(testList_2, "testLst_2: ", ' ');


    auto tList2Iter = testList_2.begin();
    std::advance(tList2Iter, 5);

    testList_2.insert(tList2Iter, "123");

    showList(testList_1, "testLst_1: ", ' ');
    showList(testList_2, "testLst_2: ", ' ');

    DONE

    return EXIT_SUCCESS;
}

template <typename Type>
void showList(const MyList<Type> & __myList, const std::string __msg, const char __note)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::MyDelay;

    NOTIFY_LOG("List size = [" + std::to_string(__myList.size()) + "]\n");

    auto tempIter = __myList.begin();

    if (!__msg.empty()) { CORRECT_LOG(__msg + "{"); }

    if (__myList.empty()) { CORRECT_LOG("}\n\n"); return; }

    for (const Type & n : __myList) 
    { 
        ++tempIter;
        NOTIFY_LOG(n);
        delay(45);

        if (tempIter != __myList.end()) { std::cout.put(__note); }
    }

    CORRECT_LOG("}\n\n");
}