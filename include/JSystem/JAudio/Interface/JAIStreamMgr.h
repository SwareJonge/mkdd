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
    ~JAIStreamMgr() {}
    void startStreamDirect(const char *fileName, JAISoundHandle *handle, int category, const JGeometry::TVec3f *pos);
    bool startSound(JAISoundID soundID, JAISoundHandle *handle, const JGeometry::TVec3<f32> *pos);
    void freeDeadStream_();
    void calc();
    void stop();
    void stop(u32 fadeCount);
    void stopSoundID(JAISoundID soundID);
    void mixOut();
    JAIStream *newStream_();
    bool isActive() const;

    JAIAudience *getAudience() { return mAudience; }
    JAISoundParamsMove *getParams() { return &mParams; }
    JAIStreamAramMgr *getStreamAramMgr() { return streamAramMgr_; }
    void setStreamDataMgr(JAIStreamDataMgr *dataMgr)
    {
#line 138
        JUT_ASSERT(!isActive());
        streamDataMgr_ = dataMgr;
    }
    void setStreamAramMgr(JAIStreamAramMgr *aramMgr)
    {
#line 157
        JUT_ASSERT(!isActive());
        streamAramMgr_ = aramMgr;
    }
    void pause(bool doPause) { mActivity._0.flags.flag2 = doPause; }

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
