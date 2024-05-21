#ifndef JAUDIO_JAISEQ_H
#define JAUDIO_JAISEQ_H

#include "JSystem/JAudio/Interface/JAISeqDataMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASTrack.h"

#include "JSystem/JSupport/JSUList.h"

class JAISeqMgr;
class JAISoundChild;

class JAISeq : public JASPoolAllocObject<JAISeq>, public JAISound, public JSULink<JAISeq>
{
public:
    static const int NUM_CHILDREN = 32;

    class TInner
    {
    public:
        TInner() : mSeqData(NULL, 0) {}

        JASTrack outputTrack;                     // 000
        JAISoundChild *mSoundChild[NUM_CHILDREN]; // 248
        JAITempoMgr mTempoMgr;                    // 2C8
        JASSoundParams mSoundParams;              // 2D8
        JAISeqData mSeqData;                      // 2EC
        int mCategory;                            // 2F4
        JAISeqMgr *seqMgr;                        // 2F8
        JAISoundStrategyMgr<JAISeq> *strategyMgr; // 2FC
    };

    virtual s32 getNumChild() const;
    virtual JAISoundChild *getChild(int);
    virtual void releaseChild(int);
    virtual JAISeq *asSeq();
    virtual JASTrack *getTrack();
    virtual JASTrack *getChildTrack(int);
    virtual JAITempoMgr *getTempoMgr();
    virtual bool JAISound_tryDie_();

    JAISeq(JAISeqMgr *, JAISoundStrategyMgr<JAISeq> *);
    void JAISeqMgr_startID_(JAISoundID, const JGeometry::TVec3f *, JAIAudience *, int, int);
    void playSeqData_(const JASSoundParams &, JAISoundActivity);
    void reserveChildTracks_(int);
    void releaseChildTracks_();
    bool prepare_getSeqData_();
    bool prepare_(const JASSoundParams &, JAISoundActivity);
    void JAISeqMgr_calc_();
    void die_();
    void mixOut_(const JASSoundParams &, JAISoundActivity);
    void JAISeqMgr_mixOut_(const JASSoundParams &, JAISoundActivity);

    JAISeqData &getSeqData() { return inner_.mSeqData; }

    TInner inner_;                              // 0a8
    JAISoundStrategyMgr__unknown<JAISeq> *_3a8; // 3a8
};

#endif