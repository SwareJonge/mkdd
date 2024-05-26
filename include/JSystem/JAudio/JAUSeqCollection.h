#ifndef JAUSEQCOLLECTION_H
#define JAUSEQCOLLECTION_H

#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"

class JAISeqData;
class JAISeqDataRegion;

struct JAUSeqCollectionData
{
    s8 _0;
    s8 _1;
    u16 _2;
    u32 _4;
    u32 _8;
};

class JAUSeqCollection
{
public:
    JAUSeqCollection();
    void init(void const *);
    bool getSeqData(int, int, JAISeqData *);
    bool getSeqDataRegion(JAISeqDataRegion *);

    bool isValid() { return _8; }

    u16 _0;
    const u32 *_4;
    const JAUSeqCollectionData *_8;
    int _c;
};


class JAUSeqDataMgr_SeqCollection : public JAISeqDataMgr, public JAUSeqCollection
{
public:
    JAUSeqDataMgr_SeqCollection();
    bool setSeqDataUser(JAISeqDataUser *);
    int releaseSeqData();
    s32 getSeqData(JAISoundID, JAISeqData *);
    ~JAUSeqDataMgr_SeqCollection();

    void init(const void *bsc) { JAUSeqCollection::init(bsc); }

    JAISeqDataUser *user_; // 14
};

#endif