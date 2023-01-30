#include "stack.h"

template<class T>
void Stack<T>::push(T const & newItem)
{
    myStack.pushR(newItem);
}

template <class T>
T Stack<T>::pop()
{
    return myStack.popR();
}

template <class T>
void Stack<T>::add(const T& theItem)
{
    push(theItem);
}

template <class T>
T Stack<T>::remove()
{
    return pop();
}

template <class T>
T Stack<T>::peek()
{
    return myStack.peekR();
}

template <class T>
bool Stack<T>::isEmpty() const
{
    return myStack.isEmpty();
}
