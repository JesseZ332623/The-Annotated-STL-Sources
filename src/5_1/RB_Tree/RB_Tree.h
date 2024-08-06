#include "./RB_Tree_Iterator.h"
#include "../simple_allocator/simpleAlloc.h"

#include <limits>
#include <algorithm>

/**
 * @brief 一个 RB-Tree 的实现
 * 
 * @tparam Key          键的类型
 * @tparam Value        值的类型
 * @tparam KeyOfValue   通过键得到的值的类型，一般是一个仿函数 或 Lamba 表达式 
 * @tparam Compare      红黑树节点间的比较规则
 * @tparam Alloc        红黑树节点分配器
 */
template <
    typename Key, typename Value, typename KeyOfValue, 
    typename Compare, typename Alloc = std::allocator<RBTree_Node<Value>>
>
class RB_Tree
{
    protected:  
        typedef void *                              void_pointer;
        typedef RBTree_Node<Value> *                base_ptr;
        typedef RBTree_Node<Value>                  rb_tree_node;
        typedef Simple_Alloc<rb_tree_node, Alloc>   rb_tree_node_allocator;
        typedef RB_TREE_COLOR_TYPE                  color_type;

    public:
        typedef Key                 key_type;
        typedef Value               value_type;
        typedef value_type *        pointer;
        typedef const value_type *  const_pointer;
        typedef value_type &        reference;
        typedef const value_type &  const_reference;
        typedef rb_tree_node *      link_type;

        typedef std::size_t         size_type;
        typedef std::ptrdiff_t      differece_type;
    
    protected:
        /**
         * @brief 分配一个树节点空间。
         */
        link_type get_node() { return rb_tree_node_allocator::allocate(); }

        /**
         * @brief 销毁一个树节点空间。
        */
        void put_node(link_type __node_ptr) { 
            rb_tree_node_allocator::deallocate(__node_ptr); 
        }

        /**
         * @brief 分配节点并构造节点值。
        */
        link_type create_node(const value_type & __value)
        {
            link_type temp_node = this->get_node();

            try
            {
                std::construct_at(&temp_node->value_field, __value);
                //std::_Construct(&temp_node->value_field, __value);
            }
            catch (...)
            {
                /*创建失败就得销毁*/
                this->put_node(temp_node);
            }
            
            return temp_node;
        }

        /**
         * @brief 复制一个节点的值和颜色
         */
        link_type clone_node(link_type __node_ptr)
        {
            link_type temp_node = this->create_node(__node_ptr->value_field);
            temp_node->color  = __node_ptr->color;

            temp_node->parent = nullptr;
            temp_node->left   = nullptr;
            temp_node->right  = nullptr;

            return temp_node;
        }

        /**
         * @brief 析构并释放单个节点
         */
        void destory_node(link_type __node_ptr)
        {
            std::destroy_at(&__node_ptr->value_field);
            this->put_node(__node_ptr);
        }

        protected:
            size_type node_count;   // 树中节点的数量
            link_type header;       // 树中的一个节点
            Compare   key_compare;  // 节点间的排序规则

            /**
             * @brief 访问 header 的父节点
            */
            link_type & root(void)  const    { return (link_type &)(this->header->parent); }

            /**
             * @brief 访问 header 的左子节点
            */
            link_type & leftmost(void) const { return (link_type &)(this->header->left); }

            /**
             * @brief 访问 header 的右子节点
            */
            link_type & rightmost(void) const { return (link_type &)(this->header->right); }

            /**
             * @brief 获取某个节点的左子节点
            */
            static link_type & left(link_type x)
            { return (link_type &)(x->left); }

            /**
             * @brief 获取某个节点的右子节点
            */
            static link_type & right(link_type x)
            { return (link_type &)(x->right); }

            /**
             * @brief 获取某个节点的父节点
            */
            static link_type & parent(link_type x)
            { return (link_type &)(x->parent); }

            /**
             * @brief 获取某个节点的节点值
            */
            static reference value(link_type x)
            { return x->value_field; }

            /**
             * @brief 获取某个节点的键值
            */
            static const Key & key(link_type x)
            { return KeyOfValue()(value(x)); }
            
            /**
             * @brief 获取某个节点的节点颜色
            */
            static color_type & color(link_type x)
            { return (color_type &)(x->color); }

            /**
             * @brief 获取书中值最小的节点
             */
            static link_type min_value(link_type x)
            { return (link_type) RBTree_Node_Base::min_value(x); }
            
            /**
             * @brief 获取树中值最大的节点
             */
            static link_type max_value(link_type x)
            { return (link_type) RBTree_Node_Base::max_value(x); }

#if false
            /**
             * @brief 获取某个节点的左子节点
            */
            static link_type & left(base_ptr x)
            { return (link_type &)(x->left); }

            /**
             * @brief 获取某个节点的右子节点
            */
            static link_type & right(base_ptr x)
            { return (link_type &)(x->right); }

            /**
             * @brief 获取某个节点的父节点
            */
            static link_type & parent(base_ptr x)
            { return (link_type &)(x->parent); }

            /**
             * @brief 获取某个节点的节点值
            */
            static reference value(base_ptr x)
            { return ((link_type)x)->value_field; }

            /**
             * @brief 获取某个节点的键值
            */
            static const Key & key(base_ptr x)
            { return KeyOfValue()(value(link_type(x))); }
            
            /**
             * @brief 获取某个节点的节点颜色
            */
            static color_type & color(base_ptr x)
            { return (color_type &)(link_type(x)->color); }

#endif
    public:
        typedef RBTree_Iterator<value_type, reference, pointer> iterator;

    private:
        iterator insert(base_ptr x, base_ptr y, const value_type & value);

        link_type copy(link_type x, link_type p);

        /**
         * @brief 移除掉红黑树中的指定节点 x
        */
        void erase(link_type x);

        /**
         * @brief 销毁掉整棵树，header 节点除外
        */
        void clear(void);

        /**
         * @brief 初始化一株红黑树。
        */
        void init(void)
        {
            this->header = this->get_node();        // 分配一个树节点空间给 header
            this->color(header) = RB_TREE_RED;      // header 节点必为红节点

            this->root()      = nullptr;            // 此时 hearder 节点的父节点为空
            this->leftmost()  = this->header;       // 暂时让 hearder 节点的左子节点指向它自己
            this->rightmost() = this->header;       // 暂时让 hearder 节点的右子节点指向它自己
        }

    public:
        /**
         * @brief 红黑树的默认构造函数
         * 
         * @param __comp    指定节点间的比较规则
        */
        RB_Tree(const Compare & __comp = Compare()) : node_count(0ULL), key_compare(__comp)
        { this->init(); }

        /**
         * @brief 销毁掉整棵红黑树
        */
        ~RB_Tree() { this->clear(); this->put_node(this->header); }

        /**
         * @brief 获取这颗树的排序规则函数对象。
        */
        Compare     key_comp() { return key_compare; }

        iterator    begin()    { return this->leftmost(); }
        iterator    end()      { return this->header; }
        bool        empty()    { return (this->node_count == 0); }
        size_type   size()     { return node_count; }
        size_type   max_size() { return SIZE_MAX; }

    public:
        /**
         * @brief 保持节点值独一无二的插入
         * 
         * @param __value                       要插入的节点值
         * @return std::pair<iterator, bool>    插入后节点所在的位置，以及是否插入成功
        */
        std::pair<iterator, bool> insert_unique(const value_type & __value);

        /**
         * @brief 允许出现重复值的插入
         * 
         * @param __value           要插入的节点值
         * @return iterator         插入后节点所在的位置
        */
        iterator insert_equal(const value_type & __value);
};

template <
    typename Key, typename Value, typename KeyOfValue, 
    typename Compare, typename Alloc
>
void RB_Tree<Key,  Value, KeyOfValue, Compare, Alloc>::clear(void)
{

#if false
    std::for_each(
        this->begin(), this->end(),
        [&](iterator node_iter) -> void { this->destory_node(node_iter.node); }
    );
#endif

    this->node_count = 0ULL;
}