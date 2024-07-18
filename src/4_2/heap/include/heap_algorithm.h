#ifndef __HEAP_ALGORITHM_H__
#define __HEAP_ALGORITHM_H__

#include <iterator>

/**
 * @brief 往 heap 中插入元素
 * 
 * @tparam RandomAccessIterator     随机访问迭代器
 * @param __first   heap 容器的第一个元素 
 * @param __last    heap 容器的最后一个元素
 * 
 * @return no-return
*/
template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator __first, RandomAccessIterator __last);

/**
 * @brief 移除 heap 的元素（移除 heap 的根节点，也就是权值最高的节点）
 * 
 * @tparam RandomAccessIterator     随机访问迭代器
 *
 * @param __first   heap 容器的第一个元素 
 * @param __last    heap 容器的最后一个元素
 * 
 * @return no-return
 */
template <typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator __first, RandomAccessIterator __last);

/**
 * @brief 给整个 Heap 排序，目前只能实现降序的排序
 * 
 * @tparam RandomAccessIterator 
 * @param __first 
 * @param __last 
 */
template <typename RandomAccessIterator>
inline void sort_heap(RandomAccessIterator __first, RandomAccessIterator __last);

/**
 * @brief 构建一个 Heap
 * 
 * @tparam RandomAccessIterator 
 * @param __first 
 * @param __last 
 */
template <typename RandomAccessIterator>
void make_heap(RandomAccessIterator __first, RandomAccessIterator __last);


template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator __first, RandomAccessIterator __last)
{
    typedef std::ptrdiff_t      distance_type;
    typedef decltype(*__first)  value_type;

    __push_heap_aux(__first, __last, distance_type(__first), value_type(__first));
}

template <typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator __first, RandomAccessIterator __last)
{
    typedef decltype(*__first)  value_type;

    __pop_heap_aux(__first, __last, value_type(__first));
}

template <typename RandomAccessIterator>
inline void sort_heap(RandomAccessIterator __first, RandomAccessIterator __last)
{
    while ((__last - __first) > 1) { pop_heap(__first, __last--); }
}

template <typename RandomAccessIterator>
void make_heap(RandomAccessIterator __first, RandomAccessIterator __last)
{
    typedef std::ptrdiff_t          distance_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type  value_type;


    __make_heap(__first, __last, value_type(__first), distance_type(__first));
}

/**
 * @brief  push_heap() 算法的辅助函数
 * 
 * @tparam RandomAccessIterator 
 * @tparam Type 
 * 
 * @return no-return
*/
template <typename RandomAccessIterator, typename Type>
void __push_heap_aux(
            RandomAccessIterator __first, RandomAccessIterator __last, Type *
        )
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type  value_type;
    typedef std::ptrdiff_t                                          distance_type;

    __push_heap(__first, Distance((__last - __first) - 1), distance_type(0), Type(*(__last - 1)));
}

/**
 * @brief pop_heap 的辅助函数
 * 
 * @tparam RandomAccessIterator 
 * @tparam Distance 
 * @tparam Type 
 * 
 * @param __first 
 * @param __last 
 * @param __result 
 * @param __value 
 * 
 * @return no-return
 */
template <typename RandomAccessIterator, typename Distance, typename Type>
void __pop_heap_aux(
                    RandomAccessIterator __first, RandomAccessIterator __last, Type *
                )
{

    typedef std::ptrdiff_t      distance_type;

    __pop_heap(__first, __last - 1, Type(*(__last - 1)), distance_type(__first));
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
 * 
 * @return no-return
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

/**
 * @brief __pop_heap_aux() 的辅助函数
 * 
 * @tparam RandomAccessIterator 
 * @tparam Type 
 * @tparam Distance 
 * 
 * @param __first 
 * @param __last 
 * @param __result 
 * @param __value 
 * 
 * @return no-return
 */
template <typename RandomAccessIterator, typename Type, typename Distance>
void __pop_heap(
        RandomAccessIterator __first, RandomAccessIterator __last, RandomAccessIterator __result,
        Type __value, Distance *
    )
{
    *__result = *__first;

    __adjust_heap(__first, Distance(0), Distance(__last - __first), __value);
}

/**
 * @brief __pop_heap() 的辅助函数
 * 
 * @tparam RandomAccessIterator 
 * @tparam Distance 
 * @tparam Type 
 * @param __first 
 * @param __holdIndex 
 * @param __len 
 * @param __value 
 * 
 * @return no-return
 */
template <typename RandomAccessIterator, typename Distance, typename Type>
void __adjust_heap(
                    RandomAccessIterator __first, 
                    Distance __holdIndex, Distance __len, 
                    Type __value
                )
{
    Distance topIndex = __holdIndex;
    Distance secondChlild = 2 * __holdIndex + 2;

    while (secondChlild < __len)
    {
        if (*(__first + secondChlild) < *(__first + (secondChlild - 1)))
        {
            --secondChlild;
        }

        *(__first + __holdIndex) = *(__first + secondChlild);
        __holdIndex = secondChlild;
        secondChlild = 2 * (secondChlild + 1);
    }

    if (secondChlild = __len)
    {
        *(__first + __holdIndex) = *(__first + (secondChlild - 1));
        __holdIndex = secondChlild - 1;
    }

    //__push_heap(__first, __holdIndex, topIndex, __value);
}

template <typename RandomAccessIterator, typename Type, typename Distance>
void __make_heap(RandomAccessIterator __first, RandomAccessIterator __last, Type *, Distance *)
{
    Distance len = __last - __first;

    if (len < 2) { return; }

    Distance parent = (len - 2) / 2;

    while (true)
    {
        __adjust_heap(__first, parent, len, Type(*(__first + parent)));

        if (!parent) { return; }

        --parent;
    }
}


#endif // __HEAP_ALGORITHM_H__