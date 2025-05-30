#ifndef GEOCAR_H
#define GEOCAR_H

#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JSupport/JSUList.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/DarkAnmMgr.h"
#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"
#include "dolphin/types.h"

class GeoCar;

class GeoCarSupervisor : public GeoObjSupervisor, JKRDisposer
{
public:
    GeoCarSupervisor();
    void entryCar(GeoCar *, bool);
    int getCarNumber();
    void hideAllCar();

    // Vtable
    virtual ~GeoCarSupervisor();
    virtual void reset();
    virtual void calc();

private:
    s16 mEntryId;
    JSUList<GeoCar> mList; // 0x40
};                         // Size: 0x4c

class GeoCar : public GeographyObj, public StateObserver
{
public:
    GeoCar(u32, bool);                                                                                    // 0x801bab60
    GeoCar(const CrsData::SObject &);                                                                     // 0x801bacc0
    void init(bool);                                                                                      // 0x801bada0
    void calcFrPos();                                                                                     // 0x801bb124
    void calcBcPos();                                                                                     // 0x801bb160
    void setTargetPoint(u16);                                                                             // 0x801bb50c
    void resetDrivePower();                                                                               // 0x801bb688
    void calcFrontAndBackPosition();                                                                      // 0x801bb6cc
    bool isHorn();                                                                                        // 0x801bb730
    void setTevColor();                                                                                   // 0x801bb994
    void recvStateRequest();                                                                              // 0x801bb9c0
    void checkKartCollision();                                                                            // 0x801bba20
    void setCrushSE(u32);                                                                                 // 0x801bbba4
    void checkItemCollision();                                                                            // 0x801bbbfc
    void getTargetPosition(JGeometry::TVec3f *);                                                          // 0x801bbf20
    void setAllObjectCollision(bool);                                                                     // 0x801bc05c
    f32 getTireAnmRate();                                                                                 // 0x801bc0e0
    f32 getTireAnmRate(f32);                                                                              // 0x801bc118
    void initFuncParking();                                                                               // 0x801bc12c
    void doFuncParking();                                                                                 // 0x801bc130
    void initFuncMove();                                                                                  // 0x801bc134
    void doFuncMove();                                                                                    // 0x801bc168
    void checkTargetPoint();                                                                              // 0x801bc4b0
    void calcNewPose(JGeometry::TVec3f *, JGeometry::TVec3f *, JGeometry::TVec3f *, JGeometry::TVec3f *); // 0x801bc670
    void initFuncDamage();                                                                                // 0x801bcab8
    void doFuncDamage();                                                                                  // 0x801bcb5c
    void initFuncRecover();                                                                               // 0x801bcdd4
    void doFuncRecover();                                                                                 // 0x801bcdf4
    void initFuncDeath();                                                                                 // 0x801bce58
    void doFuncDeath();                                                                                   // 0x801bce88
    void initFuncWait();                                                                                  // 0x801bceb8
    void doFuncWait();                                                                                    // 0x801bcebc

    // Inline
    void sendStateReqExplosion() { mStateReq |= 1; } // 0x801ba8b0
    void sendStateReqSlowdown() { mStateReq |= 2; }  // 0x801ba8d8
    bool tstCarCollision() { return _1e0 & 1; }       // 0x801ba930
    void setCarCollision() { _1e0 |= 1;}       // 0x801bb468
    bool isDeath() { return StateObserver::getState() == 4; } // 0x801bb924
    bool recvStateReqExplosion() { // 0x801bba08
        bool ret = mStateReq & 1;
        mStateReq &= ~1;
        return ret;
    }

    bool recvStateReqSlowdown() { 
        bool ret = mStateReq & 2;
        mStateReq &= ~2;
        return ret;
    }

    void clrCarCollision() { _1e0 &= ~1; }

    void getScale(JGeometry::TVec3f *scale) { scale->set(mScale); }  
    void getBasePosition(JGeometry::TVec3f *pos) { pos->set(mBasePos); }  
    void getFrontDirection(JGeometry::TVec3f *frDir) { frDir->set(mFrDir); }
    DarkAnmPlayer *getDarkAnm() { return mDarkAnm; }
    CrsArea *getShadowArea() { return mShadowArea; }

    s16 getCarID() { return mCarID; }
    void setCarID(s16 id) { mCarID = id; }

    static StateFuncSet<GeoCar> sTable[6]; // 0x80396058                                                                                                                  // 0x80396058
    static f32 sHornDistance;              // 0x80414658
    static f32 sHornDot;                   // 0x8041465c
    static f32 sAccValue;                  // 0x80414660
    static f32 sGravity;                   // 0x80414664
    static f32 sDamageGravity;             // 0x80414668
    static f32 sAirResistance;             // 0x8041466c
    static f32 sHandlePower;               // 0x80414670
    static f32 sTargetRadius;              // 0x80414674
    static f32 sTargetAmp;                 // 0x80414678
    static f32 sSafeDistance;              // 0x8041467c
    static f32 sDamageVel0;                // 0x80414680
    static f32 sDamageShakeAmp;            // 0x80414684
    static s16 sDamageShakeVel;            // 0x80414688
    static u16 sSupervisorCreateNum;       // 0x804163f0
    static GeoCarSupervisor *sSupervisor;  // 0x804163f4
    // Inline/Unused
    void drawPrimForDebug(u32);
    void getPointLinkSignal();
    // void sHandleMinDot;
    // void sDebugTarget;
    // void sDebugParam;

    //virtual ~GeoCar() {}                                // 0x801bcf0c, overide
    virtual void createModel(JKRSolidHeap *, u32);      // 0x801bb700, overide
    virtual void reset();                               // 0x801bb19c, overide
    virtual void calc();                                // 0x801bb810, overide
    virtual u32 getSoundID() const;                     // 0x801bbb98, overide
    virtual const char *getBmdFileName() = 0;           // 0x0, overide
    virtual void createColModel(J3DModelData *) = 0;    // 0x0, overide 
    virtual void setRMtx();                             // 0x801bb960
    virtual u32 getSoundCarType() const { return 4; }   // 0x801bcf88
    virtual GeoCarSupervisor *getSupervisor() { return sSupervisor; } // 0x801bcf90, overide
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; }   // 0x801bcf98, overide
    virtual const Vec &getTirePosOffset(int) = 0;       // 0x0
    virtual bool isBlast() { return false; }            // 0x801ba8c0
    virtual u16 getPathID();                            // 0x801bc0c4
    virtual bool isHitVibrationOn() { return true; }    // 0x801bbad4
    virtual int getVibrationLimitSpeed() { return 50; } // 0x801bbacc
    virtual void InitExec();                            // 0x801bbdb8
    virtual void MoveExec();                            // 0x801bbecc 
    virtual void hitKart(int);                          // 0x801bbaf8
    virtual void hitItemBomb();                         // 0x801bbd3c
    virtual void hitItemWanWan();                       // 0x801bbd68
    virtual void hitItemGTurtleBig();                   // 0x801bbd90
    virtual void hitItemGTurtle() {}                    // 0x801bbd1c
    virtual void hitItemRTurtle() {}                    // 0x801bbd18
    virtual void hitItemFireBall();                     // 0x801bbd64
    virtual void hitItemYoshiEgg() {}                   // 0x801bbd14
    virtual void initCallBackDeath() {}                 // 0x801bce84
    virtual void doCallBackDeath() {}                   // 0x801bceb4
   
    static GeoCarSupervisor *getCarSupervisor() { return sSupervisor; }
private:
    friend class GeoCarSupervisor;
protected:
    f32 _158;                       //
    f32 mZigZagRate;                //
    f32 _160;                       //
    JGeometry::TVec3f mFrScale;     // 164
    JGeometry::TVec3f mBcScale;     // 170
    JGeometry::TVec3f mBasePos;     // 17C
    f32 _188;                       //
    JGeometry::TVec3f mFrDir;       // 18C
    JGeometry::TVec3f mUpDir;       // 198
    JGeometry::TVec3f mLfDir;       // 1A4
    JGeometry::TVec3f _1b0;         //
    GameAudio::NpcCarSoundMgr *mNpcSoundMgr;      // 1b4
    s16 mCarID;                     // 1C0
    JSULink<GeoCar> mLink;          // 1C4
    f32 mAccValue;                  // 1D4
    f32 mTargetRadius;              // 1D8
    f32 mHandlePower;               // 1DC
    u16 _1e0;                       //
    u16 mStateReq;                  // 1E2
    JGeometry::TVec3f mFrPos;       // 1E4
    JGeometry::TVec3f mBcPos;       // 1F0
    f32 mMoveAccValue;                  //
    JGeometry::TVec3f _200;         //
    JGeometry::TVec3f _20c[3];      //
    u16 mPointIdx;                  // 230
    s16 _232;                       //
    s16 mDamageShakeVel;            // 234
    f32 mDamageShakeAmp;            // 238
    JGeometry::TVec3f _23c;         //
    JGeometry::TVec3f mDamagePos;   //
    s16 _254;                       //
    CrsArea *mShadowArea;           // 258
    ObjDarkAnmPlayer *mDarkAnm;     // 25C
};


// Outside class members

// 0x// void JSUList<GeoCar>::initiate() // JSUList<GeoCar>.h; // 0x801ba318
// virtual JSUList<GeoCar>::~JSUList() // JSUList<GeoCar>.h; // 0x801ba338
// void JSUList<GeoCar>::JSUList() // JSUList<GeoCar>.h; // 0x801ba384
// virtual GeoObjSupervisor::~GeoObjSupervisor() // GeoObjSupervisor.h; // 0x801ba3b4
// virtual JSULink<GeoObjSupervisor>::~JSULink() // JSULink<GeoObjSupervisor>.h; // 0x801ba41c
// void GeoObjSupervisor::GeoObjSupervisor() // GeoObjSupervisor.h; // 0x801ba468
// void JSULink<GeoObjSupervisor>::JSULink(GeoObjSupervisor *) // JSULink<GeoObjSupervisor>.h; // 0x801ba4b8
// virtual GeoCarSupervisor::~GeoCarSupervisor() // GeoCarSupervisor.h; // 0x801ba4e8
// void GeographyObj::getBounds(unsigned char) // GeographyObj.h; // 0x801ba8c8
// void JSUListIterator<GeoCar>::goNext() // JSUListIterator<GeoCar>.h; // 0x801ba8e8
// void JSULink<GeoCar>::getNext() const // JSULink<GeoCar>.h; // 0x801ba91c
// void JSUListIterator<GeoCar>::JSUListIterator(const JSUListIterator<GeoCar> &) // JSUListIterator<GeoCar>.h; // 0x801ba924
// void JSUListIterator<GeoCar>::operator!= (const JSULink<GeoCar> *) const // JSUListIterator<GeoCar>.h; // 0x801ba93c
// void JSUList<GeoCar>::getEnd() const // JSUList<GeoCar>.h; // 0x801ba950
// void JSUListIterator<GeoCar>::operator++ (int) // JSUListIterator<GeoCar>.h; // 0x801ba958
// void JSUListIterator<GeoCar>::operator-> () const // JSUListIterator<GeoCar>.h; // 0x801ba9a4
// void JSULink<GeoCar>::getObject() const // JSULink<GeoCar>.h; // 0x801ba9c8
// void JSUListIterator<GeoCar>::JSUListIterator(JSULink<GeoCar> *) // JSUListIterator<GeoCar>.h; // 0x801ba9d0
// void JSUList<GeoCar>::getFirst() const // JSUList<GeoCar>.h; // 0x801ba9d8
// void JSUList<GeoCar>::append(JSULink<GeoCar> *) // JSUList<GeoCar>.h; // 0x801baa50
// virtual JSULink<GeoCar>::~JSULink() // JSULink<GeoCar>.h; // 0x801bac44
// void JSULink<GeoCar>::JSULink(GeoCar *) // JSULink<GeoCar>.h; // 0x801bac90
// void GeographyObj::setItemColReaction(unsigned char, unsigned char) // GeographyObj.h; // 0x801bb088
// void GeographyObj::getKind() const // GeographyObj.h; // 0x801bb0b0
// void GeographyObj::IsPathMove() const // GeographyObj.h; // 0x801bb444
// void StateObserver::getState() const // StateObserver.h; // 0x801bb958
// void GeographyObj::tstIsHitKartFlg(int) const // GeographyObj.h; // 0x801bbadc
// void KartCtrl::GetKartStatus(int) // KartCtrl.h; // 0x801bbb6c
// void ItemObj::getKind() const // ItemObj.h; // 0x801bbd20
// void GeographyObj::getColItemObj() const // GeographyObj.h; // 0x801bbd28
// void GeographyObj::tstItemHitting() const // GeographyObj.h; // 0x801bbd30
// void GeoSignal::getSupervisor() // GeoSignal.h; // 0x801bc054
// void GeographyObj::setAllCheckKartHitFlag() // GeographyObj.h; // 0x801bc0a8
// void GeographyObj::setObjFlagCheckItemHitting() // GeographyObj.h; // 0x801bc0b4
// void JMAAbs(f32); // 0x801bc4a8
// void CrsData::PathData::isClosed() const // CrsData.h; // 0x801bc664
// void stRandom::getRandom() // stRandom.h; // 0x801bcb3c
// void CrsGround::exceptValley(bool) // CrsGround.h; // 0x801bcd70
// void JMASCos(short); // 0x801bcd78
// void JMACosShort(short); // 0x801bcd98
// void JMath::TSinCosTable<11, f32>::cosShort(short) const // JMath.h; // 0x801bcdc4

#endif // GEOCAR_H
