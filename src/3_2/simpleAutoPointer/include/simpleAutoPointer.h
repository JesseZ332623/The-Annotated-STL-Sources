#ifndef _SIMPLE_AUTO_POINTER_H_
#define _SIMPLE_AUTO_POINTER_H_

/**
 * @brief   一个简易的 auto_ptr 实现
 * 
 * @tparam  Type 要动态管理内存的类型
*/
template <typename Type>
class myAutoPointer
{
    private:
        Type * pointer;

    public:
        /**
         * @brief 参数构建函数：传入一片内存的指针并交由 pointer 管理
         * 
         * @param __ptr 一片内存的首地址（默认为空指针）
        */
        explicit myAutoPointer(Type * __ptr = nullptr) : pointer(__ptr) {}

        /**
         * @brief 拷贝构建函数：传入另一个 auto_ptr 对象，该对象需要调用 release() 移交所有权
         * 
         * @param __autoPtr myAutoPointer 类的引用
        */
        myAutoPointer(myAutoPointer & __autoPtr) : pointer(__autoPtr.release()) {}

        /**
         * @brief 模板拷贝构建函数： 
         * 
         * @brief - 传入另一个 auto_ptr 对象，该对象需要调用 release() 移交所有权
         * @brief - 这通常用于子类到父类的转换
         * 
         * @tparam U 另一片内存的类型
         * 
         * @param rhs myAutoPointer 类的引用
        */
        template <typename U> 
        myAutoPointer(myAutoPointer<U> & rhs) : pointer(rhs.release()) {}

        /**
         * @brief 模板拷贝运算符重载
         * 
         * @brief - 传入另一个 auto_ptr 对象，该对象需要调用 release() 移交所有权
         * @brief - 这通常用于子类到父类的转换
         * 
         * @tparam U 另一片内存的类型
         * 
         * @param rhs myAutoPointer 类的引用
         * 
         * @return 对象本身
        */
        template <typename U>
        myAutoPointer<Type> & operator=(myAutoPointer<U> & rhs) 
        {
            /*避免自赋值*/
            if (this != &rhs) { reset(rhs.release()); }
            return *this;
        }
        
        /**
         * @brief 获取当前管理的内存的首地址
        */
        Type * get() const { return pointer; }

        /**
         * @brief 移交内存的所有权
        */
        Type * release() 
        { 
            Type * tempPointer = pointer; 
            pointer = nullptr;
            return tempPointer;
        }

        /**
         * @brief 用于重设指针
         * 
         * @param __ptr 一片内存的首地址（默认为空指针）
         * 
         * @return non-return
        */
        void reset(Type * __ptr = nullptr)
        {
            /*在传入地址于原地址不相同时*/
            if (__ptr != pointer) 
            { 
                delete pointer;     // 释放原指针指向的内存块
                pointer = __ptr;    // 切换成新传入的地址
            }
        }

        /**
         * @brief 重载 * 符号用于解引用
        */
        Type & operator*() const { return *pointer; }

        /**
         *  @brief 重载 -> 符号用于访问直接访问指针
        */
        Type * operator->() const { return pointer; }

        ~myAutoPointer() { delete pointer; }
};

#endif // _SIMPLE_AUTO_POINTER_H_