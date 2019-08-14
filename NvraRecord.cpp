//
//  NvraRecord.cpp
//  VoteR3
//
//  Created by Miguel Barrios on 9/10/18.
//  Copyright © 2018 Miguel Barrios. All rights reserved.
//

#include "NvraRecord.h"
#include <iostream>
using namespace std;

void NvraRecord::addNum(unsigned int num)
{
    nums[numCtr] = num;
    ++numCtr;
}
void NvraRecord::addString(std::string string)
{
    strings[strCtr] = string;
    ++strCtr;
}
unsigned int NvraRecord::getNum(unsigned int num) const
{
    return nums[num];
}
std::string NvraRecord::getString(unsigned int index) const
{
    return strings[index];
}

void NvraRecord::setNum(unsigned int num, unsigned int index)
{
    int correctCollum = 0;
    
    if(index < 3)
        correctCollum = index;
    else if(index < 11)
        correctCollum = index - 1;
    else if(index > 12)
        correctCollum = index - 3;
            
    this -> nums[correctCollum] = num;
}
void NvraRecord::setString(string string, unsigned int index)
{
    int correctCollum;
    
    if(index == 3)
        correctCollum = 0;
    else if(index == 11)
        correctCollum = 1;
    else
        correctCollum = 2;
    
    this -> strings[correctCollum] = string;
}

std::ostream& operator<<(std::ostream& os, const NvraRecord& record)
{
    os << record.nums[0] << ";" << record.nums[1] << ";" << record.nums[2] << ";" << record.strings[0] << ";"
    << record.nums[3] << ";" << record.nums[4] << ";" << record.nums[5] << ";" << record.nums[6] << ";"
    << record.nums[7] << ";" << record.nums[8] << ";" << record.nums[9] << ";"<< record.strings[1]<< ";"
    << record.strings[2]<< ";"  << record.nums[10] << ";"<< record.nums[11] << ";" << record.nums[12] << ";"
    << record.nums[13] << ";"  << record.nums[14] << ";" << record.nums[15] << ";" << record.nums[16] << ";"
    << record.nums[17] << ";" << record.nums[18] << ";"  << record.nums[19] << ";" << record.nums[20];
    
    return os;
}
