#ifndef _LIST_ITEM_H_
#define _LIST_ITEM_H_

template <typename Type>
class MyListItem
{
    private:
        Type value;
        MyListItem * nextItem;

    public:
        MyListItem(Type __value, MyListItem<Type> * __next) : value(__value), nextItem(__next) {}

        Type getValue() const { return value; }
        
        MyListItem * next() const { return nextItem; }

        void setNext(MyListItem * __nextItem) { nextItem = __nextItem; } 
};

#endif // _LIST_ITEM_H_