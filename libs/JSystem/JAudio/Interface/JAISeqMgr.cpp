#include "JSystem/JAudio/Interface/JAISeqMgr.h"
#include "JSystem/JAudio/Interface/JAISeq.h"

// once again .text sections don't match for debug, however it still works

#ifdef DEBUG
#pragma sym on 
#endif

bool JAISeqMgr::isUsingSeqData(const JAISeqDataRegion &dataRegion)
{
    for (JSULink<JAISeq> *link = mSeqList.getFirst(); link != NULL; link = link->getNext())
    {
        if (dataRegion.intersects(link->getObject()->getSeqData()))
        {
            return true;
        }
    }
    return false;
}

int JAISeqMgr::releaseSeqData(const JAISeqDataRegion &dataRegion)
{
    bool usingSeq = false;
    for (JSULink<JAISeq> *link = mSeqList.getFirst(); link != NULL; link = link->getNext())
    {
        if (dataRegion.intersects(link->getObject()->getSeqData()))
        {
            link->getObject()->stop();
            usingSeq = true;
        }
    }

    ReleaseSeqResult res;
    if (!usingSeq)
    {
        res = RELEASE_SEQ_2;
    }
    else
    {
        res = RELEASE_SEQ_1;
    }
    return res;
}

JAISeqMgr::JAISeqMgr(bool p1) : JASGlobalInstance<JAISeqMgr>(p1), mAudience(NULL)
{
    seqDataMgr_ = NULL;
    strategyMgr_ = NULL;
    _70 = 16;
    mMove.init();
    mActivity.init();
}

void JAISeqMgr::freeDeadSeq_()
{
    JSULink<JAISeq> *link;
    JSULink<JAISeq> *next;
    for (link = mSeqList.getFirst(); link; link = next)
    {
        JAISeq *seq = link->getObject();
        next = link->getNext();
        if (seq->status_.isDead())
        {
            mSeqList.remove(link);
            delete seq;
        }
    }
}

bool JAISeqMgr::startSound(JAISoundID soundID, JAISoundHandle *handle,
                           const JGeometry::TVec3f *pos)
{
    if (handle != NULL && *handle)
    {
        (*handle)->stop();
    }
    JAISoundInfo *soundInfo = JASGlobalInstance<JAISoundInfo>::getInstance();
    int category = (soundInfo != NULL) ? soundInfo->getCategory(soundID) : -1;
    JAISeqData aJStack_38(NULL, 0);
#line 81
    JUT_ASSERT(seqDataMgr_);
    if (seqDataMgr_->getSeqData(soundID, &aJStack_38) == JAI_ASYNC_RESULT_0)
    {
        return false;
    }
    JAISeq *jaiSeq = beginStartSeq_();
    if (jaiSeq != NULL)
    {
        jaiSeq->JAISeqMgr_startID_(soundID, pos, mAudience, category, _70);
        if (endStartSeq_(jaiSeq, handle) != 0)
        {
            if (soundInfo != NULL)
            {
                soundInfo->getSeqInfo(soundID, jaiSeq);
            }
            return true;
        }
    }

    return false;
}

void JAISeqMgr::calc()
{
    mMove.calc();
    for (JSULink<JAISeq> *link = mSeqList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->JAISeqMgr_calc_();
    }
    freeDeadSeq_();
}

void JAISeqMgr::stop()
{
    for (JSULink<JAISeq> *link = mSeqList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->stop();
    }
}

void JAISeqMgr::stop(u32 fadeCount)
{
    for (JSULink<JAISeq> *link = mSeqList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->stop(fadeCount);
    }
}

void JAISeqMgr::stopSoundID(JAISoundID soundID)
{
    if (soundID.isAnonymous())
        return;

    for (JSULink<JAISeq> *link = mSeqList.getFirst(); link != NULL; link = link->getNext())
    {
        if ((u32)link->getObject()->getID() == (u32)soundID)
        {
            link->getObject()->stop();
        }
    }
}

void JAISeqMgr::mixOut()
{
    for (JSULink<JAISeq> *link = mSeqList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->JAISeqMgr_mixOut_(mMove.mParams, mActivity);
    }
}

JAISeq *JAISeqMgr::beginStartSeq_()
{
    JAISeq *seq = new JAISeq(this, strategyMgr_);
    if (seq == NULL)
    {
#line 254
        JUT_WARNING_F2("%s", "JASPoolAllocObject::<JAISeq>::operator new failed .\n");
    }
    return seq;
}

bool JAISeqMgr::endStartSeq_(JAISeq *seq, JAISoundHandle *handle)
{
    JAISeq *sound = seq->getObject();
    if (sound != NULL)
    {
        if (sound->status_.isAlive())
        {
            mSeqList.append(seq);
            if (handle != NULL)
            {
                sound->attachHandle(handle);
            }
            return true;
        }
        delete sound;
    }
    return false;
}

#include "JSystem/JAudio/JASFakeMatch9.h"

#ifdef DEBUG
#pragma sym off
#endif