#ifndef __DEFAULT_ALLOC_TEMPLATE_H_
#define __DEFAULT_ALLOC_TEMPLATE_H_

#include <cstddef>
#include <cstring>
#include "./mallocAllocTemplate.h"

#define SGI_METHODS true
#define MY_METHODS false

/*第二级 SGI 配置器 __DEFAULT_ALLOC_TEMPLATE_H_ 刨析*/

/*
    在 SGI 的分配器实现中，针对分配较小内存（通常是小于 128 字节）会带来很多内存碎片造成额外开销的问题，
    设计了有别于第一级配置器的第二级配置器，它通常会申请一大块内存池，并维护与之对应的 free-list。
    若下次还有相同的内存需求，直接从自由链表中划出，如果使用者释放内存块，就重新回收到自由链表中。

    此外，为了方便管理，第二级配置器会主动将任何小额的内存需求量上调至 8 的倍数（比如 要申请 30 字节，就对齐到 32 字节），
    并维护 16 个 free-list，分别对应 8，16，24，32，40，48，56，64，72，80，88，96，104，112，120，128 字节的小额区块。

    由于使用联合体（union）的原因，维护链表不会因为需要额外的指针而浪费内存。
*/

namespace SGIAllocator
{

enum {__ALIGN = 8};                             // 小型区块的上调边界              
enum {__MAX_BYTES = 128};                       // 小型区块的上限
enum {__NFPREELISTS = __MAX_BYTES / __ALIGN};   // free-list 数量

/**
 * @tparam Threads  用于多线程，当前不讨论这种情况
 * @tparam Inst     可能用于后面的具体化，但是目前完全没排上用场
*/
template <bool Threads, int Inst>
class __DefaultAllocTemplate
{
    private:
        /**
         * @brief           该函数将内存对齐至 8 的倍数
         * 
         * @param __bytes   传入的字节数
         * 
         * @return          对齐后的字节数
        */
        static std::size_t roundUp(std::size_t __bytes) 
        { 
            /*
                这条语句拆解分析如下:
                1. ((__bytes) + __ALIGN - 1)    在原始字节数的基础上加上 7，目的是为了离对齐边界更近一些
                2. ~(__ALIGN - 1)               取反 0x00000007 的所有位 为 0xFFFFFFF8
                3. 最后将 （1）的值和（2）的值进行与运算，得到比这个数对齐后的字节数（如 119 对齐到 120，98 对齐到 104）
            */
            return ((__bytes) + __ALIGN - 1) & ~(__ALIGN - 1); 
        }

    private:
        union Obj           // free-list 节点的构成
        {
            union Obj * freeListLink;       // 指向下一个节点的指针
            char clientData[1];             // 指向一个实际区块的指针
        };
    
    private:
        /*
            16 个持有不同大小内存的自由链表，
            考虑到多线程环境，使用 volatile 关键字避免编译器做出不正确的优化。
        */
        static Obj * volatile freeList[__NFPREELISTS];

        /**
         * @brief           通过传入的字节数，计算需要调用哪一个大小的链表（下标从 0 开始）
         * 
         * @param __bytes   传入的字节数
         * 
         * @return          返回需要调用第几号链表
        */
        static std::size_t freeListIndex(std::size_t __bytes) 
        { 
#if SGI_METHODS
            /*
                这条语句分析拆解如下：
                1. (((__bytes) + __ALIGN - 1)       将传入的字节数和 7 相加，使其更接近对齐后的内存
                2. ... / __ALIGN                    计算（1）得出的数据是 8 的 n 倍（小数点会被截断）
                3. ... - 1                          由于下标从 0 开始，因此需要 - 1
            */
            return (((__bytes) + __ALIGN - 1) / __ALIGN - 1); 
#elif MY_METHODS    

            /**
             * 而我的实现会直截了当一些，
             * 直接调用 roundUp() 对齐内存，然后除以 8 再 - 1 得到下标（显然，在频繁调用下，没有 SGI 实现的有效率）
            */
            return roundUp(__bytes) / __ALIGN - 1;
#endif
        }

        /**
         * @brief 当 allcate() 发现 free-list 中没有可用区块时，就调用 reFill()，准备为链表重新填充空间。
         * 
         * @brief - 新的空间将来自 Memory Pool（由 chunkAlloc() 调用）。默认获取 20 个大小为 __n 的节点。
         * 
         * @param   __n 要分配的单个节点的大小，默认 __n 已经经过对齐。
         * 
         * @return 一个指向了大小为 __n 的内存的指针，通常拿给客端去调用。
        */
        static void * reFill(std::size_t __n)
        {
            /*默认要填充的节点数*/
            int defaultNodeCount = 20;

            /*
                调用 chunkAlloc 去内存池中获取 defaultNode 个大小为 __n 的节点，返回地址给 chunk

                - tips: 该函数的第二个参数按引用传递，因此不能直接写个右值进去。
            */
            char * chunk = (char *)chunkAlloc(__n, defaultNodeCount);

            /*填充空间前的目标链表地址*/
            Obj * volatile *myFreeList;
            Obj * result;
            Obj * currentObj, *nextObj;
            int listIndex;

            /*若只填充了一个节点，直接交给客端即可*/
            if (defaultNodeCount == 1) { return (Obj *)chunk; }

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
                if (defaultNodeCount - 1 == listIndex)
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

        /**
         * @brief               从内存池中取出空间给 free-list 使用
         * 
         * @param __size        单个节点的大小
         * @param __nodeCount   要从内存池中拿出的总节点数
         * 
         * @return              返回以链表形式组织的内存，交由一个通用指针管理
        */
        static void * chunkAlloc(std::size_t __size, int & __nodeCount)
        {
            char * result;
            std::size_t totalBytes = __size * __nodeCount;                  // 需要为 free-list 分配的总字节数
            std::size_t remainingBytes = endFreeList - startFreeList;       // Memory Pool 的剩余空间

            // 若剩余空间充足（Memory Pool 的剩余空间 >= 需要为 free-list 分配的总字节数）
            if (remainingBytes >= totalBytes)
            {
                /*
                    将 result 指向链表的第一个节点，
                    再将 startFreeList 偏移到下一个节点，最后返回第一个节点的指针给 reFill()
                */
                result = startFreeList;
                startFreeList += totalBytes;

                return result;
            }
            // 若剩余空间不能完全满足需求量，但足够供应 1 个或以上的区块（Memory Pool 的剩余空间 >= 单个节点的大小）
            else if (remainingBytes >= __size)
            {
                // 改变 __nodeCount 的值（Memory Pool 的剩余空间 / 单个节点的大小）
                __nodeCount = remainingBytes / __size;

                // 重新计算需要分配的总字节数
                totalBytes = __size * __nodeCount;

                /*
                    将 result 指向链表的第一个节点，
                    再将 startFreeList 偏移到下一个节点，最后返回第一个节点的指针给 reFill()
                */
                result = startFreeList;
                startFreeList += totalBytes;

                return result;
            }
            // 内存池的剩余空间连一个区块的大小都无法提供
            else
            {
                // 需要往内存池中填充的字节数 = 2 * 需要为 free-list 分配的总字节数 + (堆空间大小 / 2 ^ 4 再对齐至 8 的倍数)
                std::size_t bytesToGet = 2 * totalBytes + roundUp(heapSize >> 4);

                // 让内存池中的零头空间还有利用价值
                if (remainingBytes > 0)
                {
                    // 寻找合适的 free-list
                    Obj * volatile * myFreeList = freeList + freeListIndex(remainingBytes);

                    // 调整 free-list 将内存池中残存的空间编入
                    ((Obj *)startFreeList)->freeListLink = *myFreeList;
                    *myFreeList = (Obj *)startFreeList;
                }

                // 配置 heap 空间，来补充内存池
                startFreeList = (char *)malloc(bytesToGet);

                if (startFreeList == nullptr)
                {
                    int i;
                    Obj * volatile * myFreeList, * ptr;

                    for (i = __size; i <= __MAX_BYTES; i += __ALIGN)
                    {
                        myFreeList = freeList + freeListIndex(i);
                        ptr = *myFreeList;

                        if (ptr != nullptr)
                        {
                            *myFreeList = ptr->freeListLink;
                            startFreeList = (char *)ptr;
                            endFreeList = startFreeList + i;

                            return chunkAlloc(__size, __nodeCount);
                        }
                    }
                    endFreeList = nullptr;
                    startFreeList = (char *)mallocAlloc::allocate(bytesToGet);
                }
                heapSize += bytesToGet;
                endFreeList = startFreeList + bytesToGet;

                return chunkAlloc(__size, __nodeCount);
            }
        }

        static char * startFreeList;
        static char * endFreeList;
        static std::size_t heapSize;

    public:
        /**
         * @brief 空间配置函数 allcate，传入需要配置的空间大小，
         * 根据是否大于 128 bytes 来决定分配器的使用。
         * 
         * @brief - 大于 128 bytes，直接调用一级分配器，使用 malloc 分配内存，具体实现在 `./mallocAllocTemplate.h`
         * 
         * @brief - 小于 128 bytes，检查对应的 free-list，
         *           若链表内由可用的区块，就直接从表中拨出，若没有可用区块，就将区块大小上调至 8 倍数边界，
         *           然后调用 reFill()，准备为 free-list 重新填充空间。
         * 
         * @param __n   要分配的内存大小
         * 
         * @return      分配完成后内存块的首地址
        */
        static void * allocate(std::size_t __n)
        {
            Obj * volatile * myFreeList;
            Obj * result;

            /*若分配的内存大于 128 bytes，就直接调用第一级分配器*/
            if (__n > (std::size_t)__MAX_BYTES) { return (mallocAlloc::allocate(__n)); }

            /*根据需要分配的内存大小，寻找 16 个 free-list 中合适的一个*/
            myFreeList = freeList + freeListIndex(__n);

            result = *myFreeList;

            /*若没找到可用的 free-list*/
            if (result == nullptr) 
            { 
                /*准备重新填充自由链表*/
                void * refillPointer = reFill(roundUp(__n));

                return refillPointer;
            }

            /*调整 free-list，将链表指针指向已经拨出的节点之后*/
            *myFreeList = result->freeListLink;

            /*拨出节点，为客端所用*/
            return result;
        }

        /**
         * @brief 空间释放函数 deallocate，传入需要释放的空间大小，
         * 根据是否大于 128 bytes 来决定调用哪一级分配器。
         * 
         * @brief 1. 大于 128 bytes，直接调用一级分配器，将这块内存 free，具体实现在 ./mallocAllocTemplate.h
         * 
         * @brief 2. 小于 128 bytes，调用第二级分配器，找出要对应的 free-list，并将其回收。
         * 
         * @param __ptr     要释放的内存块起始地址
         * @param __n       要释放的内存块大小
        */
        static void deallocate(void * __ptr, std::size_t __n)
        {
            /*获取要释放的内存的地址，存于 tempNodePointer 中*/
            Obj * tempNodePointer = (Obj *)__ptr;

            Obj * volatile * myFreeList;
            
            /*若要释放的内存块大于 128 bytes，调用第一级配置器*/
            if (__n > (std::size_t)__MAX_BYTES)
            {
                mallocAlloc::deallocate(__ptr, __n);
            }

            /*
                寻找合适的链表
                调用 freeListIndex() 计算合适的下标，然后从链表数组 freeList 中偏移，
                将结果保存在 myFreeList 中。
            */
            myFreeList = freeList + freeListIndex(__n);

            /*
                将客端的内存归还给这个链表

                1. 将要释放内存节点的指针指向当前链表的表头。
                   此时指针 tempNodePointer 对应的节点就成了该链表新的头节点。

                2. 最后将原链表头节点的指针指向新加入的节点（头插法），完成内存的回收。
            */
            tempNodePointer->freeListLink = *myFreeList;
            *myFreeList = tempNodePointer;
        }

        /**
         * @brief 为指定的内存块重新分配内存（这种实现或许有一定风险）
         * 
         * @param __ptr         目标内存块地址
         * @param __oldSize     旧内存块大小
         * @param __newSize     新内存块大小
         * 
         * @return              扩容完成的内存块地址
        */
        static void * reallocate(void * __ptr, std::size_t __oldSize, std::size_t __newSize)
        {
            /*若新扩展的内存大于 128 字节，直接调用第一级分配器在堆上分配内存，而不去直接操作链表*/
            if (__newSize > __MAX_BYTES) { return mallocAlloc::reallocate(__ptr, __oldSize, __newSize); }

            /*根据新的内存重新选择链表并划出内存交由 newNodePointer 管理*/
            void * newNodePointer = allocate(__newSize);

            /*根据新旧内存的大小确定要拷贝内存的大小*/
            std::size_t copySize = (__oldSize < __newSize) ? __oldSize : __newSize;

            /*直接拷贝内存到新分配的链表中（这个操作或许相当危险，回产生内存碎片）*/
            std::memcpy(newNodePointer, __ptr, copySize);

            /*归还旧数据*/
            deallocate(__ptr, __oldSize);

            return newNodePointer;
        }
};

template <bool Threads, int Inst>
char * __DefaultAllocTemplate<Threads, Inst>::startFreeList = nullptr;

template <bool Threads, int Inst>
char * __DefaultAllocTemplate<Threads, Inst>::endFreeList = nullptr;

template <bool Threads, int Inst>
std::size_t __DefaultAllocTemplate<Threads, Inst>::heapSize = 0;

template <bool Threads, int Inst>
typename __DefaultAllocTemplate<Threads, Inst>::Obj * volatile
__DefaultAllocTemplate<Threads, Inst>::freeList[__NFPREELISTS] = {nullptr};
}

typedef SGIAllocator::__DefaultAllocTemplate<false, 0> sgiAlloc;

#endif // __DEFAULT_ALLOC_TEMPLATE_H_