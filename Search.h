// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
template <typename T>
long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator)
{
    // implementation goes here
    long long left = 0;
    long long right = array.getSize() - 1;
    long long middle = (left + right) / 2;
    while(left <= right)
    {
        int location = comparator.compare(array[middle], item);
        if(location == 1)
        {
            right = middle - 1;
        }
        else  if(location == -1)
        {
            left = middle + 1;
        }
        else
        {//backtracks to find first occurrence of item
            for(long long index = middle; index >= 0; --index)
            {
                if(comparator.compare(array[index], item) != 0)
                {
                    return index + 1;
                }
            }
            return 0;
        }
        
        middle = (left + right) / 2;
    }
    if(left > middle)
        return (left + 1) * -1;
    
    return (middle + 1) * -1;
}

#endif

