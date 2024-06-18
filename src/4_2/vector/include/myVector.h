#ifndef _MY_VECTOR_H_
#define _MY_VECTOR_H_

//#include <initializer_list>
#include <memory>
#include <new>

#include "./simpleAlloc.h"

template <typename Type, typename Alloc = std::allocator<Type>>
class My_Vector
{
    public:
        using valueType  = Type;
        using pointer    = valueType *;
        using iterator   = valueType *;
        using constIterator = const valueType *;
        using reference  = valueType &;
        using sizeType   = std::size_t;
        using differenceType = std::ptrdiff_t;

    protected:

        using dataAllocator = Simple_Alloc<valueType, Alloc>;

        iterator start;         // 指向数组之首的迭代器
        iterator finish;        // 指向目前数组使用空间之尾的迭代器
        iterator endOfStorage;  // 指向目前数组可用空间之尾的迭代器

        /**
         * @brief 实现数组的扩容以及新值的插入操作。
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

        void deallocate()
        {
            if (this->start) { dataAllocator::deallocate(start, endOfStorage - start); }
        }

        iterator allocate_and_fill(sizeType __n, const Type & __value)
        {
            iterator result = dataAllocator::allocate(__n);
            std::uninitialized_fill_n(result, __n, __value);

            return result;
        }
        
        void fillInitialize(sizeType __n, const Type & __value)
        {
            start        = allocate_and_fill(__n, __value);
            finish       = start + __n;
            endOfStorage = finish; 
        }

    public:
        iterator begin()                { return this->start; }
        iterator end()                  { return this->finish; }
        constIterator begin() const     { return this->start; }
        constIterator end()   const     { return this->finish; }
        
        sizeType size()     const       { return sizeType(this->end() - this->begin()); }
        sizeType capacity() const       { return sizeType(endOfStorage - this->begin()); }

        bool empty() const { return (this->begin() == this->end()); }


        reference operator[](sizeType __n) { return *(begin() + __n); }

        My_Vector() : start(nullptr), finish(nullptr), endOfStorage(nullptr) {}
        My_Vector(sizeType __n, const Type & __value)       { this->fillInitialize(__n, __value); }
        My_Vector(int __n, const Type & __value)            { this->fillInitialize(__n, __value); }
        My_Vector(long int __n, const Type & __value)       { this->fillInitialize(__n, __value); }
        explicit My_Vector(sizeType __n)                    { this->fillInitialize(__n, Type()); }

        reference front() { return (*begin()); }
        reference back()  { return *(end() - 1); }

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
            std::destroy(this->end() - 1, this->end());
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

                    iterator newStart  = dataAllocator::allocate(allocateLength);
                    iterator newFinish = newStart;

                    try
                    {
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

                    std::destroy(this->start, this->finish);
                    dataAllocator::deallocate(this->start, oldSize);

                    this->start  = newStart;
                    this->finish = newFinish;
                    this->endOfStorage = newStart + allocateLength;
                }
            }
        }
        
        ~My_Vector() 
        {
            std::destroy(start, finish);
            this->deallocate();
        }
};  

#endif // _MY_VECTOR_H_