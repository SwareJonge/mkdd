#ifndef JAUDIO_JAISE_H
#define JAUDIO_JAISE_H

#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASTrack.h"

#include "JSystem/JSupport/JSUList.h"

class JAISeMgr;

class JAISe : JASPoolAllocObject<JAISe>, public JSULink<JAISe>, public JAISound
{
public:
    class TInner
    {
    public:
        TInner() : mSeqData(NULL, 0) {}

        u32 mProperPriority;                           // 000
        u32 mPriority;                                 // 004
        JASSoundParams mSoundParams;                   // 008
        JASTrack track;                                // 01C
        JAISeqData mSeqData;                           // 264
        u8 _26c;                                       // 26c
        JAISeMgr *seMgr;                               // 270
        JAISoundStrategyMgr<JAISe> *mSoundStrategyMgr; // 274
        JAISoundStrategyMgr__unknown<JAISe> *_278;     // 278
        JAITempoMgr mTempoMgr;                         // 27C
    };

    virtual s32 getNumChild() const;
    virtual JAISoundChild *getChild(int);
    virtual void releaseChild(int);
    virtual JASTrack *getTrack();
    virtual JASTrack *getChildTrack(int);
    virtual JAISe *asSe();
    virtual JAITempoMgr *getTempoMgr();
    virtual bool JAISound_tryDie_();

    JAISe(JAISeMgr *, JAISoundStrategyMgr<JAISe> *, u32);
    void mixOut_(const JASSoundParams &);
    void stopTrack_();
    void startTrack_(const JASSoundParams &);
    void JAISeCategoryMgr_mixOut_(bool, const JASSoundParams &, JAISoundActivity);
    void JAISeCategoryMgr_calc_();
    void JAISeMgr_startID_(JAISoundID, const JGeometry::TVec3f *, JAIAudience *);
    bool prepare_getSeqData_();
    void prepare_();

    JAISeqData *getSeqData() { return &inner_.mSeqData; }
    u32 JAISeCategoryMgr_getProperPriority_() const { return inner_.mProperPriority; }
    u32 JAISeCategoryMgr_getPriority_() const { return inner_.mPriority; }
    bool isFarAway() const { return inner_.mPriority == -1; }
    //bool isFarAway() const { return priority_ == -1; }
    
    TInner inner_; // AC
    
};

#endif /* JAUDIO_JAISE_H */