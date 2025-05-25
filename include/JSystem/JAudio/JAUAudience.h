#ifndef JAUDIO_JAUAUDIENCE_H
#define JAUDIO_JAUAUDIENCE_H

#include "JSystem/JAudio/Interface/JAIAudible.h"
#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASSoundParams.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Util.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/mtx.h"

// Copied from https://github.com/zeldaret/tp/blob/main/include/Z2AudioLib/Z2Audience.h
// Z2Audio probably used this as a base but refactored it

struct JAUAudibleParam {
    f32 getDopplerPower() const {
        return _00.bytes.b0_0 * (1.0f / 15.0f);
    }
    union {
        struct {
            u16 f0;
            u16 f1;
        } half;
        struct {
            u8 b0_0 : 4;
            u8 b0_4 : 1;
            u8 b0_5 : 1;
            u8 b0_6 : 1;
            u8 b0_7 : 1;
            u8 b1_0 : 1;
            u8 b1_1 : 1;
            u8 b1_2_7 : 6;
            u8 b2;
            u8 b3;
        } bytes;
        u32 raw;
    } _00;
};


class JAUAudibleAbsPos {
public:
    JAUAudibleAbsPos() {}
    ~JAUAudibleAbsPos() {}

    void calc(const JGeometry::TVec3f &p1) {
        _0C.sub(p1, _00);
        _00.set(p1);
    }

    JGeometry::TVec3f _00;
    JGeometry::TVec3f _0C;
};

class JAUDopplerAudibleAbsPos {
public:
    JAUDopplerAudibleAbsPos() {}
    ~JAUDopplerAudibleAbsPos() {}

    void calc(const JGeometry::TVec3f &p1) {
        _0C.sub(p1, _00);
        _00.set(p1);
    }

    JGeometry::TVec3f _00;
    JGeometry::TVec3f _0C;
};

class JAUAudibleRelPos {
public:
    JAUAudibleRelPos() {}

    JGeometry::TVec3f _00;
    f32 _0C;
    f32 _10;
};

class JAUAudibleChannel {
public:
    JAUAudibleChannel() {}

    JASSoundParams params_;
    JGeometry::TVec3f _14;
    JGeometry::TVec3f _20;
    f32 _2C;
    f32 _30;
};

class JAUAudience3DSetting {
public:
    void init();
};

class JAUAudience_withSetting : public JAIAudience {
public:
    JAUAudience_withSetting() {
        setting_._04 = 7000.0f;
        setting_._08 = 800.0f;
        setting_.minVolume = 0.005f;
        setting_._14 = 8000.0f;
        setting_._10 = -4000.0f;
        setting_._28 = 0.1f;
        setting_._24 = 0.0f;
        setting_._18 = 1.0f;
        setting_._1c = 0.8f;
        setting_._20 = 0.0f;
        setting_._2c = 0.5f;
        setting_._38 = 0xff;
        setting_._30 = 3000.0f;
        setting_._34 = 1.5f;
        updateSetting();
    }
    ~JAUAudience_withSetting() {}
    void updateSetting();

    struct {
        f32 _04;
        f32 _08;
        f32 minVolume;
        f32 _10;
        f32 _14;
        f32 _18;
        f32 _1c;
        f32 _20;
        f32 _24;
        f32 _28;
        f32 _2c;
        f32 _30;
        f32 _34;
        u32 _38;
        f32 _3c;
        f32 _40;
        f32 _44;
        f32 _48;
        f32 _4c;
    } setting_;
};

class JAUAudience_withSetting_doppler : public JAUAudience_withSetting {
public:
    JAUAudience_withSetting_doppler() {}

    ~JAUAudience_withSetting_doppler() {
        
    }
};


class JAUAudienceState {
public:
    JAUAudienceState() {}
    //~JAUDopplerAudienceState() {}

    void init() {
        Mtx m;
        PSMTXIdentity(m);
        resetMtx(m);
    }
    void resetMtx(const Mtx in) {
        JGeometry::TPos3f m;
        mMtx.set(in);
        m.setPositionFromLookAt(mMtx);
        m.getTransInline(_3c);
        _48.set(_3c);
        _30.zero();

    }

    JGeometry::TPos3f mMtx;
    JGeometry::TVec3f _30;
    JGeometry::TVec3f _3c;
    JGeometry::TVec3f _48;

};

class JAUDopplerAudienceState {
public:
    JAUDopplerAudienceState() {}
    //~JAUDopplerAudienceState() {}

    void init() {
        Mtx m;
        PSMTXIdentity(m);
        resetMtx(m);
    }
    void resetMtx(const Mtx in) {
        JGeometry::TPos3f m;
        mMtx.set(in);
        m.setPositionFromLookAt(mMtx);
        m.getTransInline(_3c);
        _48.set(_3c);
        _30.zero();
    }

    JGeometry::TPos3f mMtx;
    JGeometry::TVec3f _30;
    JGeometry::TVec3f _3c;
    JGeometry::TVec3f _48;

}; // Size: 0x54

class JAUDopplerAudibleChannel : public JAUAudibleChannel {
public:
    JAUDopplerAudibleChannel() {}
}; // Size: 0x34

template <int N, class Channel, class AbsPos>
class JAUGenericAudible_ : public JAIAudible {
public:
    JAUGenericAudible_() {}
    ~JAUGenericAudible_() {}

    void calc() {
        absPos_.calc(mPos);
    }

    JASSoundParams *getOuterParams(int index) {
#line 483
        JUT_ASSERT(index >= 0);
        JUT_ASSERT(index < MAX_CHANNELS);
        JASSoundParams *params = &channel_[index]->params_;
        if (params) {
            return params;
        }
        return NULL;
    }

    Channel *getChannel(int index) {
#line 494
        JUT_ASSERT(index >= 0);
        JUT_ASSERT(index < MAX_CHANNELS);
        return channel_[index];
    }

    JAUAudibleParam &getAudibleParam() { return audiblep; }
    const JAUAudibleParam getAudibleParam() const { return audiblep; }

    static const int MAX_CHANNELS = N;

    JAUAudibleParam audiblep;
    AbsPos absPos_;
    Channel *channel_[N];
    Channel channels[N];
};

template <int N, class Audible, class State, class Audience>
class JAUAudience_ : public Audience {
public:
    JAUAudience_() : numPlayers_(N) {
        for (int i = 0; i < N; i++) {
            state[i].init();
        }
    }
    ~JAUAudience_() {
        
    }

    int getMaxChannels() { return numPlayers_; }

    f32 calcVolume_(f32 p1) const {
        if (p1 > this->setting_._04) {
            return this->setting_.minVolume;
        } 

        if (p1 < this->setting_._08) {
            return 1.0f;
        }

        return JGeometry::TUtilf::clamp(1.0f + this->setting_._44 * (p1 - this->setting_._08), 0.0f, 1.0f);
    }

    f32 calcPitchDoppler_(const JGeometry::TVec3f &p1,
                                  const JGeometry::TVec3f &p2,
                                  const JGeometry::TVec3f &p3, f32 p4) const {
        f32 temp1 = p4 * p1.dot(p2);
        f32 temp2 = p4 * p1.dot(p3);
        return (this->setting_._30 + temp1) / (this->setting_._30 + temp2);
    }

    f32 calcPitch_(JAUAudibleChannel *channel, const Audible *audible, int index) {
        JAUAudibleParam audParam = audible->getAudibleParam();
        if ((*(u8*)&audParam._00.raw >> 4) & 0xf) { // probably fake
            f32 dopplerPwr = calcPitchDoppler_(channel->_20, state[index]._30, audible->absPos_._0C, audible->getAudibleParam().getDopplerPower());
            
            if (channel->_30 > 0.0f) {
                if (channel->_30 > (dopplerPwr * this->setting_._34)) {
                    return channel->_30;
                } 
                if (dopplerPwr > (channel->_30 * this->setting_._34)) {
                    return channel->_30;
                } 
                channel->_30 = dopplerPwr;
                return dopplerPwr;
            } 
            if (dopplerPwr > 0.0f) {
                channel->_30 = dopplerPwr;
                return dopplerPwr;
            }
            return 1.0f;
        }
        return 1.0f;
    }

    void mixChannelOut(const JASSoundParams &params, JAIAudible *pAudible, int index) {
        Audible *audible = (Audible *)pAudible;
        JAUAudibleChannel *channel = audible->getChannel(index);

        if (!channel) {
            return;
        }

        JASSoundParams newParams;
        if (audible->getAudibleParam()._00.bytes.b0_5) {            
            newParams.mVolume = calcVolume_(channel->_2C);
        } else {
            newParams.mVolume = 1.0f;
        }

        if (audible->getAudibleParam()._00.bytes.b1_1) {
            f32 dolby;
            if (channel->_14.z > this->setting_._14) {
                dolby = this->setting_._18;
            } else {
                dolby = channel->_2C < this->setting_._08 ? 1.0f :
                    this->setting_._44 * (channel->_2C - this->setting_._08) + 1.0f;
                if (channel->_14.z < this->setting_._10) {
                    dolby = this->setting_._20;
                }
                else if (channel->_14.z < 0.0f) {
                    dolby = this->setting_._3c * channel->_14.z + this->setting_._1c;
                }
                else {
                    dolby = this->setting_._40 * channel->_14.z + this->setting_._1c;
                }
            }
            newParams.mDolby = dolby;
        } else {
            newParams.mDolby = this->setting_._1c;
        }

        if (audible->getAudibleParam()._00.bytes.b1_0) {
            if (numPlayers_ > 2) {
                newParams.mPan = (index & 1) ? 1.0f : 0.0f;
            } else {
                f32 pan = channel->_2C <= JGeometry::TUtilf::epsilon() ? 0.0f : channel->_14.x / channel->_2C;
                newParams.mPan = pan * this->setting_._2c + 0.5f;
            }
        } else {
            newParams.mPan = 0.5f;
        }
    

        newParams.mPitch = calcPitch_(channel, audible, index);
        f32 fxMix;
        if (audible->getAudibleParam()._00.bytes.b0_6) {
            if (channel->_2C > this->setting_._04) {
                fxMix = this->setting_._28;
            } else {
                fxMix = channel->_2C < this->setting_._08 ? this->setting_._24 :
                    this->setting_._4c * (channel->_2C - this->setting_._08) + this->setting_._24;
            }
        } else {
            fxMix = 0.0f;
        }
        newParams.mFxMix = fxMix;

        audible->getOuterParams(index)->combine(params, newParams);
    }

    u32 calcDeltaPriority_(f32 p1, bool p2) const {
        if (p1 > this->setting_._04) {
            if (p2) {
                return -1;
            }
            return this->setting_._38;
        }

        if (p1 < this->setting_._08) {
            return 0;
        }

        return this->setting_._48 * (p1 - this->setting_._08);
    }
    
    u32 calcPriority(JAIAudible *i_audible) {
        u32 deltaPriority[N];
        Audible *audible = (Audible*)i_audible;
        JAUAudibleChannel *channel; // regswap most likely caused by missing inline(convertAbsToRel)
        if (!audible->getAudibleParam()._00.bytes.b0_4) {
            for (int i = 0; i < numPlayers_; i++) {
                channel = audible->getChannel(i);
                if (channel != NULL) {
                    JGeometry::TVec3f diff;
                    diff.sub(audible->mPos, state[i]._3c);
                    PSMTXMultVecSR(state[i].mMtx, &diff, &channel->_14);
                    channel->_2C = channel->_20.normalize(diff);                    
                }
            }
            return 0;
        } else {
            u32 rv = 0xffffffff;
            for (int i = 0; i < numPlayers_; i++) {
                channel = (audible)->getChannel(i);
                if (channel != NULL) {
                    JGeometry::TVec3f diff;
                    diff.sub(audible->mPos, state[i]._3c);
                    PSMTXMultVecSR(state[i].mMtx, &diff, &channel->_14);
                    channel->_2C = channel->_20.normalize(diff);
                    deltaPriority[i] = calcDeltaPriority_(channel->_2C, audible->getAudibleParam()._00.bytes.b0_7);
                    if (deltaPriority[i] < rv) {
                        rv = deltaPriority[i];
                    }
                }
            }
            return rv;
        }

    }
    void deleteAudible(JAIAudible *audible) {
        delete audible;
    }
    JAIAudible *newAudible(const JGeometry::TVec3f &p1, JAISoundID soundID, const JGeometry::TVec3f *p3, u32 channel) {
        u32 uVar4 = channel | ~((1 << numPlayers_) + -1);
        if (uVar4 == 0xffffffff) {
            JUT_WARNING_F2("%s", "You masked all audible channels !")
            return NULL;
        }
        return new Audible();
    }
    
    int numPlayers_;
    State state[N];
};

template <int N>
class JAUAudible : public JAUGenericAudible_<N, JAUAudibleChannel, JAUAudibleAbsPos>, public JASPoolAllocObject< JAUAudible<N> > {
public:
    JAUAudible() {
    }
    ~JAUAudible() {
       
    }
};

template <int N>
class JAUDopplerAudible : public JAUGenericAudible_<N, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos>, public JASPoolAllocObject< JAUDopplerAudible<N> > {
public:
    JAUDopplerAudible() {
        
    }
    ~JAUDopplerAudible() {
        
    }
};


template <int N>
class JAUAudience : public JAUAudience_<N, JAUAudible<N>, JAUAudienceState, JAUAudience_withSetting> {
public:
    JAUAudience() {}
    ~JAUAudience() {
       
    }

};

template <int N>
class JAUDopplerAudience : public JAUAudience_<N, JAUDopplerAudible<N>, JAUDopplerAudienceState, JAUAudience_withSetting_doppler> {
public:
    JAUDopplerAudience() {
        
    }
    ~JAUDopplerAudience() {
       ;
    }

};

#endif
