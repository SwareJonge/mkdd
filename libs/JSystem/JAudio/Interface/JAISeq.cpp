#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAISeqMgr.h"
#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"

// File originally had sym on but this messes stuff up even more
// Don't auto reformat

namespace
{
    bool JASTrack_isFreeOrStopped(JASTrack *track)
    {
        u32 status = track->getStatus();
        return status == JASTrack::STATUS_FREE || status == JASTrack::STATUS_STOPPED;
    }
}

JAISeq::JAISeq(JAISeqMgr *seqMgr, JAISoundStrategyMgr<JAISeq> *strategyMgr) : JSULink<JAISeq>(this)
{
    inner_.seqMgr = seqMgr;
    inner_.strategyMgr = strategyMgr;
    inner_.mSoundParams.init();
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        inner_.mSoundChild[i] = NULL;
    }
}

void JAISeq::JAISeqMgr_startID_(JAISoundID soundID, const JGeometry::TVec3f *pos, JAIAudience *audience, int category, int p5)
{
    inner_.mCategory = category;
    reserveChildTracks_(p5);
    start_JAISound_(soundID, pos, audience);
    if (inner_.strategyMgr)
    {
        _3a8 = inner_.strategyMgr->calc(soundID);
    }
    else
    {
        _3a8 = NULL;
    }
}

void JAISeq::playSeqData_(const JASSoundParams &params, JAISoundActivity activity)
{
#line 72
    JUT_ASSERT(JASTrack_isFreeOrStopped( & inner_.outputTrack ))
    inner_.outputTrack.setSeqData(inner_.mSeqData._0, inner_.mSeqData._4);
    if (audible_)
    {
        initTrack_JAISound_(&inner_.outputTrack);
    }
    else
    {
        inner_.outputTrack.setChannelMgrCount(1);
        inner_.outputTrack.assignExtBuffer(0, &inner_.mSoundParams);
    }
    mixOut_(params, activity);
    inner_.outputTrack.startSeq();
}

void JAISeq::reserveChildTracks_(int count)
{
#line 92
    JUT_ASSERT(inner_.outputTrack.getStatus() == JASTrack::STATUS_FREE)
    for (int i = 0; i < 2; i++)
    {
        JASTrack *track = new JASTrack();
        if (track)
        {
            inner_.outputTrack.connectChild(i, track);
            for (int j = 0; j < 16; j++)
            {
                if (i * 16 + j >= count)
                    continue;
                
                JASTrack *track2 = new JASTrack();
                if (track2)
                {
                    track->connectChild(j, track2);
                }
                else 
                {
                    JUT_WARNING_F2("%s", "JASPoolAllocObject::<JASTrack>::operator new failed .\n")
                }
            }
        }
        else
        {
#line 118
            JUT_WARNING_F2("%s", "JASPoolAllocObject::<JASTrack>::operator new failed .\n")
        }
    }
}

void JAISeq::releaseChildTracks_()
{
    for (u32 i = 0; i < 2; i++)
    {
        JASTrack *track = inner_.outputTrack.getChild(i);
        if (track)
        {
            for (u32 j = 0; j < 16; j++)
            {
                JASTrack *track2 = track->getChild(j);
                if (track2)
                {
                    delete track2;
                }
            }
            delete track;
        }
    }
}

bool JAISeq::prepare_getSeqData_()
{
    if (soundID_.isAnonymous())
    {
        return true;
    }
    JAISeqDataMgr *seqDataMgr = inner_.seqMgr->getSeqDataMgr();
#line 156
    JUT_ASSERT(seqDataMgr)
    switch (seqDataMgr->getSeqData(soundID_, &inner_.mSeqData))
    {
    case 0:
        JUT_REPORT_MSG("Failed In Loading Sequence Data:SEQ(0x%08x)\n", (u32)soundID_);
        status_.state.unk = 2;
        break;
    case 1:
        increasePrepareCount_JAISound_();
        return false;
    case 2:
        return true;
    }
    return false;
}

bool JAISeq::prepare_(const JASSoundParams &params, JAISoundActivity activity)
{
    switch (status_.state.unk)
    {
    case 6:
#line 187
        JUT_ASSERT(false)
        break;
    case 2:
        stop_JAISound_();
        break;
    case 0:
        if (prepare_getSeqData_())
        {
            status_.state.unk = 5;
            playSeqData_(params, activity);
            return true;
        }
        break;
    case 1:
        if (prepare_getSeqData_())
        {
            status_.state.unk = 3;
        }
        return false;
    case 4:
        status_.state.unk = 5;
        playSeqData_(params, activity);
        return true;
    case 5:
        return true;
    }
    return false;
}

void JAISeq::JAISeqMgr_calc_()
{
    if (inner_.outputTrack.getStatus() == 2 || (inner_.outputTrack.getStatus() == 0 && status_.state.unk == 2))
    {
        die_();
        return;
    }
    if (calc_JAISound_())
    {
        inner_.mTempoMgr.calc();
        for (int i = 0; i < 32; i++)
        {
            if (inner_.mSoundChild[i])
            {
                inner_.mSoundChild[i]->calc();
            }
        }
        if (_3a8)
        {
            _3a8->calc(this);
        }
    }
}

void JAISeq::die_()
{
#line 263
    JUT_ASSERT(JASTrack_isFreeOrStopped( & inner_.outputTrack ))
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (inner_.mSoundChild[i])
        {
            delete inner_.mSoundChild[i];
            inner_.mSoundChild[i] = NULL;
        }
    }

    if (_3a8)
    {
        JUT_ASSERT(inner_.strategyMgr)
        inner_.strategyMgr->virtual4(_3a8);
        _3a8 = NULL;
    }
    releaseChildTracks_();
    die_JAISound_();
}

bool JAISeq::JAISound_tryDie_()
{
    if (JASTrack_isFreeOrStopped(&inner_.outputTrack))
    {
        die_();
        return true;
    }
    fader_.forceOut();
    status_.state.flags.flag1 = 1;
    JASCriticalSection cs;
    inner_.outputTrack.stopSeq();
    return false;
}

void JAISeq::mixOut_(const JASSoundParams &params, JAISoundActivity activity)
{
    inner_.outputTrack.setTempoRate(inner_.mTempoMgr.getTempo());
    inner_.outputTrack.mute(activity._0.flags.flag1 || status_.isMute());
    inner_.outputTrack.pause(activity._0.flags.flag2 || status_.isPaused());
    JASSoundParams stackParams;
    params_.mixOutAll(params, &stackParams, fader_.getIntensity());
    if (_3a8)
    {
        _3a8->virtual4(this, stackParams);
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
        for (int i = 0; i < 32; i++)
        {
            if (inner_.mSoundChild[i])
            {
                JASTrack *track = JAISeq::getChildTrack(i);
                if (track)
                {
                    inner_.mSoundChild[i]->mixOut(track);
                }
            }
        }
    }
}

void JAISeq::JAISeqMgr_mixOut_(const JASSoundParams &params, JAISoundActivity activity)
{
    if (prepare_(params, activity) && inner_.outputTrack.getStatus() == 1)
    {
        mixOut_(params, activity);
    }
}

s32 JAISeq::getNumChild() const
{
    return NUM_CHILDREN;
}

JAISoundChild *JAISeq::getChild(int index)
{
#line 365
    JUT_ASSERT(status_.isAlive());
    JUT_ASSERT(index >= 0);
    JUT_ASSERT(index < NUM_CHILDREN);
    if (inner_.mSoundChild[index])
        return inner_.mSoundChild[index];
    inner_.mSoundChild[index] = new JAISoundChild();
    if (inner_.mSoundChild[index] == NULL)
    {
        JUT_WARNING_F2("%s", "JASPoolAllocObject::<JAISoundChild>::operator new failed .\n")
        return NULL;
    }
    return inner_.mSoundChild[index];
}

void JAISeq::releaseChild(int index)
{
    if (inner_.outputTrack.getStatus() == 1 && inner_.mSoundChild[index])
    {
        JASTrack *track = getChildTrack(index);
        if (track)
        {
            track->assignExtBuffer(0, NULL);
        }
        delete inner_.mSoundChild[index];
        inner_.mSoundChild[index] = NULL;
    }
}

JASTrack *JAISeq::getTrack()
{
    return &inner_.outputTrack;
}

JASTrack *JAISeq::getChildTrack(int index)
{
    u32 childIdx = u32(index) >> 4;
    JASTrack *track = inner_.outputTrack.getChild(childIdx);
    if (track)
    {
        JASTrack *track2 = track->getChild(index - (childIdx << 4));
        return track2;
    }
    return NULL;
}

JAISeq *JAISeq::asSeq()
{
    return this;
}

JAITempoMgr *JAISeq::getTempoMgr()
{
    return &inner_.mTempoMgr;
}

#include "JSystem/JAudio/JASFakeMatch6.h"

