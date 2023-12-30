
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/System/JASReport.h"
#include "JSystem/JAudio/JAUSectionHeap.h"
#include "JSystem/JAudio/JAUAudioArchive.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

JAUAudioArcLoader::JAUAudioArcLoader(JAUSection *section)
{
#line 13
    JUT_ASSERT(section->isOpen());
    JUT_ASSERT(section->isBuilding());
    mSection = section;
}

bool JAUAudioArcLoader::load(const void *data)
{
    return parse(data);
}

bool JAUAudioArcLoader::load(const char *path)
{
    const void *archive = mSection->loadDVDFile(path, true, EXPAND_SWITCH_DEFAULT);
    return archive && load(archive);
}

void JAUAudioArcLoader::readWS(u32 bank_no, const void *data, u32 flag)
{
    mSection->newWaveBank(bank_no, data);
    if (flag)
    {
        mSection->loadWaveArc(bank_no, flag);
    }
}

void JAUAudioArcLoader::readBNK(u32 bankNum, const void *data)
{
    mSection->newBank(data, bankNum);
}

void JAUAudioArcLoader::readBSC(const void *data, u32 size)
{
    mSection->newSeSeqCollection(data, size);
}

void JAUAudioArcLoader::readBST(const void *data, u32 size)
{
    mSection->newSoundTable(data, size, true);
}

void JAUAudioArcLoader::readBSTN(const void *data, u32 size)
{
    mSection->newSoundNameTable(data, size, true);
}

void JAUAudioArcLoader::readBMS(u32 soundID, const void *data, u32 size)
{
    mSection->newStaticSeqData(soundID, data, size);
}

void JAUAudioArcLoader::readBMS_fromArchive(u32 soundID)
{
    mSection->newStaticSeqData(soundID);
}

void JAUAudioArcLoader::newDynamicSeqBlock(u32 size)
{
    JAUSectionHeap *sectionHeap = mSection->asSectionHeap();
#line 67
    JUT_ASSERT(sectionHeap);
    sectionHeap->newDynamicSeqBlock(size);
}

void JAUAudioArcLoader::readBSFT(const void *data)
{
    mSection->newStreamFileTable(data, true);
}

void JAUAudioArcLoader::beginBNKList(u32 p1, u32 cnt)
{
    mSection->beginNewBankTable(p1, cnt);
}

void JAUAudioArcLoader::endBNKList()
{
    mSection->endNewBankTable();
}

void JAUAudioArcLoader::readMaxSeCategory(int categoryIndex, int activSE, int inactiveSE)
{
    JAISeMgr *seMgr = JASGlobalInstance<JAISeMgr>::getInstance();
    if (seMgr)
    {
        seMgr->getCategory(categoryIndex)->setMaxActiveSe(activSE);
        seMgr->getCategory(categoryIndex)->setMaxInactiveSe(inactiveSE);
    }
}

JAUAudioArcLoader_withoutCopy::JAUAudioArcLoader_withoutCopy(JAUSection *section)
{
    JUT_ASSERT(section->isOpen());
    JUT_ASSERT(section->isBuilding());
    mSection = section;
}

bool JAUAudioArcLoader_withoutCopy::load(const void *data)
{
    return parse(data);
}

bool JAUAudioArcLoader_withoutCopy::load(const char *path)
{
    const void *archive = mSection->loadDVDFile(path, true, EXPAND_SWITCH_DEFAULT);
    return archive && load(archive);
}

void JAUAudioArcLoader_withoutCopy::readWS(u32 bank_no, const void *data, u32 flag)
{
    mSection->newWaveBank(bank_no, data);
    if (flag)
    {
        mSection->loadWaveArc(bank_no, flag);
    }
    JASReport(".ws resource remains at Heap Head\n");
}

void JAUAudioArcLoader_withoutCopy::readBNK(u32 bankNum, const void *data)
{
    mSection->newBank(data, bankNum);
    JASReport(".bnk resource remains at Heap Head\n");
}

void JAUAudioArcLoader_withoutCopy::readBSC(const void *data, u32 size)
{
    mSection->newSeSeqCollection(data, 0);
}

void JAUAudioArcLoader_withoutCopy::readBST(const void *data, u32 size)
{
    mSection->newSoundTable(data, 0, false);
}

void JAUAudioArcLoader_withoutCopy::readBSTN(const void *data, u32 size)
{
    mSection->newSoundNameTable(data, 0, true);
}

void JAUAudioArcLoader_withoutCopy::readBMS(u32 soundID, const void *data, u32 size)
{
    mSection->newStaticSeqData(soundID, data, 0);
}

void JAUAudioArcLoader_withoutCopy::readBMS_fromArchive(u32 soundID)
{
    mSection->newStaticSeqData(soundID);
}

void JAUAudioArcLoader_withoutCopy::newDynamicSeqBlock(u32 size)
{
    JAUSectionHeap *sectionHeap = mSection->asSectionHeap();
    JUT_ASSERT(sectionHeap);
    sectionHeap->newDynamicSeqBlock(size);
}

void JAUAudioArcLoader_withoutCopy::readBSFT(const void *data)
{
    mSection->newStreamFileTable(data, true);
}

void JAUAudioArcLoader_withoutCopy::beginBNKList(u32 p1, u32 cnt)
{
    mSection->beginNewBankTable(p1, cnt);
}

void JAUAudioArcLoader_withoutCopy::endBNKList()
{
    mSection->endNewBankTable();
}

void JAUAudioArcLoader_withoutCopy::readMaxSeCategory(int categoryIndex, int activSE, int inactiveSE)
{
    JAISeMgr *seMgr = JASGlobalInstance<JAISeMgr>::getInstance();
    if (seMgr)
    {
        seMgr->getCategory(categoryIndex)->setMaxActiveSe(activSE);
        seMgr->getCategory(categoryIndex)->setMaxInactiveSe(inactiveSE);
    }
}