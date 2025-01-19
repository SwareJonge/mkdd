#ifndef KARTCAMERA_H
#define KARTCAMERA_H

#include <JSystem/J2D/J2DGrafContext.h>
#include <JSystem/J3DU/J3DUClipper.h>
#include <JSystem/JGeometry/Vec.h>

#include "Kaneshige/Course/CrsArea.h"

#include "Yamamoto/kartCtrl.h"
#include "Yamamoto/kartLine.h"
#include "Yamamoto/KartDemoCam.h"

#include "types.h"

class KartCam
{
public:
    KartCam(KartBody *body, KartCtrl *ctrl, u8, u8);
    void DegubCamera(char *name, u32 id);
    void SetStartDemoPort();
    void SetAwardDemoPort();
    void SetOneScreenPort();
    void SetSubScreenPort();
    void SetTwoScreenPort(u8);
    void SetFourScreenPort(u8);
    void DoPort(int);
    void Init(bool);
    void DoInit();
    void DoResetInit();
    void DoRaceCtrl();
    void DoCameraMode();
    void DoCamera();

    // Start
    // Inline/Unused
    // void SetMTXLookAt(JGeometry::TVec3f *, JGeometry::TVec3f *, JGeometry::TVec3f *);
    // void SetCameraLookPos(JGeometry::TVec3f *);
    // void SetCameraPos(JGeometry::TVec3f *);
    // void RaceBackView();
    // void InitParallelView();
    // void InitHangRescueView();
    // void InitLaunchView();
    // void InitLandView();

    void SetTargetNum(u8);
    void SetClipper();
    void CheckClipper();
    void MakeShaker(float);
    void DoShaker(JGeometry::TVec3f *, JGeometry::TVec3f);
    void SetPtr();
    JGeometry::TVec3f *GetCameraPos(); // TODO: does this return a reference or a pointer
    JGeometry::TVec3f *GetCameraLookPos();
    void SetFovyData();
    void First4ScreenPort(u8);
    void Second4ScreenPort(u8);
    void Third4ScreenPort(u8);
    void Fourth4ScreenPort(u8);
    void First2ScreenPort(u8);
    void Second2ScreenPort(u8);
    void SetVictoryScreenPort(u8);
    void DoMoveCamera(JGeometry::TVec3f *, JGeometry::TVec3f *);
    void DoRoof(JGeometry::TVec3f *, CrsArea *);
    void DoColCamera();
    void DoSea(JGeometry::TVec3f *, CrsGround *);
    void DoGround(JGeometry::TVec3f *, CrsGround *);
    void GroundCheck(JGeometry::TVec3f *, JGeometry::TVec3f *, CrsArea *);
    void OutViewCalc();
    void InitOutView();
    void DoChangFovy();
    void DoLookChase();
    void DoYRotation();
    void DoXRotation();
    void DoDist();
    void DoCamPos(f32, JGeometry::TVec3f *);
    void OutView();
    void InitRaceBackView();
    void ParallelView();
    void InitBackView();
    void BackView();
    void HangRescueView();
    void InitDropRescueView();
    void DropRescueView();
    void LaunchView();
    void LandView();
    void InitPipeView();
    void PipeView();

    f32 GetFovy() { return mFovY; }
    void SetFovy(f32 fov) { mFovY = fov; }
    u8 GetCameraMode() { return mMode; }
    void SetFlag(u16 flag) { mFlags |= flag; }
    void ClrFlag(u16 flag) { mFlags &= ~flag; }

    // Inlines
    /*void CheckClipper()
    {
        if (_16c == mAspect && _174 == mFar && _178 == mFovY)
            return;
        SetClipper();
    }*/

    /*void SetClipper()
    {
        mClipper.setAspect(mAspect);
        mClipper.setNear(mAspect);
        mClipper.setFar(100.0f);
        mClipper.setFovy(mFovY);
        mClipper.calcViewFrustum();

        _16c = mAspect;
        _170 = 100.0f;
        _174 = mFar;
        _178 = mNear;
    }*/

    void SetTarget() { mTargetBody = mCtrl->getKartBody(mTargetIdx); }
    void SetCameraMode(u8 mode) { mMode = mode; }

    J3DUClipper *GetClipper() { return &mClipper; }
    KartDemoCam *GetDemoCam() { return mDemoCam; }

    MtxPtr GetMtx() { return mMtx; }
    f32 GetVpWidth() { return mVpWidth; }
    f32 GetVpHeight() { return mVpHeight; }
    int GetVpPosv() { return mVpPosV; }
    int GetVpPosh() { return mVpPosH; }
    f32 GetHeight() { return mHeight; } // KartCam.h // 0x801afd7c
    f32 GetWidth() { return mWidth; }   // KartCam.h // 0x801afd84    
    int GetPosv() { return mPosV; }     // KartCam.h // 0x801afd94
    int GetPosh() { return mPosH; }     // KartCam.h // 0x801afd8c
    f32 GetAspect() { return mAspect; }
    f32 GetNear() { return mNear; }
    f32 GetFar() { return mFar; }

    KartBody *getBody() { return mTargetBody; }
    void setVideo(bool video) { _1d8 = video; }

// TODO: figure out what stuff is private(if that even was the case)
//private:
    KartBody *mTargetBody;            //
    KartCtrl *mCtrl;                  //
    J3DUClipper mClipper;             //
    Spline *mPathSpline;              //
    KartDemoCam *mDemoCam;            //
    CrsArea mArea;                    //
    Mtx mMtx;                         //
    u8 _B0[0xE0 - 0xB0];              //
    u8 mTargetIdx;                    // e0
    u8 mMode;                         // e1
    u8 _e2;                           //
    u8 mCamNo;                        // e3
    u8 mPtr;                          // e4
    u8 _e5;                           //
    u8 _e6;                           //
    u8 _e7;                           //
    u16 mFlags;                       // e8
    u16 _ea;                          //
    int mPosV;                        // ec
    int mPosH;                        // f0
    int mVpPosV;                      // f4
    int mVpPosH;                      // f8
    int _fc;                          //
    int _100;                         //
    int _104;                         //
    int _108;                         //
    int _10c;                         //
    int _110;                         //
    int mCurCamIdx;                   // 114
    f32 mStartX;                      // 118
    f32 mEndX;                        // 11c
    f32 mStartY;                      // 120
    f32 mEndY;                        // 124
    f32 mWidth;                       // 128
    f32 mHeight;                      // 12c
    f32 _130;                         //
    f32 _134;                         //
    f32 _138;                         //
    f32 _13c;                         //
    f32 mVpWidth;                     // 140
    f32 mVpHeight;                    // 144
    f32 mFovY;                        // 148
    f32 _14c;                         //
    f32 mNear;                        // 150
    f32 mFar;                         // 154
    f32 _158;                         //
    u8 _15c[0x164 - 0x15c];           //
    f32 _164;                         //
    f32 mAspect;                      // 168
    f32 _16c;                         //
    f32 _170;                         //
    f32 _174;                         //
    f32 _178;                         //
    f32 _17c;                         //
    f32 _180;                         //
    f32 _184;                         // possible matrix?
    f32 _188;                         //
    f32 _18c;                         //
    f32 _190;                         //
    f32 _194;                         //
    f32 _198;                         //
    f32 _19c;                         //
    f32 _1a0;                         //
    f32 _1a4;                         //
    f32 _1a8;                         //
    f32 _1ac;                         //
    f32 _1b0;                         //
    f32 _1b4;                         //
    f32 _1b8;                         //
    f32 _1bc;                         //
    f32 _1c0;                         //
    f32 _1c4;                         //
    f32 _1c8;                         //
    f32 _1cc;                         //
    f32 _1d0;                         //
    ECourseID mCourseID;              // 1d4
    bool _1d8;                        //
    u8 _1d9[0x1dc - 0x1d9];           //
    JGeometry::TVec3f _1dc;           //
    JGeometry::TVec3f mCameraPos;     //
    JGeometry::TVec3f mCameraLookPos; //
    JGeometry::TVec3f _200;           //
    JGeometry::TVec3f _20c;           //
    JGeometry::TVec3f _218;           //
    JGeometry::TVec3f mPathData[4];   //
    JGeometry::TVec3f _254;           //
    JGeometry::TVec3f _260;           //
    u8 _26c[0x294 - 0x26c];           //

}; // 294

class GrafPort : public J2DPerspGraph
{
public:
    GrafPort(int vpX, int vpY, int vpW, int vpH, f32 fovY, f32 near, f32 far);
    /*    : J2DPerspGraph(vpX, vpY, vpW, vpH, fovY, near, far)
    {
        mCamAspect = 1.0f;
        mCamNear = 0.0f;
        mCamFar = 0.0f;
    }*/

    virtual ~GrafPort() {}

    virtual void setPort(); // 14, overide
    /*{
        J2DGrafContext::setPort();
        KartMtxPerspective(mMtx44, mFovY, mCamAspect, mCamNear, mCamFar, RaceMgr::getCurrentManager()->isMirror());
        GXSetProjection(mPosMtx, GX_PERSPECTIVE);
    }*/

    void SetParam(f32 aspect, f32 near, f32 far);
    /*{
        mCamAspect = aspect;
        mCamNear = near;
        mCamFar = far;
    }*/

private:
    f32 mCamAspect; // cc
    f32 mCamNear;   // d0
    f32 mCamFar;    // d4
};

struct ViewData
{
    int _0, _4;
    f32 width;  // 8
    f32 height; // c
    f32 aspect; // 10
    f32 fovY;
    f32 _18;
    f32 _1c;
    f32 _20;
    f32 _24;
}; // 28

struct MiniData
{
    f32 _0;
    f32 _4;
    f32 _8;
    f32 fovY;
}; // 10

extern ViewData viewdata[8];
extern MiniData balloonMinidata[3];
extern MiniData turtleMinidata[3];
extern MiniData bombMinidata[3];
extern MiniData devilMinidata[3];
extern MiniData balloonPakkunMinidata[3];
extern MiniData bombPakkunMinidata[3];
extern MiniData devilPakkunMinidata[3];

#endif // KARTCAMERA_H
