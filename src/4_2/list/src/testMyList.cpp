#include "../include/list.h"

#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>
#include <algorithm>

//#include <list>

template <typename Type>
void showList(const MyList<Type> & __myList, const std::string __msg, const char __note);

int main(int argc, char const *argv[])
{
    system("cls");

    using namespace MyLib::SimpleContainerOperator;

    int array[] = {5, 478456, 2356, 4, 546234, 32, 1021, 1101};
    int array_2[] = {1, 8, 9, 12};

    MyList<int> list_1(array, array + 8);
    MyList<int> list_2(array_2, array_2 + 4);
    
    showList(list_1, "list_1 = ", ' ');
    showList(list_2, "list_2 = ", ' ');

    list_1.sort();

    showList(list_1, "list_1 = ", ' ');

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
        CORRECT_LOG(n);
        delay(45);

        if (tempIter != __myList.end()) { std::cout.put(__note); }
    }

    CORRECT_LOG("}\n\n");
}