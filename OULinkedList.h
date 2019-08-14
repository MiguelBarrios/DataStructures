//
//  OULinkedList.h
//  Project2VoteR
//
//  Created by Miguel Barrios on 10/22/18.
//  Copyright © 2018 Miguel Barrios. All rights reserved.
//

#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

using namespace std;

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
    template <typename typeTwo>
    friend class OULinkedListEnumerator;
private:
    Comparator<T>* comparator = NULL;                // used to determine list order and item equality
    unsigned long size = 0;                            // actual number of items currently in list
    OULink<T>* first = NULL;                        // pointer to first link in list
    OULink<T>* last = NULL;                            // pointer to last link in list
public:
    OULinkedList(Comparator<T>* comparator);        // creates empty linked list with comparator
    virtual ~OULinkedList();                        // deletes all links and their data items
    void printList();                               //Used for testing list order

    // if an equivalent item is not already present, insert item in order and return true
    // if an equivalent item is already present, leave list unchanged and return false
    bool insert(const T* item);
    
    // if item is greater than item at last, append item at end and return true
    // if item is less than or equal to item at last, leave list unchanged and return false
    bool append(const T* item);
    
    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave list unchanged and return false
    bool replace(T* item);
    
    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave list unchanged and return false
    bool remove(T* item);
    
    // if any items are present, return a copy of the first item
    // if no items are present, throw new ExceptionLinkedListAccess
    T get() const;
    
    // if an equivalent item is present, return a copy of the first such item
    // if an equivalent item is not present, throw a new ExceptionLinkedListAccess
    T find(const T* item) const;
    
    unsigned long getSize() const;                    // returns the current number of items in the list
    
    OULinkedListEnumerator<T> enumerator() const;    // create an enumerator for this linked list
    
    //Retunns a templated array or items
    TemplatedArray<T>* transferToArray();
};

template <typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator)
{
    this -> comparator = comparator;
}

template <typename T>
OULinkedList<T>::~OULinkedList()
{
    delete first;
}

// if item is greater than item at last, append item at end and return true
// if item is less than or equal to item at last, leave list unchanged and return false
template <typename T>
bool OULinkedList<T>::append(const T* item)
{
    if(first == NULL)                           //If list is empy assigns item to first and last link
    {
        OULink<T>* link = new OULink<T>(item);
        first = link;
        last = link;
        ++size;
        return true;
    }
    
     int placeInList = comparator -> compare(*item, *last -> data);
     if(placeInList == 1)
     {
         OULink<T>* newLink = new OULink<T>(item);
         last -> next = newLink;
         last = newLink;
         ++size;
         return true;
     }
     return false;
}

template <typename T>
bool OULinkedList<T>::insert(const T* item)
{
    //If item belongs at the end of list
    if(append(item))
    {
        return true;
    }//If item belongs at the front of the List
    else if(comparator -> compare(*item, *first -> data) == -1)
    {
        OULink<T>* link = new OULink<T>(item);
        
        link -> next = first;
        first = link;
        ++size;
        return true;
    }
    
    else
    {
        OULink<T>* current = first;
        
        while(current -> next != NULL)
        {
            int placeInList = comparator -> compare(*item, *current -> next -> data);
            if(placeInList == 0)
            {
                return false;
            }
            else if(placeInList == -1)
            {
                OULink<T>* temp = current -> next;
                OULink<T>* link = new OULink<T>(item);
                current -> next = link;
                link -> next = temp;
                ++size;
                return true;
            }
            current = current -> next;
        }
    }
    return false;
}

//Replaces record if already present in list
template <typename T>
bool OULinkedList<T>::replace(T* item)
{
    OULink<T>* current = first;
    
    while(current -> data != NULL)
    {
        if(comparator -> compare(*item, *current -> data) == 0)
        {
            current -> data = item;
            return true;
        }
        current = current -> next;
    }
    return false;
}

//removes record if already in list
template <typename T>
bool OULinkedList<T>::remove(T *item)
{
    OULink<T>* current = first;
    
    if(comparator -> compare(*current -> data, *item) == 0)
    {
        if(size == 1)                   //Item to be removed is at the the only element in list
        {
            delete first;
            first = NULL;
            size = 0;
        }
        else                           //Item is the first link in lest
        {
            first = first -> next;
            current -> next = NULL;
            delete current;
            --size;
        }
        return true;
    }
    else
    {
        while(current -> next != NULL)          //itterates list to find element
        {
            OULink<T>* previous = current;
            current = current -> next;
            if(comparator -> compare(*current -> data, *item) == 0)
            {
                if(comparator -> compare(*last -> data, *item) == 0)
                {
                    delete last;
                    last = previous;
                    last -> next = NULL;
                    --size;
                    return true;
                }
                else
                {
                    previous -> next = current -> next;
                    current -> next = NULL;
                    delete current;
                    --size;
                    return true;
                }
            }
        }
    }
    return false;
}

template <typename T>
T OULinkedList<T>::get() const
{
    if(size == 0)
        throw new ExceptionLinkedListAccess;
    
    return *first -> data;
}

template <typename T>
T OULinkedList<T>::find(const T* item) const
{
    if(size == 0)
        throw new ExceptionLinkedListAccess;
    
    OULink<T>* current = first;
    //while(current -> next != NULL)
    while(current != NULL)
    {
        if(comparator -> compare(*current -> data, *item) == 0)
        {
            T copy = *current -> data;
            return copy;
        }
        current = current -> next;
    }
    throw new ExceptionLinkedListAccess;            //Item is not present
}

template <typename T>
unsigned long OULinkedList<T>::getSize() const
{
    return size;
}

template <typename T>
void OULinkedList<T>::printList()
{
    OULink<T>* current = first;
    
    while(current -> data != NULL)
    {
        cout << *current -> data << endl;
        if(current -> next == NULL)
            break;
        current = current -> next;
    }
}

template <typename T>
OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const
{
    OULinkedListEnumerator<T> enumerator(first);
    return enumerator;
}

template <typename T>
TemplatedArray<T>* OULinkedList<T>::transferToArray()
{
    TemplatedArray<T>* array = new TemplatedArray<T>(size);
    OULink<T>* current = first;
    
    while(current != NULL)
    {
        array -> add(current -> data);
        
        if(current -> next == NULL)
            break;
        current = current -> next;
    }
    return array;
}
#endif // !OU_LINKED_LIST
