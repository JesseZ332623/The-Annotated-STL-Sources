#ifndef __HEAP_ALGORITHM_H__
#define __HEAP_ALGORITHM_H__

#include <iterator>

/**
 * @brief 往 heap 中插入元素
 * 
 * @tparam RandomAccessIterator     随机访问迭代器
 * @param __first   heap 容器的第一个元素 
 * @param __last    heap 容器的最后一个元素
*/
template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator __first, RandomAccessIterator __last);

template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator __first, RandomAccessIterator __last)
{
    typedef std::ptrdiff_t      distance_type;
    typedef decltype(*__first)  value_type;

    __push_heap_aux(__first, __last, distance_type(__first), value_type(__first));
}

/**
 * @brief  push_heap() 算法的辅助函数
 * 
 * @tparam RandomAccessIterator 
 * @tparam Distance 
 * @tparam Type 
 * 
 * @param __first 
 * @param __last 
*/
template <typename RandomAccessIterator, typename Distance, typename Type>
void __push_heap_aux(
            RandomAccessIterator __first, RandomAccessIterator __last, 
            Distance *, Type *
        )
{
    __push_heap(__first, Distance((__last - __first) - 1), Distance(0), Type(*(__last - 1)));
}

/**
 * @brief __push_heap_aux() 算法的辅助函数
 * 
 * @tparam RandomAccessIterator     随机访问迭代器
 * @tparam Distance                 用于表示节点与节点间的 “距离”
 * @tparam Type                     要插入的值类型
 * 
 * @param __first       heap 容器（通常是 std::vector）下标为 1 的元素的迭代器
 * @param __holdIndex   插入的洞值，通常是 std::vector<Type>::end();
 * @param __topIndex    树的顶端，通常是 0
 * @param __value       要插入的值
*/
template <typename RandomAccessIterator, typename Distance, typename Type>
void __push_heap(
            RandomAccessIterator __first, 
            Distance __holdIndex, Distance __topIndex,
            Type __value
){
    Distance parent = (__holdIndex - 1) / 2;    // 找到洞节点的父节点

    /**
     * 如果还没有到达树的顶端节点，且父节点的值小于要插入的值。
     */
    while (__holdIndex > __topIndex && *(__first + parent) < __value)
    {
        // 先把当前父节点的值换到洞节点去
        *(__first + __holdIndex) = *(__first + parent);

        __holdIndex = parent;               // 更新洞节点的位置到它的父节点
        parent = (__holdIndex - 1) / 2;     // 找到更新后洞节点的父节点
    }
    
    // 所有条件达成后，再把要插入的值赋给合适的节点
    *(__first + __holdIndex) = __value;
}

#endif // __HEAP_ALGORITHM_H__