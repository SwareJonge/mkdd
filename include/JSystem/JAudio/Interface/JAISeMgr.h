#ifndef JAUDIO_JAISEMGR_H
#define JAUDIO_JAISEMGR_H

#include "JSystem/JAudio/Interface/JAISe.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JSupport/JSUList.h"

struct JAIAudience;

struct JASNonCopyable
{
    JAIAudience *mAudience;
}; // Size: 0x4

struct JAISeCategoryArrangementItem
{
    u8 mMaxActiveSe;
    u8 mMaxInactiveSe;
};

struct JAISeCategoryArrangement
{
    JAISeCategoryArrangementItem mItems[16];
};
// might have it's own header
class JAISeCategoryMgr : public JAISeqDataUser, JASNonCopyable
{
public:
    JAISeCategoryMgr()
    {
        mParams.init();
        mMaxActiveSe = 0;
        mMaxInactiveSe = 0;
        mAudience = nullptr;
    }
    void JAISeMgr_calc_();
    void JAISeMgr_calcAudibleSounds_();
    void JAISeMgr_freeDeadSe_();
    bool JAISeMgr_acceptsNewSe_(u32 prio) const;
    void sortByPriority_();
    void stop(u32 fadeCount);
    void stop();
    void stopAudibleSounds();
    void stopSoundID(JAISoundID soundID);
    void pause(bool doPause);
    void JAISeMgr_mixOut_(const JAISoundParamsMove &moveParams, JAISoundActivity activity);
    int getNumAudibleSe() const;

    //virtual ~JAISeCategoryMgr() {}
    virtual bool isUsingSeqData(const JAISeqDataRegion &region);
    virtual JAISeqDataResult releaseSeqData(const JAISeqDataRegion &region);

    void JAISeMgr_appendSe_(JAISe *se) { mSeList.append(se); }

    JAISoundParamsMove *getParams() { return &mParams; }
    int getMaxSe() const
    {
        return (mMaxActiveSe == 0) ? 0 : mMaxActiveSe + mMaxInactiveSe;
    }
    int getMaxActiveSe() const { return mMaxActiveSe; }
    void setMaxActiveSe(int se) { mMaxActiveSe = se; }
    void setMaxInactiveSe(int se) { mMaxInactiveSe = se; }
    int getMaxInactiveSe() const { return mMaxInactiveSe; }
    JSUList<JAISe> *getSeList() { return &mSeList; }
    int getNumSe() const { return mSeList.getNumLinks(); }
    JAIAudience *getAudience() { return mAudience; }

    JAISoundParamsMove mParams; // 08
    JSUList<JAISe> mSeList;     // 58
    int mMaxInactiveSe;         // 64
    int mMaxActiveSe;           // 68


}; // Size: 0x6C

class JAISeMgr : public JASGlobalInstance<JAISeMgr>, public JAISeqDataUser
{
public:
    JAISeMgr(bool);
    void setCategoryArrangement(const JAISeCategoryArrangement &catArrangement);
    void getCategoryArrangement(JAISeCategoryArrangement *catArrangement);
    void stop();
    void stopAudibleSoundsSync();
    void stopAudibleSounds();
    void stopSoundID(JAISoundID soundID);
    void initParams();
    void setAudience(JAIAudience *audience);
    void setSeqDataMgr(JAISeqDataMgr *seqDataMgr);
    void resetSeqDataMgr();
    JAISe *newSe_(int category, u32 prio);
    void calc();
    void calcAudibleSounds();
    void mixOut();
    bool startSound(JAISoundID soundID, JAISoundHandle *handle, const JGeometry::TVec3f *pos);
    int getNumActiveSe() const;
    int getNumAudibleSe() const;

    static const s32 NUM_CATEGORIES = 16;

    
    virtual bool isUsingSeqData(const JAISeqDataRegion &region);
    virtual JAISeqDataResult releaseSeqData(const JAISeqDataRegion &region);
    virtual ~JAISeMgr() {}; // weak
    JAISeCategoryMgr *getCategory(int categoryIndex)
    {
#line 216
        JUT_ASSERT(categoryIndex >= 0)
        JUT_ASSERT(categoryIndex < NUM_CATEGORIES)
        return &mCategoryMgrs[categoryIndex];
    }
    JAIAudience *getAudience(int categoryIndex)
    {
        if (categoryIndex >= 0 && categoryIndex < NUM_CATEGORIES)
        {
            JAIAudience *rv = mCategoryMgrs[categoryIndex].getAudience();
            if (rv)
                return rv;
        }
        return mAudience;
    }
    JAIAudience *getAudience() { return mAudience; }
    JAISeqDataMgr *getSeqDataMgr() { return mSeqDataMgr; }
    JAISoundParamsMove *getParams() { return &mParams; }
    bool isActive() const { return getNumActiveSe() > 0; }

    static JAISeMgr *getInstance() { return sInstance; }
private:
    JAISoundActivity mActivity;                     // 004
    JAIAudience *mAudience;                         // 008
    JAISeqDataMgr *mSeqDataMgr;                     // 00C
    JAISoundStrategyMgr<JAISe> *mStrategyMgr;       // 010
    JAISeCategoryMgr mCategoryMgrs[NUM_CATEGORIES]; // 014
    JAISoundParamsMove mParams;                     // 6D4

}; // Size: 0x724

#endif /* JAISEMGR_H */
