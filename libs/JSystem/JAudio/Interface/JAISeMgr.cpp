
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASReport.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"

DECL_WEAK JAISoundInfo *JASGlobalInstance<JAISoundInfo>::sInstance;

// sym on is trolling me here(no match on debug)

bool JAISeCategoryMgr::isUsingSeqData(const JAISeqDataRegion &dataRegion)
{
    JSULink<JAISe> *link = mSeList.getFirst();
    while (link != NULL)
    {
        JAISe *obj = link->getObject();
        if (dataRegion.intersects(*obj->getSeqData()))
        {
            return true;
        }
        link = link->getNext();
    }

    return false;
}

int JAISeCategoryMgr::releaseSeqData(const JAISeqDataRegion &dataRegion)
{
    bool usingSeq = false;
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        if (dataRegion.intersects(*link->getObject()->getSeqData()))
        {
            link->getObject()->stop();
            usingSeq = true;
        }
    }
    // Not matching on debug(is this a ternaty + enum?)
    int rv;
    if (usingSeq)
    {
        rv = 1;
    }
    else {
        rv = 2;
    }
    return rv;
}

void JAISeCategoryMgr::JAISeMgr_calc_()
{
    mParams.calc();

    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->JAISeCategoryMgr_calc_();
    }
    sortByPriority_();
}

void JAISeCategoryMgr::JAISeMgr_calcAudibleSounds_()
{
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        if (link->getObject()->audible_)
            link->getObject()->JAISeCategoryMgr_calc_();
    }
}

void JAISeCategoryMgr::JAISeMgr_freeDeadSe_()
{
    JSULink<JAISe> *i;
    JSULink<JAISe> *next;
    for (i = mSeList.getFirst(); i != NULL; i = next)
    {
        next = i->getNext();
        if (i->getObject()->status_.isDead())
        {
#line 71
            JUT_ASSERT(! i->getObject() ->isHandleAttached())
            mSeList.remove(i);
            delete i->getObject();
        }
    }
}

bool JAISeCategoryMgr::JAISeMgr_acceptsNewSe_(u32 prio) const
{
    int maxSe = getMaxSe();
    if (maxSe <= 0)
    {
        return true;
    }
    int stopCount = 0;
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        if (prio < link->getObject()->JAISeCategoryMgr_getProperPriority_())
        {
            return true;
        }
        if (link->getObject()->isStopping())
            continue;
        
        stopCount++;
    }
    if (stopCount < maxSe)
        return true;
    return false;
}

void JAISeCategoryMgr::sortByPriority_()
{
    for (u32 i = mSeList.getNumLinks(); i != 0; i--)
    {
        JSULink<JAISe> *link = mSeList.getFirst();
        u32 lowestPriority = link->getObject()->JAISeCategoryMgr_getPriority_();
        JSULink<JAISe> *current = link->getNext();
        for (int j = 1; j < i; j++)
        {
            u32 itemPriority = current->getObject()->JAISeCategoryMgr_getPriority_();
            if (itemPriority < lowestPriority)
            {
                lowestPriority = itemPriority;
                link = current;
            }
            current = current->getNext();
        }
#line 130
        bool result = mSeList.remove(link);
        JUT_ASSERT(result);
        result = mSeList.append(link);
        JUT_ASSERT(result);
    }
}

int JAISeCategoryMgr::getNumAudibleSe() const {
    int cnt = 0;
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        if (link->getObject()->audible_)
            cnt++;
    }
    return cnt;
}

void JAISeCategoryMgr::stop(u32 fadeCount)
{
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->stop(fadeCount);
    }
}

void JAISeCategoryMgr::stop()
{
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->stop();
    }
}

void JAISeCategoryMgr::stopAudibleSounds()
{
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        if (link->getObject()->audible_)
            link->getObject()->stop();
    }
}

void JAISeCategoryMgr::stopSoundID(JAISoundID soundID)
{
    if (!soundID.isAnonymous())
    {
        for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
        {
            if ((u32)link->getObject()->getID() == (u32)soundID)
            {
                link->getObject()->stop();
            }
        }
    }
}

void JAISeCategoryMgr::pause(bool doPause)
{
    for (JSULink<JAISe> *link = mSeList.getFirst(); link != NULL; link = link->getNext())
    {
        link->getObject()->pause(doPause);
    }
}

void JAISeCategoryMgr::JAISeMgr_mixOut_(const JAISoundParamsMove &moveParams,
                                        JAISoundActivity activity)
{
    JASSoundParams params;
    params.combine(moveParams.mParams, mParams.mParams);
    JSULink<JAISe> *it = mSeList.getFirst();
    int activeCount = getMaxActiveSe();

    if (activeCount > 0)
    {
        for (; it != NULL && activeCount > 0 && !it->getObject()->isFarAway(); it = it->getNext(), activeCount--)
        {
            it->getObject()->JAISeCategoryMgr_mixOut_(true, params, activity);
        }
        for (; it != NULL; it = it->getNext())
        {
            it->getObject()->JAISeCategoryMgr_mixOut_(false, params, activity);
        }
    }
    else
    {
        for (; it != NULL && !it->getObject()->isFarAway(); it = it->getNext())
        {
            it->getObject()->JAISeCategoryMgr_mixOut_(true, params, activity);
        }
        for (; it != NULL; it = it->getNext())
        {
            it->getObject()->JAISeCategoryMgr_mixOut_(false, params, activity);
        }
    }
}

JAISeMgr::JAISeMgr(bool param_0) : JASGlobalInstance<JAISeMgr>(param_0)
{
    mAudience = NULL;
    mSeqDataMgr = NULL;
    mStrategyMgr = NULL;
    mParams.init();
    mActivity.init();
}

bool JAISeMgr::isUsingSeqData(JAISeqDataRegion const &param_0)
{
    for (int i = 0; i < 16; i++)
    {
        if (mCategoryMgrs[i].JAISeCategoryMgr::isUsingSeqData(param_0))
        {
            return true;
        }
    }
    return false;
}

int JAISeMgr::releaseSeqData(JAISeqDataRegion const &param_0)
{
    u8 r30 = 0;
    for (int i = 0; i < 16; i++)
    {
        switch (mCategoryMgrs[i].JAISeCategoryMgr::releaseSeqData(param_0))
        {
        case 0:
            return 0;
        case 1:
            r30 = 1;
            break;
        }
    }
    return (r30 != 0) ? 1 : 0;
}

void JAISeMgr::setCategoryArrangement(const JAISeCategoryArrangement &catArrangement)
{
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].setMaxActiveSe(catArrangement.mItems[i].mMaxActiveSe);
        mCategoryMgrs[i].setMaxInactiveSe(catArrangement.mItems[i].mMaxInactiveSe);
    }
}

void JAISeMgr::getCategoryArrangement(JAISeCategoryArrangement *out)
{
    // fabricated
    for (int i = 0; i < 16; i++)
    {
        int active =  mCategoryMgrs[i].getMaxActiveSe();
        int inactive = mCategoryMgrs[i].getMaxInactiveSe();

        JUT_ASSERT(active <= 255)
        JUT_ASSERT(inactive <= 255)

        out->mItems[i].mMaxActiveSe = active;
        out->mItems[i].mMaxInactiveSe = inactive;
    }
}

void JAISeMgr::stop()
{
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].stop();
    }
}

void JAISeMgr::stopAudibleSoundsSync()
{
    stopAudibleSounds();

    while (getNumAudibleSe() > 0)
    {
        calcAudibleSounds();
        if (getNumAudibleSe() == 0)
            break;
        mixOut();
        JASDriver::waitSubFrame();
    }
}

void JAISeMgr::stopAudibleSounds()
{
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].stopAudibleSounds();
    }
}

void JAISeMgr::stopSoundID(JAISoundID param_0)
{
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].stopSoundID(param_0);
    }
}

void JAISeMgr::initParams()
{
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].getParams()->init();
    }
}

void JAISeMgr::setAudience(JAIAudience *audience)
{
#line 347
    JUT_ASSERT(! isActive());
    mAudience = audience;
}

void JAISeMgr::setSeqDataMgr(JAISeqDataMgr *dataMgr)
{
#line 361
    JUT_ASSERT(! isActive());
    resetSeqDataMgr();
    mSeqDataMgr = dataMgr;
    mSeqDataMgr->setSeqDataUser(this);
}

void JAISeMgr::resetSeqDataMgr()
{
    if (mSeqDataMgr)
    {
        mSeqDataMgr->setSeqDataUser(NULL);
        mSeqDataMgr = NULL;
    }
}

JAISe *JAISeMgr::newSe_(int category, u32 prio)
{

    JUT_ASSERT(category < NUM_CATEGORIES)
    if (category < 0)
        category = 0;

    JAISeCategoryMgr &categoryMgr = mCategoryMgrs[category];

    if (!categoryMgr.JAISeMgr_acceptsNewSe_(prio))
    {
        return NULL;
    }

    JAISe *ret = new JAISe(this, mStrategyMgr, prio);
    if(ret == NULL) 
    {
        JUT_WARNING_F2("%s", "JASPoolAllocObject::<JAISe>::operator new failed .\n")
        return NULL;
    }

    categoryMgr.JAISeMgr_appendSe_(ret);
    return ret;
}

void JAISeMgr::calc()
{
    mParams.calc();
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].JAISeMgr_calc_();
    }
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].JAISeMgr_freeDeadSe_();
    }
}

void JAISeMgr::calcAudibleSounds()
{
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].JAISeMgr_calcAudibleSounds_();
        mCategoryMgrs[i].sortByPriority_();
    }
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].JAISeMgr_freeDeadSe_();
    }
}

void JAISeMgr::mixOut()
{
    for (int i = 0; i < 16; i++)
    {
        mCategoryMgrs[i].JAISeMgr_mixOut_(mParams, mActivity);
    }
}

bool JAISeMgr::startSound(JAISoundID soundID, JAISoundHandle *handle,
                          const JGeometry::TVec3f *param_2)
{
    if (handle && *handle)
    {
        (*handle)->stop();
    }

    JAISoundInfo *pSoundInfo = JASGlobalInstance<JAISoundInfo>::getInstance();
   
    int categoryIndex;
    u32 prio;
    if (pSoundInfo != NULL)
    {
        categoryIndex = pSoundInfo->getCategory(soundID);
        prio = pSoundInfo->getPriority(soundID);
    }
    else
    {
        categoryIndex = soundID.mId.mBytes.b1;
        prio = 0;
    }
    JAISe *pJAISe = newSe_(categoryIndex, prio);
    if (pJAISe == NULL)
    {
        JASReport("cannot new Se %08x.", (u32)soundID);
        return false;
    }
    JAIAudience *pAudience = getAudience(categoryIndex);
    pJAISe->JAISeMgr_startID_(soundID, param_2, pAudience);
    if (pSoundInfo != NULL)
    {
        pSoundInfo->getSeInfo(soundID, pJAISe);
    }
    if (handle != NULL)
    {
        pJAISe->attachHandle(handle);
    }
    return true;
}

int JAISeMgr::getNumActiveSe() const
{
    int sum = 0;
    for (int i = 0; i < 16; i++)
    {
        sum += mCategoryMgrs[i].getNumSe();
    }
    return sum;
}

int JAISeMgr::getNumAudibleSe() const
{
    int sum = 0;
    for (int i = 0; i < 16; i++)
    {
        sum += mCategoryMgrs[i].getNumAudibleSe();
    }
    return sum;
}

#include "JSystem/JAudio/JASFakeMatch2.h"
