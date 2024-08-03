#include "../include/priority_queue.h"

#include <MyLib/myLogerDef.h>

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    std::vector<std::size_t> tempStrings = {0, 1, 2, 3, 4, 8, 9, 3, 5};

    priority_queue<std::size_t> priQueue(tempStrings.begin(), tempStrings.end());


    system("cls");

    NOTIFY_LOG("priority queue = { ");
    while (!priQueue.empty())
    {
        NOTIFY_LOG(std::to_string(priQueue.top()) + ' ');
        priQueue.pop();
    }

    NOTIFY_LOG("}");
    
    return EXIT_SUCCESS;
}
