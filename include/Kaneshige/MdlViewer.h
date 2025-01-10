#ifndef MDLVIEWER_H
#define MDLVIEWER_H

#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JSupport.h"
#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/DrawBuffer.h"
#include "Kaneshige/HioMgr.h"
#include "dolphin/mtx.h"
#include "types.h"

class MdlViewer;

class MVGrafPort : public J2DPerspGraph {
public:
    MVGrafPort(f32, f32, f32, f32, f32, f32, f32);

    virtual ~MVGrafPort() {}
    virtual void setPort();
};

class MVActor
{
public:
    MVActor();
    ~MVActor() {}
    virtual void setCurViewNo(u32) = 0;
    virtual void calc() = 0;
    virtual void update() = 0;
    virtual void viewCalc(u32) = 0;

    JSULink<MVActor> mLink;
};

class MVLight {
public:
    MVLight(MdlViewer *);

    void setGXObj();
    void draw();

    virtual void reset();
    virtual void calc();
    virtual MtxPtr getEffectMtx() { return nullptr; }

    MdlViewer *getViewer() { return mViewer; }

protected:
    JGeometry::TVec3f mPos;
    JUTColor mColor;
private:
    MdlViewer *mViewer;
    GXLightObj mLightObj;
};

class MVChaseLight : public MVLight {
public:
    MVChaseLight(MdlViewer *);
    virtual void reset();
    virtual void calc();
    virtual MtxPtr getEffectMtx() { return mEfctMtx; }

    JGeometry::TVec3f _58;
    Mtx mEfctMtx; // 64
};

class MVCamera {
public:
    MVCamera(MdlViewer *);

    void reset();
    void calc();
    void setPort();

    void getFrontDirection(JGeometry::TVec3f *out) { out->set(mFrDir); }
    void getCameraPosition(JGeometry::TVec3f *out) { out->set(mCamPos); }
    void getTargetPosition(JGeometry::TVec3f *out) { out->set(mTargetPos); }

    MtxPtr getViewMtx() { return mViewMtx; }
private:
    JGeometry::TVec3f mTargetPos;
    JGeometry::TVec3f mCamPos;
    s16 _18;
    s16 _1a;
    f32 _1c;
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mLfDir;
    f32 mFovY;
    f32 mNear;
    f32 mFar;
    Mtx mViewMtx;
};

class MVAmbient {
public:
    MVAmbient(MdlViewer *);

    void reset();
    void calc();
    void draw();
private:
    JUTColor mColor;
};


class MVDrawBuffer : public DrawBuffer {
public:
    MVDrawBuffer(MdlViewer *, u32 size);
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);
private:
    MdlViewer *mViewer;
};

class MdlViewer { // Autogenerated
public:
    MdlViewer(); // 0x801d98cc
    void reset(); // 0x801d99cc
    void calc(); // 0x801d9a20
    void draw(); // 0x801d9aa0
    void append(MVActor *); // 0x801d9b04
    void setCurViewNoActor(u32); // 0x801d9b4c
    void calcActor(); // 0x801d9ca8
    void updateActor(); // 0x801d9d2c
    void viewCalcActor(u32); // 0x801d9db0
    // Inline/Unused
    ~MdlViewer();
    void appendHio(HioNode *);
    // Inline
    MVCamera *getCamera() { return mCamera; }
private:
    int _0; //
    MVCamera *mCamera; // 04
    MVAmbient *mAmbient; // 08
    MVChaseLight *mChaseLight; // 0C
    MVDrawBuffer *mDrawBuffer; // 10
    JSUList<MVActor> mActorList; // 14
}; // class MdlViewer

#endif
