#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASWSParser.h"

u32 JASWSParser::sUsedHeapSize;

u32 JASWSParser::getGroupCount(const void *ptr)
{
    return ((THeader *)ptr)->mCtrlGroupOffset.ptr(ptr)->mCtrlGroupCount;
}

JASWaveBank *JASWSParser::createWaveBank(const void *ptr, JKRHeap *pHeap)
{
    if (getGroupCount(ptr) == 1)
    {
        return createSimpleWaveBank(ptr, pHeap);
    }
    else
    {
        return createBasicWaveBank(ptr, pHeap);
    }
}

JASBasicWaveBank *JASWSParser::createBasicWaveBank(const void *ptr, JKRHeap *pHeap)
{
    if (pHeap == NULL)
        pHeap = JASDram;

    u32 oldSize = pHeap->getFreeSize();
    const THeader *headerPtr = (const THeader *)ptr;

    JASBasicWaveBank *basicBank = new (pHeap, 0) JASBasicWaveBank();
    if (basicBank == NULL)
        return NULL;

    TCtrlGroup *group = headerPtr->mCtrlGroupOffset.ptr(headerPtr);
    basicBank->setGroupCount(group->mCtrlGroupCount, pHeap);
    u32 waveTableSize = 0;

    for (u32 i = 0; i < group->mCtrlGroupCount; i++)
    {
        TCtrlScene *scene = group->mCtrlSceneOffsets[i].ptr(headerPtr);
        TCtrl *ctrl = scene->mCtrlOffset.ptr(headerPtr);
        JASBasicWaveBank::TWaveGroup *waveGroup = basicBank->getWaveGroup(i);
        TWaveArchiveBank *archiveBank = headerPtr->mArchiveBankOffset.ptr(headerPtr);
        TWaveArchive *waveArchive = archiveBank->mArchiveOffsets[i].ptr(headerPtr);
        waveGroup->setWaveCount(ctrl->mWaveCount, pHeap);
        
        for (int j = 0; j < ctrl->mWaveCount; j++)
        {
            TWave *wave = waveArchive->mWaveOffsets[j].ptr(headerPtr);

            JASWaveInfo waveInfo;
            waveInfo._00 = wave->_01;
            waveInfo._01 = wave->_02;
            waveInfo._04 = wave->_04;
            waveInfo._08 = wave->mOffset;
            waveInfo._0c = wave->_0C;
            waveInfo._10 = wave->_10;
            waveInfo._14 = wave->_14;
            waveInfo._18 = wave->_18;
            waveInfo._1c = wave->_1C;
            waveInfo._20 = wave->_20;
            waveInfo._22 = wave->_22;

            TCtrlWave *ctrlWave = ctrl->mCtrlWaveOffsets[j].ptr(headerPtr);
            u32 tableSize = JSULoHalf(ctrlWave->mTableSize);
            waveGroup->setWaveInfo(j, tableSize, waveInfo);
            if (waveTableSize < tableSize)
            {
                waveTableSize = tableSize;
            }
        }
        waveGroup->setFileName(waveArchive->mFileName);
    }
    basicBank->setWaveTableSize(waveTableSize + 1, pHeap);
    sUsedHeapSize += oldSize - pHeap->getFreeSize();

    return basicBank;
}

JASSimpleWaveBank *JASWSParser::createSimpleWaveBank(const void *ptr, JKRHeap *pHeap)
{
    if (pHeap == NULL)
        pHeap = JASDram;

    u32 oldSize = pHeap->getFreeSize();
    const THeader *headerPtr = (const THeader *)ptr;
    TCtrlGroup *group = headerPtr->mCtrlGroupOffset.ptr(headerPtr);

    if (group->mCtrlGroupCount != 1)
        return NULL;

    JASSimpleWaveBank *simpleBank = new (pHeap, 0) JASSimpleWaveBank();
    if (simpleBank == NULL)
        return NULL;

    u32 waveTableSize = 0;
    TCtrlScene *scene = group->mCtrlSceneOffsets[0].ptr(headerPtr);
    TCtrl *ctrl = scene->mCtrlOffset.ptr(headerPtr);
    TWaveArchiveBank *archiveBank = headerPtr->mArchiveBankOffset.ptr(headerPtr);
    TWaveArchive *waveArchive = archiveBank->mArchiveOffsets[0].ptr(headerPtr);

    for (u32 i = 0; i < ctrl->mWaveCount; i++)
    {
        TCtrlWave *ctrlWave = ctrl->mCtrlWaveOffsets[i].ptr(headerPtr);
        u16 tableSize = JSULoHalf(ctrlWave->mTableSize);
        if (waveTableSize < tableSize)
        {
            waveTableSize = tableSize;
        }
    }

    simpleBank->setWaveTableSize(waveTableSize + 1, pHeap);
    for (u32 i = 0; i < ctrl->mWaveCount; i++)
    {
        TWave *wave = waveArchive->mWaveOffsets[i].ptr(headerPtr);

        JASWaveInfo waveInfo;
        waveInfo._00 = wave->_01;
        waveInfo._01 = wave->_02;
        waveInfo._04 = wave->_04;
        waveInfo._08 = wave->mOffset;
        waveInfo._0c = wave->_0C;
        waveInfo._10 = wave->_10;
        waveInfo._14 = wave->_14;
        waveInfo._18 = wave->_18;
        waveInfo._1c = wave->_1C;
        waveInfo._20 = wave->_20;
        waveInfo._22 = wave->_22;

        TCtrlWave *ctrlWave = ctrl->mCtrlWaveOffsets[i].ptr(headerPtr);
        u16 size = JSULoHalf(ctrlWave->mTableSize);
        simpleBank->setWaveInfo(size, waveInfo);
    }
    simpleBank->setFileName(waveArchive->mFileName);

    sUsedHeapSize += oldSize - pHeap->getFreeSize();

    return simpleBank;
}

u32 JASWSParser::getUsedHeapSize() { return sUsedHeapSize; }