#ifndef __SHOW_CONTAINER_H__
#define __SHOW_CONTAINER_H__

#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

#ifndef NEED_PTR
#define NEED_PTR false
#endif

using namespace MyLib::MyLoger;
using namespace MyLib::SimpleContainerOperator;

static auto showStdContainElement = 
            [](const short n) { CORRECT_LOG(n); putchar(' '); };

template <typename Type>
void showContain(const Type * __begin, const Type * __end, const std::string & __describe);

template <typename Type>
void showContain(const Type * __begin, const Type * __end, const std::string & __describe)
{
    if (!__begin || !__end) {
        throw std::invalid_argument("showContain(): Fanction argument is nullptr!\n");
    }

    NOTIFY_LOG(__describe + '\n');
    while (__begin != __end)
    {
        CORRECT_LOG(*__begin); putchar(' ');

#if NEED_PTR      
        putchar('\t');
        CORRECT_LOG(__begin);
        putchar('\n');
#endif

        ++__begin;
    }
    putchar('\n');
    printSplitLine(30, '-');
}

#endif // __SHOW_CONTAINER_H__