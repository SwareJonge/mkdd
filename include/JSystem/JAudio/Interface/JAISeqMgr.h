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
    bool startSound(JAISoundID, JAISoundHandle *, JGeometry::TVec3<f32> const *);
    void calc();
    void stop();
    void stop(u32);
    void stopSoundID(JAISoundID);
    void mixOut();
    JAISeq *beginStartSeq_();
    bool endStartSeq_(JAISeq *, JAISoundHandle *);

    virtual ~JAISeqMgr() {}
    virtual bool isUsingSeqData(JAISeqDataRegion const &);
    virtual int releaseSeqData(JAISeqDataRegion const &);

    void setAudience(JAIAudience *param_0) { mAudience = param_0; }
    JAIAudience *getAudience() { return mAudience; }
    JAISeqDataMgr *getSeqDataMgr() { return seqDataMgr_; }
    void setSeqDataMgr(JAISeqDataMgr *param_0)
    {
#line 124
        JUT_ASSERT(!isActive());
        resetSeqDataMgr();
        seqDataMgr_ = param_0;
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
    void pause(bool i_pause) { mActivity._0.flags.flag2 = i_pause; }

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