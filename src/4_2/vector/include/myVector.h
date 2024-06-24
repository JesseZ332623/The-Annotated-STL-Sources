#ifndef _MY_VECTOR_H_
#define _MY_VECTOR_H_

#include <initializer_list>
#include <memory>
#include <iterator>
#include <exception>

#include "../../simple_allocator/simpleAlloc.h"

template <typename Type, typename Alloc = std::allocator<Type>>
class My_Vector
{
    public:
        using valueType            = Type;
        using pointer              = valueType *;
        using iterator             = valueType *;                           // 正向迭代器
        using constIterator        = const valueType *;                     // 正向只读迭代器
        using reverseIterator      = std::reverse_iterator<iterator>;       // 反向迭代器    
        using constReverseIterator = std::reverse_iterator<constIterator>;  // 反向只读迭代器
        using reference            = valueType &;
        using constReference       = const valueType &;
        using sizeType             = std::size_t;
        using differenceType       = std::ptrdiff_t;

    protected:
        /**
         * 
        */
        using dataAllocator = Simple_Alloc<valueType, Alloc>;

        iterator start;         // 指向数组之首的迭代器
        iterator finish;        // 指向目前数组使用空间之尾的迭代器
        iterator endOfStorage;  // 指向目前数组可用空间之尾的迭代器

        /**
         * @brief 实现数组的扩容以及新值的插入操作（在 insert() 上也可用）。
         * 
         * @param __pos 要插入的位置
         * @param __n   要插入的值
         * 
         * @return no return
        */
        void insertAux(iterator __pos, const Type & __n)
        {
            /**
             * 若数组还有多余的空间，就不进行扩容操作，在 insert() 操作时有大用。
            */
            if (this->finish != this->endOfStorage)
            {
                std::_Construct(this->finish, *(this->finish - 1));
                ++this->finish;
                Type nCopy = __n;
                std::copy_backward(__pos, finish - 2, finish - 1);
                *__pos = nCopy;
            }
            else
            {
                // 确定旧数组的长度
                const sizeType oldSize      = this->size();

                /**
                 *  确定新数组的长度，先判断旧数组长度是否为 0，
                 * 
                 *  是：只分配一个 Type 长度的内存
                 *  否：分配旧数组长度 * 2 的内存
                */
                const sizeType allocaLength = (oldSize != 0) ? (2 * oldSize) : 1;

                /* 
                    调用分配器的 allocate() 方法，
                    分配长度为 allocaLength * sizeof(Type) 字节的内存交由 newStart 迭代器管理，
                    后面要把旧数组和末尾插入的新数据全部拷贝到此处。
                */
                iterator newStart  = dataAllocator::allocate(allocaLength);

                /*
                    由于新数组只完成了分配却没有构建，
                    所以代表新数组尾部迭代器的 newFinish 暂时指向新数组开头。
                */
                iterator newFinish = newStart;

                try
                {
                    /*
                        将旧数组，范围在 [this->start, __pos)（__pos 一般为 this->end()）
                        内的所有数据先拷贝到新数组 newStart 处。
                        返回最后一个复制对象之后一个的位置的迭代器给 newFinish
                    */
                    newFinish = std::uninitialized_copy(this->start, __pos, newStart);

                    /*
                        调用 _Construct() 函数给新元素构建为 __n 的值
                    */
                    std::_Construct(newFinish, __n);
                    ++newFinish;    // 调整水位

                    /**
                     * 将安插点的原内容也拷贝过来，在 insert() 的实现中会有大用
                    */
                    newFinish = std::uninitialized_copy(__pos, this->finish, newFinish);
                }
                catch (...)
                {
                    /*
                        若拷贝，构造的过程中出现任何异常，都需要停止拷贝，
                        析构并释放新数组的所有内存，最后抛出异常。
                    */
                    std::destroy(newStart, newFinish);
                    dataAllocator::deallocate(newStart, allocaLength);
                    throw;
                }

                // 完成拷贝后析构并释放旧数组
                std::destroy(this->begin(), this->end());
                dataAllocator::deallocate(this->begin(), this->size());

                // 调整迭代器，指向新的数组
                this->start        = newStart;
                this->finish       = newFinish;
                this->endOfStorage = newStart + allocaLength;
            }
        }

        /**
         * @brief 辅助函数，调用分配器，销毁整个 `vector` 的数据。
        */
        void deallocate()
        {
            if (this->start) { dataAllocator::deallocate(start, endOfStorage - start); }
        }

        /**
         * @brief 辅助函数，为 `__n` 个 `Type` 类型的值分配内存并统一构建初值 `__value`，
         *        返回操作完成后的数据首地址。
        */
        iterator allocate_and_fill(sizeType __n, const Type & __value)
        {
            iterator result = dataAllocator::allocate(__n);
            std::uninitialized_fill_n(result, __n, __value);

            return result;
        }
        
        /**
         * @brief 辅助函数，通常被构造函数调用，为 `__n` 个 `Type` 类型的值分配内存并统一构建初值 `__value`，
         *        并设置 vector 的头尾指针到这片新构建的内存上。
        */
        void fillInitialize(sizeType __n, const Type & __value)
        {
            this->start        = allocate_and_fill(__n, __value);
            this->finish       = start + __n;
            this->endOfStorage = finish; 
        }

    public:
        iterator begin() noexcept               { return this->start; }
        iterator end()   noexcept               { return this->finish; }
        constIterator begin() const noexcept    { return this->start; }
        constIterator end()   const noexcept    { return this->finish; }
        constIterator cbegin() const noexcept   { return this->start; }
        constIterator cend()   const noexcept   { return this->finish; }
        reverseIterator rbegin() noexcept       { return reverseIterator(this->end()); }
        reverseIterator rend()   noexcept       { return reverseIterator(this->begin()); }
#if true
        constReverseIterator rcbegin() const noexcept { return constReverseIterator(this->cend()); }
        constReverseIterator rcend()   const noexcept { return constReverseIterator(this->cbegin()); }
#endif

        sizeType size()     const       { return sizeType(this->end() - this->begin()); }
        sizeType capacity() const       { return sizeType(endOfStorage - this->begin()); }

        reference front() { return *begin(); }
        reference back()  { return *(end() - 1); }

        constReference front() const { return (*cbegin()); }
        constReference back()  const  { return *(cend() - 1); }

        bool empty() const { return (this->begin() == this->end()); }

        reference      operator[](sizeType __n)       { return *(begin() + __n); }
        constReference operator[](sizeType __n) const { return *(begin() + __n); }
        My_Vector &    operator= (const My_Vector<Type> & __vec)
        {
            if (this == &__vec) { return *this; }

            this->~My_Vector();

            this->start  = dataAllocator::allocate(__vec.capacity());
            this->finish = std::uninitialized_copy(__vec.begin(), __vec.end(), this->begin());

            this->endOfStorage = this->begin() + __vec.capacity();

            return *this;
        }

        My_Vector & operator= (My_Vector<Type> && __vec)
        {
            if (this == &__vec) { return *this; }

            this->~My_Vector();

            this->start         = __vec.start;
            this->finish        = __vec.finish;
            this->endOfStorage  = __vec.endOfStorage;

            __vec.start         = nullptr;
            __vec.finish        = nullptr;
            __vec.endOfStorage  = nullptr;

            return *this;
        }

        reference at(sizeType __n)
        {
            if (__n >= this->size())
            {
                throw std::out_of_range(
                    "invalid argument __n = " + std::to_string(__n)  +   \
                    " current size = " + std::to_string(this->size()) + ".\n"
                );
            }

            return (*this)[__n];
        }

        constReference at(sizeType __n) const
        {
            if (__n >= this->size())
            {
                throw std::out_of_range(
                    "invalid argument __n = " + std::to_string(__n)  +   \
                    " current size = " + std::to_string(this->size()) + ".\n"
                );
            }

            return (*this)[__n];
        }

        My_Vector() : start(nullptr), finish(nullptr), endOfStorage(nullptr) {}
        My_Vector(sizeType __n, const Type & __value)       { this->fillInitialize(__n, __value); }
        My_Vector(int __n, const Type & __value)            { this->fillInitialize(__n, __value); }
        My_Vector(long int __n, const Type & __value)       { this->fillInitialize(__n, __value); }
        explicit My_Vector(sizeType __n)                    { this->fillInitialize(__n, Type()); }

        /**
         * @brief 从初始化参数列表拷贝数据到 vector
        */
        My_Vector(const std::initializer_list<valueType> & __initList)
        {
            this->start  = dataAllocator::allocate(__initList.size());
            this->finish = std::uninitialized_copy(__initList.begin(), __initList.end(), this->begin());

            this->endOfStorage = this->finish;
        }

        /**
         * @brief 从 C 风格数组拷贝数据到 vector 
        */
        My_Vector(const valueType * __first, const valueType * __last)
        {
            this->start  = dataAllocator::allocate(sizeType(__last - __first));
            this->finish = std::uninitialized_copy(__first, __last, this->begin());

            this->endOfStorage = this->finish;
        }

        /**
         * @brief 拷贝构造函数
        */
        explicit My_Vector(const My_Vector<Type> & __vec)
        {
            this->start  = dataAllocator::allocate(__vec.capacity());
            this->finish = std::uninitialized_copy(__vec.begin(), __vec.end(), this->begin());

            this->endOfStorage = this->begin() + __vec.capacity();
        }

        /**
         * @brief 移动构造函数
        */
        My_Vector(My_Vector && __vec)
        {
            this->start         = __vec.start;
            this->finish        = __vec.finish;
            this->endOfStorage  = __vec.endOfStorage;

            __vec.start         = nullptr;
            __vec.finish        = nullptr;
            __vec.endOfStorage  = nullptr;
        }

        /**
         * @brief 往 vector 末尾添加元素 
        */
        void push_back(const Type & __n)
        {
            if (this->finish != this->endOfStorage)
            {
                std::_Construct(this->finish, __n);
                ++finish;
            }
            else { insertAux(this->end(), __n); }
        }

        /**
         * @brief 删除 vector 末尾的元素
        */
        void pop_back()
        {
            std::_Destroy(this->end() - 1);
            --this->finish;
        }

        /**
         * @brief 清除 [__first, __last) 中的所有元素 
         * 
         * @return  要清除范围的第一个迭代器
        */
        iterator erase(iterator __first, iterator __last)
        {
            iterator newIter = std::copy(__last, this->finish, __first);
            std::destroy(newIter, this->finish);

            this->finish = this->finish - (__last - __first);

            return __first;
        }

        /**
         *  @brief 清除指定位置 __pos 的元素 
        */
        iterator erase(iterator __pos)
        {
            /**
             *  确保 __pos 的位置不是最后一个元素，避免多余的拷贝操作。
            */
            if (__pos + 1 != this->end()) { std::copy(__pos + 1, this->finish, __pos); }

            --this->finish;

            std::destroy(this->end() - 1, this->end());

            return __pos;
        }

        void clear() { this->erase(this->start, this->finish); }

        /**
         * @brief 从  __pos 开始，插入 __n 个元素，每一个元素的初值都为 __x
        */
        void insert(iterator __pos, sizeType __n, const Type & __x)
        {
            if (__n != 0)   // 总不能插入 0 个元素吧 。。。
            {
                /**
                 * 计算数组还剩下多少空间，看看是不是大于等于要插入的元素数 
                */
                if (sizeType(this->endOfStorage - this->finish) >= __n)
                {
                    Type xCopy = __x;

                    /**
                     * 计算插入点之后的现有元素个数。
                    */
                    const sizeType insertAfter = this->finish - __pos;

                    // 保存当前的数组使用空间之尾的迭代器
                    iterator oldFinish = this->finish;

                    /**
                     * 当插入点之后的现有元素个数 大于 要插入的元素数 时
                    */
                    if (insertAfter > __n)
                    {
                        std::uninitialized_copy(this->finish - __n, this->finish, this->finish);
                        this->finish += __n;

                        std::copy_backward(__pos, oldFinish - __n, oldFinish);
                        std::fill(__pos, oldFinish, xCopy);
                    }
                    else // 插入点之后的现有元素个数 小于 要插入的元素数 时
                    {
                        std::uninitialized_fill_n(this->finish, __n - insertAfter, xCopy);
                        this->finish += __n - insertAfter;
                        std::uninitialized_copy(__pos, oldFinish, this->finish);
                        this->finish += insertAfter;
                        std::fill(__pos, oldFinish, xCopy);
                    }
                }
                else // 数组内剩下的备用空间不足，需要重新分配内存
                {
                    const sizeType oldSize = this->size();   // 记录旧数组的大小

                    /**
                     * 确定要分配的新数组大小，
                     * 要么是原数组的两倍长，要么是原数组长度 + 要插入元素的个数。
                    */
                    const sizeType allocateLength = oldSize + std::max(oldSize, __n);

                    /**
                     * 给新数组分配内存并设置新数组的头尾指针
                    */
                    iterator newStart  = dataAllocator::allocate(allocateLength);
                    iterator newFinish = newStart;

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
                        std::destroy(newStart, newFinish);
                        dataAllocator::deallocate(newStart, allocateLength);
                        throw;
                    }

                    /**
                     * 最后销毁旧数组，并把设置指针到新数组
                    */
                    std::destroy(this->start, this->finish);
                    dataAllocator::deallocate(this->start, oldSize);

                    this->start  = newStart;
                    this->finish = newFinish;
                    this->endOfStorage = newStart + allocateLength;
                }
            }
        }
        /**
         * 修改 vector 内部数组的大小。
         * 
        */
        void resize(sizeType __newSize)
        {
            /**
             * 若传入的值为 0，直接 clear() 打发了。
             */
            if (__newSize == 0) { this->clear(); }

            /**
             * 增加数组尺寸
            */
            if (__newSize > this->size())
            {
                /**
                 * 增加的量并没有超过 capacity() 的返回值，
                 * 无需扩容操作，仅调整 finish 指针的位置即可。
                */
                if ((this->size() + (__newSize - this->size())) < this->capacity())
                {
                    sizeType appendSize = __newSize - this->size();
                    this->finish = std::fill_n(this->end(), appendSize, 0);
                }
                /**
                 * 增加的量超过了 capacity() 的返回值，
                 * 就需要扩容操作，并调整  finish 和 endOfStorage 指针的位置。
                */
                else
                {
                    /**
                     * 计算两个数据：
                     * 1. 仅仅需要追加初值的元素数
                     * 2. 需要分配且构造初值的元素数
                    */
                    sizeType appendSize   = this->capacity() - this->size();
                    sizeType allocateSize = __newSize - this->capacity();

                    // 先填充数组可用空间
                    this->finish = std::fill_n(this->end(), appendSize, 0);
                    
                    // 剩下的部分可以通过循环并使用 insertAux() 函数进行插入和扩容
                    for (sizeType index = 0; index < allocateSize; ++index)
                    {
                        this->insertAux(this->end(), 0);
                    }
                }
            }
            /**
             * 缩小数组尺寸，只需要抹掉 [this->start + __newSize, this->finish) 范围内的数据，
             * 并调整 finish 指针即可到 this->start + __newSize 即可。
            */
            else 
            {
                this->erase(this->begin() + __newSize, this->end());
            }
        }

        void resize(sizeType __newSize, const valueType & __x)
        {
            /**
             * 若传入的值为 0，直接 clear() 打发了。
             */
            if (__newSize == 0) { this->clear(); }

            /**
             * 增加数组尺寸
            */
            if (__newSize > this->size())
            {
                /**
                 * 增加的量并没有超过 capacity() 的返回值，
                 * 无需扩容操作，仅调整 finish 指针的位置即可。
                */
                if ((this->size() + (__newSize - this->size())) < this->capacity())
                {
                    sizeType appendSize = __newSize - this->size();
                    this->finish = std::fill_n(this->end(), appendSize, __x);
                }
                /**
                 * 增加的量超过了 capacity() 的返回值，
                 * 就需要扩容操作，并调整  finish 和 endOfStorage 指针的位置。
                */
                else
                {
                    /**
                     * 计算两个数据：
                     * 1. 仅仅需要追加初值的元素数
                     * 2. 需要分配且构造初值的元素数
                    */
                    sizeType appendSize   = this->capacity() - this->size();
                    sizeType allocateSize = __newSize - this->capacity();

                    // 先填充数组可用空间
                    this->finish = std::fill_n(this->end(), appendSize, __x);
                    
                    // 剩下的部分可以通过循环并使用 insertAux() 函数进行插入和扩容
                    for (sizeType index = 0; index < allocateSize; ++index)
                    {
                        this->insertAux(this->end(), __x);
                    }
                }
            }
            /**
             * 缩小数组尺寸，只需要抹掉 [this->start + __newSize, this->finish) 范围内的数据，
             * 并调整 finish 指针即可到 this->start + __newSize 即可。
            */
            else 
            {
                this->erase(this->begin() + __newSize, this->end());
            }
        }

        /**
         * @brief 预分配容器的容量
         * 
         * @param __newSize 请求的容量大小
        */
        void reserve(sizeType __newSize)
        {
            /**
             * 若请求的新大小小于或等于当前容量，
             * 则不会做任何操作直接返回即可。
            */
            if (__newSize <= this->capacity()) { return; }

            /**
             * 若请求的大小大于当前容量，则需要扩容该数组。
            */
            sizeType allocateSize = __newSize - this->capacity();

            for (sizeType index = 0; index < allocateSize; ++index)
            {
                this->insertAux(this->end(), 0);
            }
        }
        
        ~My_Vector() 
        {
            std::destroy(start, finish);
            this->deallocate();
        }
};  

#endif // _MY_VECTOR_H_