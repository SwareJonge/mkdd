#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASWSParser.h"

u32 JASWSParser::sUsedHeapSize;

u32 JASWSParser::getGroupCount(const void *ptr)
{
    return ((THeader *)ptr)->ctrlGroupOffset.ptr(ptr)->ctrlGroupCount;
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

    TCtrlGroup *group = headerPtr->ctrlGroupOffset.ptr(headerPtr);
    basicBank->setGroupCount(group->ctrlGroupCount, pHeap);
    u32 waveTableSize = 0;

    for (u32 i = 0; i < group->ctrlGroupCount; i++)
    {
        TCtrlScene *scene = group->ctrlSceneOffsets[i].ptr(headerPtr);
        TCtrl *ctrl = scene->ctrlOffset.ptr(headerPtr);
        JASBasicWaveBank::TWaveGroup *waveGroup = basicBank->getWaveGroup(i);
        TWaveArchiveBank *archiveBank = headerPtr->archiveBankOffset.ptr(headerPtr);
        TWaveArchive *waveArchive = archiveBank->archiveOffsets[i].ptr(headerPtr);
        waveGroup->setWaveCount(ctrl->waveCount, pHeap);

        for (int j = 0; j < ctrl->waveCount; j++)
        {
            TWave *wave = waveArchive->waveOffsets[j].ptr(headerPtr);

            JASWaveInfo waveInfo;
            waveInfo._00 = wave->_01;
            waveInfo._01 = wave->_02;
            waveInfo._04 = wave->_04;
            waveInfo._08 = wave->offset;
            waveInfo._0c = wave->_0C;
            waveInfo._10 = wave->_10;
            waveInfo._14 = wave->_14;
            waveInfo._18 = wave->_18;
            waveInfo._1c = wave->_1C;
            waveInfo._20 = wave->_20;
            waveInfo._22 = wave->_22;

            TCtrlWave *ctrlWave = ctrl->ctrlWaveOffsets[j].ptr(headerPtr);
            u32 tableSize = JSULoHalf(ctrlWave->tableSize);
            waveGroup->setWaveInfo(j, tableSize, waveInfo);
            if (waveTableSize < tableSize)
            {
                waveTableSize = tableSize;
            }
        }
        waveGroup->setFileName(waveArchive->fileName);
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
    TCtrlGroup *group = headerPtr->ctrlGroupOffset.ptr(headerPtr);

    if (group->ctrlGroupCount != 1)
        return NULL;

    JASSimpleWaveBank *simpleBank = new (pHeap, 0) JASSimpleWaveBank();
    if (simpleBank == NULL)
        return NULL;

    u32 waveTableSize = 0;
    TCtrlScene *scene = group->ctrlSceneOffsets[0].ptr(headerPtr);
    TCtrl *ctrl = scene->ctrlOffset.ptr(headerPtr);
    TWaveArchiveBank *archiveBank = headerPtr->archiveBankOffset.ptr(headerPtr);
    TWaveArchive *waveArchive = archiveBank->archiveOffsets[0].ptr(headerPtr);

    for (u32 i = 0; i < ctrl->waveCount; i++)
    {
        TCtrlWave *ctrlWave = ctrl->ctrlWaveOffsets[i].ptr(headerPtr);
        u16 tableSize = JSULoHalf(ctrlWave->tableSize);
        if (waveTableSize < tableSize)
        {
            waveTableSize = tableSize;
        }
    }

    simpleBank->setWaveTableSize(waveTableSize + 1, pHeap);
    for (u32 i = 0; i < ctrl->waveCount; i++)
    {
        TWave *wave = waveArchive->waveOffsets[i].ptr(headerPtr);

        JASWaveInfo waveInfo;
        waveInfo._00 = wave->_01;
        waveInfo._01 = wave->_02;
        waveInfo._04 = wave->_04;
        waveInfo._08 = wave->offset;
        waveInfo._0c = wave->_0C;
        waveInfo._10 = wave->_10;
        waveInfo._14 = wave->_14;
        waveInfo._18 = wave->_18;
        waveInfo._1c = wave->_1C;
        waveInfo._20 = wave->_20;
        waveInfo._22 = wave->_22;

        TCtrlWave *ctrlWave = ctrl->ctrlWaveOffsets[i].ptr(headerPtr);
        u16 size = JSULoHalf(ctrlWave->tableSize);
        simpleBank->setWaveInfo(size, waveInfo);
    }
    simpleBank->setFileName(waveArchive->fileName);

    sUsedHeapSize += oldSize - pHeap->getFreeSize();

    return simpleBank;
}

u32 JASWSParser::getUsedHeapSize() { return sUsedHeapSize; }