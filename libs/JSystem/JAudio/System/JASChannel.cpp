#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASDsp.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JGeometry/Util.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JSupport.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/os/OSMessage.h"

OSMessageQueue JASChannel::sBankDisposeMsgQ;
OSMessage JASChannel::sBankDisposeMsg[BANK_NUM];
OSMessage JASChannel::sBankDisposeList[BANK_NUM];
int JASChannel::sBankDisposeListSize;

template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;

JASChannel::JASChannel(JASChannel::Callback cb, void *cbData) :
mStatus(0),
mDspCh(NULL),
mCb(cb),
mCbData(cbData),
mUpdateTimer(0),
mBankDisposeID(NULL),
mOscillators(),
mVibrate(),
mTremolo(),
mParams(),
mSoundParams(),
mKey(0),
mVelocity(127),
mKeySweep(0.0f),
mKeySweepTarget(0.0f),
mKeySweepCount(0),
mSkipSamples(0),
mWaveData()
{
    mWaveData._0 = 0;
    mWaveData.mPtr = 0;
    mMixConfig[0].whole = 0x150;
    mMixConfig[1].whole = 0x210;
    mMixConfig[2].whole = 0x352;
    mMixConfig[3].whole = 0x412;
    mMixConfig[4].whole = 0;
    mMixConfig[5].whole = 0;
    mPriority = 0x13f;
    mPauseFlag = false;
}

JASChannel::~JASChannel() {
    if (mDspCh != NULL) {
#line 62
        JUT_WARNING_F2("%s", "~JASChannel:: mDspCh != NULL");
        mDspCh->drop();
    }

    if (mCb) {
        mCb(CB_STOP, this, NULL, mCbData);
    }
}

int JASChannel::play() {
    JASDSPChannel *chan = JASDSPChannel::alloc(JSULoByte(mPriority), dspUpdateCallback, this);
    if (!chan) {
        delete this;
        return 0;
    }

    mDspCh = chan;
    chan->start();
    mStatus = STATUS_PLAYING;
    return 1;
}

int JASChannel::playForce() {
    JASDSPChannel *chan = JASDSPChannel::allocForce(JSULoByte(mPriority), dspUpdateCallback, this);
    if (!chan) {
        delete this;
        return 0;
    }

    mDspCh = chan;
    chan->start();
    mStatus = STATUS_PLAYING;
    return 1;
}

void JASChannel::release(u16 release) {
    if (mStatus != STATUS_PLAYING)
        return;

    if (release != 0)
        setDirectRelease(release);
    
    for (u32 i = 0; i < OSC_NUM; i++) {
        if (mOscillators[i].isValid()) {
            mOscillators[i].release();
        }
    }

#line 124
    JUT_ASSERT(mDspCh);
    mDspCh->setPriority(JSUHiByte(mPriority));
    mStatus = STATUS_RELEASE;

}

void JASChannel::setOscInit(u32 oscnum, const JASOscillator::Data *data) {
#line 136
    JUT_ASSERT(oscnum < OSC_NUM);
    mOscillators[oscnum].initStart(data);
}

void JASChannel::setMixConfig(u32 bus, u16 mixConfig) {
#line 142
    JUT_ASSERT(bus < BUSOUT_CPUCH);
    mMixConfig[bus].whole = mixConfig;
}

f32 JASChannel::calcEffect(const JASChannel::PanVector *panVector) {
    return panVector->mSound + panVector->mEffect + panVector->mChannel;
}

f32 JASChannel::calcPan(const JASChannel::PanVector *panVector) {
    return 0.5f + (panVector->mSound - 0.5f) + (panVector->mEffect - 0.5f)
        + (panVector->mChannel - 0.5f);
}

void JASChannel::effectOsc(u32 oscnum, JASOscillator::EffectParams *effectParams) {
#line 165
    JUT_ASSERT(oscnum < OSC_NUM);
    f32 value = mOscillators[oscnum].getValue();
    switch (mOscillators[oscnum].getTarget()) {
    case JASOscillator::TARGET_PITCH:
        effectParams->mPitch *= value;
        break;
    case JASOscillator::TARGET_VOLUME:
        effectParams->mVolume *= value;
        break;
    case JASOscillator::TARGET_PAN:
        value -= 0.5;
        effectParams->mPan += value;
        break;
    case JASOscillator::TARGET_FXMIX:
        effectParams->mFxMix += value;
        break;
    case JASOscillator::TARGET_DOLBY:
        effectParams->mDolby += value;
        break;
    case JASOscillator::TARGET_5:
        effectParams->_14 *= value;
        break;
    case JASOscillator::TARGET_6:
        effectParams->_18 *= value;
        break;
    default:
#line 194
        JUT_WARNING_F2("%s", "Invalid osc target");
        break;
    }
}

void JASChannel::copyOsc(u32 oscnum, JASOscillator::Data *oscData) {
    // UNUSED, fabricated
    JUT_ASSERT(oscnum < OSC_NUM);
    JUT_ASSERT_F(mOscillators[oscnum].mData, "Cannot copy osc because mData == NULL")
    mOscillators[oscnum].mData = oscData;
}

void JASChannel::setKeySweepTarget(s32 sweep, u32 count) {
    if (count == 0) {
        mKeySweep = sweep;
    }
    else {
        mKeySweep = 0.0f;
        mKeySweepTarget = sweep;
    }   
    mKeySweepCount = count;
}

void JASChannel::updateEffectorParam(JASDsp::TChannel *channel, u16 *mixerVolume, const JASOscillator::EffectParams &effectParams) {
    PanVector pan_vector, fxmix_vector, dolby_vector;
    pan_vector.mSound = mSoundParams.mPan;
    pan_vector.mChannel = mParams.mPan;
    pan_vector.mEffect = effectParams.mPan;
    fxmix_vector.mSound = mSoundParams.mFxMix;
    fxmix_vector.mChannel = mParams.mFxMix;
    fxmix_vector.mEffect = effectParams.mFxMix;
    dolby_vector.mSound = mSoundParams.mDolby;
    dolby_vector.mChannel = mParams.mDolby;
    dolby_vector.mEffect = effectParams.mDolby;

    f32 pan = 0.5f;
    f32 dolby = 0.0f;
    switch (JASDriver::getOutputMode()) {
    case 0:
        break;
    case 1:
        pan = calcPan(&pan_vector);
        break;
    case 2:
        pan = calcPan(&pan_vector);
        dolby = calcEffect(&dolby_vector);
        break;
    }
    f32 fxmix = calcEffect(&fxmix_vector);
    f32 volume = mVelocity / 127.0f;
    volume = volume * volume;
    volume = mSoundParams.mVolume * effectParams.mVolume * mParams.mVolume * (effectParams._18 * mTremolo.getValue() + 1.0f) * volume;
    
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    pan = JASCalc::clamp01(pan);
    fxmix = JASCalc::clamp01(fxmix);
    dolby = JASCalc::clamp01(dolby);

    if (!isDolbyMode()) {
        updateMixer(volume, pan, fxmix, dolby, mixerVolume);
    } else {
        updateAutoMixer(channel, volume, pan, fxmix, dolby);
    }
}

s32 JASChannel::dspUpdateCallback(u32 status, JASDsp::TChannel *dspChannel, void *arg) {
    JASChannel *channel = static_cast<JASChannel*>(arg);
    switch (status) {
    case JASDSPChannel::CB_PLAY:
        return channel->updateDSPChannel(dspChannel);
    case JASDSPChannel::CB_START:
        return channel->initialUpdateDSPChannel(dspChannel);
    case JASDSPChannel::CB_STOP:
    case JASDSPChannel::CB_TIMER:
        channel->mDspCh->free();
        channel->mDspCh = NULL;
        delete channel;
        return -1;
    default:
#line 311
        JUT_WARNING_F2("Unexpected JASDSPChannel::UpdateStatus %d", status);
        return 0;
    }
}

s32 JASChannel::initialUpdateDSPChannel(JASDsp::TChannel *dspChannel) {
    if (isDolbyMode()) {
        dspChannel->initAutoMixer();
    }
    
    if (mCb != NULL) {
        mCb(CB_START, this, dspChannel, mCbData);
    }

    if (mWaveData.mInfo._24[0] == 0) {
#line 334
        JUT_CRITICAL_WARNING_F("%s", "Lost wave data while playing");
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }
    
    if (checkBankDispose()) {
#line 345
        JUT_CRITICAL_WARNING_F("%s", "Lost bank data while playing");
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }

    switch (mWaveData._0) {
    case 0:
        dspChannel->setWaveInfo(mWaveData.mInfo, mWaveData.mPtr, mSkipSamples);
        break;
    case 2:
        dspChannel->setOscInfo(mWaveData.mPtr);
        break;
    }

    for (u8 i = 0; i < 6; i++) {
        MixConfig mix_config = mMixConfig[i];
        u32 output_mode = JASDriver::getOutputMode();
        if (output_mode == 0) {
            switch (mix_config.parts.upper) {
            case 8:
                mix_config.parts.upper = 11;
                break;
            case 9:
                mix_config.parts.upper = 2;
                break;
            }
        } else if (output_mode == 1 && mix_config.parts.upper == 8) {
            mix_config.parts.upper = 11;
        }
        dspChannel->setBusConnect(i, mix_config.parts.upper);
    }

    JASOscillator::EffectParams effect_params;
    for (u32 i = 0; i < 2; i++) {
        if (mOscillators[i].isValid()) {
            mOscillators[i].update();
            effectOsc(i, &effect_params);
        }
    }
    mVibrate.resetCounter();
    mTremolo.resetCounter();
    u16 mixer_volume[6];
    updateEffectorParam(dspChannel, mixer_volume, effect_params);
    for (u8 i = 0; i < 6; i++) {
        dspChannel->setMixerInitVolume(i, mixer_volume[i]);
    }

    f32 pitch = JASCalc::pow2(mParams._8 + (mKey + mKeySweep) / 12.0f
                                                + effect_params._14 * mVibrate.getValue());
    pitch = mSoundParams.mPitch * effect_params.mPitch * pitch * mParams.mPitch * 4096.0f;
    if (pitch < 0.0f) {
        pitch = 0.0f;
    }
    dspChannel->setPitch(pitch);

    dspChannel->setPauseFlag(mPauseFlag);
    return 0;
}

s32 JASChannel::updateDSPChannel(JASDsp::TChannel *dspChannel) {
#line 429
    JUT_ASSERT(mStatus == STATUS_PLAY || mStatus == STATUS_RELEASE)
    JUT_ASSERT(mDspCh);
    if (mCb) {
        mCb(CB_PLAY, this, dspChannel, mCbData);
    }

    if (mWaveData.mInfo._24[0] == 0) {
#line 441
        JUT_CRITICAL_WARNING_F("%s", "Lost wave data while playing");
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }
    
    if (checkBankDispose()) {
#line 452
        JUT_CRITICAL_WARNING_F("%s", "Lost bank data while playing");
        mDspCh->free();
        mDspCh = NULL;
        delete this;
        return -1;
    }

    dspChannel->setPauseFlag(mPauseFlag);
    
    if (mPauseFlag) {
        if (mOscillators[0].isRelease()) {
            mDspCh->free();
            mDspCh = NULL;
            delete this;
            return -1;
        }
        return 0;
    }
    
    f32 inc = 32028.5f / JASDriver::getDacRate();
    mVibrate.incCounter(inc);
    mTremolo.incCounter(inc);
    if (mUpdateTimer != 0) {
        mUpdateTimer--;
        if (mUpdateTimer == 0 && mCb != NULL) {
            mCb(CB_TIMER, this, dspChannel, mCbData);
        }
    }
    inc = 48000.0f / JASDriver::getDacRate();
    JASOscillator::EffectParams effect_params;
    for (u32 i = 0; i < 2; i++) {
        if (mOscillators[i].isValid()) {
            mOscillators[i].incCounter(inc);
            effectOsc(i, &effect_params);
            if (i == 0 && mOscillators[i].isStop()) {
                mDspCh->free();
                mDspCh = NULL;
                delete this;
                return -1;
            }
        }
    }

    u16 mixer_volume[6];
    updateEffectorParam(dspChannel, mixer_volume, effect_params);
    for (u8 i = 0; i < 6; i++) {
        dspChannel->setMixerVolume(i, mixer_volume[i]);
    }

    f32 pitch = JASCalc::pow2(mParams._8 + (mKey + mKeySweep) / 12.0f
                                                + effect_params._14 * mVibrate.getValue());
    pitch = mSoundParams.mPitch * effect_params.mPitch * pitch * mParams.mPitch * 4096.0f;
    if (pitch < 0.0f) {
        pitch = 0.0f;
    }
    dspChannel->setPitch(pitch);

    if (mKeySweepCount != 0) {
        mKeySweep += (mKeySweepTarget - mKeySweep) / mKeySweepCount;
        mKeySweepCount--;
    }
    return 0;
}

void JASChannel::updateAutoMixer(JASDsp::TChannel *dspChannel, f32 volume, f32 p3, f32 p4 , f32 p5) {
    f32 clamped = JASCalc::clamp01(volume);
    dspChannel->setAutoMixer(clamped * JASDriver::getChannelLevel_dsp(), p3 * 127.5f, p5 * 127.5f, p4 * 127.5f, 0);
}

void JASChannel::updateMixer(f32 volume, f32 pan, f32 fxmix, f32 dolby, u16* volumeOut) {
    for (u32 i = 0; i < BUSOUT_CPUCH; i++) {
        f32 mixerVolume = volume;
        MixConfig config = mMixConfig[i];
        if (config.parts.upper == 0) {
            volumeOut[i] = 0;
        } else {
            f32 scale;

            if (config.parts.lower0 != 0) {
                switch (config.parts.lower0) {
                case 1:
                    scale = pan;
                    break;
                case 2:
                    scale = fxmix;
                    break;
                case 3:
                    scale = dolby;
                    break;
                case 5:
                    scale = 1.0f - pan;
                    break;
                case 6:
                    scale = 1.0f - fxmix;
                    break;
                case 7:
                    scale = 1.0f - dolby;
                    break;
                }

                switch (config.parts.lower0) {
                case 2:
                case 6:
                    mixerVolume *= scale;
                    break;
                default:
                    mixerVolume *= JMASinRadian(scale * JGeometry::TUtilf::PI() * 0.5f);
                    break;
                }
            }

            if (config.parts.lower1 != 0) {
                switch (config.parts.lower1) {
                case 1:
                    scale = pan;
                    break;
                case 2:
                    scale = fxmix;
                    break;
                case 3:
                    scale = dolby;
                    break;
                case 5:
                    scale = 1.0f - pan;
                    break;
                case 6:
                    scale = 1.0f - fxmix;
                    break;
                case 7:
                    scale = 1.0f - dolby;
                    break;
                }

                switch (config.parts.lower1) {
                case 3:
                case 7:
                    mixerVolume *= JMASinRadian((scale * 0.34776f + 0.32612f) * JGeometry::TUtilf::PI() * 0.5f);
                    break;
                case 2:
                case 6:
                    mixerVolume *= scale;
                    break;
                default:
                    mixerVolume *= JMASinRadian(scale * JGeometry::TUtilf::PI() * 0.5f);
                    break;
                }
            }

            volumeOut[i] = JASCalc::clamp01(mixerVolume) * JASDriver::getChannelLevel_dsp();
        }
    }
}

void JASChannel::free() {
#line 628
    JUT_ASSERT(mStatus == STATUS_RELEASE || mStatus == STATUS_STOP);
    mCb = NULL;
    mCbData = NULL;
}

void JASChannel::initBankDisposeMsgQueue() {
   OSInitMessageQueue(&sBankDisposeMsgQ, sBankDisposeMsg, BANK_NUM); 
   sBankDisposeListSize = 0;
}

void JASChannel::receiveBankDisposeMsg() {
    OSMessage msg;
    sBankDisposeListSize = 0;
    while (OSReceiveMessage(&sBankDisposeMsgQ, &msg,OS_MESSAGE_NOBLOCK)) {
        sBankDisposeList[sBankDisposeListSize] = msg;
        sBankDisposeListSize++;
    }
}

void JASChannel::sendBankDisposeMsg(OSMessage msg) {
    OSSendMessage(&sBankDisposeMsgQ, msg, OS_MESSAGE_BLOCK);
}

bool JASChannel::checkBankDispose() const {
    if (!mBankDisposeID) {
        return false;
    }

    for (int i = 0; i < sBankDisposeListSize; i++) {
        if (mBankDisposeID == sBankDisposeList[i]) {
            return true;
        }
    }
    return false;
}
