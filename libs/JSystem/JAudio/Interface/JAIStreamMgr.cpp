#include "JSystem/JAudio/Interface/JAIStreamMgr.h"
#include "JSystem/JAudio/Interface/JAIStreamDataMgr.h"

#include "JSystem/JAudio/System/JASHeapCtrl.h"

#ifdef DEBUG
#pragma sym on
#endif

JAIStreamMgr::JAIStreamMgr(bool p1) : JASGlobalInstance<JAIStreamMgr>(p1)
{
    streamDataMgr_ = NULL;
    streamAramMgr_ = NULL;
    strategyMgr_ = NULL;
    mAudience = NULL;

    mParams.init();
    mActivity.init();
}

void JAIStreamMgr::startStreamDirect(const char *fileName, JAISoundHandle *handle, int category, const JGeometry::TVec3<float> *pos)
{
    // UNUSED
    JUT_ASSERT_F(false, "Cannot open file %s\n")
}

bool JAIStreamMgr::startSound(JAISoundID soundID, JAISoundHandle *handle, const JGeometry::TVec3f *pos)
{
#line 37
    JUT_ASSERT(streamDataMgr_)

    if (handle && *handle)
        (*handle)->stop();

    s32 entryID = streamDataMgr_->getStreamFileEntry(soundID);
    if (entryID < 0)
    {
#line 46
        JUT_WARNING_F2("Cannot find the stream file entry for ID:%08x\n", (u32)soundID);
        return false;
    }

    JAIStream *stream = newStream_();
    JAISoundInfo *soundInfo = JASGlobalInstance<JAISoundInfo>::getInstance();

    int category = -1;
    if (soundInfo)
    {
        category = soundInfo->getCategory(soundID);
    }

    if (!stream)
        return false;

    stream->JAIStreamMgr_startID_(soundID, entryID, pos, mAudience, category);
    if (soundInfo)
    {

        soundInfo->getStreamInfo(soundID, stream);
    }

    if (handle)
    {
        stream->attachHandle(handle);
    }

    return false;
}

void JAIStreamMgr::freeDeadStream_()
{
    for (JSULink<JAIStream> *it = mStreamList.getFirst(); it != NULL;)
    {
        JAIStream *stream = it->getObject();
        JSULink<JAIStream> *next = it->getNext();
        if (stream->status_.isDead())
        {
            mStreamList.remove(it);
            u32 aramAddr = stream->JAIStreamMgr_getAramAddr_();
            if (aramAddr)
            {
                u8 result = streamAramMgr_->deleteStreamAram(aramAddr);
#line 105
                JUT_ASSERT(result)
            }
            delete stream;
        }
        it = next;
    }
}

void JAIStreamMgr::calc()
{
    mParams.calc();
    for (JSULink<JAIStream> *it = mStreamList.getFirst(); it != NULL; it = it->getNext())
        it->getObject()->JAIStreamMgr_calc_();

    freeDeadStream_();
}

void JAIStreamMgr::stop(u32 fadeCount)
{
    for (JSULink<JAIStream> *it = mStreamList.getFirst(); it != NULL; it = it->getNext())
        it->getObject()->stop(fadeCount);
}

void JAIStreamMgr::mixOut()
{
    for (JSULink<JAIStream> *it = mStreamList.getFirst(); it != NULL; it = it->getNext())
        it->getObject()->JAIStreamMgr_mixOut_(mParams, mActivity);
}

JAIStream *JAIStreamMgr::newStream_()
{
    if (streamAramMgr_ == NULL)
    {
#line 229
        JUT_WARNING_F2("%s", "JAIStreamAramMgr must be set.\n");
        return NULL;
    }

    JAIStream *stream = new JAIStream(this, strategyMgr_);
    if (stream == NULL)
    {
#line 235
        JUT_WARNING_F2("%s", "JASPoolAllocObject::<JAIStream>::operator new failed .\n")
        return NULL;
    }
    mStreamList.append(stream);

    return stream;
}

#ifdef DEBUG
CW_FORCE_STRINGS(JAIStreamMgr_cpp, "category >= 0"); // string from JAIStreamMgr::stopCategory(int)
#endif

#include "JSystem/JAudio/JASFakeMatch10.h"

#ifdef DEBUG
#pragma sym off
#endif
