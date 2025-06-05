#include "JSystem/JAudio/Interface/JAISe.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/System/JASTrack.h"

// sym not correct here

JAISe::JAISe(JAISeMgr *seMgr, JAISoundStrategyMgr<JAISe> *strategyMgr, u32 prio) : JSULink<JAISe>(this)
{
    inner_.mSoundStrategyMgr = strategyMgr;
    inner_.seMgr = seMgr;
    inner_.mProperPriority = prio;
}

void JAISe::mixOut_(const JASSoundParams &params)
{
    inner_.track.setTempoRate(inner_.mTempoMgr.getTempo());
    JASSoundParams stackParams;
    params_.mixOutAll(params, &stackParams, fader_.getIntensity());
    if (inner_._278)
    {
        inner_._278->virtual4(this, stackParams);
    }
    if (audible_)
    {
        int maxChannels = audience_->getMaxChannels();
        for (int i = 0; i < maxChannels; i++)
        {
            audience_->mixChannelOut(stackParams, audible_, i);
        }
    }
    else
    {
        inner_.mSoundParams = stackParams;
    }
}

void JAISe::stopTrack_()
{
    switch (inner_.track.getStatus())
    {
    case 0:
        break;
    case 1:
        inner_.track.stopSeq();
        break;
    case 2:
        break;
    }
}

void JAISe::startTrack_(const JASSoundParams &params)
{
#line 72
    JUT_ASSERT(inner_.track.getStatus() != JASTrack::STATUS_RUN)
    if (inner_.track.getStatus() == JASTrack::STATUS_STOPPED)
    {
        inner_.track.init();
    }
    if (audible_)
    {
        initTrack_JAISound_(&inner_.track);
    }
    else
    {
        params_.mixOutAll(params, &inner_.mSoundParams, fader_.getIntensity());
        inner_.track.setChannelMgrCount(1);
        inner_.track.assignExtBuffer(0, &inner_.mSoundParams);
    }
    inner_._26c = 1;
    inner_.track.setSeqData(inner_.mSeqData._0, inner_.mSeqData._4);
    inner_.track.startSeq();
}

void JAISe::JAISeCategoryMgr_mixOut_(bool p1, const JASSoundParams &params, JAISoundActivity activity)
{
    mixOut_(params);
    if (isStopping())
    {
        stopTrack_();
    }
    else if (p1)
    {
        if (status_.isPlaying())
        {
            if (inner_._26c)
            {
                
                switch (inner_.track.getStatus())
                {
      
                case JASTrack::STATUS_STOPPED:
                    if (status_._1.flags.flag1)
                    {
                        startTrack_(params);
                    }
                    else
                    {
                        stop_JAISound_();
                    }
                    break;
                case JASTrack::STATUS_RUN:
                    inner_.track.pause(status_.isPaused() || activity._0.flags.flag2);
                    inner_.track.mute(status_.isMute() || activity._0.flags.flag1);
                    break;
                }
            }
            else
            {
                startTrack_(params);
            }
        }
    }
    else if (status_._1.flags.flag1)
    {
        if (status_._1.flags.flag3)
        {
            inner_.track.pause(true);
        }
        else
        {
            stopTrack_();
        }
    }
    else
    {
        stopTrack_();
        stop_JAISound_();
    }
}

void JAISe::JAISeCategoryMgr_calc_()
{
    prepare_();
    if (calc_JAISound_())
    {
        inner_.mTempoMgr.calc();
        if (inner_._278)
        {
            inner_._278->calc(this);
        }
    }
    if (isStopping() || priority_ == -1)
    {
        inner_.mPriority = -1;
    }
    else
    {
        inner_.mPriority = inner_.mProperPriority + priority_;
    }
}

bool JAISe::JAISound_tryDie_()
{
    switch (inner_.track.getStatus())
    {
    case 0:
    case 2:
        if (inner_._278)
        {
            inner_.mSoundStrategyMgr->virtual4(inner_._278);
            inner_._278 = NULL;
        }
        die_JAISound_();
        return true;
    case 1:
        break;
    }
    return false;
}

void JAISe::JAISeMgr_startID_(JAISoundID soundID, const JGeometry::TVec3<f32> *pos, JAIAudience *audience)
{
#line 221
    JUT_ASSERT(inner_.track.getStatus() == JASTrack::STATUS_FREE)
    start_JAISound_(soundID, pos, audience);
    inner_._26c = 0;
    if (inner_.mSoundStrategyMgr)
    {
        inner_._278 = inner_.mSoundStrategyMgr->calc(soundID);
    }
    else
    {
        inner_._278 = NULL;
    }
}

s32 JAISe::getNumChild() const
{
    return 0;
}

JAISoundChild *JAISe::getChild(int)
{
    return NULL;
}

void JAISe::releaseChild(int)
{
    /* empty function */
}

bool JAISe::prepare_getSeqData_()
{
#line 271
    JUT_ASSERT(inner_.track.getStatus() != JASTrack::STATUS_RUN);
    JUT_ASSERT(inner_.seMgr);
    JUT_ASSERT( inner_.seMgr ->getSeqDataMgr());
    switch (inner_.seMgr->getSeqDataMgr()->getSeqData(soundID_, &inner_.mSeqData))
    {
    case 0:
        JUT_REPORT_MSG("Failed In Loading Sequence Data:SE(0x%08x)\n", (u32)soundID_);
        status_.state.unk = 2;
        return false;
    case 1:
        increasePrepareCount_JAISound_();
        return false;
    case 2:
        return true;
    default:
#line 296
        JUT_ASSERT(false);
        return false;
    }
}

void JAISe::prepare_()
{
    switch (status_.state.unk)
    {
    case 6:
        JUT_ASSERT(false);
        break;
    case 2:
        stop_JAISound_();
        break;
    case 0:
        if (prepare_getSeqData_())
        {
            status_.state.unk = 5;
        }
        break;
    case 1:
        if (prepare_getSeqData_())
        {
            status_.state.unk = 3;
        }
        break;
    case 4:
        status_.state.unk = 5;
        break;
    case 3:
    case 5:
        break;
    }
}

JASTrack *JAISe::getTrack()
{
    return &inner_.track;
}

JASTrack *JAISe::getChildTrack(int index)
{
    return inner_.track.getChild(index);
}

JAISe *JAISe::asSe()
{
    return this;
}

JAITempoMgr *JAISe::getTempoMgr()
{
    return &inner_.mTempoMgr;
}

#include "JSystem/JAudio/JASFakeMatch2.h"
