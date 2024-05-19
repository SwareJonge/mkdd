#ifndef JAUDIO_JAISTREAM_H
#define JAUDIO_JAISTREAM_H

#include "JSystem/JAudio/System/JASAramStream.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JSupport/JSUList.h"

class JAIStreamMgr;

class JAIStream : public JASPoolAllocObject<JAIStream>, public JAISound, public JSULink<JAIStream>
{
public:
    struct TInner
    {
        JASAramStream mAramStream;
    };
    static const int CHANNEL_MAX = 6;

    JAIStream(JAIStreamMgr *, JAISoundStrategyMgr<JAIStream> *);
    void JAIStreamMgr_startID_(JAISoundID, s32, const JGeometry::TVec3f *,
                               JAIAudience *, int);
    bool prepare_prepareStream_();
    void prepare_();
    void prepare_startStream_();
    void JAIStreamMgr_mixOut_(const JASSoundParams &, JAISoundActivity);
    void die_JAIStream_();
    bool JAISound_tryDie_();
    void JAIStreamMgr_calc_();
    s32 getNumChild() const;
    JAISoundChild *getChild(int);
    void releaseChild(int);
    JASTrack *getTrack();
    JASTrack *getChildTrack(int);
    JAIStream *asStream();
    JAITempoMgr *getTempoMgr();

    ~JAIStream() {}

    u32 JAIStreamMgr_getAramAddr_() { return mAramAddr; }

    TInner mInner;                                 // 0A8
    int _290;                                      // 290
    s32 _294;                                      // 294
    int _298;                                      // 298
    u32 mAramAddr;                                 // 29C
    JAISoundChild *mChilds[6];                     // 2A0
    JAIStreamMgr *mStreamMgr;                      // 2B8
    JAISoundStrategyMgr__unknown<JAIStream> *_2bc; // 2BC
    JAISoundStrategyMgr<JAIStream> *mStrategyMgr;  // 2C0
    bool _2c4;                                       // 2C4
    u8 _2c5;                                       // 2C5
    u8 _2c6;                                       // 2C6
};

#endif