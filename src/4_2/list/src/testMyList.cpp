#include "../include/list.h"

#include <MyLib/myLogerDef.h>
#include <algorithm>

template <typename Type>
void showList(const MyList<Type> & __myList, const char __note);

int main(int argc, char const *argv[])
{
    system("cls");

    MyList<std::string> stringList;

    stringList.push_back("123123123");
    stringList.push_back("This is a test string.");
    stringList.push_back("This is a test string.");
    stringList.push_back("Try one's best.");
    stringList.push_back("This is a worse world.");
    stringList.push_back("Why are you billy me??");
    stringList.push_back("Do you like van you see~");
    stringList.push_back("Oh my fucking god!");

    showList(stringList, '\n');

    stringList.unique();

    showList(stringList, '\n');

    return EXIT_SUCCESS;
}

template <typename Type>
void showList(const MyList<Type> & __myList, const char __note)
{
    using namespace MyLib::MyLoger;

    printSplitLine(45, '-');

    NOTIFY_LOG("List size = [" + std::to_string(__myList.size()) + "]\n");

    auto tempIter = __myList.begin();

    for (const Type & n : __myList) 
    { 
        ++tempIter;
        CORRECT_LOG(n);

        if (tempIter != __myList.end()) { std::cout.put('\n'); }
    }

    std::cout.put('\n'); 
    printSplitLine(45, '-');
}