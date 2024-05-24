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

bool JAUBankTable::isAllRegistered() const
{
    // UNUSED
    JAUBankTable *table = new JAUBankTable(0, NULL, 0);
}