#ifndef JAUDIO_JAUSEQDATABLOCKMGR_H
#define JAUDIO_JAUSEQDATABLOCKMGR_H

#include "JSystem/JAudio/Interface/JAISound.h"
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
    JAISeqDataRegion region_;       // 14
    int _1c;                        //
};

class JAUSeqDataBlocks : JSUList<JAUSeqDataBlock>
{
public:
    JAUSeqDataBlocks() {}
    ~JAUSeqDataBlocks(){}

    void getSeqData(JAISoundID);
    void seekFreeBlock(u32);
    void append(JSULink<JAUSeqDataBlock> *);
    void remove(JSULink<JAUSeqDataBlock> *);
    void hasFailedBlock(JAISoundID);
};

class JAUDynamicSeqDataBlocks
{
public:
    JAUDynamicSeqDataBlocks();
    ~JAUDynamicSeqDataBlocks() {}
    void setSeqDataArchive(JKRArchive *);
    s32 getSeqData(JAISoundID, JAISeqDataUser *, JAISeqData *, bool);
    bool appendDynamicSeqDataBlock(JAUSeqDataBlock *);
    bool removeDynamicSeqDataBlock(JAUSeqDataBlock *, JAISeqDataUser *);
    bool loadDynamicSeq(JAISoundID, bool, JAISeqDataUser *);
    void releaseIdleDynamicSeqDataBlock(JAISeqDataUser *);
    void releaseIdleDynamicSeqDataBlock_(JAISeqDataUser *, u32);
    void rearrangeLoadingSeqs_();

    JKRArchive *getSeqDataArchive() { return seqDataArchive_; }

    JAUSeqDataBlocks _0;
    JAUSeqDataBlocks _c;
    JAUSeqDataBlocks _18;
    JKRArchive *seqDataArchive_; // 24
};

#endif /* JAUDIO_JAUSEQDATABLOCKMGR_H */