#ifndef __LIST_H_
#define __LIST_H_

/**
 * list 容器不仅是一个双向链表，还是一个环状双向链表，
 * 因此只需要一个指针，就能完整表现整个链表。 
 * 
 * 所以需要再增加一个空白节点，
 * 让这个空白节点作为 end() 迭代器所指向的位置，就能实现 STL 对于 “前闭后开” 区间的要求。
*/

#include "./list_Iterator.h"
#include "../../simple_allocator/simpleAlloc.h"

#include <memory>
#include <type_traits>
#include <initializer_list>

/**
 * @brief 按照 STL 标准实现的双向链表
 * 
 * @tparam Type  双向链表节点数据类型
 * @tparam Alloc 容器使用的分配器类型，默认为 `std::allocator<ListNode<Type>>`
*/
template <typename Type, typename Alloc = std::allocator<ListNode<Type>>>
class MyList
{
    protected:
        using valueType      = Type;
        using listNode       = ListNode<Type>;                              // 链表节点类型
        using nodeAllocator  = Simple_Alloc<listNode, Alloc>;               // 链表节点分配器类型

        using iterator       = ListIterator<Type, Type &, Type *>;               // 双向链表迭代器
        using constIterator  = const ListIterator<Type, Type &, Type *>;         // 双向链表只读迭代器

        using reference = Type &;                                                // 表节点数据的引用

        using sizeType          = std::size_t;
        using differenceType    = std::ptrdiff_t;

    public:
        using linkType = listNode *;    // 链表节点指针

    protected:
        /**
         * 链表节点指针，初始情况下指向空闲节点。 
        */
        linkType nodePointer;

        sizeType nodeCount;     // 链表节点计数

        /**
         * @brief 调用分配器，为单个 `ListNode<Type>` 类型的节点分配内存。
        */
        linkType getNode(void) { return nodeAllocator::allocate(); }
        
        /**
         * @brief 调用分配器，释放单个 `ListNode<Type>` 类型的节点的内存。
        */
        void freeNode(linkType __node) { nodeAllocator::deallocate(__node); }

        /**
         * @brief 调用分配器和全局构建函数，分配并构建单个链表节点。
         * 
         * @param __data 需要交给全局构造函数构建的节点数据
         * 
         * @return 构建完成的链表节点指针
        */
        linkType createNode(const Type & __data) {

            linkType nodePtr = this->getNode();
            std::_Construct(&nodePtr->nodeData, __data);

            return nodePtr;
        }

        /**
         * @brief 析构并释放单个链表节点的内存。
         * 
         * @param __nodePtr 链表节点指针。
        */
        void destroyNode(linkType __nodePtr) {

            std::destroy_n(&__nodePtr->nodeData, 1);
            freeNode(__nodePtr);

        }

        /**
         * @brief 辅助函数，初始化一张空链表。
         * 
         * @brief - 分配一个空节点，
         *          不构建值且令节点内头尾指针都指向自己
        */
        void emptyInitialize(void)
        {
            this->nodePointer = this->getNode();

            this->nodePointer->next = this->nodePointer;
            this->nodePointer->prev = this->nodePointer;
        }

        /**
         * @brief 辅助函数，把 `[__first, __last)` 内的所有元素移动到 `__pos` 之前。
         * 
         * @brief - 该方法详细的操作示意见：`list\\document\\transferOperator.dio`
        */
        void transfer(iterator __pos, iterator __first, iterator __last)
        {   
            // 至少确保 [__first, __last) 的范围必须在 __pos 之后。
            if (__pos != __last)
            {
                (*(linkType((*__last.node).prev))).next  = __pos.node;     // (1)
                (*(linkType((*__first.node).prev))).next = __last.node;    // (2)
                (*(linkType((*__pos.node).prev))).next   = __first.node;   // (3)
                linkType tmp = linkType((*__pos.node).prev);               // (4)
                (*__pos.node).prev   = (*__last.node).prev;                // (5)
                (*__last.node).prev  = (*__first.node).prev;               // (6)
                (*__first.node).prev = tmp;                                // (7)
            }
        }

        /**
         * @brief 辅助函数，将链表的 `__n` 个节点值填充为 `__value`。
        */
        void fillInitialize(sizeType __n, const valueType & __value)
        {
            for (; __n; --__n) { this->push_back(__value); }
        }

        /**
         * @brief 辅助函数，
         *        将链表的 `__n` 个节点值调用节点数据类型的默认构造函数 `valueType()` 进行填充。
        */
        void defaultInitialize(sizeType __n)
        {
            for (; __n; --__n) { this->push_back(valueType()); }
        }

    public:

        /**
         * @brief 默认构造函数。
         * 
         * @brief - 初始化节点计数并构建空链表。 
        */
        MyList() : nodeCount(0ULL) { this->emptyInitialize(); }

        /**
         * @brief 创造 `__n` 个链表节点，
         *        每一个节点都调用节点数据的默认构造函数进行构建。
        */
        explicit MyList(sizeType __n) : nodeCount(0ULL)
        {
            this->emptyInitialize();
            this->defaultInitialize(__n);
        }

        /**
         * @brief 创造 `__n` 个链表节点，
         *        每一个节点数据都设为 `__value`。
        */
        MyList(sizeType __n, const valueType & __value) : nodeCount(0ULL)
        {
            this->emptyInitialize();
            this->fillInitialize(__n, __value);
        }

        /**
         * @brief 从初始化列表中获取数据并构造链表。
        */
        explicit MyList(std::initializer_list<Type> __initList) : nodeCount(0ULL)
        {
            this->emptyInitialize();
            
            for (const Type & n : __initList)
            {
                this->push_back(n);
            }
        }

        /**
         * @brief 从 [__first, __last) 迭代器范围内获取数据并构建链表。
         * 
         * @tparam InputIterator 待拷贝容器迭代器类型
         * @tparam std::enable_if_t<!std::is_integral<InputIterator>::value> 
         *         用于对模板参数进行限制，当 InputIterator 是整数类型时，不实例化该构造函数。
        */
        template <
                    typename InputIterator, 
                    typename = std::enable_if_t<!std::is_integral<InputIterator>::value>
            >
        MyList(InputIterator __first, InputIterator __last) : nodeCount(0ULL)
        {
            this->emptyInitialize();

            InputIterator tempIter = __first;

            while (__first != __last)
            {
                this->push_back(*__first);
                ++tempIter;
                __first = tempIter;
            }
        }

        MyList(MyList &&) = default;

        ~MyList() = default;

        /**
         * @brief 获取链表第一个元素的迭代器。 
        */
        iterator begin() { return (*nodePointer).next; }
        constIterator begin() const { return (*nodePointer).next; }

        /**
         * @brief 获取链表最后一个元素后面一个元素的迭代器。 
        */
        iterator end() { return this->nodePointer; }
        constIterator end() const { return this->nodePointer; }

        /**
         * @brief 判断链表是否为空（检查 nodePointer 有没有停在空节点即可）。
        */
        bool empty() const { return (this->nodePointer->next == this->nodePointer); }

        /**
         * @brief 获取链表的节点数。
        */
        sizeType size() const { return this->nodeCount; }

        /**
         * @brief 设置链表节点计数，
         *        这在 merge() splice() 等会大幅改变链表节点数的时候会用到。
        */
        void setSize(sizeType __newSize) { this->nodeCount = __newSize; }
        
        /**
         * @brief 获取链表第一个节点值的引用。 
        */
        reference front() { return *(this->begin()); }

        /**
         * @brief 获取链表最后一个节点值的引用。
        */
        reference back()  { return *(--this->end()); }

        /**
         * @brief 从链表的 __pos 迭代器所指向的位置之前插入一个新节点。
         * 
         * @param __pos     要在链表中插入的位置
         * @param __data    要交给 `createNode()` 构建链表节点的数据
         * 
         * @return 插入完成后新节点的迭代器
        */
        iterator insert(iterator __pos, const valueType & __data)
        {
            linkType tempNode = this->createNode(__data);

            tempNode->next    = __pos.node;
            tempNode->prev    = __pos.node->prev;

            __pos.node->prev->next = tempNode;
            __pos.node->prev = tempNode;

            ++this->nodeCount;

            return tempNode;
        }

        /**
         * @brief 从链表尾部插入节点
         * 
         * @brief - 直接调用 `insert()` 即可
        */
        void push_back(const Type & __data)  { this->insert(this->end(), __data); }
        
        /**
         * @brief 从链表尾部插入节点（针对右值参数类型进行优化）
        */
        void push_back(Type && __data)  { this->insert(this->end(), std::move(__data)); }

        /**
         * @brief 从链表头部插入节点
         * 
         * @brief - 直接调用 `insert()` 即可
        */
        void push_front(const Type & __data) { this->insert(this->begin(), __data); }

        /**
         * @brief 移除链表中迭代器 __pos 所指向的节点
         * 
         * @param __pos 要删除的目标节点的迭代器
         * 
         * @return 删除完成后 __pos 迭代器后面一个节点的迭代器
        */
        iterator erase(iterator __pos)
        {   
            /**
             * 首先，拿到 __pos 前一个和后一个的节点指针。
            */
            linkType nextPtr = __pos.node->next;
            linkType prevPtr = __pos.node->prev;

            /**
             * 然后把这两个指针接起来，
             * 就可以将 __pos 迭代器所指向的节点从链表中 “剥离开”。
            */
            prevPtr->next = nextPtr;
            nextPtr->prev = prevPtr;
            
            /**
             * 最后把剥离的节点析构并自减节点计数即可。
            */
            this->destroyNode(__pos.node);
            --this->nodeCount;
            
            return iterator(nextPtr);
        }

        /**
         * @brief 删除链表的第一个节点。
        */
        void pop_front() { this->erase(this->begin()); }

        /**
         * @brief 删除链表的最后一个节点
         * 
         * @brief - 由于 `end()` 所指向的是链表环中的空闲迭代器，
         *          直接使用 `--end()` 会修改尾部迭代器，
         *          因此需要临时声明一个迭代器保存 `end()` 在进行操作。
        */
        void pop_back() 
        {
            iterator tempIter = this->end();
            this->erase(--tempIter);
        }

        /**
         * @brief 清除整个链表，使链表回到初始状态。
        */
        void clear();

        /**
         * @brief 将链表节点中所有节点值为 `__value` 的节点移除。
        */
        void remove(const Type & __value);

        /**
         * @brief 移除数值相同的连续元素。
         * 
         * @brief - 注意：只有 “连续而相同的元素” 才会被移除到只剩一个。
         * 
         * @brief - 如：`{1，2，2，3，4，1}` -----> `{1，2，3，4，1}`
        */
        void unique();
        
        /**
         * @brief 将整张链表 `__x` 拼接到 `__pos` 迭代器之前
         * 
         * @brief - 注意：链表 `__x` 必须不同于 `(*this)`
        */
        void splice(iterator __pos, MyList & __x) 
        {
            if (!__x.empty()) 
            { 
                this->transfer(__pos, __x.begin(), __x.end());

                this->setSize(this->size() + __x.size());
                __x.setSize(0ULL);
            }
        }

        /**
         * @brief 将迭代器 `iter` 所指元素接合于 `__pos` 之前，
         *        __pos 和 iter 可以指向同一个链表的节点
        */
        void splice(iterator __pos, MyList & __x, iterator iter)
        {
            iterator tempIter = iter;
            ++tempIter;
            
            /**
             * 若 iter 后面一个节点就是 __pos，
             * 那就没有执行 transfer() 的必要
            */
            if (__pos == iter || __pos == tempIter) { return; }

            this->transfer(__pos, iter, tempIter);

            if (&__x != this) { __x.setSize(__x.size() - 1); ++this->nodeCount; }

        }
        
        /**
         * @brief 将迭代器 `[__first, __last)` 内的所有元素接合到 `__pos` 所指位置之前。
         * 
         *  @brief - 注意：`[__first, __last)` 和 `__pos` 所指的节点都可以在同一个链表内，
         *           但是 `__pos` 不能在 `[__first, __last)` 范围内
        */
        void splice(iterator __pos, MyList & __x, iterator __first, iterator __last)
        {
            if (__first != __last)  // 区间内不可以只有一个或没有节点
            {   
                if (&__x != this) 
                {  
                    sizeType spliceNodeCount = std::distance(__first, __last);
                    __x.setSize(__x.size() - spliceNodeCount);
                    this->setSize(this->size() + spliceNodeCount);
                }

                this->transfer(__pos, __first, __last);
            }
        }
};

template <typename Type, typename Alloc>
void MyList<Type, Alloc>::clear()
{
    /**
     * 由于 nodePointer 是停留在链表的空闲链表节点，
     * 需要给他移出来才是链表的第一个节点。
    */
    linkType currentNodePtr = this->nodePointer->next;
    
    /**
     * 遍历整张表，通过 `destroyNode()` 逐个删除节点。
    */
    while (currentNodePtr != this->nodePointer)
    {
        linkType tempNodePtr = currentNodePtr;
        currentNodePtr = currentNodePtr->next;
        this->destroyNode(tempNodePtr);
    }

    /**
     * 最后把整张表还原成初始的状态。
    */
    this->nodePointer->next = this->nodePointer;
    this->nodePointer->prev = this->nodePointer;
    this->nodeCount = 0ULL;
}

template <typename Type, typename Alloc>
void MyList<Type, Alloc>::remove(const Type & __value)
{
    /**
     * 拿到链表的头尾节点。
    */
    iterator first = this->begin();
    iterator last  = this->end();

    while (first != last)   // 遍历链表
    {
        iterator tempIter = first;
        ++tempIter;

        /**
         * 解引用迭代器拿到链表值和 __value 进行比对，
         * 若相同则移除即可。
        */
        if (*first == __value) { this->erase(first); }

        first = tempIter;   // 更新迭代器
    }

}

template <typename Type, typename Alloc>
void MyList<Type, Alloc>::unique()
{
    /**
     * 拿到链表的头尾节点。
    */
    iterator first = this->begin();
    iterator last  = this->end();

    // 若是空链表则什么也不做
    if (first == last) { return; }

    /**
     * next 迭代器会保存在 first 后面一个节点的迭代器。
    */
    iterator next  = first;

    /**
     * 遍历整张链表，
     * 操作 next 和 first 迭代器去比较相邻两个节点的值是否相等，图示如下：
     * 
     * list = {1, 5, 56, 56, 5}
     *     
     *    first next
     * 1. {[1], [5], 56, 56, 5}
     * 2. {1, [5], [56], 56, 5}
     * 3. {1, 5, [56], [56](X), 5}
     * 4. {1, 5, 56, [], [5]}
     * 
     * After unique() 
     * list = {1, 5, 56, 5}
    */
    while (++next != last)
    {
        if (*next == *first) { this->erase(next); }

        else { first = next; }

        next = first;
    }
}

#endif // __LIST_H_