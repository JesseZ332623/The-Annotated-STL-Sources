#include "./include/defaultAllocTemplate.h"

template <bool Threads, int Inst>
void * __DefaultAllocTemplate<Threads, Inst>::reFill(std::size_t __n)
{
    /*默认要填充的节点数*/
    int defaultNode = 20;

    /*
        调用 chunkAlloc 去内存池中获取 defaultNode 个大小为 __n 的节点，返回地址给 chunk

        - tips: 该函数的第二个参数按引用传递，因此不能直接写个右值进去。
    */
    char * chunk = (char *)chunkAlloc(__n, defaultNode);

    /*填充空间前的目标链表地址*/
    Obj * volatile myFreeList;
    Obj * result;
    Obj * currentObj, *nextObj;
    int listIndex;

    /*若只填充了一个节点，直接交给客端即可*/
    if (defaultNode == 1) { return (Obj *)chunk; }

    /*获取 填充空间前的目标链表地址*/
    myFreeList = freeList + freeListIndex(__n);

    /*
        将在 chunk 指向的空间内建立 free-list，
        因此 result 指向了填充而来的内存的起始位置
    */
    result = (Obj *)chunk;

    /*
        引导 free-list 指向新填充的空间

        - *myFreeList 指向了由 chunkAlloc 创建的节点中的第一个节点（节点 "下标" 从 0 开始）

        - nextObj 和 *myFreeList 一样，指向了由 chunkAlloc 创建的节点中的第一个节点（节点 "下标" 从 0 开始）
    */
    *myFreeList = nextObj = (Obj *)(chunk + __n);

    /*
        通过循环，在这片内存中建立起一张单向链表，并重新交由 *myFreeList 管理。
    */
    for (listIndex = 1; ; ++listIndex)
    {
        /*当前节点指向了下一个节点*/
        currentObj = nextObj;

        /*下一个节点，指向了它的下一个节点*/
        nextObj = (Obj *)((char *)nextObj + __n);

        /*当所有的节点都连接完毕时，把最后一个节点的 next 指针指向空*/
        if (defaultNode - 1 == listIndex)
        {
            currentObj->freeListLink = nullptr;
            break;
        }
        else
        {
            /*当前节点的 next 指针指向下一个节点*/
            currentObj->freeListLink = nextObj;
        }
    }

    return result;
}

template <bool Threads, int Inst>
void * __DefaultAllocTemplate<Threads, Inst>::chunkAlloc(std::size_t __size, int & __nodeCount)
{

}