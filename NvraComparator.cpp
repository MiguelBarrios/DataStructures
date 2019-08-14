
#include <stdio.h>
#include "NvraComparator.h"
#include "NvraRecord.h"

using namespace std;

NvraComparator::NvraComparator(unsigned int column)
{
    this -> column = column;
}

int NvraComparator::compare(const NvraRecord& item1, const NvraRecord &item2) const
{
    if(this -> column == 3 || this -> column == 11 || this -> column == 12)
    {
        string one;
        string two;
        if(column == 3)
        {
            one = item1.getString(0);
            two = item2.getString(0);
        }
        else if(column == 11)
        {
            one = item1.getString(1);
            two = item2.getString(1);
        }
        else
        {
            one = item1.getString(2);
            two = item2.getString(2);
        }
        if(one < two)
            return -1;
        else if(one > two)
            return 1;
        else
            return 0;
                
    }
    else
    {
        int correctCollum = 0;
        
        if(column < 3)
            correctCollum = column;
        else if(column < 11)
            correctCollum = column - 1;
        else if(column > 12)
            correctCollum = column - 3;
        
        if(item1.getNum(correctCollum) < item2.getNum(correctCollum))
            return -1;
        else if(item1.getNum(correctCollum) > item2.getNum(correctCollum))
            return 1;
        else
            return 0;
    }
    
    return 0;
}
