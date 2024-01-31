#include "./include/mallocAllocOomHandler.h"

void ReleaseMemory()
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

void myAllocHandler()
{
    void * result = VirtualAlloc(nullptr, BIG_MEMORY, MEM_COMMIT, PAGE_READWRITE);

    if (result != nullptr) { return; }

    ReleaseMemory();

    result = VirtualAlloc(nullptr, BIG_MEMORY, MEM_COMMIT, PAGE_READWRITE);

    ExitProcess(EXIT_FAILURE);
}