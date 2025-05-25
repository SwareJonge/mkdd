#ifndef JAUSEQCOLLECTION_H
#define JAUSEQCOLLECTION_H

#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"

class JAISeqData;
class JAISeqDataRegion;

struct JAUSeqCollectionData
{
    s8 groupId;
    s8 _1;
    u16 _2;
    u32 size;
    u32 _8;
};

class JAUSeqCollection
{
public:
    JAUSeqCollection();
    void init(void const *);
    bool getSeqData(int, int, JAISeqData *);
    bool getSeqDataRegion(JAISeqDataRegion *);

    bool isValid() { return collection_ != NULL; }

    u16 groupId_;
    const u32 *_4;
    const JAUSeqCollectionData *collection_;
    int size_;
};


class JAUSeqDataMgr_SeqCollection : public JAISeqDataMgr, public JAUSeqCollection
{
public:
    JAUSeqDataMgr_SeqCollection();
    ~JAUSeqDataMgr_SeqCollection() {}
    bool setSeqDataUser(JAISeqDataUser *);
    JAISeqDataResult releaseSeqData();
    JAISeqDataResult getSeqData(JAISoundID, JAISeqData *);

    void init(const void *bsc) { JAUSeqCollection::init(bsc); }

    JAISeqDataUser *user_; // 14
};

#endif
