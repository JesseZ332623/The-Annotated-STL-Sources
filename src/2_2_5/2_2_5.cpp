#include "./include/2_2_5.h"

#include <windef.h>
#include <winbase.h>
#include <vector>
#include <ctime>

#define BIG_MEMORY 1024 * 1024 * 100

static void ReleaseMemory();
static void myAllocHandler();

template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f);

static void ReleaseMemory()
{
    HANDLE hProcess = GetCurrentProcess();
    PMEMORY_BASIC_INFORMATION pmem = nullptr;
    PVOID hightestAddress = (PVOID)0x7FFFFFFFFFFF;
    MEMORY_BASIC_INFORMATION mbi; 

    while (VirtualQueryEx(hProcess, pmem, &mbi, sizeof(mbi)))
    {
        if ((mbi.State == MEM_COMMIT) && (mbi.RegionSize > BIG_MEMORY) && (mbi.Protect = PAGE_READWRITE))
        {
            VirtualFreeEx(hProcess, mbi.BaseAddress, mbi.RegionSize, MEM_DECOMMIT);
            break;
        }
    }
}

static void myAllocHandler()
{
    void * result = VirtualAlloc(nullptr, BIG_MEMORY, MEM_COMMIT, PAGE_READWRITE);

    if (result != nullptr) { return; }

    ReleaseMemory();

    result = VirtualAlloc(nullptr, BIG_MEMORY, MEM_COMMIT, PAGE_READWRITE);

    ExitProcess(EXIT_FAILURE);
}

template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f)
{
    /*
        使用迭代器遍历整个容器，对容器内的每一个元素都执行 f 操作。
    */
    for (; first != last; ++first) { f(*first); }

    /*返回操作函数，这样可以支持链式调用*/
    return f;
}

class testClass
{
    private:
        std::vector<double> dataArray;

    public:
        testClass(const double* __begin, const double* __end) : dataArray(__begin, __end) {}

        friend std::ostream & operator<<(std::ostream & __os, testClass & __t)
        {
            for (const double n : __t.dataArray) { __os << n << ' '; }
            return __os;
        }

        ~testClass() {}
};

int main(int argc, char const *argv[])
{
    mallocAlloc myMallocAlloc;

    myMallocAlloc.setMallocHandler(myAllocHandler);
    std::srand((unsigned int)std::time(nullptr));

    int *mydat = (int *)myMallocAlloc.allocate(sizeof(int));

    *mydat = 114514;

    printf("%d\n", *mydat);

    mydat = (int *)myMallocAlloc.reallocate(mydat, 0, sizeof(int) * 5);

    for (int index = 0; index < 5; ++index) { mydat[index] = std::rand() % 114514 + 1; }

    for_each(mydat, mydat + 5, [](const int n) { printf("%d ", n); });

    myMallocAlloc.deallocate(mydat);

    return EXIT_SUCCESS;
}
