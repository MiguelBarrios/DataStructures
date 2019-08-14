//
//  OULinkedListEnumerator.h
//  Project2VoteR
//
//  Created by Miguel Barrios on 10/22/18.
//  Copyright © 2018 Miguel Barrios. All rights reserved.
//

#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
    OULink<T>* current;
public:
    OULinkedListEnumerator(OULink<T>* first);
    bool hasNext() const;
    T next();
    T peek() const;
};

template <typename T>
OULinkedListEnumerator<T>::OULinkedListEnumerator(OULink<T>* first)
{
    current = first;
}

template <typename T>
bool OULinkedListEnumerator<T>::hasNext() const
{
    if(current == NULL)
    {
        return false;
    }
    return true;
}

template <typename T>
T OULinkedListEnumerator<T>::next()
{
    if(current == NULL)
    {
        throw new ExceptionEnumerationBeyondEnd;
    }
    T object = *current -> data;
    current = current -> next;
    return object;
    
}

template <typename T>
T OULinkedListEnumerator<T>::peek() const
{
    if(!hasNext())
    {
        throw new ExceptionEnumerationBeyondEnd;
    }
    T object = *current -> data;
    return object;
}

#endif // !OU_LINKED_LIST_ENUMERATOR
