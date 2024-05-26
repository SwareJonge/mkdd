#include "JSystem/JAudio/JAUBankTable.h"

void JAUBankTable::copyToDefault()
{
    // UNUSED
}

void JAUBankTableDictionary::seekBankTable(u32)
{
    // UNUSED
}

void JAUBankTableDictionary::removeBankTable(u32)
{ 
    // UNUSED
}

void JAUBankTableDictionary::appendBankTable(JSULink<JAUBankTable> *link)
{
    append(link);
}

bool JAUBankTable::isAllRegistered() const // fabricated
{
    for(u32 i = 0; i < _c; i++) 
    {
        if (getBank(i) == NULL)
            return false;
    }

    return true;
}