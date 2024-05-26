#ifndef JAUDIO_JAUSECTIONHEAP_H
#define JAUDIO_JAUSECTIONHEAP_H

#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JAudio/JAUBankTable.h"
#include "JSystem/JAudio/JAUSeqDataBlockMgr.h"
#include "JSystem/JAudio/JAUSoundTable.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "std/bitset.h"

class JASVoiceBank;
class JAUDisposer_ {
    virtual ~JAUDisposer_();
};
class JAUSectionHeap;
class JAIStreamDataMgr;
class JAISeMgr;

class JAUSection : public JKRDisposer, public JSULink<JAUSection>
{
public:
    struct TSectionData
    {
        TSectionData();
        void resetRegisteredBankTables();
        void resetRegisteredWaveBankTables();

        JAUDynamicSeqDataBlocks seqDataBlocks_;    //
        JSUList<JAUSeqDataBlock> _28;              // 
        std::bitset<255> registeredWaveBankTables; // 34
        std::bitset<255> registeredBankTables;     // 54
        JAUBankTableDictionary bankTableDict;      // 74
        JAISeqDataMgr *sectionalSeqDataMgr;        // 80
        const void *mBstDst;                       // 84
        const void *mBstnDst;                      // 88
        JSUList<JAUDisposer_> _8c;                 //
        s32 _98;                                   //
    };

    JAUSection(JAUSectionHeap *, u32, s32);
    virtual ~JAUSection();
    virtual void dispose();

    void finishBuild();
    void releaseAllBank_();
    JAUSoundTable *newSoundTable(const void *, u32, bool);
    JAUSoundNameTable *newSoundNameTable(const void *, u32, bool);
    JAUSoundNameTable *newSoundNameTable(const char *path, bool p2);
    JAIStreamDataMgr *newStreamFileTable(const void *, bool);
    JAISeqDataMgr *newSeSeqCollection(const void *, u32);
    u8 *newStaticSeqDataBlock_(JAISoundID, u32);
    bool newStaticSeqData(JAISoundID, const void *, u32);
    bool newStaticSeqData(JAISoundID);
    const void *loadDVDFile(const char *path, bool, JKRExpandSwitch expSwitch);
    void *newCopy(const void *data, u32 size, s32 alignment);
    JASWaveBank *newWaveBank(u32, const void *);
    bool loadWaveArc(u32, u32);
    bool loadWaveArc(u32);
    JASBank *newBank(const void *, u32);
    JASVoiceBank *newVoiceBank(u32, u32);
    bool beginNewBankTable(u32, u32);
    JAUBankTable *endNewBankTable();
    void storeSeCategoryArrangement(JAISeMgr *);
    bool releaseSeqData_();
    bool isDisposable_() const;
    void disposeUserDisposers_();

    bool isBuilding() { return mIsBuilding; }
    bool isOpen() const;
    JAUSectionHeap *asSectionHeap() { return (JAUSection *)sectionHeap_ == this ? sectionHeap_ : NULL; }
    JKRSolidHeap *getHeap_();

    u32 _28;                              // 28
    bool mIsBuilding;                     // 2C
    JAUSectionHeap *sectionHeap_;         // 30
    JAUBankTableLink *buildingBankTable_; // 34
    TSectionData data_;                   // 38
};

class JAUSectionHeap : public JAUSection, JASGlobalInstance<JAUSectionHeap>, JAISeqDataMgr
{
public:
    struct TSectionHeapData
    {
        TSectionHeapData()
        {
            seSeqDataMgr_ = NULL;
            streamDataMgr_ = NULL;
            soundTable = NULL;
            soundNameTable = NULL;
            seqDataUser = NULL;
        }

        JAUWaveBankTable waveBankTable;            // 000
        JAISeqDataUser *seqDataUser;               // 404 page error
        JAUDynamicSeqDataBlocks dynamicSeqBlocks_; // 408
        JAISeqDataMgr *seSeqDataMgr_;              // 430
        JAIStreamDataMgr *streamDataMgr_;          // 434
        JAUSoundTable *soundTable;                 // 438
        JAUSoundNameTable *soundNameTable;         // 43C
    };

    void setSeqDataArchive(JKRArchive *);
    bool loadDynamicSeq(JAISoundID, bool);
    void releaseIdleDynamicSeqDataBlock();
    JAUSectionHeap(JKRSolidHeap *, bool, s32);
    JAUSection *getOpenSection();
    JAUSection *pushNewSection();
    bool popSection();
    JAUSection *getSection(int);
    int getNumSections() const;
    bool setSeqDataUser(JAISeqDataUser *);
    bool newDynamicSeqBlock(u32);
    s32 getSeqData(JAISoundID, JAISeqData *);
    int releaseSeqData();
    ~JAUSectionHeap() {}

    JAUWaveBankTable &getWaveBankTable() { return sectionHeapData_.waveBankTable; }

    JKRSolidHeap *heap_;               // D8
    int _dc;                           // DC
    JSUList<JAUSection> sectionList_;  // E0
    TSectionHeapData sectionHeapData_; // EC
};

inline JKRSolidHeap *JAUSection::getHeap_() { return sectionHeap_->heap_; }
inline bool JAUSection::isOpen() const { return sectionHeap_->getOpenSection() == this; }

JAUSectionHeap *JAUNewSectionHeap(bool);

// TODO: probably move this to a different file(JASFakeMatch?)
#define createJASInstance(TYPE) \
    DECL_WEAK TYPE *JASGlobalInstance<TYPE>::sInstance

createJASInstance(JAUSectionHeap);
createJASInstance(JAUSoundNameTable);
createJASInstance(JAUSoundTable);

#endif