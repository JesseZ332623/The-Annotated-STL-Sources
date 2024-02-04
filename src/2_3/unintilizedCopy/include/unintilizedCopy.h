#include <./bits/stl_algobase.h>
#include <type_traits>
#include "./cppAlloc.h"
#include <./bits/stl_uninitialized.h>

template <typename InputIterator, typename ForwardIterator, typename Type>
inline ForwardIterator 
uninitializedCopy(InputIterator __first, InputIterator __last, ForwardIterator __result)
{
    return __uninitializedCopy(__first, __last, __result, std::value_type(__result));
}

template <typename InputIterator, typename ForwardIterator, typename Type>
inline ForwardIterator 
__uninitializedCopy(InputIterator __first, InputIterator __last, ForwardIterator __result, Type *)
{
    //using ifPODType = std::__type_traits<Type>::is_POD_type;

    return __uninitializedCopyAux(__first, __last, std::is_pod);
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator 
__uninitializedCopyAux(InputIterator __first, InputIterator __last, ForwardIterator __result, std::__true_type)
{
    return std::copy(__first, __last, __result);
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator 
__uninitializedCopyAux(InputIterator __first, InputIterator __last, ForwardIterator __result, std::__false_type)
{
    ForwardIterator currentIter = __result;

    for (currentIter; __first != __last; ++currentIter, ++__first)
    {
        CPP_ALLOC::__construct(&(*currentIter), *__first);
    }

    return currentIter;
}