#ifndef GEORABBITMARK_H
#define GEORABBITMARK_H

#include "JSystem/JGeometry/Vec.h"
#include "types.h"

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

class GeoShine;
class GeoRabbitMark;

class GeoRabbitMarkSupervisor : public GeoObjSupervisor, JKRDisposer
{
public:
    GeoRabbitMarkSupervisor();
    virtual ~GeoRabbitMarkSupervisor();
    virtual void reset();
    virtual void calc();
    void entry(GeoRabbitMark *);
    int getRabbitKartNo();
    void release();
    void getMarkPosition(JGeometry::TVec3f *);
    void changeOwner(int);
    void startWinnerEffect();
    void setWinnerPosition();

    s16 getWinFrame() const // not sure if this returns int or short, int makes more sense to me but this is kaneshige
    {
        return mWinTime * 60;
    }

    s16 getWinTime() const
    {
        return mWinTime;
    }

private:
    GeoRabbitMark *mMark;
    s16 mWinTime;
    s16 mMinTime;
    s16 mDecTime;
};

class GeoRabbitMark : public GeographyObj, StateObserver
{
public:
    GeoRabbitMark(const CrsData::SObject &);     // 0x801cd788
    
    virtual void reset();                        // 0x801cd8dc
    void resetPose(bool);                        // 0x801cda04
    virtual void createColModel(J3DModelData *); // 0x801cdb70
    virtual void doKartColCallBack(int);         // 0x801cdb98
    void equip(int);                             // 0x801cdc24
    virtual void calc();                         // 0x801cdc7c
    void release();                              // 0x801cdd74
    void startFastRot();                         // 0x801cddbc
    void startWinnerEffect();                // 0x801cde10
    void setWinnerPosition();                // 0x801cde70
    void initFuncWait();                     // 0x801ce0fc
    void doFuncWait();                       // 0x801ce108
    bool isRidingBossPakkun();               // 0x801ce244
    f32 getEquipOfsY();                      // 0x801ce2b8
    void initFuncEquip();                    // 0x801ce35c
    void doFuncEquip();                      // 0x801ce4fc
    void initFuncRelease();                  // 0x801ce63c
    void doFuncRelease();                    // 0x801ce6cc
    bool isResetGndAttr(CrsGround::EAttr);   // 0x801ce8c4
    void setBoundSE();                       // 0x801ce904

    void getPosition(JGeometry::TVec3f *pos) { pos->set(mPos); }
    void getFootPosition(JGeometry::TVec3f *pos) { pos->scaleAdd(-sFootLength, mUpDir, mPos); }

    virtual void InitExec();
    virtual void MoveExec();
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; }
    //virtual ~GeoRabbitMark() {} 

    static const s16 cNonOwner = -1; // was this an enum?

    bool isNonOwner() const { return mOwnerNo == cNonOwner; }

    f32 getMarkScale() const { return mMarkScale; }
    f32 getFootLength() const { return sFootLength; } 

    virtual GeoRabbitMarkSupervisor *getSupervisor() { return sSupervisor; }
    static GeoRabbitMarkSupervisor *getSupervisor() { return sSupervisor; }


    friend class GeoRabbitMarkSupervisor;
private:
    static StateFuncSet<GeoRabbitMark> sTable[3]; // 0x803979d8
    static f32 sFootLength;                       // 0x80414728
    static f32 sEquipOfsY1;                       // 0x8041472c
    static f32 sEquipOfsYPakkun1;                 // 0x80414730
    static f32 sEquipOfsY2;                       // 0x80414734
    static f32 sEquipOfsYPakkun2;                 // 0x80414738
    static f32 sEquipOfsY34;                      // 0x8041473c
    static f32 sEquipOfsYPakkun34;                // 0x80414740
    static f32 sGravity;                          // 0x80414744
    static f32 sReflectK;                         // 0x80414748
    static f32 sNormalScale;                      // 0x8041474c
    static f32 sEquipScale;                       // 0x80414750
    static f32 sEqpSprMinLength;                  // 0x80414754
    static f32 sEqpSpringK;                       // 0x80414758
    static f32 sEqpAirRegist;                     // 0x8041475c
    static f32 sEqpChaseRate;                     // 0x80414760
    static f32 sRelV0;                            // 0x80414764
    static f32 sRelGravity;                       // 0x80414768
    static f32 sRelReflectK;                      // 0x8041476c
    static u16 sSupervisorCreateNum;              // 0x804164b8
    static GeoRabbitMarkSupervisor *sSupervisor;  // 0x804164bc

private:
    GeoShine *mShines[4];
    bool mIsDemoMode;
    JGeometry::TVec3f mLfDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mEqpSpr;
    f32 mEqpSprLength;
    f32 _1a0;
    f32 mMarkScale;
    s16 mState; // 1 = resetting, 0 is initialized
    s16 _1aa;
    s16 mPrevOwnerNo;
    s16 mOwnerNo;
    s16 _1b0;
    JGeometry::TVec3f mWinnerPos;
    s16 _1c0;
};

#endif
