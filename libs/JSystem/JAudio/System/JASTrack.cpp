#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JAudio/System/JASBank.h"
#include "JSystem/JAudio/System/JASBankTable.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JUtility/JUTAssert.h"

// TODO: cleanup

namespace JASDsp {
static const u32 FILTER_MODE_IIR = 0x00000020;
}

JASDefaultBankTable JASTrack::sDefaultBankTable;
JASTrack::TList JASTrack::sTrackList;

#include "JSystem/JAudio/JASFakeMatch.h"

const JASOscillator::Point JASTrack::sAdsTable[4] = {
    {0, 0, 0x7fff},
    {0, 0, 0x7fff},
    {0, 0, 0},
    {0xe, 0, 0},
};

const JASOscillator::Data JASTrack::sEnvOsc = {0, 1.0f, NULL, NULL, 1.0f, 0.0f};

const JASOscillator::Data JASTrack::sPitchEnvOsc = {1, 1.0f, NULL, NULL, 1.0f, 0.0f};

JASTrack::JASTrack() : mDefaultChannelMgr(this), 
mMove(),
mChannelMgrCount(1), 
mStatus(0),
mNode()
{
    mChannelMgrs[0] = &mDefaultChannelMgr;
    for (int i = 1; i < 4; i++) {
        mChannelMgrs[i] = NULL;
    }
    init();
}

JASTrack::~JASTrack() {
#line 69
    JUT_ASSERT(mStatus != STATUS_RUN);
    for(int i = 1; i < 4; i++) {
        delete mChannelMgrs[i];
    }
}

void JASTrack::setChannelMgrCount(u32 count) {
#line 78
    JUT_ASSERT(mStatus != STATUS_RUN);
    JUT_ASSERT(count >= 1 && count <= CHANNEL_MGR_MAX);
    mChannelMgrCount = 1;
    mChannelMgrs[0]->init();

    for (u32 i = 1; i < count; i++, mChannelMgrCount++) {
        if (mChannelMgrs[i] == NULL) {
            mChannelMgrs[i] = new TChannelMgr(this);
            if (mChannelMgrs[i] == NULL) {
                JUT_WARNING_F2("%s", "Not enough JASTrackChannelMgr\n");
                return;
            }            
        }
        else {
            mChannelMgrs[i]->init();
        }
    }
    for (int i = mChannelMgrCount; i < CHANNEL_MGR_MAX; i++) {
        if (mChannelMgrs[i] != NULL) {            
            delete mChannelMgrs[i];
            mChannelMgrs[i] = NULL;
        }
    }
}

void JASTrack::init() {
#line 107
    JUT_ASSERT(mStatus == STATUS_FREE || mStatus == STATUS_STOPPED);
    mSeqCtrl.init();
    mTrackPort.init();
    initTimed();
    mRegisterParam.init();
    mOscParam[0] = sEnvOsc;
    mOscParam[1] = sPitchEnvOsc;
    for (int i = 0; i < 4; i++) {
        mOscPoint[i] = sAdsTable[i];
    }
    mParent = NULL;
    for (int i = 0; i < 16; i++) {
        mChildren[i] = NULL;
    }
    
    for(int i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i]) {
            mChannelMgrs[i]->init();
        }
    }
    
    mBankTable = &sDefaultBankTable;
    _1d8 = 1.0f;
    _1dc = 1.0f;
    mVibDepth = 0.0f;
    mVibPitch = 1.0f;
    mTremDepth = 0.0f;
    mTremPitch = 1.0f;
    mVibDelay = 0;
    mTremDelay = 0;
    mPanPower = 1.0f;
    mDirectRelease = 0;
    mSkipSample = 0;
    mTempo = 120;
    mTimebase = 0x30;
    mTempoRate = 1.0f;
    updateTempo();
    mTranspose = 0;
    _22b = 60;
    mBankNumber = 0;
    mProgNumber = 0xf0;
    mBendSense = 0xc;
    mNoteOnPrio = 0x40;
    mReleasePrio = 0;
    mGateRate = 100;
    mMixConfig[0] = 0x150;
    mMixConfig[1] = 0x210;
    mMixConfig[2] = 0x352;
    mMixConfig[3] = 0x412;
    mMixConfig[4] = 0;
    mMixConfig[5] = 0;
    for (int i = 0; i < 8; i++) {
        mFIR[i] = 0;
    }
    mFIR[0] = 0x7fff;
    for (int i = 0; i < 8; i++) {
        mIIR[i] = 0;
    }
    mIIR[0] = 0x7fff;
    mFilterMode = 0;
    mFlags.pause = false;
    mFlags.mute = false;
    mFlags.flag2 = true;
    mFlags.flag5 = true;
    mFlags.autoDelete = false;
    mFlags.flag4 = false;
    mFlags.flag6 = false;
    mStatus = 0;
    setChannelMgrCount(1);
}

void JASTrack::initTimed() {
    mMove.params.volume.mValue = 1.0f;
    mMove.params.pitch.mValue = 0.0f;
    mMove.params.pan.mValue = 0.5f;
    mMove.params.fxmix.mValue = 0.0f;
    mMove.params.dolby.mValue = 0.0f;
    mMove.params.distFilter.mValue = 0.0f;
    for (int i = 0; i < 6; i++) {
        mMove.array[i].mCount = 0;
        mMove.array[i].mTarget = mMove.array[i].mValue;
    }
}

void JASTrack::inherit(const JASTrack& parent) {
    mFlags.flag2 = parent.mFlags.flag2;
    mBankNumber = parent.mBankNumber;
    mProgNumber = parent.mProgNumber;
    mNoteOnPrio = parent.mNoteOnPrio;
    mReleasePrio = parent.mReleasePrio;
    mSkipSample = parent.mSkipSample;
    for (int i = 0; i < 8; i++) {
        mFIR[i] = parent.mFIR[i];
    }
    for (int i = 0; i < 8; i++) {
        mIIR[i] = parent.mIIR[i];
    }
    mFilterMode = parent.mFilterMode;
    for (int i = 0; i < 6; i++) {
        mMixConfig[i] = parent.mMixConfig[i];
    }
}

void JASTrack::assignExtBuffer(u32 index, JASSoundParams *soundParams) {
#line 241
    JUT_ASSERT(index < mChannelMgrCount);
    mChannelMgrs[index]->mSoundParams = soundParams;
}

void JASTrack::setSeqData(void *p1, u32 p2) {
#line 249
    JUT_ASSERT(mStatus == STATUS_FREE);
    mSeqCtrl.start(p1, p2);
}

void JASTrack::startSeq() {
#line 257
    JUT_ASSERT(mParent == 0);
    JUT_ASSERT(mStatus == STATUS_FREE);
    JAS_CS_START
    sTrackList.append(this);
    mStatus = 1;
    JAS_CS_END
}

void JASTrack::stopSeq() {
    JAS_CS_START
    mFlags.flag6 = 1;
    JAS_CS_END
}

void JASTrack::start() {
#line 281
    JUT_ASSERT(mParent != 0);
    JUT_ASSERT(mStatus == STATUS_FREE);
    mStatus = 1;
}

void JASTrack::close() {
    for (int i = 0; i < 16; i++) {
        JASTrack *child = mChildren[i];
        if (child != NULL) {
            child->close();
            if (child->mFlags.autoDelete) {
                delete child;
                mChildren[i] = NULL;
            }
        }
    }
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL) {
            mChannelMgrs[i]->releaseAll();
        }
    }
    mParent = NULL;
    mStatus = 2;
}

bool JASTrack::connectChild(u32 track_no, JASTrack *child) {
#line 318
    JUT_ASSERT(track_no < MAX_CHILDREN);
    if (mChildren[track_no] != NULL) {
        return false;
    }
    child->mParent = this;
    mChildren[track_no] = child;
    return true;
}

void JASTrack::closeChild(u32 track_no) {
#line 332
    JUT_ASSERT(track_no < MAX_CHILDREN);
    JASTrack *child = mChildren[track_no];
    if (child != NULL) {
        getRootTrack()->updateSeq(false, 1.0f);
        child->close();
        if (child->mFlags.autoDelete) {
            delete child;
            mChildren[track_no] = NULL;
        }
    }
}

JASTrack *JASTrack::openChild(u32 trk_no) {
#line 350 
    JUT_ASSERT(trk_no < MAX_CHILDREN);
    JASTrack *child = mChildren[trk_no];
    if (child != NULL) {
        switch (child->mStatus) {
        case 0:
            break;
        case 1:
            getRootTrack()->updateSeq(false, 1.0f);
            child->close();
            // no break
        case 2:
            bool auto_delete = child->mFlags.autoDelete;
            child->init();
            child->mFlags.autoDelete = auto_delete;
            mChildren[trk_no] = NULL;
            connectChild(trk_no, child);
            break;
        }
    } else {
        child = new JASTrack();
        if (child == NULL) {
#line 380
            JUT_WARNING_F2("%s", "Not enough JASTrack\n");
            return NULL;
        }
        child->mFlags.autoDelete = true;
        connectChild(trk_no, child);
    }
    child->setChannelMgrCount(mChannelMgrCount);
    child->inherit(*this);
    return child;
}

void JASTrack::connectBus(int line, int mix) {
#line 396
    JUT_ASSERT(line < JASChannel::BUSOUT_CPUCH);
    mMixConfig[line] = mix;
}

void JASTrack::setLatestKey(u8 p1) {
    _22b = p1;
    _22b += getTransposeTotal();
}

JASChannel *JASTrack::channelStart(JASTrack::TChannelMgr *channelMgr, u32 p2, u32 p3, u32 updateTimer) {
    JASBank *bank = NULL;
    if (mBankTable != NULL) {
        bank = mBankTable->getBank(mBankNumber);
    }
    JASChannel *channel = JASBank::noteOn(bank, mProgNumber, p2, p3,
                                          mNoteOnPrio | mReleasePrio << 8,
                                          channelUpdateCallback, channelMgr);
    if (channel == NULL) {
        return NULL;
    }
    channel->setUpdateTimer(updateTimer);
    channel->setSkipSamples(mSkipSample);
    channel->setVibrateDelay(mVibDelay);
    channel->setTremoloDelay(mTremDelay);

    for (u32 i = 0; i < 6; i++) {
        channel->setMixConfig(i, mMixConfig[i]);
    }
    overwriteOsc(channel);
    if (mDirectRelease != 0) {
        channel->setDirectRelease(mDirectRelease);
    }
    return channel;
}

int JASTrack::noteOn(u32 noteid, u32 p2, u32 p3) {
#line 476
    JUT_ASSERT(noteid != 0 && noteid < TChannelMgr::CHANNEL_MAX);
    if (isMute()) {
        return 0;
    }
    int ret = 1;
    p2 += getTransposeTotal();
    for (int i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL) {
            mChannelMgrs[i]->noteOff(noteid, 0);
            JASChannel *channel = channelStart(mChannelMgrs[i], p2, p3, 0);
            if (channel == NULL) {
                ret = 0;
            }
            mChannelMgrs[i]->mChannels[noteid] = channel;
        }
    }
    return ret;
}

int JASTrack::gateOn(u32 p1, u32 velocity, f32 time, u32 flags) {
    p1 += getTransposeTotal();
    if (mGateRate != 100) {
        time *= mGateRate / 100.0f;
    }
    u32 uvar2 = seqTimeToDspTime(time);
    u32 update_timer = (flags & 6) ? 0 : uvar2;
    u32 uvar7 = (flags & 1) ? _22b : p1;

    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr *channel_mgr = mChannelMgrs[i];
        if (channel_mgr != NULL) {
            if (!mFlags.flag4) {
                channel_mgr->noteOff(0, 0);
                if (!isMute()) {
                    channel_mgr->mChannels[0] =
                        channelStart(channel_mgr, uvar7, velocity, update_timer);
                }
            } else {
                JASChannel *channel = channel_mgr->mChannels[0];
                if (channel != NULL) {
                    channel->mKey = uvar7 - channel->mWaveData.mInfo._01;
                    channel->mVelocity = velocity;
                    channel->mUpdateTimer = update_timer;
                }
            }
            if ((flags & 1) && channel_mgr->mChannels[0] != NULL) {
                channel_mgr->mChannels[0]->setKeySweepTarget(p1 - uvar7, uvar2);
            }
        }
    }
    mFlags.flag4 = (flags >> 1) & 1;
    _22b = p1;
    return 1;
}

int JASTrack::noteOff(u32 noteid, u16 p2) {
#line 553
    JUT_ASSERT(noteid < TChannelMgr::CHANNEL_MAX);
    int ret = 1;
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL) {
            if (!mChannelMgrs[i]->noteOff(noteid, p2)) {
                ret = 0;
            }
        }
    }
    return ret;
}

bool JASTrack::checkNoteStop(u32 noteid) const {
#line 571
    JUT_ASSERT(noteid < TChannelMgr::CHANNEL_MAX);
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL && mChannelMgrs[i]->mChannels[noteid] != NULL) {
            return false;
        }
    }
    return true;
}

void JASTrack::overwriteOsc(JASChannel *channel) {
    for (u32 i = 0; i < 2; i++) {
        if (mOscParam[i].mTable) {
#line 589
            JUT_ASSERT(mOscParam[i].rel_table == 0);
            channel->setOscInit(i, &mOscParam[i]);
        }
    }
}

void JASTrack::updateTimedParam() {
    for (u32 i = 0; i < 6; i++) {
        MoveParam_ *param = &mMove.array[i];
        if (param->mCount != 0) {
            param->mValue += (param->mTarget - param->mValue) / param->mCount;
            param->mCount--;
        }
    }
}

void JASTrack::updateTrack(f32 p1) {
    updateTempo();
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr *channel_mgr = mChannelMgrs[i];
        if (channel_mgr != NULL) {
            f32 volume = mMove.params.volume.mValue;
            volume = volume * volume;
            f32 pitch = 1.0f;
            f32 fvar6 = mMove.params.pitch.mValue * mBendSense * (1.0f / 3.0f);
            f32 pan = (mMove.params.pan.mValue - 0.5f) * mPanPower;
            f32 fxmix = mMove.params.fxmix.mValue;
            f32 dolby = mMove.params.dolby.mValue;
            JASSoundParams *sound_params = channel_mgr->mSoundParams;
            if (sound_params != NULL) {
                volume *= sound_params->mVolume;
                pitch *= sound_params->mPitch;
                pan += sound_params->mPan - 0.5f;
                fxmix += sound_params->mFxMix;
                dolby += sound_params->mDolby;
            }
            pan *= p1;
            if (mParent == NULL) {
                channel_mgr->mParams.mVolume = volume;
                channel_mgr->mParams.mPitch = pitch;
                channel_mgr->mParams.mPan = pan;
                channel_mgr->mParams.mFxMix = fxmix;
                channel_mgr->mParams.mDolby = dolby;
                channel_mgr->mParams._8 = fvar6;
            } else {
                TChannelMgr *parent_channel_mgr = mParent->mChannelMgrs[i];
                if (parent_channel_mgr == NULL) {
                    parent_channel_mgr = mParent->mChannelMgrs[0];
                }
                channel_mgr->mParams.mVolume = parent_channel_mgr->mParams.mVolume * volume;
                channel_mgr->mParams.mPitch = parent_channel_mgr->mParams.mPitch * pitch;
                channel_mgr->mParams.mPan = (parent_channel_mgr->mParams.mPan - 0.5f) + pan;
                channel_mgr->mParams.mFxMix = parent_channel_mgr->mParams.mFxMix + fxmix;
                channel_mgr->mParams.mDolby = parent_channel_mgr->mParams.mDolby + dolby;
                channel_mgr->mParams._8 = parent_channel_mgr->mParams._8 + fvar6;
            }
            channel_mgr->mParams.mPan += 0.5f;
        }
    }
}

void JASTrack::updateTempo() {
    if (mParent == NULL) {
        _1dc = mTempoRate * (mTimebase * mTempo * (4.0f / 3.0f) / JASDriver::getDacRate());
    } else {
        mTempo = mParent->mTempo;
        mTimebase = mParent->mTimebase;
        _1dc = mParent->_1dc;
    }
}

void JASTrack::updateSeq(bool p1, f32 p2) {
    if (!p1) {
        p1 = mFlags.flag5;
    }
    mFlags.flag5 = false;
    if (p1) {
        updateTrack(p2);
    }
    f32 fvar1 = p2 * mPanPower;
    for (int i = 0; i < 16; i++) {
        JASTrack *child = mChildren[i];
        if (child && child->mStatus == STATUS_RUN) {
            child->updateSeq(p1, fvar1);
        }
    }
}

u32 JASTrack::seqTimeToDspTime(f32 seqTime) {
    f32 dsp_time;
    if (mFlags.flag2) {
        dsp_time = seqTime / _1dc;
    } else {
        dsp_time = seqTime * (120.0f / mTimebase);
        dsp_time *= JASDriver::getSubFrames() / 10.0f;
    }
    return dsp_time;
}

void JASTrack::setParam(u32 target, f32 targetf, u32 count) {
#line 729
    JUT_ASSERT(target < TIMED_PARAMS);
    MoveParam_ *param = &mMove.array[target];
    param->mTarget = targetf;
    if (count == 0) {
        param->mValue = param->mTarget;
    }
    param->mCount = count;
}

void JASTrack::noteOffAll(u16 p1) {
    for (u8 i = 0; i < 8; i++) {
        noteOff(i, p1);
    }
    for (int i = 0; i < 16; i++) {
        JASTrack *child = mChildren[i];
        if (child && child->mStatus == STATUS_RUN) {
            child->noteOffAll(p1);
        }
    }
}

void JASTrack::mute(bool mute) {
    mFlags.mute = mute;
    if (mute) {
        noteOffAll(10);
    }
}

void JASTrack::setOscTable(u32 osc_no, const JASOscillator::Point *table) {
#line 781
    JUT_ASSERT(osc_no < OSC_NUM);
    mOscParam[osc_no].mTable = table;
    if (osc_no != 0) {
        mOscParam[osc_no].rel_table = table;
    }
}

void JASTrack::setOscAdsr(s16 p1, s16 p2, s16 p3, s16 p4, u16 directRelease) {
    mOscParam[0] = sEnvOsc;
    mOscParam[0].mTable = mOscPoint;
    mOscPoint[0]._2 = p1;
    mOscPoint[1]._2 = p2;
    mOscPoint[2]._2 = p3;
    mOscPoint[2]._4 = p4;
    mDirectRelease = directRelease;
}


void JASTrack::setFIR(const s16 *FIR) {
    for (int i = 0; i < 8; i++) {
        mFIR[i] = FIR[i];
    }
    mFilterMode &= JASDsp::FILTER_MODE_IIR;
    mFilterMode |= 8;
}

void JASTrack::setIIR(const s16 *IIR) {
    for (int i = 0; i < 8; i++) {
        mIIR[i] = IIR[i];
    }
    mFilterMode |= JASDsp::FILTER_MODE_IIR;
}

u16 JASTrack::readPortSelf(u32 p1) {
    return mTrackPort.readImport(p1);
}

void JASTrack::writePortSelf(u32 p1, u16 p2) {
    mTrackPort.writeExport(p1, p2);
}

void JASTrack::writePort(u32 p1, u16 p2) {
    mTrackPort.writeImport(p1, p2);
    if (p1 == 0 || p1 == 1) {
        JASSeqCtrl::IntrType intr;
        if (p1 == 0) {
            intr = JASSeqCtrl::INTRTYPE_VALUE_2;
        } else {
            intr = JASSeqCtrl::INTRTYPE_VALUE_3;
        }
        mSeqCtrl.interrupt(intr);
    }
}

u16 JASTrack::readPort(u32 p1) {
    return mTrackPort.readExport(p1);
}

void JASTrack::setChannelPauseFlag(bool pause) {
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr *mgr = mChannelMgrs[i];
        if (mgr) {
            mgr->setPauseFlag(pause);
        }
    }
}

void JASTrack::pause(bool pause) {
    if (mFlags.pause == pause)
        return;
    mFlags.pause = pause;
    
    setChannelPauseFlag(pause);
    for (int i = 0; i < MAX_CHILDREN; i++) {
        JASTrack *child = mChildren[i];
        if (child) {
            child->setChannelPauseFlag(pause);
        }
    }
    mSeqCtrl.interrupt(pause ? JASSeqCtrl::INTRTYPE_VALUE_0 : JASSeqCtrl::INTRTYPE_VALUE_1);
}

int JASTrack::getTransposeTotal() const {
    if (mParent != NULL) {
        return mTranspose + mParent->getTransposeTotal();
    } else {
        return mTranspose;
    }
}

bool JASTrack::isMute() const {
    if (mParent != NULL) {
        return mFlags.mute || mParent->isMute();
    } else {
        return mFlags.mute;
    }
}

void JASTrack::setTempo(u16 tempo) {
#line 937 
    JUT_ASSERT(mParent == 0);
    mTempo = tempo;
    mFlags.flag5 = true;
}

void JASTrack::setTempoRate(f32 tempoRate) {
#line 944
    JUT_ASSERT(mParent == 0);
    mTempoRate = tempoRate;
    mFlags.flag5 = true;
}

void JASTrack::setTimebase(u16 timebase) {
#line 951
    JUT_ASSERT(mParent == 0);
    mTimebase = timebase;
    mFlags.flag5 = true;
}

void JASTrack::updateChannel(JASChannel *ch, JASDsp::TChannel *dspCh) {
    ch->setVibrate(mVibDepth, mVibPitch);
    ch->setTremolo(mTremDepth, mTremPitch);
    if (mFilterMode & 0x20) {
        dspCh->setIIRFilterParam(mIIR);
    }
    if (mFilterMode & 0x1f) {
        dspCh->setFIR8FilterParam(mFIR);
    }
    dspCh->setFilterMode(mFilterMode);
    dspCh->setDistFilter(mMove.params.distFilter.mValue * 32767.0f);
}

void JASTrack::channelUpdateCallback(u32 p1, JASChannel *ch, JASDsp::TChannel *dspCh, void *arg) {
    TChannelMgr *channel_mgr = static_cast<TChannelMgr*>(arg);
    JASTrack *track = channel_mgr->mTrack;
    switch (p1) {
    case 0:
    case 1:
        ch->mParams = channel_mgr->mParams;
        track->updateChannel(ch, dspCh);
        break;
    case 3:
        ch->release(0);
        ch->free();
        channel_mgr->mChannels[0] = NULL;
        break;
    case 2:
        for (int i = 0; i < 8; i++) {
            if (ch == channel_mgr->mChannels[i]) {
                channel_mgr->mChannels[i] = NULL;
                break;
            }
        }
        break;
    }
}

JASTrack *JASTrack::getRootTrack() {
    JASTrack *track = this;
    while (track->mParent != NULL) {
        track = track->mParent;
    }
    return track;
}

int JASTrack::tickProc() {
    if (mFlags.pause) {
        return 0;
    }
    int ret = mSeqCtrl.tickProc(this);
    updateTimedParam();
    mFlags.flag5 = true;
    if (ret < 0) {
        return -1;
    }
    for (int i = 0; i < 16; i++) {
        JASTrack *child = mChildren[i];
        if (child != NULL && child->mStatus == 1) {
#line 1063
            JUT_ASSERT(this == child->mParent);
            ret = child->tickProc();
            if (ret < 0) {
                getRootTrack()->updateSeq(false, 1.0f);
                child->close();
                if (child->mFlags.autoDelete) {
                    delete child;
                    mChildren[i] = NULL;
                }
            }
        }
    }
    return 0;
}

int JASTrack::seqMain() {
#line 1081
    JUT_ASSERT(mStatus == STATUS_RUN);
    if (mFlags.flag6) {
        updateSeq(true, 1.0f);
        close();
        return -1;
    } 
    while (_1d8 >= 1.0f) {
        _1d8 -= 1.0f;
        if (tickProc() < 0) {
            updateSeq(false, 1.0f);
            close();
            return -1;
        }
    }
    _1d8 += _1dc;
    updateSeq(false, 1.0f);
    return 0;
    
}

s32 JASTrack::TList::cbSeqMain(void *arg) {
    static_cast<JASTrack::TList*>(arg)->seqMain();
    return 0;
}

void JASTrack::TList::append(JASTrack *track) {
   if (!mCallbackRegistered) {
        if (!JASDriver::registerSubFrameCallback(cbSeqMain, this)) {
#line 1130 
            JUT_WARNING_F2("%s", "Failed: registerSubFrameCallback");
            return;
        }
        mCallbackRegistered = true;
    }
    Push_back(track);
}

void JASTrack::TList::seqMain() {
    iterator it, next;
    for (it = begin(); it != end(); it = next) {
        next = it;
        ++next;
        if (it->seqMain() < 0) {
            Remove(&*it);
            if (it->mFlags.autoDelete) {
                delete &*it;
            }
        }
    }
}

JASTrack::TChannelMgr::TChannelMgr(JASTrack *track) : mSoundParams(NULL), mTrack(track) {
    for (int i = 0; i < 8; i++) {
        mChannels[i] = NULL;
    }
    for (int i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::init() {
    mSoundParams = NULL;
    mParams.init();
    for (int i = 0; i < 8; i++) {
        mChannels[i] = NULL;
    }
    for (int i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::releaseAll() {
    for (int i = 0; i < 8; i++) {
        JASChannel *channel = mChannels[i];
        if (channel != NULL) {
            channel->release(0);
            channel->free();
            mChannels[i] = NULL;
        }
    }
}

bool JASTrack::TChannelMgr::noteOff(u32 noteid, u16 p2) {
#line 1197
    JUT_ASSERT(noteid < CHANNEL_MAX);
    JASChannel *channel = mChannels[noteid];
    if (channel == NULL) {
        return false;
    }
    if (p2 == 0) {
        channel->release(0);
    } else {
        channel->release(p2);
    }
    channel->free();
    mChannels[noteid] = NULL;
    return true;
}

void JASTrack::TChannelMgr::setPauseFlag(bool pause) {
    for (int i = 0; i < 8; i++) {
        if (mChannels[i] != NULL) {
            mChannels[i]->setPauseFlag(pause);
        }
    }
}
