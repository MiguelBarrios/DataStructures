#ifndef OU_LINK_H
#define OU_LINK_H
#include "NvraRecord.h"

template <typename T>
class OULink {
    
    template <typename typeOne>
    friend class OULinkedList;
    template <typename typeTwo>
    friend class OULinkedListEnumerator;
     
private:
    T* data = NULL;                                        // pointer to data item of any type
    OULink* next = NULL;                                // pointer to next link
public:
    OULink(const T* item);
    virtual ~OULink();
};

template <typename T>
OULink<T>::OULink(const T* item)
{
    data = new T(*item);
}

template <typename T>
OULink<T>::~OULink()
{
    delete data;
    data = NULL;
    if(next != NULL)
        delete next;
    next = NULL;
}

#endif // !OU_LINK_H
