#include "JSystem/JAudio/JAUSoundAnimator.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JUtility/JUTAssert.h"

#include "JSystem/JAudio/JASFakeMatch.h"

int JAUSoundAnimation::getStartSoundIndex(f32 frame) const {
    // UNUSED
    // Size: 0xe0(mismatch)
    int i;
    for (i = 0; i < getNumSounds(); i++) {
        if (getSound(i)->_04 >= frame) {
            break;
        }
    }
    return i;
}

int JAUSoundAnimation::getEndSoundIndex(f32 frame) const {
    // UNUSED
    // Size: 0xdc(mismatch)
    int i;
    for (i = 0; i < getNumSounds(); i++) {
        if (getSound(i)->_04 > frame) {
            break;
        }
    }
    return i;
}

JAUSoundAnimator::JAUSoundAnimator(JAISoundHandles *handles) {
    animation_ = NULL;
    handles_ = handles;
    reverse_ = false;
}

void JAUSoundAnimator::setHandles(JAISoundHandles *handles) {
    // UNUSED
    // Size: 0xe4(mismatch)
    JUT_ASSERT(!handles_ || !handles_->isActive());
    handles_ = handles;
}

void JAUSoundAnimator::removeAnimation() {
    // UNUSED
    // size: 0x0c
    animation_ = NULL;
}

void JAUSoundAnimator::ageSounds_() {
#line 61
    JUT_ASSERT(handles_);

    JAISoundHandles *handles = handles_; // fake?
    for (int i = 0; i < handles->getNumHandles(); i++) {
        if (handles->getHandle(i) == NULL) {
            continue;
        }

        switch (handles->getHandle(i)->getAnimationState()) {
        case 0:
            break;
        case 1: {
            if (handles->getHandle(i)->hasLifeTime()) {
                handles->getHandle(i)->stop();
            } else {
                JAUSoundAnimationSound* sound =
                    (JAUSoundAnimationSound*)handles->getHandle(i)->getUserData();
                if (sound->stopsWhenAnimationChanges()) {
                    handles->getHandle(i)->stop();
                } else {
                    handles->getHandle(i)->setAnimationState(2);
                }
            }
            break;
        }
        case 2:
            break;
        }
    }
}

void JAUSoundAnimator::startAnimation(const JAUSoundAnimation *animation, bool p2, f32 startFrame, f32 endFrame) {
    ageSounds_();
    animation_ = animation;
    
    if (!animation_) {
        return;
    }    
    reverse_ = p2;
    _28 = 0;

    if (reverse_) {
        curSoundIndex_ = animation_->getNumSounds() - 1;
        curSoundFrame_ = FLT_MAX;
    } else {
        curSoundIndex_ = 0;
        curSoundFrame_ = 0.0f;
    }

    setLoopStartFrame(startFrame);
    startSoundIndex_ = animation_->getStartSoundIndex(startFrame);
    setLoopEndFrame(endFrame);
    endSoundIndex_ = animation_->getEndSoundIndex(endFrame);
}

void JAUSoundAnimator::updateSoundLifeTime_(f32 frame, f32 duration) {
    if (!animation_) {
        return;
    }
    
    JAISoundHandles *handles = handles_; // fake?
    for (int i = 0; i < handles->getNumHandles(); i++) {
        if (handles->getHandle(i) != NULL) {
            switch (handles->getHandle(i)->getAnimationState()) {
                case 0:
                    break;
                case 1: {
                    JAUSoundAnimationSound* animationSound =
                        (JAUSoundAnimationSound*)handles->getHandle(i)->getUserData();
#line 147
                    JUT_ASSERT(animationSound);
                    bool pauseOrStop = duration == 0.0f && animationSound->stopsWhenSpeedIsZero();

                    modifySoundParams(&handles->getHandle(i)->getAuxiliary(), animationSound, duration);
                    if (animationSound->setsLifeTime()) {
                        if (pauseOrStop) {
                            handles->getHandle(i)->pause(true);
                        } else if (animationSound->isNoting(frame)) {
                            handles->getHandle(i)->updateLifeTime(1);
                            handles->getHandle(i)->pause(false);
                        } else {
                            handles->getHandle(i)->stop();
                        }
                    } else if (pauseOrStop) {
                        handles->getHandle(i)->stop();
                    } else if (animationSound->stopsWhenNoteOff() &&
                                animationSound->isNotingOff(frame, reverse_)) {
                        handles->getHandle(i)->stop();
                    }
                    break;
                }
                case 2:
                    break;
            }
        }
    }
}

void JAUSoundAnimator::updateAnimation(f32 frame, const JGeometry::TVec3f &pos, JAISoundStarter *starter) {
#line 191
    JUT_ASSERT(handles_);
    JAISoundHandles *handles = handles_; // fake?
    for (s32 i = 0; i < handles->getNumHandles(); i++) {
        if (handles->getHandle(i) != NULL && handles->getHandle(i)->isAnimated()) {
            handles->getHandle(i)->setPos(pos);
        }
    }

    if (!animation_) {
        return;
    }

    if (!starter) {
#line 206
        starter = JAISoundStarter::getInstance();
        JUT_ASSERT(starter);
    }

    f32 duration = (frame - curSoundFrame_);
    if (!reverse_) {
        if (duration < 0.0f) {
            duration += (loopEndFrame_ - loopStartFrame_);
            while (curSoundIndex_ < endSoundIndex_) {
                startSound_(pos, duration, starter);
            }
            curSoundIndex_ = startSoundIndex_;
            curSoundFrame_ = frame;
            if (_28 < 0xffff) {
                _28++;
            }
        }

        updateSoundLifeTime_(frame, duration);
        while (curSoundIndex_ < animation_->getNumSounds() &&
                    animation_->getSound(curSoundIndex_)->isNotingOn(frame, false)) {
            startSound_(pos, duration, starter);
        }
    } else {
        if (duration > 0.0f) {
            duration -= (loopEndFrame_ - loopStartFrame_);
            while (curSoundIndex_ >= startSoundIndex_) {
                startSound_(pos, duration, starter);
            }
            curSoundIndex_ = endSoundIndex_ - 1;
            curSoundFrame_ = frame;
            if (_28 < 0xffff) {
                _28++;
            }
        }
        updateSoundLifeTime_(frame, duration);
        while (curSoundIndex_ >= 0 &&
                    animation_->getSound(curSoundIndex_)->isNotingOn(frame, true)) {
            startSound_(pos, duration, starter);
        }
    }

    curSoundFrame_ = frame;
}

JAISoundHandle *JAUSoundAnimator::getFreeHandle(const JAUSoundAnimationSound *animationSound) {
    for (int i = 0; i < handles_->getNumHandles(); i++) {
        if (!(*handles_)[i]) {
            return &(*handles_)[i];
        }
    }

    return NULL;
}

void JAUSoundAnimator::startSound_(const JGeometry::TVec3f &pos, f32 duration, JAISoundStarter *starter) {
#line 275
    JUT_ASSERT(curSoundIndex_ >= 0);
    JUT_ASSERT(curSoundIndex_ < animation_->getNumSounds());
    
    const JAUSoundAnimationSound *animationSound = animation_->getSound(curSoundIndex_);
    u32 user_data = (u32)animationSound;
    if (reverse_) {
        curSoundIndex_--;
    } else {
        curSoundIndex_++;
    }

    JAISoundHandle *handle = handles_->getHandleUserData(user_data);
    if (!handle) {
        handle = getFreeHandle(animationSound);
    }

    if (handle) {
        if (playsSound(animationSound, pos, duration)) {
            JAISoundID soundID = getSoundID(animationSound, pos, duration);
            
            if (starter->startSound(soundID, handle, &pos)) {
                if (*handle) {
                    (*handle)->setAnimationState(1);
                    (*handle)->setUserData(user_data);
                    if (animationSound->setsLifeTime()) {
                        (*handle)->setLifeTime(1, false);
                    }
                    modifySoundParams(&(*handle)->getAuxiliary(), animationSound, duration);
                }
            }
        }
    }    
}

u32 JAUSoundAnimator::getSoundID(const JAUSoundAnimationSound *animSound, const JGeometry::TVec3f&, f32) {
    return animSound->mSoundId;
}

bool JAUSoundAnimator::playsSound(const JAUSoundAnimationSound *animSound, const JGeometry::TVec3f&, f32) {
    if (animSound->playsOnlyOnce()) {
        if (_28 != animSound->_16) {
            return false;
        }
    }
    else if (animSound->playsAtIntervals() && animSound->_16 != _28 % animSound->_19) {
        return false;
    }
    if (reverse_) {
        if (animSound->playsOnlyForward()) {
            return false;
        }
    }
    else if (animSound->playsOnlyReverse()) {
        return false;
    }

    return true;
}

void JAUSoundAnimator::modifySoundParams(JASSoundParams *params, const JAUSoundAnimationSound *animationSound, f32 duration) {
    f32 fVar3 = (duration < 0.0f ? -duration - 1.0f : duration- 1.0f) ;
    
    f32 fVar1 = (f32)animationSound->_14;
    if (animationSound->_18 != 0) {
        fVar1 = (f32)animationSound->_18 * 2.0f * fVar3 + fVar1;
    }

    const s8 var15 = animationSound->_15;
    params->mVolume = fVar1 * 0.007874016f;
    if (var15) {
        params->mPitch = (f32)animationSound->mPitch + (f32)var15 * fVar3 * 0.03125f;
    }
    else {
        params->mPitch = animationSound->mPitch;
    }
    params->mPan = (f32)animationSound->_17 * 0.007874016f;
}
