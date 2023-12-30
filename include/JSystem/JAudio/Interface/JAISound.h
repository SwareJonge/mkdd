#ifndef JAUDIO_JAISOUND_H
#define JAUDIO_JAISOUND_H

#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JAudio/Interface/JAIAudible.h"
#include "JSystem/JAudio/Interface/JAISoundParams.h"
#include "JSystem/JAudio/System/JASTrack.h"

#include "types.h"

class JAISound;

class JAISoundHandle
{
public:
    JAISoundHandle() { sound_ = NULL; }

    JAISound *getSound()
    {
#line 41
        JUT_ASSERT(sound_ != 0);
        return sound_;
    }

    JAISound *operator->()
    {
#line 58
        JUT_ASSERT(sound_ != 0)
        return sound_;
    }

    operator bool() const { return isSoundAttached(); }

    bool isSoundAttached() const { return sound_ != NULL; }

private:
    JAISound *sound_;
};

class JAISoundID
{
public:
    operator u32() const { return this->mId.mFullId; }
    void operator=(JAISoundID const &other) { mId.mFullId = other.mId.mFullId; };

    JAISoundID(u32 pId) { mId.mFullId = pId; };

    JAISoundID(JAISoundID const &other) { mId = other.mId; };

    JAISoundID() {}

    bool isAnonymous() { return mId.mFullId == 0xffffffff; }
    void setAnonymous() { mId.mFullId = -1; }

    union
    {
        u32 mFullId;
        struct
        {
            u8 b0;
            u8 b1;
            u8 b2;
            u8 b3;
        } mBytes;
        struct
        {
            u16 mSoundType;
            u16 mShortId;
        } mAdvancedId; // Debug doesn't have an inline for referencing the short ID so I assume
                       // it's similar to this
    } mId;
};

struct JAISoundStatus_ {
    void init()
    {
        _0.pack = 0;
        _1.pack = 0;
        state.pack16 = 0;
        user_data = 0;
    }

    bool isAlive() { return state.pack != 6; }
    bool isDead() { return state.pack == 6; }
    bool isPlaying() { return state.pack == 5; }
    bool isMute() { return _0.flags.mute; }
    bool isPaused() { return _0.flags.paused; }
    void pauseWhenOut() { _1.flags.flag6 = 1;}

    union
    {
        struct
        {
            u8 mute : 1;
            u8 paused : 1;
            u8 flag3 : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
        u8 pack;
    } _0;

    union
    {
        struct
        {
            u8 flag1 : 1;
            u8 flag2 : 1;
            u8 flag3 : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
        u8 pack;
    } _1;

    struct
    {
        struct
        {
            u8 flag1 : 1;
            u8 flag2 : 1;
            u8 animationState : 2;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
        u8 pack;
        u16 pack16; // HUH
    } state;
    u32 user_data;
};

struct JAISoundFader
{
    void forceIn()
    {
        mIntensity = 1.0f;
        mTransition.zero();
    }
    void forceOut()
    {
        mIntensity = 0.0f;
        mTransition.zero();
    }
    void fadeIn(u32 fadeCount)
    {
        if (fadeCount == 0)
            forceIn();
        else
            mTransition.set(1.0f, mIntensity, fadeCount);
    }
    void fadeOut(u32 fadeCount)
    {
        if (fadeCount != 0)
            mTransition.set(0.0f, mIntensity, fadeCount);
        else
            forceOut();
    }
    void fadeInFromOut(u32 fadeCount)
    {
        mIntensity = 0.0f;
        fadeIn(fadeCount);
    }
    bool isOut() { return (mTransition.mCount == 0 && mIntensity < 0.01f); }
    void calc() { mIntensity = mTransition.apply(mIntensity); }
    f32 getIntensity() { return mIntensity; }

    f32 mIntensity;
    JAISoundParamsTransition::TTransition mTransition;
}; // Size: 0x10

class JAIAudible;
class JAIAudience;
class JAISe;
class JAISeq;
class JAISoundChild;
class JAIStream;
class JAITempoMgr;

class JAISound
{
public:
    void releaseHandle();
    void attachHandle(JAISoundHandle *);
    JAISound();
    void start_JAISound_(JAISoundID, JGeometry::TVec3f const *, JAIAudience *);
    bool acceptsNewAudible() const;
    void newAudible(JGeometry::TVec3f const &, JGeometry::TVec3f const *, u32,
                                   JAIAudience *);
    void stop();
    void stop(u32 fadeout);
    void die_JAISound_();
    void increasePrepareCount_JAISound_();
    bool calc_JAISound_();
    void initTrack_JAISound_(JASTrack *);

    virtual s32 getNumChild() const = 0;
    virtual JAISoundChild *getChild(int) = 0;
    virtual void releaseChild(int) = 0;
    virtual JAISe *asSe();
    virtual JAISeq *asSeq();
    virtual JAIStream *asStream();
    virtual JASTrack *getTrack() = 0;
    virtual JASTrack *getChildTrack(int) = 0;
    virtual JAITempoMgr *getTempoMgr() = 0;
    virtual bool JAISound_tryDie_() = 0;

    JAISoundID getID() const;
    u8 getAnimationState() const { return status_.state.flags.animationState; }
    bool isAnimated() const { return getAnimationState() != 0; }
    void setAnimationState(u8 pState)
    {
        status_.state.flags.animationState = pState;
    }
    u32 getUserData() const { return status_.user_data; }
    void setUserData(u32 pUserData) { status_.user_data = pUserData; }
    JAIAudible *getAudible() const { return audible_; }
    bool isHandleAttached() const { return handle_ != NULL; }
    bool hasLifeTime() const { return status_._1.flags.flag2; }
    void removeLifeTime_() { status_._1.flags.flag1 = false; }
    void stop_JAISound_()
    {
        status_.state.flags.flag5 = 0;
        status_.state.flags.flag1 = 1;
    }
    bool isStopping()
    {
        bool isStopping = false;
        if (status_.state.flags.flag1)
        {
            isStopping = !status_.state.flags.flag5 || fader_.isOut();
        }
        return isStopping;
    }

    void pause(bool doPause)
    {
        status_._0.flags.paused = doPause;
    }

    void updateLifeTime(u32 newLifeTime)
    {
        if (lifeTime_ < newLifeTime)
        {
            lifeTime_ = newLifeTime;
        }
    }

    void setLifeTime(u32 newLifeTime, bool pause)
    {
        lifeTime_ = newLifeTime;
        setComesBack(pause);
        status_._1.flags.flag2 = 1;
    }

    void setComesBack(bool pause)
    {
        status_._1.flags.flag1 = 1;
        if (pause)
        {
            status_.pauseWhenOut();
        }
    }

    bool setPos(const JGeometry::TVec3f &pos)
    {
        if (audible_ != NULL)
        {
            audible_->setPos(pos);
        }
        return audible_ != NULL;
    }

    void fadeIn(u32 fadeCount)
    {
        fader_.fadeInFromOut(fadeCount);
        return;
    }

    s32 getCount() const { return count_; }

    JAISoundParamsMove &getAuxiliary() { return params_.mMove; }

private:
    JAISoundHandle *handle_;
    JAIAudible *audible_;
    JAIAudience *audience_;
    u32 lifeTime_;
    s32 prepareCount_;
    JAISoundID soundID_;
    JAISoundStatus_ status_;
    JAISoundFader fader_;
    u32 priority_;
    s32 count_;
    JAISoundParams params_;
}; // Size: 0x98

#endif