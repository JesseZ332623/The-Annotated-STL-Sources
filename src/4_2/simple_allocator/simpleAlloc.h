#ifndef _SIMPLE_ALLOC_H_
#define _SIMPLE_ALLOC_H_

#include <cctype>

template <typename Type, typename Alloc>
class Simple_Alloc
{
public:
    static Type *allocate(std::size_t __n)
    {
        Alloc allocInstance;
        return (!__n) ? nullptr : allocInstance.allocate(__n * sizeof(Type));
    }

    static Type *allocate(void)
    {
        Alloc allocInstance;
        return (Type *)allocInstance.allocate(sizeof(Type));
    }

    static void deallocate(Type *__ptr, std::size_t __n)
    {
        Alloc allocInstance;
        if (__n != 0)
        {
            allocInstance.deallocate(__ptr, __n * sizeof(Type));
        }
    }

    static void deallocate(Type *__ptr)
    {
        Alloc allocInstance;
        allocInstance.deallocate(__ptr, sizeof(Type));
    }
};

#endif // _SIMPLE_ALLOC_H_