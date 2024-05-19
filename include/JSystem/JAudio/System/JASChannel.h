#ifndef JAUDIO_JASCHANNEL_H
#define JAUDIO_JASCHANNEL_H

#include "JSystem/JAudio/System/JASMemPool.h"
#include "JSystem/JAudio/System/JASLfo.h"
#include "JSystem/JAudio/System/JASOscillator.h"
#include "JSystem/JAudio/System/JASSoundParams.h"
#include "JSystem/JAudio/System/JASWaveInfo.h"
#include "dolphin/os/OSMessage.h"

class JASDSPChannel;

namespace JASDsp
{
    struct TChannel;
}

class JASChannelParams
{
public:
    JASChannelParams()
    {
        mVolume = 1.0f;
        mPitch = 1.0f;
        _8 = 0.0f;
        mPan = 0.5f;
        mFxMix = 0.0f;
        mDolby = 0.0f;
    }
    void init()
    {
        mVolume = 1.0f;
        mPitch = 1.0f;
        _8 = 0.0f;
        mPan = 0.5f;
        mFxMix = 0.0f;
        mDolby = 0.0f;
    }

    f32 mVolume; // 00
    f32 mPitch;  // 04
    f32 _8;      // 08
    f32 mPan;    // 0c
    f32 mFxMix;  // 10
    f32 mDolby;  // 14
};

class JASChannel : public JASPoolAllocObject_MultiThreaded<JASChannel>
{
public:
    typedef void (*Callback)(u32, JASChannel *, JASDsp::TChannel *, void *);

    enum CallbackType
    {
        CB_PLAY,  // 0
        CB_START, // 1
        CB_STOP,  // 2
        CB_TIMER, // 3
    };

    enum Status
    {
        STATUS_INACTIVE, // 0
        STATUS_ACTIVE,   // 1
        STATUS_RELEASE,  // 2
    };

    struct PanVector
    {
        f32 mSound;   // 00
        f32 mEffect;  // 04
        f32 mChannel; // 08
    };

    union MixConfig
    {
        u16 whole;
        struct
        {
            u8 upper;
            u8 lower0 : 4;
            u8 lower1 : 4;
        } parts;
    };

    JASChannel(Callback, void *);
    ~JASChannel();
    int play();
    int playForce();
    void release(u16);
    void setOscInit(u32, JASOscillator::Data const *);
    void setMixConfig(u32, u16);
    static f32 calcEffect(JASChannel::PanVector const *);
    static f32 calcPan(JASChannel::PanVector const *);
    void effectOsc(u32, JASOscillator::EffectParams *);
    void setKeySweepTarget(s32, u32);
    void updateEffectorParam(JASDsp::TChannel *, u16 *,
                             JASOscillator::EffectParams const &);
    static s32 dspUpdateCallback(u32, JASDsp::TChannel *, void *);
    s32 initialUpdateDSPChannel(JASDsp::TChannel *);
    s32 updateDSPChannel(JASDsp::TChannel *);
    void updateAutoMixer(JASDsp::TChannel *, f32, f32, f32, f32);
    void updateMixer(f32, f32, f32, f32, u16 *);
    void free();
    static void initBankDisposeMsgQueue();
    static void receiveBankDisposeMsg();
    bool checkBankDispose() const;

    void setPauseFlag(bool param_0) { mPauseFlag = param_0; }
    void setUpdateTimer(u32 param_0) { mUpdateTimer = param_0; }
    void setBankDisposeID(const void *param_0) { mBankDisposeID = param_0; }
    void setDirectRelease(u16 param_0) { mOscillators[0].setDirectRelease(param_0); }
    void setVibrate(f32 param_0, f32 param_1)
    {
        mVibrate.setDepth(param_0);
        mVibrate.setPitch(param_1);
    }
    void setVibrateDelay(u16 param_0) { mVibrate.setDelay(param_0); }
    void setTremolo(f32 param_0, f32 param_1)
    {
        mTremolo.setDepth(param_0);
        mTremolo.setPitch(param_1);
    }
    void setTremoloDelay(u16 param_0) { mTremolo.setDelay(param_0); }
    void setPriority(u16 param_0) { mPriority = param_0; }
    void setParams(const JASChannelParams &param_0) { mParams = param_0; }
    void setInitVolume(f32 param_0) { mSoundParams.mVolume = param_0; }
    void setInitFxmix(f32 param_0) { mSoundParams.mFxMix = param_0; }
    void setInitPitch(f32 param_0) { mSoundParams.mPitch = param_0; }
    void setInitPan(f32 param_0) { mSoundParams.mPan = param_0; }
    void setInitDolby(f32 param_0) { mSoundParams.mDolby = param_0; }
    void setKey(s32 param_0) { mKey = param_0; }
    void setVelocity(u32 param_0) { mVelocity = param_0; }
    void setSkipSamples(u32 param_0) { mSkipSamples = param_0; }
    bool isDolbyMode() { return mMixConfig[0].whole == 0xffff; }

    int mStatus;                   // 000
    bool mPauseFlag;               // 004
    JASDSPChannel *mDspCh;         // 008
    Callback mCallback;            // 00c
    void *mCallbackData;           // 010
    u32 mUpdateTimer;              // 014
    const void *mBankDisposeID;    // 018
    JASOscillator mOscillators[2]; // 01c
    JASLfo mVibrate;               // 05c
    JASLfo mTremolo;               // 074
    MixConfig mMixConfig[6];       // 08c
    u16 mPriority;                 // 098
    JASChannelParams mParams;      // 09c
    JASSoundParams mSoundParams;   // 0b4
    s16 mKey;                      // 0c8
    u16 mVelocity;                 // 0ca
    f32 mKeySweep;                 // 0cc
    f32 mKeySweepTarget;           // 0d0
    u32 mKeySweepCount;            // 0d4
    u32 mSkipSamples;              // 0d8
    struct
    {
        u32 _0;
        JASWaveInfo _4;
    } _dc;    // 0dc
    int _104; // 104

    static OSMessageQueue sBankDisposeMsgQ;
    static OSMessage sBankDisposeMsg[16];
    static OSMessage sBankDisposeList[16];
    static int sBankDisposeListSize;
};

#endif