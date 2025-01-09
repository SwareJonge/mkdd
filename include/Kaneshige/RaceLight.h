#ifndef RACELIGHT_H
#define RACELIGHT_H

#include "JSystem/JGeometry.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/LightMgr.h"
#include "Yamamoto/kartCamera.h"

class RaceSceneLight : public LtObjDiffuse
{
public:
    RaceSceneLight(const char *, u32, const JUtility::TColor &, const JGeometry::TVec3f &);
    void getTagName(u32); // UNUSED

    virtual ~RaceSceneLight() {} 
    virtual MtxPtr getEffectMtx() { return mEffectMtx; } 
    virtual void calc();

    KartCam *getTargetCamera() const { return mTargetCam; }
    void getOffsetPosition(JGeometry::TVec3f *out) { out->set(mOffsetPos); }
    void getFrDirection(JGeometry::TVec3f *out) { out->set(mFrDir); }
    void getUpDirection(JGeometry::TVec3f *out) { out->set(mUpDir); }
    void getLfDirection(JGeometry::TVec3f *out) { out->set(mLfDir); }

    static u32 getSceneLightTagName(u32 viewNo) { return 0x53434e30 + viewNo; } // SCN0
private:
    KartCam *mTargetCam;
    JGeometry::TVec3f mOffsetPos;
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mLfDir;
    Mtx mEffectMtx;
};

class RaceBalloonLight : public LtObjDiffuse
{
public:
    RaceBalloonLight(u32);
    virtual ~RaceBalloonLight() {}
    virtual MtxPtr getEffectMtx() { return mEffectMtx; }
    virtual void calc();

private:
    KartCam *mTargetCam;
    JGeometry::TVec3f mOffsetPos;
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mLfDir;
    Mtx mEffectMtx;
};

class RaceKartLight : public LtObjDiffuse
{
public:
    RaceKartLight(RaceSceneLight *, int);
   
    virtual ~RaceKartLight() {}
    virtual MtxPtr getEffectMtx() { return mEffectMtx; }
    virtual void draw();
    virtual void calc();
    
protected:
    RaceSceneLight *mSceneLight;
    s16 mKartIdx;
    JGeometry::TVec3f mOffsetPos;
    Mtx mEffectMtx;
    LtObjAmbient mAmbient;
    bool _110;
    JUtility::TColor mLightColor;
    JUtility::TColor mAmbientColor;
    JGeometry::TVec3f mLightOffsetPos;
};

class RaceCupLight : public LtObjDiffuse
{
public:
    RaceCupLight(RaceSceneLight *);

    virtual ~RaceCupLight() {} // overide
    virtual void draw();       // overide
    virtual void calc();       // overide

    static u32 getTagName(u32 viewNo) { return 0x43555030 + viewNo; } // CUP0
private:
    RaceSceneLight *mSceneLight;
    LtObjAmbient mAmbient;
};

class MenuSceneLight : public LtObjDiffuse 
{
public:
    MenuSceneLight(Mtx m);

    virtual ~MenuSceneLight() {
        sSceneLight = nullptr;
    }
    virtual void calc();
    virtual MtxPtr getEffectMtx() { return mEffectMtx; }
    virtual void getColor(JUtility::TColor *out) const { out->set(mColor); }

    void getCameraPos(JGeometry::TVec3f *out) { out->set(mCameraPos); }
    void getTargetPos(JGeometry::TVec3f *out) { out->set(mTargetPos); }

    static MenuSceneLight *createAndAppend(JKRHeap *, Mtx m);

    static MenuSceneLight *sSceneLight;

    JGeometry::TVec3f mCameraPos;
    JGeometry::TVec3f mTargetPos;
    JGeometry::TVec3f mOffsetPos;
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mLfDir;
    Mtx mEffectMtx;
};

class MenuSubLight : public LtObjDiffuse 
{
public:
    MenuSubLight(int id, MenuSceneLight *sceneLight, u8 loadNo);
    virtual ~MenuSubLight();
    virtual MtxPtr getEffectMtx() { return mEffectMtx; }
    virtual void calc();

    static MenuSubLight *createAndAppend(JKRHeap *, int id, MenuSceneLight *, u8 loadNo);

    static MenuSubLight *sSubLight[2];

    int mId;
    MenuSceneLight *mSceneLight;
    JGeometry::TVec3f mOffsetPos;
    Mtx mEffectMtx;
};

class MenuAmbient : public LtObjAmbient 
{
public:
    MenuAmbient(JUtility::TColor color);
    virtual ~MenuAmbient();

    static MenuAmbient *createAndAppend(JKRHeap *, JUtility::TColor color);

    static MenuAmbient *sAmbient;
};

#endif
