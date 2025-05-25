#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"
#define JAUSECTIONHEAP_FIX

#include "JSystem/JAudio/JAUSectionHeap.h"
#include "JSystem/JAudio/JAUSeqCollection.h"
#include "JSystem/JAudio/JAUSoundInfo.h"
#include "JSystem/JAudio/JAUStreamFileTable.h"
#include "JSystem/JAudio/System/JASBNKParser.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASResArcLoader.h"
#include "JSystem/JAudio/System/JASWaveBank.h"
#include "JSystem/JAudio/System/JASWSParser.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

// The scopes are real

namespace {
    class TPushCurrentHeap
    {
    public:
        TPushCurrentHeap(JKRHeap *heap) { heap_ = JKRSetCurrentHeap(heap); }
        ~TPushCurrentHeap() { JKRSetCurrentHeap(heap_);}
    private:
        JKRHeap *heap_;
    };

    class TStreamDataMgr : public JAIStreamDataMgr
    {
    public:
        TStreamDataMgr(const void *bsft)
        {
            JAUStreamFileTable fileTable;
            fileTable.init(bsft);
            if (!fileTable.isValid())
            {
                mFileCount = 0;
                mEntries = NULL;
                return;
            }
            mFileCount = fileTable.getNumFiles();
            mEntries = new s32[mFileCount];
            if (!mEntries)
            {
                mFileCount = 0;
                return;
            }
            for (u32 i = 0; i < mFileCount; i++)
            {
                mEntries[i] = DVDConvertPathToEntrynum(fileTable.getFilePath(i));
            }
        }

        virtual ~TStreamDataMgr() {}
        virtual s32 getStreamFileEntry(JAISoundID soundId)
        {
            u32 shortID = soundId.mId.mAdvancedId.mShortId;
            if (shortID >= mFileCount)
                return -1;
            return mEntries[shortID];
        }
        
        bool isValid() { return mFileCount; }

        u32 mFileCount;
        s32 *mEntries;
    };

    bool JKRSolidHeap_isEmpty(JKRSolidHeap *heap) {
        u32 sizeBeforeFree = heap->getFreeSize();
        heap->freeAll();
        return (heap->getFreeSize() - sizeBeforeFree) == 0;
    }
}

JAUSection::TSectionData::TSectionData() 
{
    resetRegisteredWaveBankTables();
    resetRegisteredBankTables();
    mBstDst = NULL;
    mBstnDst = NULL;
    sectionalSeqDataMgr = NULL;
}

void JAUSection::TSectionData::resetRegisteredBankTables()
{
    for(u32 i = 0; i < 255; i++)
        registeredBankTables.reset(i);
}

void JAUSection::TSectionData::resetRegisteredWaveBankTables()
{
    for (u32 i = 0; i < 255; i++)
        registeredWaveBankTables.reset(i);
}

JAUSection::JAUSection(JAUSectionHeap *sectionHeap, u32 p2, s32 p3) : JKRDisposer(), JSULink<JAUSection>(this), _28(p2), sectionHeap_(sectionHeap)
{
    buildingBankTable_ = NULL;
    mIsBuilding = true;
    data_._98 = p3;

    if (this != sectionHeap_)
    {
        data_.seqDataBlocks_.setSeqDataArchive(sectionHeap_->sectionHeapData_.dynamicSeqBlocks_.getSeqDataArchive());
    }
}

void JAUSection::finishBuild() {
#line 141
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());

    {
        TPushCurrentHeap pushHeap(getHeap_());
    }

    data_._98 -= getHeap_()->getFreeSize();
    getHeap_()->freeTail();
    mIsBuilding = false;
}

bool JAUSection::isDisposable_() const
{
    for (u32 i = 0; i < 255; i++)
    {
        if (data_.registeredWaveBankTables.test(i))
        {
            JASWaveBank *waveBank = sectionHeap_->getWaveBankTable().getWaveBank(i);
#line 158
            JUT_ASSERT(waveBank);
            if (waveBank->isLoading()) 
                return false;
        }
    }
    return true;
}

void JAUSection::disposeUserDisposers_() 
{
    JSULink<JAUDisposer_> *link = data_._8c.getLast();

    if (link)
    {
        JUT_REPORT_MSG("<Ignore these warnings />\n");
        do 
        {
            JSULink<JAUDisposer_> *prev = link->getPrev();
            delete link->getObject();
            data_._8c.remove(link);
            link = prev;
        } while (link);
        JUT_REPORT_MSG("</ Ignore these warnings>\n");
    }
}

void JAUSection::releaseAllBank_() 
{
    for (u32 i = 0; i < 255; i++) {
        if (data_.registeredBankTables.test(i))
        {
            JASCriticalSection cs;
            JASBank *bank = JASGlobalInstance<JASDefaultBankTable>::getInstance()->getBank(i);
#line 195
            JUT_ASSERT(bank);
            JASChannel::sendBankDisposeMsg(bank);
            JASGlobalInstance<JASDefaultBankTable>::getInstance()->registBank(i, NULL);
        }
    }

    for (u32 i = 0; i < 255; i++)
    {
        if (data_.registeredWaveBankTables.test(i))
        {
            JASWaveBank *waveBank = sectionHeap_->getWaveBankTable().getWaveBank(i);
            JUT_ASSERT(waveBank);
            JUT_ASSERT(! waveBank->isLoading());
            for (u32 j = 0; j < waveBank->getArcCount(); j++) {
                JASWaveArc *archive = waveBank->getWaveArc(j);
                archive->erase();
            }
            sectionHeap_->getWaveBankTable().registWaveBank(i, NULL);
            data_.registeredWaveBankTables.reset(i);
        }        
    }

    data_.resetRegisteredBankTables();
}

bool JAUSection::releaseSeqData_()
{
    bool ret = true;

    for (JSULink<JAUSeqDataBlock> *link = data_._28.getFirst(); link != data_._28.getEnd();)
    {
        JSULink<JAUSeqDataBlock> *next = link->getNext();
        if(data_.seqDataBlocks_.removeDynamicSeqDataBlock(link->getObject(), sectionHeap_->sectionHeapData_.seqDataUser)) {
            data_._28.remove(link);
        }
        else
        {
            ret = false;
        }
        link = next;
    }
    
    if (data_.sectionalSeqDataMgr)
    {
        data_.sectionalSeqDataMgr->releaseSeqData();
#line 250
        JUT_ASSERT(sectionHeap_->sectionHeapData_.seSeqDataMgr_ == data_.sectionalSeqDataMgr);
#line 256
        JUT_ASSERT(false);
        data_.sectionalSeqDataMgr = NULL;
        sectionHeap_->sectionHeapData_.seSeqDataMgr_ = NULL;
    }

    return ret;
}

void JAUSection::dispose()
{
#line 265
    JUT_ASSERT(! data_.registeredBankTables.any());
    JUT_ASSERT(! data_.registeredWaveBankTables.any());

    if (data_.mBstDst) {
        sectionHeap_->sectionHeapData_.soundTable->~JAUSoundTable();
        sectionHeap_->sectionHeapData_.soundTable = NULL;
    }

    if (data_.mBstnDst)
    {
        sectionHeap_->sectionHeapData_.soundNameTable->~JAUSoundNameTable();
        sectionHeap_->sectionHeapData_.soundNameTable = NULL;
    }
}

JAUSoundTable *JAUSection::newSoundTable(const void *bst, u32 n, bool p3)
{
#line 283
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    JUT_ASSERT(bst);
    JUT_ASSERT(asSectionHeap() == this);
    JUT_ASSERT(sectionHeap_->sectionHeapData_.soundTable == 0);
    {
        TPushCurrentHeap pushHeap(getHeap_());
        void *bstDst = (void *)bst;
        if(n != 0) 
        {
            bstDst = newCopy(bst, n, 4);
            JUT_ASSERT(bstDst);
        }
        JAUSoundTable *soundTable = new JAUSoundTable(p3);
        JUT_ASSERT(soundTable);
        soundTable->init(bstDst);
        sectionHeap_->sectionHeapData_.soundTable = soundTable;
        data_.mBstDst = bstDst;
    }
    return sectionHeap_->sectionHeapData_.soundTable;
}

JAUSoundNameTable *JAUSection::newSoundNameTable(const void *bstn, u32 n, bool p3)
{
#line 313
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    JUT_ASSERT(bstn);
    JUT_ASSERT(asSectionHeap() == this);
    JUT_ASSERT(sectionHeap_->sectionHeapData_.soundNameTable == 0);
    {
        TPushCurrentHeap pushHeap(getHeap_());
        void *bstnDst = (void *)bstn;
        if (n != 0)
        {
            bstnDst = newCopy(bstn, n, 4);
            JUT_ASSERT(bstnDst);
        }
        JAUSoundNameTable *soundNameTable = new JAUSoundNameTable(p3);
        JUT_ASSERT(soundNameTable);
        soundNameTable->init(bstnDst);
        sectionHeap_->sectionHeapData_.soundNameTable = soundNameTable;
        data_.mBstnDst = bstnDst;
    }
    return sectionHeap_->sectionHeapData_.soundNameTable;
}

JAIStreamDataMgr *JAUSection::newStreamFileTable(const void *bsft, bool p2)
{
#line 343
    JUT_ASSERT(asSectionHeap() == this);
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    JUT_ASSERT(sectionHeap_->sectionHeapData_.streamDataMgr_ == 0);
    {
        TPushCurrentHeap pushHeap(getHeap_());
        JAIStreamDataMgr *streamDataMgr = NULL;
        if (p2)
        {
            TStreamDataMgr *dataMgr = new TStreamDataMgr(bsft);
            if (dataMgr->isValid())
            {
                streamDataMgr = dataMgr;
            }
        }
        else
        {
            JAUStreamDataMgr_StreamFileTable *fileTable = new JAUStreamDataMgr_StreamFileTable();
            fileTable->init(bsft);
            if (fileTable->isValid())
            {
                streamDataMgr = fileTable;
            }
        }
        sectionHeap_->sectionHeapData_.streamDataMgr_ = streamDataMgr;
    }
    return sectionHeap_->sectionHeapData_.streamDataMgr_;
}

JAISeqDataMgr *JAUSection::newSeSeqCollection(const void *bsc, u32 n)
{ 
#line 396
    JUT_ASSERT(asSectionHeap() == this);
    JUT_ASSERT(sectionHeap_->sectionHeapData_.seSeqDataMgr_ == 0);
    if (n)
    {
        bsc = newCopy(bsc, n, 4);
    }

    JUT_ASSERT(bsc);
    {
        TPushCurrentHeap pushHeap(getHeap_());
        JAUSeqDataMgr_SeqCollection *seSeqDataMgr = new JAUSeqDataMgr_SeqCollection();
        JUT_ASSERT(seSeqDataMgr);
        seSeqDataMgr->init(bsc);
        sectionHeap_->sectionHeapData_.seSeqDataMgr_ = seSeqDataMgr;
        data_.sectionalSeqDataMgr = seSeqDataMgr;
    }
    return sectionHeap_->sectionHeapData_.seSeqDataMgr_;
}

u8* JAUSection::newStaticSeqDataBlock_(JAISoundID soundID, u32 size)
{
#line 417
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    JUT_ASSERT(size);
    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataBlock *seqDataBlock = new JAUSeqDataBlock();
        if (!seqDataBlock)
        {            
            return NULL;
        }
        JSULink<JAUSeqDataBlock> *link = new JSULink<JAUSeqDataBlock>(seqDataBlock);
        if (!link)
        {
            JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
            return NULL;
        }
        u8 *addr = new (0x20) u8[size];
        if (!addr)
        {
            JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
            return NULL;
        }
        seqDataBlock->region.addr = addr;
        seqDataBlock->region.size = size;
        seqDataBlock->soundID_ = soundID;

        JASCriticalSection cs;
        if (data_.seqDataBlocks_.appendDynamicSeqDataBlock(seqDataBlock))
        {
            data_._28.append(link);
            return addr;
        }
        JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
    }
    return NULL;
}

bool JAUSection::newStaticSeqData(JAISoundID soundID, const void *src, u32 size)
{
    {
        u8 *dst = newStaticSeqDataBlock_(soundID, size);
        if (dst)
        {
            memcpy(dst, src, size);
            return true;
        }
    }
    return false;
}

bool JAUSection::newStaticSeqData(JAISoundID soundID) {
    JKRArchive *seqArchive = data_.seqDataBlocks_.getSeqDataArchive();
#line 479
    JUT_ASSERT(seqArchive);
    JAUSoundInfo *soundInfo = JASGlobalInstance<JAUSoundInfo>::getInstance();
    if (!soundInfo)
    {
        JUT_WARNING_F2("%s", "cannot JAUSoundInfo::getInstance().");
        return false;
    }
    else {
        u16 resID = soundInfo->getBgmSeqResourceID(soundID);
        u32 resSize = JASResArcLoader::getResSize(seqArchive, resID);
        //JAISoundID newSoundID;
        //newSoundID.setAnonymous();
        u8 *block = newStaticSeqDataBlock_(0xffffffff, resSize);
        if (block)
        {
            data_.seqDataBlocks_.loadDynamicSeq(soundID, true, sectionHeap_->sectionHeapData_.seqDataUser);
            return true;
        }
    }
    return false;
}

// Cast is needed, sadly
const void *JAUSection::loadDVDFile(const char *path, bool defaultAlignment, JKRExpandSwitch expSwitch)
{
#line 500
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());

    void *ret;
    {
        TPushCurrentHeap pushHeap(getHeap_());
        ret = (u8*)JKRDvdToMainRam(path, NULL, expSwitch, 0, getHeap_(), defaultAlignment ? JKRDvdRipper::ALLOC_DIR_BOTTOM : JKRDvdRipper::ALLOC_DIR_TOP, 0, NULL, NULL);
    }
    return ret;
}


void* JAUSection::newCopy(const void *src, u32 n, s32 alignment) 
{
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());

    void *dst = new (getHeap_(), alignment) u8[n];
    if(dst) {
        memcpy(dst, src, n);
    }
    return dst;
}

JASWaveBank *JAUSection::newWaveBank(u32 bank_no, const void *ws)
{
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    {
        TPushCurrentHeap push(getHeap_());
        JASWaveBank *waveBank = JASWSParser::createWaveBank(ws, getHeap_());
        if (waveBank)
        {
            JUT_ASSERT(sectionHeap_->getWaveBankTable().getWaveBank( bank_no ) == 0);
            sectionHeap_->getWaveBankTable().registWaveBank(bank_no, waveBank);
            data_.registeredWaveBankTables.set(bank_no, true);
            return waveBank;
        }
    }

    return NULL;
}

bool JAUSection::loadWaveArc(u32 bank_no, u32 flag)
{
    if (data_.registeredWaveBankTables.test(bank_no))
    {
        JASWaveBank *waveBank = sectionHeap_->getWaveBankTable().getWaveBank(bank_no);
        if (waveBank)
        {
            for (u32 i = 0; i < waveBank->getArcCount(); i++)
            {
                if (flag & 1 << i)
                {
                    JASWaveArc *archive = waveBank->getWaveArc(i);
                    archive->load(NULL);
                }
            }
            return true;
        }
    }
    return false;
}

bool JAUSection::loadWaveArc(u32 bank_no)
{
    if (data_.registeredWaveBankTables.test(bank_no))
    {
        JASWaveBank *waveBank = sectionHeap_->getWaveBankTable().getWaveBank(bank_no);
        if (waveBank)
        {
            for (u32 i = 0; i < waveBank->getArcCount(); i++)
            {
                JASWaveArc *archive = waveBank->getWaveArc(i);
                archive->load(NULL);                
            }
            return true;
        }
    }
    return false;
}

JASBank *JAUSection::newBank(const void *pBnk, u32 index) {
#line 605
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    JASWaveBank *waveBank = sectionHeap_->getWaveBankTable().getWaveBank(index);
    JUT_ASSERT(waveBank != 0);
    {
        TPushCurrentHeap push(getHeap_());
        u32 bank_no = JASBNKParser::getBankNumber(pBnk);
        JASBank *bank = JASBNKParser::createBank(pBnk, getHeap_());
        if (bank)
        {
            if (buildingBankTable_)
            {
                JUT_ASSERT(buildingBankTable_->getBank( bank_no ) == 0);
                buildingBankTable_->registBank(bank_no, bank);
            }
            else
            {
                JUT_ASSERT(JASDefaultBankTable::getInstance() ->getBank( bank_no ) == 0);
                JASDefaultBankTable::getInstance()->registBank(bank_no, bank);
                data_.registeredBankTables.set(bank_no, true);
            }
            bank->assignWaveBank(waveBank);
            return bank;
        }
    }    
    return 0;
}

bool JAUSection::beginNewBankTable(u32 p1, u32 bankCount)
{
#line 641
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    JUT_ASSERT(buildingBankTable_ == 0);
    JAUBankTableLink *bankTableLink = NULL;
    {
        TPushCurrentHeap push(getHeap_());
        JASBank **bankTable = new JASBank *[bankCount];
        if (bankTable)
        {
            bankTableLink = new JAUBankTableLink(p1, bankTable, bankCount);
            if (bankTableLink)
            {
                buildingBankTable_ = bankTableLink;
            }
            else
            {
                JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
            }
        }
        else
        {
            JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
        }
    }
    return bankTableLink;
}

JAUBankTable *JAUSection::endNewBankTable() {
    JAUBankTable *ret = NULL;
    if (buildingBankTable_) {
        data_.bankTableDict.appendBankTable(buildingBankTable_);
        buildingBankTable_ = NULL;
        ret = buildingBankTable_;
    }
    return ret;
}

#ifdef DEBUG
CW_FORCE_STRINGS(JAUSection_cpp_1, "JAUSectionHeap.h");
#endif

JAUSectionHeap *JAUNewSectionHeap(JKRSolidHeap *heap, bool p2)
{
#line 736
    JUT_ASSERT(JKRSolidHeap_isEmpty( heap ));
    TPushCurrentHeap pushHeap(heap);
    u32 size = heap->getFreeSize();
    JAUSectionHeap *sectionHeap = new JAUSectionHeap(heap, p2, size);

    return sectionHeap;
}

JAUSectionHeap *JAUNewSectionHeap(bool p1)
{
    u32 freeSize = JASDram->getFreeSize();
    JKRSolidHeap *sectionHeap = JKRCreateSolidHeap(freeSize, JASDram, true);
    JUT_ASSERT(sectionHeap);
    return JAUNewSectionHeap(sectionHeap, p1);
}

JAUSectionHeap::JAUSectionHeap(JKRSolidHeap *heap, bool p2, s32 size) : JAUSection(this, 0, size), JASGlobalInstance(p2), heap_(heap)
{
    _dc = 0;
    sectionList_.append(this);
}

JAUSection *JAUSectionHeap::getOpenSection() { return sectionList_.getLast()->getObject(); }

JAUSection* JAUSectionHeap::pushNewSection() 
{
    JSULink<JAUSection> *lastLink = sectionList_.getLast();
    JAUSection *section = lastLink->getObject();

    if (section->mIsBuilding)
    {
        section->finishBuild();
    }

    u32 tag = getNumSections();
    heap_->recordState(tag);

    {
        TPushCurrentHeap push(heap_);
        u32 freeSize = heap_->getFreeSize();
        JAUSection *section = new JAUSection(this, tag, freeSize);
        if(section) {
            if (sectionList_.append(section))
            {
                return section;
            }
        }        
    }
    heap_->restoreState(tag);

    return NULL;
}

bool JAUSectionHeap::popSection() 
{
    if(getNumSections() <= 1)
        return false;

    JSULink<JAUSection> *lastLink = sectionList_.getLast();
    JAUSection *section = lastLink->getObject();

    section->disposeUserDisposers_();

    if (!section->releaseSeqData_())
        return false;

    if (!section->isDisposable_())
        return false;

    if (sectionList_.remove(lastLink))
    {
        section->releaseAllBank_();
        u32 tag = getNumSections();
#line 810
        JUT_ASSERT(tag >= 1);
        heap_->restoreState(tag);
        return true;
    }

    return false;
}

JAUSection *JAUSectionHeap::getSection(int no)
{
    JSULink<JAUSection> *link = sectionList_.getFirst();
    for (int i = 0; i < no; i++, link = link->getNext())
    {
        if (link->getNext() == NULL)
            return NULL;
    }
    return link->getObject();
}

int JAUSectionHeap::getNumSections() const { return sectionList_.getNumLinks(); }

bool JAUSectionHeap::setSeqDataUser(JAISeqDataUser *seqDataUser) {
    sectionHeapData_.seqDataUser = seqDataUser;
    return true;
}

bool JAUSectionHeap::newDynamicSeqBlock(u32 size) {
#line 864
    JUT_ASSERT(isOpen());
    JUT_ASSERT(isBuilding());
    JUT_ASSERT(sectionHeap_ == this);
    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataBlock *seqDataBlock = new JAUSeqDataBlock();
        if (!seqDataBlock)
        {
            return false;
        }
        JSULink<JAUSeqDataBlock> *link = new JSULink<JAUSeqDataBlock>(seqDataBlock);
        if (!link)
        {
            JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
            return false;
        }
        u8 *block = new (0x20) u8[size];
        if (!block)
        {
            JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
            return false;
        }
        seqDataBlock->region.addr = block;
        seqDataBlock->region.size = size;
        seqDataBlock->soundID_.setAnonymous();

        JASCriticalSection cs;
        if (sectionHeap_->sectionHeapData_.dynamicSeqBlocks_.appendDynamicSeqDataBlock(seqDataBlock))
        {
            return true;
        }
        JUT_WARNING_F2("%s", "created UNUSED object in Heap\n");
    }
    
    return false;
}

JAISeqDataResult JAUSectionHeap::getSeqData(JAISoundID soundID, JAISeqData *seqData)
{
    for (JSULink<JAUSection> *link = sectionList_.getFirst(); link; link = link->getNext())
    {
        JAISeqDataResult result = link->getObject()->data_.seqDataBlocks_.getSeqData(soundID, sectionHeapData_.seqDataUser, seqData, false);
#line 921
        JUT_ASSERT(result != JAI_ASYNC_RESULT_RETRY);
        if (result == JAI_ASYNC_RESULT_OK)
        {
            return JAI_ASYNC_RESULT_OK;
        }
    }
    return sectionHeapData_.dynamicSeqBlocks_.getSeqData(soundID, sectionHeapData_.seqDataUser, seqData, true);
}

JAISeqDataResult JAUSectionHeap::releaseSeqData()
{
    JUT_WARNING_F2("%s", "JAUSectionHeap cannot release SeqData all at once. Please pop Section.\n")
    return JAI_ASYNC_RESULT_0;
}
