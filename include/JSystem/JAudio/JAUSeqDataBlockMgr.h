#ifndef JAUDIO_JAUSEQDATABLOCKMGR_H
#define JAUDIO_JAUSEQDATABLOCKMGR_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"
#include "JSystem/JSupport/JSUList.h"

class JAISeqData;
class JAISeqDataUser;
class JKRArchive;

struct JAUSeqDataBlock
{
    JAUSeqDataBlock();
    ~JAUSeqDataBlock() {}

    JSULink<JAUSeqDataBlock> link_; // 0
    JAISoundID soundID_;            // 10
    JAISeqDataRegion region;       // 14
    int _1c;                        //
};

class JAUSeqDataBlocks : public JSUList<JAUSeqDataBlock>
{
public:
    JAUSeqDataBlocks() {}
    ~JAUSeqDataBlocks(){}

    u8 *getSeqData(JAISoundID);
    JSULink<JAUSeqDataBlock> *seekFreeBlock(u32);
    bool append(JSULink<JAUSeqDataBlock> *);
    bool remove(JSULink<JAUSeqDataBlock> *);
    bool hasFailedBlock(JAISoundID);
};

class JAUDynamicSeqDataBlocks
{
public:
    JAUDynamicSeqDataBlocks();
    ~JAUDynamicSeqDataBlocks() {}
    void setSeqDataArchive(JKRArchive *);
    JAISeqDataResult getSeqData(JAISoundID, JAISeqDataUser *, JAISeqData *, bool);
    bool appendDynamicSeqDataBlock(JAUSeqDataBlock *);
    bool removeDynamicSeqDataBlock(JAUSeqDataBlock *, JAISeqDataUser *);
    bool loadDynamicSeq(JAISoundID, bool, JAISeqDataUser *);
    u32 releaseIdleDynamicSeqDataBlock(JAISeqDataUser *);
    JAUSeqDataBlock *releaseIdleDynamicSeqDataBlock_(JAISeqDataUser *, u32);
    void rearrangeLoadingSeqs_();

    JKRArchive *getSeqDataArchive() { return seqDataArchive_; }

    JAUSeqDataBlocks freeBlocks_;
    JAUSeqDataBlocks _c;
    JAUSeqDataBlocks loadedBlocks_;
    JKRArchive *seqDataArchive_; // 24
};

#endif /* JAUDIO_JAUSEQDATABLOCKMGR_H */
