#ifndef _SIMPLE_ALLOCATOR_H_
#define _SIMPLE_ALLOCATOR_H_

#include "./defaultAllocTemplate.h"

/*标准 SGI 分配器接口*/
namespace SimpleAllocate
{
    /**
     * @brief 将 SGI 分配器，即 `./mallocAllocTemplate.h` 和 `./defaultAllocTemplate.h` 的接口进行再次封装，使之符合 STL 标准。
     * 
     * @brief - 类内这四个成员方法都是单纯的转调用
     * @brief - 调用传递给配置器（可能是第一级也可能是第二级）的成员函数
     * @brief - 该接口使配置器的配置单位从 bytes 转化为各别元素的大小
     * 
     * @tparam  __Type  要进行分配的内存类型
     * @tparam  __Alloc 要使用的分配器类型
    */
    template <typename __Type, typename __Alloc>
    class SimpleAllocator
    {
        public:
            static __Type *allocate(std::size_t __n) { return (__n == 0) ? nullptr : (__Type *)__Alloc::allocate(__n); }

            static __Type *allocate(void) { return (__Type *)__Alloc::allocate(sizeof(__Type)); }

            static void deallocate(__Type *__buffer, std::size_t __n) { if (__n != 0) { __Alloc::deallocate(__buffer, __n); } }

            static void deallocate(__Type *__buffer) { __Alloc::deallocate(__buffer, sizeof(__Type)); }
    };

} // end SimpleAllocator

#endif // _SIMPLE_ALLOCATOR_H_
