#include "../include/list.h"

#include <MyLib/myLogerDef.h>
#include <algorithm>
#include <list>

template <typename Type>
void showList(const MyList<Type> & __myList, std::string __msg, const char __note);

int main(int argc, char const *argv[])
{
    system("cls");

    int array[] = {5, 45, 567, 5, 134, 67};

    MyList<int> list_1(array, array + 6);
    MyList<int> list_2(5, 0x7FFFFFFF);
    
    showList(list_1, "list_1 = ", ' ');
    showList(list_2, "list_2 = ", ' ');
    
    list_1.splice(list_1.end(), list_2, list_2.begin(), list_2.end());

    showList(list_1, "list_1 = ", ' ');
    showList(list_2, "list_2 = ", ' ');

    return EXIT_SUCCESS;
}

template <typename Type>
void showList(const MyList<Type> & __myList, std::string __msg, const char __note)
{
    using namespace MyLib::MyLoger;

    NOTIFY_LOG("List size = [" + std::to_string(__myList.size()) + "]\n");

    auto tempIter = __myList.begin();

    if (!__msg.empty()) { CORRECT_LOG(__msg + "{"); }

    if (__myList.empty()) { CORRECT_LOG("}\n\n"); return; }

    for (const Type & n : __myList) 
    { 
        ++tempIter;
        CORRECT_LOG(n);

        if (tempIter != __myList.end()) { std::cout.put(__note); }
    }

    CORRECT_LOG("}\n\n");
}