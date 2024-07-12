#include <iostream>

#include "../include/stack.h"

int main(int argc, char const *argv[])
{
    stack<int> stack_1;
    int index;
    
    for (index = 0; index < 1000; ++index)
    {
        stack_1.push(index);
    }

    while (!stack_1.empty())
    {
        std::cout << stack_1.top() << ' ';
        stack_1.pop();
    }

    return EXIT_SUCCESS;
}
