#ifndef JAUDIO_JAISTREAMMGR_H
#define JAUDIO_JAISTREAMMGR_H

#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JAudio/Interface/JAIStream.h"
#include "JSystem/JUtility/JUTAssert.h"

class JAIStreamAramMgr;
class JAIStreamDataMgr;

class JAIStreamMgr : public JASGlobalInstance<JAIStreamMgr>
{
public:
    JAIStreamMgr(bool);
    ~JAIStreamMgr();
    void startStreamDirect(const char *fileName, JAISoundHandle *handle, int category, const JGeometry::TVec3<float> *pos);
    bool startSound(JAISoundID, JAISoundHandle *, JGeometry::TVec3<f32> const *);
    void freeDeadStream_();
    void calc();
    void stop();
    void stop(u32);
    void stopSoundID(JAISoundID);
    void mixOut();
    JAIStream *newStream_();
    bool isActive() const;

    JAIAudience *getAudience() { return mAudience; }
    JAISoundParamsMove *getParams() { return &mParams; }
    JAIStreamAramMgr *getStreamAramMgr() { return streamAramMgr_; }
    void setStreamDataMgr(JAIStreamDataMgr *param_0)
    {
#line 138
        JUT_ASSERT(!isActive());
        streamDataMgr_ = param_0;
    }
    void setStreamAramMgr(JAIStreamAramMgr *param_0)
    {
#line 157
        JUT_ASSERT(!isActive());
        streamAramMgr_ = param_0;
    }
    void pause(bool i_pause) { mActivity._0.flags.flag2 = i_pause; }

private:
    JAISoundActivity mActivity;                   // 00
    JAISoundParamsMove mParams;                   // 04
    JAIAudience *mAudience;                       // 54
    JSUList<JAIStream> mStreamList;               // 58
    JAIStreamDataMgr *streamDataMgr_;             // 64
    JAIStreamAramMgr *streamAramMgr_;             // 68
    JAISoundStrategyMgr<JAIStream> *strategyMgr_; // 6C

}; // Size: 0x70

#endif