#ifndef JAUDIO_JAUSOUNDANIMATOR_H
#define JAUDIO_JAUSOUNDANIMATOR_H

#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JAudio/System/JASSoundParams.h"
#include "JSystem/JUtility/JUTAssert.h"

class JAUSoundAnimation;

class JAUSoundAnimationSound {
public:
    bool playsOnlyForward() const { return (mFlags & 1) != 0; }
    bool playsOnlyReverse() const { return (mFlags & 2) != 0; }
    bool stopsWhenAnimationChanges() const { return (mFlags & 4) != 0; }
    bool playsOnlyOnce() const { return (mFlags & 8) != 0; }
    bool stopsWhenNoteOff() const { return (mFlags & 0x10) != 0; }
    bool stopsWhenSpeedIsZero() const { return (mFlags & 0x20) != 0; }
    bool setsLifeTime() const { return (mFlags & 0x40) != 0; }
    bool playsAtIntervals() const { return (mFlags & 0x80) != 0; }

    bool isNoting(f32 p1) const {
        if (_04 == _08) {
            return true;
        }
        if (_04 > _08) {
            if (_08 <= p1 && p1 < _04) {
                return true;
            }
        } else {
            if (_04 <= p1 && p1 < _08) {
                return true;
            }
        }
        return false;
    }

    bool isNotingOn(f32 frame, bool reverse) const {
        if (setsLifeTime()) {
            return isNoting(frame);
        }
        if (reverse) {
            return frame <= _04;
        } else {
            return frame >= _04;
        }
    }

    bool isNotingOff(f32 frame, bool reverse) const {
        if (setsLifeTime()) {
            return !isNoting(frame);
        }
        if (_04 == _08) {
            return false;
        }
        if (reverse) {
            return frame <= _08;
        } else {
            return frame >= _08;
        }
    }

    JAISoundID mSoundId;
    f32 _04;
    f32 _08;
    f32 mPitch;
    u32 mFlags;
    u8 _14;
    s8 _15;
    u8 _16;
    u8 _17;
    s8 _18;
    u8 _19;
    s8 _1a;
    u8 _1b[5];
}; /* size 0x20 */

class JAUSoundAnimationControl {
public:
    virtual ~JAUSoundAnimationControl() = 0;
    virtual JAUSoundAnimationSound* getSound(const JAUSoundAnimation*, int) = 0;
    virtual u16 getNumSounds(const JAUSoundAnimation*) = 0;
};


class JAUSoundAnimation {
public:
    int getStartSoundIndex(f32) const;
    int getEndSoundIndex(f32) const;

    u16 getNumSounds() const {
        if (mControl != NULL) {
            return mControl->getNumSounds(this);
        } else {
            return mNumSounds;
        }
    }

    const JAUSoundAnimationSound *getSound(int i_index) const {
        if (mControl != NULL) {
            return mControl->getSound(this, i_index);
        } else {
            return &mSounds[i_index];
        }
    }

    u16 mNumSounds;
    JAUSoundAnimationControl *mControl;
    JAUSoundAnimationSound mSounds[1]; // mSounds[mNumSounds];
};

class JAUSoundAnimator {
public:
    JAUSoundAnimator(JAISoundHandles *handles);

    virtual JAISoundHandle *getFreeHandle(const JAUSoundAnimationSound *animSound);                       // 08
    virtual u32 getSoundID(const JAUSoundAnimationSound *animSound, const JGeometry::TVec3f &, f32);      // 0C not JAISoundID?
    virtual bool playsSound(const JAUSoundAnimationSound *animSound, const JGeometry::TVec3f &, f32);     // 10
    virtual void modifySoundParams(JASSoundParams *params, const JAUSoundAnimationSound *animSound, f32); // 14

    void setHandles(JAISoundHandles *);
    void removeAnimation();
    void startAnimation(const JAUSoundAnimation *animation, bool, f32, f32);
    void updateAnimation(f32, const JGeometry::TVec3f&, JAISoundStarter *starter);

    void setLoopStartFrame(f32 val) {
#line 178
        JUT_ASSERT(animation_);
        loopStartFrame_ = val;
    }

    void setLoopEndFrame(f32 val) {
#line 184
        JUT_ASSERT(animation_);
        loopEndFrame_ = val;
    }

    void setLoopFrame(f32 start, f32 end) {
        setLoopStartFrame(start);
        setLoopEndFrame(end);
    }

private:
    void ageSounds_();
    void updateSoundLifeTime_(f32, f32);
    void startSound_(const JGeometry::TVec3f&, f32, JAISoundStarter *starter);

protected:
    JAISoundHandles *handles_;           // 04
    const JAUSoundAnimation *animation_; // 08 
    bool reverse_;                       // 0C
    int curSoundIndex_;                  // 10
    f32 curSoundFrame_;                  // 14
    int startSoundIndex_;                // 18
    int endSoundIndex_;                  // 1C
    f32 loopStartFrame_;                 // 20
    f32 loopEndFrame_;                   // 24
    int _28;                             // 28

};

#endif
