#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
	unsigned long bucket = 0;								// the current bucket during the enumeration process
	OULinkedListEnumerator<T>* chainEnumerator = NULL;		// used to move through the linked list of the current bucket
	HashTable<T>* hashTable = NULL;							// pointer to the HashTable being enumerated
	// you may add additional member variables and functions here to support the operation of your code
public:
	HashTableEnumerator(HashTable<T>* hashTable);			// constructor needs a pointer to the HashTable to be enumerated
	virtual ~HashTableEnumerator();
	bool hasNext() const;									// true if there are elements that have not yet been returned via next()
	T next();												// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;											// throws ExceptionEnumerationBeyondEnd if no next item is available
};

template <typename T>
HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable)
{
    if(hashTable -> getSize() == 0)
    {
        throw new ExceptionHashTableAccess;             //MIGHT BE A DIFFERENT TYPE OF EXCEPTION
    }
    
    this -> hashTable = hashTable;
    chainEnumerator = new OULinkedListEnumerator<T>(hashTable -> table[bucket] -> enumerator());            //has not been tested
}

template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator()
{
    //??? pointers
}

template <typename T>
bool HashTableEnumerator<T>::hasNext() const                    //Has not been tested
{
    OULinkedListEnumerator<T> tempEnumerator = *chainEnumerator;
    
    if (tempEnumerator.hasNext())
    {
        return true;
    }
    else
    {
        unsigned long tempBucket = bucket;
        ++tempBucket;
        while(tempBucket < hashTable -> baseCapacity)                        //check math
        {
            tempEnumerator = hashTable -> table[tempBucket] -> enumerator();
            if(tempEnumerator.hasNext())
            {
                return true;
            }
            ++tempBucket;
        }
        return false;
    }
    
}

template<typename T>
T HashTableEnumerator<T>::next()
{
    if(chainEnumerator -> hasNext())
    {
        return chainEnumerator -> next();
    }
    else
    {
        while(!chainEnumerator -> hasNext() && bucket < hashTable -> getBaseCapacity())
        {
            ++bucket;
             chainEnumerator = new OULinkedListEnumerator<T>(hashTable -> table[bucket] -> enumerator());
        }
        return chainEnumerator -> next();
    }
    throw new ExceptionEnumerationBeyondEnd;
}

template <typename T>
T HashTableEnumerator<T>::peek() const
{
    
    if(chainEnumerator -> hasNext())
    {
        return chainEnumerator -> peek();
    }
    else
    {
        unsigned long tempBucket = bucket;
        OULinkedListEnumerator<T> tempChainEnumerator = *chainEnumerator;
        while(!tempChainEnumerator.hasNext() && tempBucket < hashTable -> getBaseCapacity())
        {
            ++tempBucket;
            tempChainEnumerator = OULinkedListEnumerator<T>(hashTable -> table[tempBucket] -> enumerator());
        }
        return tempChainEnumerator.peek();                                                           //Has not been tested
    }
    throw new ExceptionEnumerationBeyondEnd;
     
}
#endif // !HASH_TABLE_ENUMERATOR
