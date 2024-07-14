#include <iostream>
#include <list>

#include "../include/stack.h"

int main(int argc, char const *argv[])
{
    stack<int> stack_1;
    stack<int, std::list<int>> stack_2;     // 也可以使用 std::list<int> 作为栈的底层容器。
    int index;
    
    for (index = 0; index < 10; ++index)
    {
        stack_1.push(index);
    }

    while (!stack_1.empty())
    {
        std::cout << stack_1.top() << ' ';
        stack_1.pop();
    }

    for (index = 0; index < 10; ++index)
    {
        stack_2.push(index);
    }

    while (!stack_2.empty())
    {
        std::cout << stack_2.top() << ' ';
        stack_2.pop();
    }

    return EXIT_SUCCESS;
}
