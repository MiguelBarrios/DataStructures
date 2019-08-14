// These definitions may be used as a basis for your templated array class. However, you are
// not required to use them exactly as they are. You may change details as you see fit.
// Nonetheless, you must retain the signatures of all public member functions (methods) and
// their described functionality, so that they may be used in unit tests.

#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;        // capacity used in no arg constructor

template <typename T>
class TemplatedArray {
private:
    unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
    unsigned long size = 0;                                // actual number of items currently in array
    T* data = NULL;                                        // pointer to array of any type
    void doubleCapacity();                                // method to double array capacity
    void halveCapacity();                                // method to halve array capacity
public:
    TemplatedArray();                                    // constructs array with default capacity
    TemplatedArray(unsigned long capacity);                // constructs array with specified capacity
    virtual ~TemplatedArray();                            // frees array space as object is deleted
    void add(const T* item);                            // adds item, increments size, doubles capacity as necessary
    void addAt(const T* item, unsigned long index);        // adds item at index, shifts following, doubles capacity as necessary
    void replaceAt(const T* item, unsigned long index); // replaces item at index, otherwise unchanged
    void removeAt(unsigned long index);                    // removes item at index, shifts following back
    T get(unsigned long index) const;                    // returns (copy of) item at index
    unsigned long getCapacity() const;                    // returns the current capacity of the array
    unsigned long getSize() const;
    // returns the current number of items in the array
    T operator[](unsigned long index) const
    {
        return get(index);
    }
     // returns (reference to) item at index
};

template <class T>
TemplatedArray<T>::TemplatedArray()
{
    data = new T[capacity];
}

template <class T>
TemplatedArray<T>::TemplatedArray(unsigned long capacity)
{
    this -> capacity = capacity;
    data = new T[capacity];
}

template <class T>
TemplatedArray<T>::~TemplatedArray()
{
    delete [] data;
    data = NULL;
}
template <class T>
void TemplatedArray<T>::doubleCapacity()
{
    T* newArray = new T[capacity * 2];
    if(newArray == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    //Assigns data from old array to new array
    for(unsigned long i = 0; i < size; ++i)
    {
        newArray[i] = data[i];
    }
    delete [] data;
    data = nullptr;
    data = newArray;
    capacity = size * 2;
}
template <class T>
void TemplatedArray<T>::halveCapacity()
{
    T* newArray = new T[capacity / 2];
    if(newArray == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    //Assigns data from old array to new array
    for(unsigned long i = 0; i < size; ++i)
    {
        newArray[i] = data[i];
    }
    delete [] data;
    data = nullptr;
    data = newArray;
    capacity = size / 2;
}

template <class T>
void TemplatedArray<T>::add(const T* record)
{
    //checks to see if array will need to be doubled
    if(size == capacity)
    {
        doubleCapacity();
    }
    data[size] = *record;
    ++size;
}

template <class T>
void TemplatedArray<T>::addAt(const T* item, unsigned long index)
{
    if(index > size)
    {
        throw new ExceptionIndexOutOfRange;
    }
    //checks to see if array needs to be doubled
    if(size == capacity)
    {
        doubleCapacity();
    }
    
    // shifts all the elements ahead of item index
    for(unsigned long i = size - 1; i >= index; --i)
    {
        data[i + 1] = data[i];
        if(i == 0)
            break;
    }
    data[index] = *item;
    ++size;
}

template <class T>
void TemplatedArray<T>::replaceAt(const T* item,unsigned long index)
{
    if(index >= size)
    {
        throw new ExceptionIndexOutOfRange;
    }
    data[index] = *item;
}

template <class T>
void TemplatedArray<T>::removeAt(unsigned long index)
{
    if(index >= size)
    {
        throw new ExceptionIndexOutOfRange;
    }
    
    for(unsigned long i = index; i < size; ++i)
    {
        data[i] = data[i +1];
    }
    --size;
    // check to see if data can fit an halved array
    if(size < capacity / 2)
    {
        halveCapacity();
    }
}

template <class T>
T TemplatedArray<T>::get(unsigned long index) const
{
    if(index >= size)
    {
        throw new ExceptionIndexOutOfRange;
    }
    
    T record = data[index];
    return record;
}

template <class T>
unsigned long TemplatedArray<T>::getCapacity() const
{
    return capacity;
}

template <class T>
unsigned long TemplatedArray<T>::getSize() const
{
    return size;
}
#endif
