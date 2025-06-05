#ifndef JAUDIO_JASCHANNEL_H
#define JAUDIO_JASCHANNEL_H

#include "JSystem/JAudio/System/JASDsp.h"
#include "JSystem/JAudio/System/JASLfo.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASOscillator.h"
#include "JSystem/JAudio/System/JASSoundParams.h"
#include "JSystem/JAudio/System/JASWaveInfo.h"
#include "dolphin/os/OSMessage.h"

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
    typedef void (*Callback)(u32, JASChannel *channel, JASDsp::TChannel *dspChannel, void *);

    static const u16 BUSOUT_CPUCH = 6;
    static const u32 OSC_NUM = 2;
    static const int BANK_NUM = 16;

    enum CallbackType
    {
        CB_PLAY,  // 0
        CB_START, // 1
        CB_STOP,  // 2
        CB_TIMER, // 3
    };

    enum Status
    {
        STATUS_STOP,    // 0
        STATUS_PLAYING, // 1
        STATUS_PLAY = 1,
        STATUS_RELEASE, // 2
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
    void setOscInit(u32, const JASOscillator::Data *);
    void setMixConfig(u32, u16);
    static f32 calcEffect(const JASChannel::PanVector *);
    static f32 calcPan(const JASChannel::PanVector *);
    void effectOsc(u32, JASOscillator::EffectParams *);
    void setKeySweepTarget(s32, u32);
    void updateEffectorParam(JASDsp::TChannel *, u16 *,
                             const JASOscillator::EffectParams &);
    static s32 dspUpdateCallback(u32, JASDsp::TChannel *, void *);
    s32 initialUpdateDSPChannel(JASDsp::TChannel *);
    s32 updateDSPChannel(JASDsp::TChannel *);
    void updateAutoMixer(JASDsp::TChannel *, f32, f32, f32, f32);
    void updateMixer(f32, f32, f32, f32, u16 *);
    void free();
    static void initBankDisposeMsgQueue();
    static void receiveBankDisposeMsg();
    static void sendBankDisposeMsg(void*);
    bool checkBankDispose() const;

    void setPauseFlag(bool pauseFlag) { mPauseFlag = pauseFlag; }
    void setUpdateTimer(u32 updateTimer) { mUpdateTimer = updateTimer; }
    void setBankDisposeID(const void *disposeID) { mBankDisposeID = disposeID; }
    void setDirectRelease(u16 release) { mOscillators[0].setDirectRelease(release); }
    void setVibrate(f32 depth, f32 pitch)
    {
        mVibrate.setDepth(depth);
        mVibrate.setPitch(pitch);
    }
    void setVibrateDelay(u16 delay) { mVibrate.setDelay(delay); }
    void setTremolo(f32 depth, f32 pitch)
    {
        mTremolo.setDepth(depth);
        mTremolo.setPitch(pitch);
    }
    void setTremoloDelay(u16 delay) { mTremolo.setDelay(delay); }
    void setPriority(u16 prio) { mPriority = prio; }
    void setParams(const JASChannelParams &params) { mParams = params; }
    void setInitVolume(f32 volume) { mSoundParams.mVolume = volume; }
    void setInitFxmix(f32 fxMix) { mSoundParams.mFxMix = fxMix; }
    void setInitPitch(f32 pitch) { mSoundParams.mPitch = pitch; }
    void setInitPan(f32 pan) { mSoundParams.mPan = pan; }
    void setInitDolby(f32 dolby) { mSoundParams.mDolby = dolby; }
    void setKey(s32 key) { mKey = key; }
    void setVelocity(u32 vel) { mVelocity = vel; }
    void setSkipSamples(u32 skips) { mSkipSamples = skips; }
    bool isDolbyMode() { return mMixConfig[0].whole == 0xffff; }

    // Inline/Unused
    void copyOsc(u32, JASOscillator::Data *);

    int mStatus;                         // 000
    bool mPauseFlag;                     // 004
    JASDSPChannel *mDspCh;               // 008
    Callback mCb;                        // 00c
    void *mCbData;                       // 010
    u32 mUpdateTimer;                    // 014
    const void *mBankDisposeID;          // 018
    JASOscillator mOscillators[OSC_NUM]; // 01c
    JASLfo mVibrate;                     // 05c
    JASLfo mTremolo;                     // 074
    MixConfig mMixConfig[BUSOUT_CPUCH];  // 08c
    u16 mPriority;                       // 098
    JASChannelParams mParams;            // 09c
    JASSoundParams mSoundParams;         // 0b4
    s16 mKey;                            // 0c8
    u16 mVelocity;                       // 0ca
    f32 mKeySweep;                       // 0cc
    f32 mKeySweepTarget;                 // 0d0
    u32 mKeySweepCount;                  // 0d4
    u32 mSkipSamples;                    // 0d8
    struct {                             // 0dc
        u32 _0;            // 00, 0dc
        JASWaveInfo mInfo; // 04, 0e0
        int mPtr;          // 28, 104
    } mWaveData;

    static OSMessageQueue sBankDisposeMsgQ;
    static OSMessage sBankDisposeMsg[BANK_NUM];
    static OSMessage sBankDisposeList[BANK_NUM];
    static int sBankDisposeListSize;
};

#endif
