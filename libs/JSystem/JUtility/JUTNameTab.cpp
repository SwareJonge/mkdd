#include <string.h>
#include "types.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "JSystem/JUtility/JUTDbg.h"

JUTNameTab::JUTNameTab() { setResource(nullptr); }

JUTNameTab::JUTNameTab(const ResNTAB *pNameTable) { setResource(pNameTable); }

void JUTNameTab::setResource(const ResNTAB *pNameTable)
{
    mNameTable = pNameTable;

    if (pNameTable)
    {
        mNameNum = pNameTable->mEntryNum;
        mStrData = (const char *)(pNameTable->mEntries + mNameNum);
    }
    else
    {
        mNameNum = 0;
        mStrData = nullptr;
    }
}

s32 JUTNameTab::getIndex(const char *pName) const
{
#line 101
    JUT_ASSERT(mNameTable != 0);
    const ResNTAB::Entry *pEntry = mNameTable->mEntries;
    u16 keyCode = calcKeyCode(pName);

    for (u16 i = 0; i < mNameNum; pEntry++, i++)
        if (pEntry->mKeyCode == keyCode && strcmp((mNameTable->mEntries[i].mOffs + ((const char *)mNameTable)), pName) == 0)
            return i;

    return -1;
}

const char *JUTNameTab::getName(u16 index) const
{
#line 138
    JUT_ASSERT(mNameTable != 0);
    if (index < mNameNum)
        return mNameTable->getName(index);
    return nullptr;
}


u16 JUTNameTab::calcKeyCode(const char *pName) const
{
    u32 keyCode = 0;
    while (*pName)
        keyCode = (keyCode * 3) + *pName++;
    return keyCode;
}