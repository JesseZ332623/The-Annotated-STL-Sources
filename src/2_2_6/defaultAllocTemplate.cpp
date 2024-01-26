#include "./include/defaultAllocTemplate.h"

template <bool Threads, int Inst>
void * __DefaultAllocTemplate<Threads, Inst>::allcate(std::size_t __n)
{
    Obj * volatile myFreeList;
    Obj * result;

    /*若分配的内存大于 128 bytes，就直接调用第一级分配器*/
    if (__n > (std::size_t)__MAX_BYTES) 
    { 
        return (mallocAlloc::allocate(__n)); 
    }

    /*根据需要分配的内存大小，寻找 16 个 free-list 中合适的一个*/
    myFreeList = (freeList + freeListIndex(__n));

    result = *myFreeList;

    /*若没找到可用的 free-list*/
    if (result == nullptr) 
    { 
        /*准备重新填充自由链表*/
        void * refillPointer = reFill(roundUp(__n));

        return refillPointer;
    }

    /*
        调整 free-list，将链表指针指向已经拨出的节点之后
    */
    *myFreeList = result->freeListLink;

    /*拨出节点，为客端所用*/
    return result;
}