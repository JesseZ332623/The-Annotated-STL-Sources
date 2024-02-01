#ifndef __MALLOC_ALLOC_TEMPLATE_H_
#define __MALLOC_ALLOC_TEMPLATE_H_

#include "./mallocAllocOomHandler.h"

// 第一级配置器 __MALLOC_ALLOC_TEMPLATE_H_

// 第一级配置器不抛 bad_alloc 异常
#if false
#include <new>
#define __THROW_BAD_ALLOC_  throw bad_alloc

/* 
    若没有定义 __THROW_BAD_ALLOC_ 宏，
    则包含标准 I/O 流，使用 cerr 越过缓冲区向标准错误发送内存耗尽的消息，并直接失败的退出程序。
*/
#elif !defined(__THROW_BAD_ALLOC_)
#include <iostream>
#define __THROW_BAD_ALLOC_ std::cerr << "Out of Memory." << '\n'; exit(EXIT_FAILURE);
#endif

namespace SGIAllocator
{
    template <int inst>
    class __Malloc_Alloc_Template
    {
        private:
            // 以下的两个函数和一个函数指针用于处理 OOM（Out of Memory）的情况。

            /**
             * @brief 在内存不足时用于分配内存的函数
            */
            static void *oomMalloc(std::size_t);

            /**
             * @brief 在内存不足时对依旧分配的内存进行扩容或缩减的函数
            */
            static void *oomRealloc(void *, std::size_t);

            /**
             * @brief 一个函数指针，指向了一个无参且无返回值的函数，
             *        一般用来保存默认内存耗尽函数的地址，这个函数会在内存不足时试图向系统请求更多的内存。
            */
            static void (* __mallocAllocOomHandler) ();

        public:

            /**
             * @brief       第一级配置器 allocate 针对内存块较大的情况，直接调用 malloc 去分配内存
             * 
             * @param __n   要分配内存的大小
             * 
             * @return      返回分配完成后内存块的首地址
            */
            static void * allocate(std::size_t __n) 
            {
                void * result = std::malloc(__n);

                /*若 malloc 返回空指针，意味着内存耗尽，转而使用在内存耗尽时分配内存的策略。*/
                if (result == nullptr) { result = oomMalloc(__n); }

                return result;
            }

            /**
             * @brief           在第一级配置器直接使用 free 释放内存块
             * 
             * @param   __ptr   要释放的内存块起始地址
             * @param   __n     要释放内存块的大小，一般这个用不到，内存块都会有一个头部信息来记录块的大小
             * 
             * @return non-return
            */
            static void deallocate(void * __ptr, std::size_t __n = 0) { free(__ptr); }

            /**
             * @brief           第一级配置器 reallocate 针对内存块较大的情况，直接调用 realloc 去重新分配内存
             * 
             * @param   __ptr   要重新分配内存块的起始地址
             * @param   __oldSz 旧数据块大小，这个由于内存块的头部信息，基本上用不到
             * @param   __newSz 要开辟的新内存块大小
             * 
             * @return      返回分配完成后内存块的首地址
            */
            static void * reallocate(void * __ptr, std::size_t __oldSz, std::size_t __newSz)
            {
                void * result = realloc(__ptr, __newSz);

                /*若 realloc 返回空指针，意味着内存耗尽，转而使用在内存耗尽时分配内存的策略。*/
                if (result == nullptr) { result =  oomRealloc(__ptr, __newSz); }

                return result;
            }

            /**
             * @brief 一个名为 setMallocHandler 的函数，它有一个指向无参、无返回值函数的指针形参 __func
             *         setMallocHandler 自己的返回值也是一个指向无参、无返回值函数的指针。
             *         用户可以通过该函数指定自己的 oomHandler
             * 
             * @param __func 一个指向无参、无返回值函数的指针
             * 
             * @return 一个指向无参、无返回值函数的指针       
            */
            static void (*setMallocHandler(void (* __func) ())) ()
            {
                /*保存默认 oomHandler 函数的地址*/
                void (*defaultHandler)() = __mallocAllocOomHandler;

                /*切换到 用户指定的 oomHandler 函数*/
                __mallocAllocOomHandler = __func;

                /*返回默认 oomHandler 函数的地址，用于还原*/
                return defaultHandler;
            }
    };

    /*
        初始化 oomHandler 为自己设计的在 Windows 平台下内部不足的一个简单处理例程。
    */
    template <int inst>
    void (*__Malloc_Alloc_Template<inst>::__mallocAllocOomHandler)() = myAllocHandler;

    /**
        @brief oomMalloc函数:

        它在内存分配失败时会被调用，以试图获取更多内存。

        一开始它会保存当前的 __mallocAllocOomHandler 函数到 myAllocHandler。

        然后检查 __mallocAllocOomHandler 是否为空。如果为空，直接抛出异常。

        如果 __mallocAllocOomHandler 不为空，就调用该函数，让它来尝试释放或获取更多内存，接着再次尝试 malloc 分配内存。
        如果分配成功，返回结果。如果还是失败，就会在 while 循环里再次重试。
    */
    template <int inst>
    void * __Malloc_Alloc_Template<inst>::oomMalloc(std::size_t __n)
    {
        void (* myAllocHandler) ();
        void * result;

        while (true)
        {
            /*拿到用户或系统定义的内存不足处理函数的指针*/
            myAllocHandler = __mallocAllocOomHandler;

            /*若这个函数指针指向了空，意味着程序没有做好应对内存不足的情况，直接抛异常或者输出错误信息再退出即可。*/
            if (myAllocHandler == nullptr) { __THROW_BAD_ALLOC_; }

            /*如果用户定义了这样的函数，就调用它试图向操作系统获取更多的内存*/
            (*myAllocHandler)();

            /*再次调用 malloc 去分配*/
            result = malloc(__n);

            /*直到成功请求并分配内存后，返回这块已分配内存的起始地址*/
            if (result) { return result; }

        } // 不断的重复这个过程
    }

    /**
     * @brief 和 oomMalloc 函数类似，只是用于重新分配内存
    */
    template <int inst>
    void * __Malloc_Alloc_Template<inst>::oomRealloc(void *__ptr, std::size_t __n)
    {
        void (* myAllocHandler) ();
        void * result;

        while (true)
        {
            myAllocHandler = __mallocAllocOomHandler;
            if (myAllocHandler == nullptr) { __THROW_BAD_ALLOC_; }

            (*myAllocHandler) ();

            result = realloc(__ptr, __n);

            if (result) return (result);
        }
    }
}

/*具体化这个模板类，使用 mallocAlloc 来代替 __Malloc_Alloc_Template<0>*/
typedef SGIAllocator::__Malloc_Alloc_Template<0> mallocAlloc;


#endif // __MALLOC_ALLOC_TEMPLATE_H_