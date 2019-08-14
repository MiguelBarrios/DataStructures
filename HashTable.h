#ifndef HASH_TABLE
#define HASH_TABLE

#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"

const unsigned int SCHEDULE_SIZE = 25;            // the number of items in the size schedule
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };        // the size schedule (all primes)
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;    // the default position in the size schedule
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX];     // the default size of the array
const float DEFAULT_MAX_LOAD_FACTOR = 0.9f;        // the default load factor used to determine when to increase the table size
const float DEFAULT_MIN_LOAD_FACTOR = 0.4f;        // the default load factor used to determine when to decrease the table size

template <typename T>
class HashTable {
    template <typename U>
    friend class HashTableEnumerator;                        // necessary to allow the enumerator to access the table's private data
private:
    Comparator<T>* comparator = NULL;                        // used to determine item equality
    Hasher<T>* hasher = NULL;                                // used to compute hash value
    unsigned long size = 0;                                    // actual number of items currently in hash table
    float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;            // the load factor used to determine when to increase the table size
    float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;            // the load factor used to determine when to decrease the table size
    unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;    // the index of current location in the size schedule
    unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;        // the size of the array
    unsigned long totalCapacity = baseCapacity;                // the size of the array plus chains of more than one link
    OULinkedList<T>** table = NULL;                            // table will be an array of pointers to OULinkedLists of type T
    // you may add additional member variables and functions here to support the operation of your code
    
    void deleteTable();
    void expandTable();
    void contractTable();
    OULinkedList<T>** initializeTable(unsigned long size);
    
public:
    HashTable(Comparator<T>* comparator, Hasher<T>* hasher);            // creates an empty table of DEFAULT_BASE_CAPACITY
   HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
              // if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
              unsigned long size,
              float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
              float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
    
    virtual ~HashTable();
    
    // if an equivalent item is not already present, insert item at proper location and return true
    // if an equivalent item is already present, leave table unchanged and return false
    bool insert(T* item);
    
    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave table unchanged and return false
    bool replace(T* item);
    
    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave table unchanged and return false
    bool remove(T* item);
    
    // if an equivalent item is present, return a copy of the item
    // if an equivalent item is not present, throw a new ExceptionHashTableAccess
    T find(const T* item) const;
    
    unsigned long getSize() const;                        // returns the current number of items in the table
    unsigned long getBaseCapacity() const;                // returns the current base capacity of the table
    unsigned long getTotalCapacity() const;                // returns the current total capacity of the table
    float getLoadFactor() const;                        // returns the current load factor of the table
    unsigned long getBucketNumber(const T* item) const;    // returns the bucket number for an item using its hash function mod array size
    
    
};

template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher)
{
    this -> comparator = comparator;
    this -> hasher = hasher;
    
    this -> table = initializeTable(DEFAULT_BASE_CAPACITY);
}

template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher, unsigned long size, float maxLoadFactor, float minLoadFactor)
{
    this -> comparator = comparator;
    this -> hasher = hasher;
    this -> minLoadFactor = minLoadFactor;
    this -> maxLoadFactor = maxLoadFactor;
    
    
    for(int i = 3; i < 25; ++i)
    {//Finds appropreate hashTable Size
        if(SCHEDULE[i] <= size)
        {
            ++scheduleIndex;
        }
    }
    ++scheduleIndex;
    size = SCHEDULE[scheduleIndex];
    
    
    this -> table = initializeTable(size);
    this -> baseCapacity = SCHEDULE[scheduleIndex];
    this -> totalCapacity = SCHEDULE[scheduleIndex];
}

template <typename T>
OULinkedList<T>** HashTable<T>::initializeTable(unsigned long size)
{
    OULinkedList<T>** newTable = new OULinkedList<T>*[size];
    if(newTable == NULL)
    {
        throw new ExceptionMemoryNotAvailable;
    }
    
    for(unsigned long i = 0; i < size; ++i)
    {
        OULinkedList<T>* bucket = new OULinkedList<T>(comparator);
        if(bucket == NULL)
        {
            throw new ExceptionMemoryNotAvailable;
        }
        newTable[i] = bucket;
    }
    
    return newTable;
}

template <typename T>
void HashTable<T>::expandTable()
{
    ++scheduleIndex;
    baseCapacity = SCHEDULE[scheduleIndex];
    totalCapacity = baseCapacity;
    OULinkedList<T>** expandedTable = initializeTable(baseCapacity);
    
    for(unsigned int i = 0; i < SCHEDULE[scheduleIndex - 1]; ++i)
    {
        OULinkedList<T>* currentBucket = table[i];
        
        if(currentBucket -> getSize() > 0)
        {
            OULinkedListEnumerator<T> enumerator = currentBucket -> enumerator();
            while (enumerator.hasNext())
            {
                T* record = new T(enumerator.next());
                unsigned long index = getBucketNumber(record);

                expandedTable[index] -> insert(record);
            }
        }
    }

    delete table;
    table = expandedTable;
}

template <typename T>
void HashTable<T>::contractTable()                              //Has not been tested
{
    --scheduleIndex;
    baseCapacity = SCHEDULE[scheduleIndex];
    totalCapacity = baseCapacity;
    OULinkedList<T>** contractedTable = initializeTable(baseCapacity);
    
    for(unsigned int i = 0; i < SCHEDULE[scheduleIndex + 1]; ++i)
    {
        OULinkedList<T>* currentBucket = table[i];
        
        if(currentBucket -> getSize() > 0)
        {
            OULinkedListEnumerator<T> enumerator = currentBucket -> enumerator();
            while (enumerator.hasNext())
            {
                T* record = new T(enumerator.next());
                unsigned long index = getBucketNumber(record);
                
                contractedTable[index] -> insert(record);
            }
        }
    }
    
    delete table;
    table = contractedTable;

}

template <typename T>
HashTable<T>::~HashTable()                                  //has not been tested
{
    deleteTable();
    table = NULL;
}

template <typename T>
void HashTable<T>::deleteTable()                            //Has not been tested
{
    for(unsigned long i = 0; i < baseCapacity; ++i)
    {
        delete table[i];
        table[i] = NULL;
    }
}

template <typename T>
bool HashTable<T>::insert(T* item)
{
    unsigned long index = getBucketNumber(item);
    
    if(table[index] -> insert(item))
    {
        ++size;
        if(getLoadFactor() >= maxLoadFactor)
        {
            expandTable();
        }
        
        if (table[index] -> getSize() > 1)
        {
            ++totalCapacity;
        }
        return true;
    }
    
    return false;
}

template <typename T>
bool HashTable<T>::replace(T* item)                                         //has not been tested
{
    unsigned long itemIndex = getBucketNumber(item);
    
    if(table[itemIndex] -> replace(item))
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
bool HashTable<T>::remove(T* item)                                              //has not been tested
{
    unsigned long index = getBucketNumber(item);
    
    if(table[index] -> remove(item))
    {
        --size;
        if(table[index] -> getSize() >= 1)
        {
            --totalCapacity;
        }
        else if(table[index] -> getSize() == 0)         //Testt
        {
            --totalCapacity;
        }
        
        if(getLoadFactor() <= minLoadFactor)
        {
            contractTable();
        }
        return true;
    }
    
    return false;
}

template <typename T>
T HashTable<T>::find(const T* item) const                               //has not been tested
{
    OULinkedList<T>* bucket = table[getBucketNumber(item)];
    
    try
    {
        return bucket -> find(item);
    }
    catch (ExceptionLinkedListAccess* e)                //test to see if correct
    {
       delete e;
       throw new ExceptionHashTableAccess;
    }
}

template <typename T>
unsigned long HashTable<T>::getSize() const
{
    return size;
}

template <typename T>
unsigned long HashTable<T>::getBaseCapacity() const
{
    return baseCapacity;
}

template <typename T>
unsigned long HashTable<T>::getTotalCapacity() const        // the size of the array plus chains of more than one link
{
    return totalCapacity;
}

template <typename T>
float HashTable<T>::getLoadFactor() const
{
    return (float)size / (float)totalCapacity;
}

template <typename T>
unsigned long HashTable<T>::getBucketNumber(const T* item) const
{
    return hasher -> hash(*item) % baseCapacity;
}
#endif // !HASH_TABLE
