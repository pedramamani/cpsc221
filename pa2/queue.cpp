#include "queue.h"

template <class T>
void Queue<T>::enqueue(T const& newItem)
{
    myQueue.pushR(newItem);
}

template <class T>
T Queue<T>::dequeue()
{
    return myQueue.popL();
}

template <class T>
void Queue<T>::add(const T& theItem)
{
    enqueue(theItem);
}

template <class T>
T Queue<T>::remove()
{
    return dequeue();
}

template <class T>
T Queue<T>::peek()
{
    return myQueue.peekL();
}

template <class T>
bool Queue<T>::isEmpty() const
{
    return myQueue.isEmpty();
}
