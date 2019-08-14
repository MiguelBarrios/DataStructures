#ifndef INT_HASHER_H
#define INT_HASHER_H

#include "Hasher.h"

class IntHasher : public Hasher<int> {
public:
	unsigned long hash(const int& item) const;	// implement a hash function appropriate for NvraRecords in NvraHasher.cpp
};

#endif //!INT_HASHER_H