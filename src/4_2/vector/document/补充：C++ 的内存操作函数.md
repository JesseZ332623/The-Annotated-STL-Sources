# C++ 的几个基本内存操作函数（补充）

和 C 语言的 `memset()`，`memcpy()` 等内存操作函数差不多，STL 也定义了 5 个全局函数，作用于未初始化空间上，这样的功能对于容器的实现很有帮助。

除了用于构建和析构数据的 `construct()` 和 `destroy()` 外，另外 3 个函数是：

- `uninitialized_copy()`
- `uninitialized_fill()`
- `uninitialized_fill_n()`

上面三个个函数分别对应几个 STL 算法：

- `copy()`
- `fill()`
- `fill_n()`

若要使用上面那三个低层次的函数，应该包含 `<memory>` 头文件，
但它们的实际的定义是在 `<stl_uninitialized>` 头文件。

## `uninitialized_copy()` 函数

- 函数原型如下：

```C++
/**
 *  @brief 复制 [__first, __last) 范围内迭代器所指向的数据到 __result
 *  @param  __first  一个输入迭代器。
 *  @param  __last   一个输入迭代器。
 *  @param  __result 一个单向输出迭代器。
 *  @return __result + (__first - __last) 的迭代器。
 * 
 *  和 copy() 相似，但 __result 不要求是经过初始化的输出范围。
 */
template <typename _InputIterator, typename _ForwardIterator>
inline _ForwardIterator
uninitialized_copy(_InputIterator __first, _InputIterator __last, _ForwardIterator __result);
```

该函数使我们能够将内存的配置与对象的构造行为分离开来。
若作为输出目的地的 `[result, result + (last - first))` 范围内的每一个迭代器都指向未初始化的区域，则 `uninitialized_copy()` 函数会使用这个类型的拷贝构造函数给 `[result, result + (last - first))` 范围内的每一个对象产生一份复制品，放入输出范围中。

换言之，争对输入范围内的每一个迭代器 `iter`，函数都会调用这个类型的 `construct(&*(result + (iter - first)), *iter)` 构建函数，产生 `*iter` 的复制品，放置于输出范围的相对位置上。

## `uninitialized_fill()` 函数

- 函数原型如下：

```C++
/**
 *  @brief  将值 __x 拷贝到 [__first, __last) 迭代器范围所指向的内存中去。
 *  @param  __first  一个输入迭代器。
 *  @param  __last   一个输入迭代器。
 *  @param  __x      待拷贝的数据。
 *  @return 无返回值
 *
 *  和 fill() 相似，但 __result 不要求是经过初始化的输出范围。
 */
template <typename _ForwardIterator, typename _Tp>
inline void
uninitialized_fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp &__x);
```

该函数使我们能够将内存的配置与对象的构造行为分离开来。
若作为输出目的地的 `[first, last)` 范围内的每一个迭代器都指向未初始化的区域，那么该函数会在该范围内产生 `__x` 的复制品。

换言之，`uninitialized_fill()` 函数会对操作范围内的每一个迭代器 `iter`，调用这个类型的 `construct(&*iter, __x)` 构建函数，在 `iter` 所指之处产生 `__x` 的复制品。

## `uninitialized_fill_n()` 函数

- 函数原型如下：

```C++
/**
 *  @brief 将值 __x 拷贝到 [__first, __first + __n) 迭代器范围所指向的内存中去。
 *  @param  __first  一个输入迭代器。
 *  @param  __n      要拷贝并构建元素的数量。
 *  @param  __x      待拷贝的数据。
 *  @return 无返回值
 *
 *  和 fill_n() 相似，但 __result 不要求是经过初始化的输出范围。
 */
template <typename _ForwardIterator, typename _Size, typename _Tp>
inline _ForwardIterator
uninitialized_fill_n(_ForwardIterator __first, _Size __n, const _Tp &__x);
```

和 `uninitialized_fill()` 函数差不多，若作为输出目的地的 `[first, first + n)` 范围内的每一个迭代器都指向未初始化的区域，那么该函数会在该范围内产生 `__x` 的复制品。

## 注意

C++ 标准要求，以上这 3 个内存操作函数具有 `commit or rollback` 语义，意思是要么构造出 “所有必要的元素”，要么（当有任何构造函数失败时），不构造任何东西。如下面的代码片段所示：

```C++
try
{
    /**
     * 整个拷贝过程分 3 步
     * 1. 从旧数组拷贝插入点前的数据到新数组
     * 2. 拷贝要插入的 __n 个值到新数组
     * 3. 最后从旧数组拷贝插入点之后的数据到新数组
     *
     * 期间如果出现任何异常，都需要停止拷贝，销毁新数组并抛出异常。
     */
    newFinish = std::uninitialized_copy(this->start, __pos, newStart);
    newFinish = std::uninitialized_fill_n(newFinish, __n, __x);
    newFinish = std::uninitialized_copy(__pos, this->finish, newFinish);
}
catch (...)
{
    /**
     * 期间构造函数出现任何失败，
     * 都要接下异常，析构并销毁所有正在以及将要完成拷贝的所有内存块，
     * 然后再抛出已经捕获的异常，传递给外部调用者去处理。
    */
    std::destroy(newStart, newFinish);
    dataAllocator::deallocate(newStart, allocateLength);
    throw;
}
``
