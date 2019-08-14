//
//  NvraHasher.cpp
//  VoteR3.0
//
//  Created by Miguel Barrios on 11/8/18.
//  Copyright © 2018 Miguel Barrios. All rights reserved.
//

#include <stdio.h>
#include "NvraHasher.h"

unsigned long NvraHasher::hash(const NvraRecord& item) const
{
    return item.getNum(0);
}
