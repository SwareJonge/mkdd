#ifndef GAMESOUNDMGR_H
#define GAMESOUNDMGR_H

#include "Inagaki/GameAudioAudience.h"
#include "Inagaki/GameAudioAnimator.h"
#include "Inagaki/GameAudioMain.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JAudio/JAUSoundMgr.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Course/CrsArea.h"
#include "kartEnums.h"
#include "types.h"
#include "JSystem/JAudio/JAUSoundObject.h"
#include "JSystem/JKernel/JKRHeap.h"

#include "Kaneshige/Course/CrsGround.h"

namespace GameAudio {

template <class T>
class SoundMgr : public JAUDopplerSoundObject, public JKRDisposer
{
public:
    SoundMgr(Vec *, JKRHeap *, u8);

    virtual ~SoundMgr();
    virtual void init();
    virtual void setSe(u32 id);
    virtual void loop();

    void setEcho(JAISoundHandle *handlePtr, f32 mix);
    JAISoundHandle *startSoundCustom(u32 soundID, u32 p2);

    static void setKillSwAll(bool killSw);

    static T *smStart;
    static T *smEnd;

    bool mKillSw;
    u8 _49;
    u8 _4a[0x58 - 0x4a];
    T *mNext; // 58
};

template<class T>
T *SoundMgr<T>::smStart;

template<class T>
T *SoundMgr<T>::smEnd;

template<class T>
SoundMgr<T>::SoundMgr(Vec *pos, JKRHeap *heap, u8 p3) : JAUDopplerSoundObject((JGeometry::TVec3f*)pos, p3, heap) {
    _49 = 0;
    mNext = nullptr;
    if (smStart == nullptr) {
        smStart = (T*)this;
    }
    if (smEnd) {
        smEnd->mNext = (T*)this;
    }
    smEnd = (T*)this;

    if (Parameters::getDemoMode() != 0) {
        mKillSw = true;
    }
    else {
        mKillSw = false;
    }
}

template<class T>
SoundMgr<T>::~SoundMgr() {
    smStart = nullptr;
    smEnd = nullptr;
}

template<class T>
void SoundMgr<T>::loop() {
#line 139
    JUT_ASSERT_MSG(mSoundPos, "SoundMgr::loop　disable中です！！！！！\n");
    JAUDopplerSoundObject::process();
    _49 = 0;
}

template<class T>
void SoundMgr<T>::setSe(u32 id) {
    if (mKillSw)
        return;
    startSoundCustom(id, 0);
}

template<class T>
void SoundMgr<T>::init() {}

template<class T>
void SoundMgr<T>::setEcho(JAISoundHandle *handlePtr, f32 mix) {
    if (handlePtr && (*handlePtr) && mix != 0.0f) {
        ((*handlePtr))->getAuxiliary().moveFxMix(mix, 0);
    }
}

template<class T>
JAISoundHandle *SoundMgr<T>::startSoundCustom(u32 soundID, u32 p2) {
    JAISoundStarter *starter = JAISoundStarter::getInstance();
    JAISoundHandle *handle = getHandleSoundID(soundID);
    if (!handle) {
        handle = getFreeHandleNotReserved();
    }
    if (!handle) {
        handle = getLowPrioSound(soundID);
    }
    if (handle) {
        starter->startSound(soundID, handle, nullptr);
        if (*handle && !(*handle)->getAudible()) {
            (*handle)->newAudible(*mSoundPos, &_18, p2, nullptr);
        }
    }
    return handle;
}

template<class T>
void SoundMgr<T>::setKillSwAll(bool killSw) {
    for (T *mgr = smStart; mgr != nullptr; mgr = mgr->mNext) {
        mgr->mKillSw = killSw;
    }
}

template <typename T>
class SoundWithEchoMgr : public SoundMgr<T> 
{
public:
    SoundWithEchoMgr(Vec *, JKRHeap *, u8);
    virtual ~SoundWithEchoMgr();
    void checkEcho();
    void setInitialEcho(JAISoundHandle *handle);

protected:
    f32 _5c;
    f32 _60;
    CrsArea *mEchoArea; // 64
};

template<class T>
SoundWithEchoMgr<T>::SoundWithEchoMgr(Vec *pos, JKRHeap *heap, u8 p3) : SoundMgr<T>(pos, heap, p3) {
    _5c = 0.0f;
    _60 = 0.0f;
    mEchoArea = new CrsArea();
}

template<class T>
SoundWithEchoMgr<T>::~SoundWithEchoMgr() {}

template<class T>
void SoundWithEchoMgr<T>::checkEcho() {
    bool alternator = false;
    JAISoundHandles *handle = this;
    for (int i = 0; i < this->_10; i++) {
        if ((*handle)[i]) {
            if (!alternator) {
                mEchoArea->search(2, *this->mSoundPos);                
                _5c = mEchoArea->getEchoRate() * 0.8f;
                alternator = true;
            }

            if (_60 != _5c) {
                ((*handle)[i])->getAuxiliary().moveFxMix(_5c, 0);
            }            
        }
    }

    if (alternator) {
        _60 = _5c;
    }
}

template<class T>
void SoundWithEchoMgr<T>::setInitialEcho(JAISoundHandle *handlePtr) {
    if (handlePtr && *handlePtr && _5c != 0.0f) {
        (*handlePtr)->getAuxiliary().moveFxMix(_5c, 0);
    }
}

class KartSoundMgr : public SoundMgr<KartSoundMgr>
{
public:
    KartSoundMgr(Vec *, JKRHeap *, u8, u8);                           // 0x80123118
    ~KartSoundMgr();                                                  // 0x80123328
    void startSoundHandleNumber(u8, u32, u32);                        // 0x801233fc
    virtual void dispose();                                           // 0x801236d0
    virtual void init();                                              // 0x80123708
    void changeDriver(bool);                                          // 0x80123ad4
    void frameWork(u8 rank);                                          // 0x80123ad8
    void checkAfterGoalVolume();                                      // 0x80123c50
    void setWaterDepth(u8, f32);                                      // 0x80123ee4
    void setSlip(u8, u8, u8, f32);                                    // 0x80123ef4
    void setConductStatus(f32, f32, bool, bool, bool, u8, CrsArea *); // 0x80124400
    void setWaterCutoffPort(u16);                                     // 0x8012485c
    void countGoalKart();                                             // 0x80124930
    void setConductLocomotiveAccel();                                 // 0x8012495c
    void setConductLocomotiveSpeed(bool);                             // 0x80124d9c
    void setConductPressed();                                         // 0x80125194
    void setConductSignal();                                          // 0x80125490
    void setConductOutOfCourse(u8);                                   // 0x80125b04
    void setConductTrouble(f32, u8);                                  // 0x80125f60
    void setConductRace(bool);                                        // 0x801263cc
    void setConductAfterGoal(bool);                                   // 0x80127188
    void setCrushSe(CrsGround::EMat, f32);                            // 0x801271a8
    void setCrushSe(u32, f32);                                        // 0x801272e4
    void setBrakeSe(u32);                                             // 0x80127474
    void setDashSe(u32);                                              // 0x8012763c
    void setMiniturboSe(u32);                                         // 0x801276e4
    void setJumpUpSe(u32);                                            // 0x80127770
    void setBoundSe(f32);                                             // 0x801277dc
    void setWheelSpinSe();                                            // 0x8012798c
    void setSpinSe();                                                 // 0x80127aa4
    void setSpinTurnSe();                                             // 0x80127bb4
    virtual void setSe(u32);                                          // 0x80127ccc
    void setChibiPitch(JAISoundHandle *);                             // 0x80127ec8
    void adjustEngine();                                              // 0x80127fb8
    void crushRenzokuTaisaku();                                       // 0x80128140
    void slipParamSet();                                              // 0x8012816c
    void checkEcho();                                                 // 0x801288f8
    void setInvincibleBgm(u8);                                        // 0x80128aac
    void clearInvincibleBgm(u8);                                      // 0x80128c30
    void setChibiFlag(bool, bool);                                    // 0x80128e98

    static u8 smKartCount;
    static u8 smEntryKartCount;
    static u8 smGoalKartCount;

    static u8 smKartRankClassMem[7];

    u8 _5c[0x61 - 0x5c];
    u8 _61;
    u8 mKartCount; // 62
    u8 _63;
    u8 _64;
    u8 _65;
    u8 _66;
    u8 _67[0x6c - 0x67];
    f32 _6c;
    u8 _70[0x8d - 0x70];
    u8 _8d;
    u8 _8e[0x134 - 0x8e];
};

class CharacterSoundMgr : public SoundMgr<CharacterSoundMgr> {
public:
    CharacterSoundMgr(Vec *, JKRHeap *, u8, ECharID);            // 0x80118d1c
    ~CharacterSoundMgr();                                        // 0x80118df0
    void init();                                                 // 0x80118e50
    void loop();                                                 // 0x80118ea8
    void frameWork(f32, f32);                                    // 0x80118f48
    void setVoice(u8);                                           // 0x80118fb0
    void setSe(u32);                                             // 0x801195e0
    bool isSpeak();                                              // 0x80119774
    void setVoicePitch(bool);                                    // 0x80119800
    KartSoundMgr *getKartSound();                                // 0x80119980
    void checkEcho();                                            // 0x801199d8
public:
    CustomAnimator *mAnimator; // 5C
    CustomAnimationSoundStarter *mStarter; // 60
    u8 _64;
    u8 _65;
    u32 _68;
    u32 _6c;
    f32 _70;
    ECharID mCharID; // 74
    u8 _78;
    KartSoundMgr *mKartSound;
}; // Size: 0x80

class PosMove {
public:
    PosMove() {
        _0 = 0;
        _28 = 0;
    }
    u8 _0;
    u8 _1[0x28 - 0x1];
    u32 _28;
};

class ObjectSoundMgr : public SoundWithEchoMgr<ObjectSoundMgr>
{
public:
    ObjectSoundMgr(Vec *, JKRHeap *);        // 0x8012a588    
    void frameWork();                        // 0x8012a6e4    
    void setSeWithVelocityVP(u32, f32, f32); // 0x8012a75c
    void setSeWithVelocityV(u32, f32, f32);  // 0x8012a8c4
    void setSeWithVelocityP(u32, f32);       // 0x8012a9c4

    // Vtable
    virtual ~ObjectSoundMgr(); // 0x8012a5fc
    virtual void loop();       // 0x8012a65c
    virtual void setSe(u32);   // 0x8012a6fc

private:
    u8 _68;
    f32 _6c;
    CrsArea *mObjArea;
}; // class ObjectSoundMgr

class RockSoundMgr : public SoundMgr<RockSoundMgr> {
public:
    RockSoundMgr(Vec *, f32, JKRHeap *); // 0x8012b340
    ~RockSoundMgr();                     // 0x8012b3e0
    void init();                         // 0x8012b440
    void loop();                         // 0x8012b460
    void frameWork();                    // 0x8012b4e0
    void setRollSe(f32);                 // 0x8012b4ec
    void setBoundSe(f32);                // 0x8012b690
    void setBrokenSe();                  // 0x8012b848

    u8 _5c;
    f32 _60;
    int _64;
};

class NpcCarSoundMgr : public SoundWithEchoMgr<NpcCarSoundMgr> {
public:
    NpcCarSoundMgr(Vec *, u8, JKRHeap *); // 0x80129994
    ~NpcCarSoundMgr();                    // 0x80129a10
    void frameWork(f32, bool);            // 0x80129a70
    void setEngineSound(f32);             // 0x80129ac8
    void setHornSound(bool);              // 0x80129bc0
    void setSe(u32);                      // 0x80129c98
private:
    bool _68;  //
    u8 _69;  // nice
    u16 _6a; //
};

class ShoreSoundMgr : public SoundMgr<ShoreSoundMgr> {
public:
    ShoreSoundMgr(JKRHeap *heap);                               // 0x8012cb18
    ~ShoreSoundMgr();                                           // 0x8012cc38
    void loop();                                                // 0x8012cc98
    void frameWork();                                           // 0x8012ceec
    void setTide(u8, JGeometry::TVec3f *, JGeometry::TVec3f *); // 0x8012cf04
    void setStatus(bool, f32);                                  // 0x8012d814

    static CustomAudience<4> *getAudience() { return smAudience; }
private:
    static CustomAudience<4> *smAudience; // 0x80416278

    u8 _5c;
    u8 _5d;
    u8 _5e[0x60 - 0x5e]; // padding?
    f32 _60;
    JGeometry::TVec3f *_64;
    PosMove *_68;
    PosMove *_6c;
};

class CircleSoundMgr : public SoundMgr<CircleSoundMgr> {
public:
    CircleSoundMgr(Vec *, f32, f32, f32, JKRHeap *); // 0x8012dc34
    ~CircleSoundMgr();                               // 0x8012dd68
    void loop();                                     // 0x8012ddc8
    void frameWork();                                // 0x8012e02c
    void calcPlayPos(u32);                           // 0x8012e064
    void setSe(u32);                                 // 0x8012e3f4
};
}

#endif
