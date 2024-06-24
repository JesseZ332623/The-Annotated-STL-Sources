#ifndef __LIST_NODE_H_
#define __LIST_NODE_H_

/**
 * 一个双向链表节点的设计，
 * list 本身和 list 节点是不同的，需要分开实现。 
*/

template <typename Type>
struct ListNode
{
    /**
     * 下一个节点的指针类型，旧版使用的是 void * 类型，
     * 这里使用 ListNode<Type> * 类型更严谨。
    */
    using listPointer = ListNode<Type> *;

    listPointer prev;       // 指向前一个 ListNode 节点的指针
    listPointer next;       // 指向后一个 ListNode 节点的指针

    Type nodeData;          // 节点数据
};

#endif // __LIST_NODE_H_