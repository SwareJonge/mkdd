#ifndef JAUDIO_JAISEQMGR_H
#define JAUDIO_JAISEQMGR_H

#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JSupport/JSUList.h"

class JAIAudience;
class JAISeq;

class JAISeqMgr : public JASGlobalInstance<JAISeqMgr>, public JAISeqDataUser
{
public:
    enum ReleaseSeqResult
    {
        RELEASE_SEQ_1 = 1,
        RELEASE_SEQ_2 = 2,
    };
    JAISeqMgr(bool);
    void freeDeadSeq_();
    bool startSound(JAISoundID soundID, JAISoundHandle *handle, const JGeometry::TVec3<f32> *pos);
    void calc();
    void stop();
    void stop(u32 fadeCount);
    void stopSoundID(JAISoundID soundID);
    void mixOut();
    JAISeq *beginStartSeq_();
    bool endStartSeq_(JAISeq *seq, JAISoundHandle *handle);

    virtual ~JAISeqMgr() {}
    virtual bool isUsingSeqData(const JAISeqDataRegion &region);
    virtual int releaseSeqData(const JAISeqDataRegion &region);

    void setAudience(JAIAudience *audience) { mAudience = audience; }
    JAIAudience *getAudience() { return mAudience; }
    JAISeqDataMgr *getSeqDataMgr() { return seqDataMgr_; }
    void setSeqDataMgr(JAISeqDataMgr *seqDataMgr)
    {
#line 124
        JUT_ASSERT(!isActive());
        resetSeqDataMgr();
        seqDataMgr_ = seqDataMgr;
        seqDataMgr_->setSeqDataUser(this);
    }
    void resetSeqDataMgr()
    {
#line 131
        JUT_ASSERT(!isActive());
        if (seqDataMgr_)
        {
            seqDataMgr_->setSeqDataUser(NULL);
            seqDataMgr_ = NULL;
        }
    }
    JAISoundParamsMove *getParams() { return &mMove; }
    bool isActive() { return mSeqList.getNumLinks() != 0; }
    int getNumActiveSeqs() { return mSeqList.getNumLinks(); }
    void pause(bool doPause) { mActivity._0.flags.flag2 = doPause; }

private:
    JAISoundActivity mActivity;                // 04
    JAIAudience *mAudience;                    // 08
    JAISeqDataMgr *seqDataMgr_;                // 0C
    JAISoundStrategyMgr<JAISeq> *strategyMgr_; // 10
    JAISoundParamsMove mMove;                  // 14
    JSUList<JAISeq> mSeqList;                  // 64
    int _70;                                   // 70

}; // Size: 0x74

#endif