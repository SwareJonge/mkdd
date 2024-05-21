#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JAudio/System/JASReport.h"
#include "JSystem/JAudio/System/JASTrack.h"

#ifdef DEBUG
#pragma sym on
#endif

void JAISoundHandle::releaseSound()
{
    if (sound_ == NULL)
    {
        return;
    }
    sound_->handle_ = NULL;
    sound_ = NULL;
}

void JAISound::releaseHandle()
{
    if (handle_ == NULL)
    {
        return;
    }
    handle_->sound_ = NULL;
    handle_ = NULL;
}

void JAISound::attachHandle(JAISoundHandle *handle)
{
#line 55
    JUT_ASSERT(handle)
    if (handle->isSoundAttached())
    {
        handle->getSound()->stop();
    }
    if (isHandleAttached())
    {
        releaseHandle();
    }
    handle_ = handle;
    handle_->sound_ = this;
}

s32 JAISoundStatus_::lockWhenPrepared()
{
    if (state.unk == 0)
    {
        state.unk = 1;
        return 1;
    }
    return 0;
}

s32 JAISoundStatus_::unlockIfLocked()
{
    if (state.unk == 3)
    {
        state.unk = 4;
        return 1;
    }
    if (state.unk == 1)
    {
        state.unk = 0;
        return 1;
    }
    return 0;
}

void JAISoundParams::mixOutAll(const JASSoundParams &params, JASSoundParams *outParams, f32 intensity)
{
    outParams->mVolume = params.mVolume * mProperty._0 * mMove.mParams.mVolume * intensity;
    outParams->mFxMix = params.mFxMix + mProperty._4 + mMove.mParams.mFxMix;
    outParams->mPitch = params.mPitch * mMove.mParams.mPitch;
    outParams->mPan = (params.mPan + mMove.mParams.mPan) - 0.5f;
    outParams->mDolby = params.mDolby + mMove.mParams.mDolby;
}

JAISound::JAISound() : params_() {}

void JAISound::start_JAISound_(JAISoundID soundID, const JGeometry::TVec3f *pos, JAIAudience *audience)
{
    handle_ = NULL;
    soundID_ = soundID;
    status_.init();
    params_.init();
    fader_.forceIn();
    audience_ = audience;
    prepareCount_ = 0;
    count_ = 0;

    if (pos != 0 && audience_ != NULL)
    {
        audible_ = audience_->newAudible(*pos, soundID_, NULL, 0);
    }
    else
    {
        audible_ = NULL;
    }
    priority_ = 0;
}

bool JAISound::acceptsNewAudible() const
{
    bool accepts = false;
    if (audible_ == NULL && status_.state.flags.flag2 == 0)
    {
        accepts = true;
    }
    return accepts;
}

void JAISound::newAudible(const JGeometry::TVec3f &param_0,
                          const JGeometry::TVec3f *param_1, u32 param_2, JAIAudience *audience)
{
#line 153
    JUT_ASSERT(acceptsNewAudible())
    if (audience != NULL)
    {
        audience_ = audience;
    }

    JUT_ASSERT(audience_)
    audible_ = audience_->newAudible(param_0, soundID_, param_1, param_2);
}

void JAISound::stop(u32 fadeCount)
{
#line 166
    JUT_ASSERT(status_.isAlive());
    if (fadeCount == 0)
    {
        stop();
        return;
    }
    fader_.fadeOut(fadeCount);
    removeLifeTime_();
    status_._1.flags.flag2 = 0;
    status_.state.flags.flag5 = 1;
    status_.state.flags.flag1 = 1;
}

void JAISound::stop()
{
    stop_JAISound_();
    releaseHandle();
}

JAISe *JAISound::asSe()
{
    return NULL;
}

JAISeq *JAISound::asSeq()
{
    return NULL;
}

JAIStream *JAISound::asStream()
{
    return NULL;
}

void JAISound::die_JAISound_()
{
    if (audible_ != NULL)
    {
        audience_->deleteAudible(audible_);
        audible_ = NULL;
        audience_ = NULL;
    }
    fader_.forceOut();
    releaseHandle();
    status_.state.unk = 6;
}

void JAISound::increasePrepareCount_JAISound_()
{
    if ((u8)(++prepareCount_) == 0)
    {
        JASReport("It cost %d steps to prepare Sound(ID:%08x, Address%08x).\n", prepareCount_,
                  (u32)soundID_, this);
    }
}

bool JAISound::calc_JAISound_()
{
    status_.state.flags.flag2 = 1;
    if (isStopping() && JAISound_tryDie_())
    {
        return false;
    }
#line 232
    JUT_ASSERT(status_.isAlive());
    bool isPlaying = status_.isPlaying();
    if (isPlaying)
    {
        count_++;
    }
    bool isPaused = status_.isPaused();
    if (isPaused == false)
    {
        fader_.calc();
    }
    bool playing = isPlaying && isPaused == false;
    if (playing)
    {
        params_.mMove.calc();
        if (audible_ != NULL)
        {
            audible_->calc();
        }
        if (status_._1.flags.flag2 != 0)
        {
            if (lifeTime_ == 0)
            {
                stop_JAISound_();
            }
            else
            {
                lifeTime_--;
            }
        }
    }
    if (audience_ != NULL && audible_ != NULL)
    {
        u32 priority = audience_->calcPriority(audible_);
        priority_ = priority;
        if (priority == 0xFFFFFFFF && status_._1.flags.flag1 == 0)
        {
            stop_JAISound_();
        }
    }
    else
    {
        priority_ = 0;
    }

    return playing;
}

void JAISound::initTrack_JAISound_(JASTrack *track)
{
#line 288
    JUT_ASSERT(audience_);
    JUT_ASSERT(audible_);
    JASSoundParams *soundParams[8];
    int numChannels = 0;
    JUT_ASSERT(audience_->getMaxChannels() < JASTrack::TChannelMgr::CHANNEL_MAX);

    for (int i = 0; i < audience_->getMaxChannels(); i++)
    {
        JASSoundParams *currentParams = audible_->getOuterParams(i);
        if (currentParams != NULL)
            soundParams[numChannels++] = currentParams;
    }
    JUT_ASSERT(numChannels >= 1);
    track->setChannelMgrCount(numChannels);
    for (size_t i = 0; i < track->getChannelMgrCount(); i++)
    {
        track->assignExtBuffer(i, soundParams[i]);
    }
}

#include "JSystem/JAudio/JASFakeMatch.h"

#ifdef DEBUG
#pragma sym off
#endif
