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

    JSULink<JAUSeqDataBlock> _0;
    JAISoundID _10;
    u8 *_14;
    u32 _18;
    int _1c;
};

class JAUSeqDataBlocks
{
public:
    void getSeqData(JAISoundID);
    void seekFreeBlock(u32);
    void append(JSULink<JAUSeqDataBlock> *);
    void remove(JSULink<JAUSeqDataBlock> *);
    void hasFailedBlock(JAISoundID);

    JSUList<JAUSeqDataBlock> _0;
};

class JAUDynamicSeqDataBlocks
{
public:
    JAUDynamicSeqDataBlocks();
    void setSeqDataArchive(JKRArchive *);
    s32 getSeqData(JAISoundID, JAISeqDataUser *, JAISeqData *, bool);
    bool appendDynamicSeqDataBlock(JAUSeqDataBlock *);
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