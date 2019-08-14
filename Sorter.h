// Signature must be retained. Implementation must be added.
#
#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"
#include "NvraComparator.h"

using namespace std;

template <typename T>
class Sorter {
private:
    // additional member functions (methods) and variables (fields) may be added
    static void quickSort(TemplatedArray<T>& array, unsigned long start, unsigned long end, const Comparator<T>& comparator);
    static unsigned long partition(TemplatedArray<T>& array, unsigned long start, unsigned long end, const Comparator<T>& comparator);
    static TemplatedArray<T> mergeSort();
    TemplatedArray<T> subArray(TemplatedArray<T> array, int start, int stop);
public:
    static void sort(TemplatedArray<T>& array, const Comparator<T>& comparator);
};

// implementation goes here
template <typename T>
void Sorter<T>::sort(TemplatedArray<T>& array, const Comparator<T>& comparator)
{
    quickSort(array, 0, array.getSize() - 1, comparator);
}

//The general algorithm designed was borrowed from http://www.algolist.net/Algorithms/Sorting/Quicksort
template<typename T>
void Sorter<T>::quickSort(TemplatedArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator)
{
    unsigned long j = 0;
    
    if(left >= right)
    {
        return;
    }
    
    j = partition(array, left, right, comparator);
    
    quickSort(array, left, j, comparator);
    quickSort(array, j + 1, right, comparator);
    
}
template<typename T>
unsigned long Sorter<T>::partition(TemplatedArray<T>& array, unsigned long left, unsigned long right,const Comparator<T>& comparator)
{
    unsigned long leftIndex,rightIndex,midpoint;
    leftIndex = left;
    rightIndex = right;
    midpoint = (right + left) / 2;
    //T pivotValue = array.get(midpoint);
    T pivotValue = array[midpoint];
    bool done = false;
    
    while(!done)
    {
        while(comparator.compare(array.get(leftIndex), pivotValue) == -1)
        {
            ++leftIndex;
        }
        while(comparator.compare(array.get(rightIndex), pivotValue) == 1)
        {
            --rightIndex;
        }
        if(leftIndex >= rightIndex)
        {
            done = true;
        }
        else
        {
            //swaps elements at left and right indicies
            T temp = array[leftIndex];
            T rightRecord = array[rightIndex];
            array.replaceAt(&rightRecord, leftIndex);
            array.replaceAt(&temp, rightIndex);

            ++leftIndex;
            --rightIndex;
        }
    }
    return rightIndex;
}

template<typename T>
TemplatedArray<T> Sorter<T>::mergeSort()
{
    
}

template<typename T>
T subArray(TemplatedArray<T> array, int start, int stop)
{
    T subArray[stop - start];
    for(int i = start; i < stop; ++i)
    {
        subArray[i] = array[i];
    }
    
}
#endif
