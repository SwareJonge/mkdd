#ifndef KARTPERCAM_H
#define KARTPERCAM_H

#include <JSystem/JGeometry/Vec.h>
#include "Kaneshige/Course/CrsArea.h"
#include "Yamamoto/kartLine.h"
#include "types.h"

class KartBody;

class KartPerCam
{
public:
    KartPerCam() {}

    void Init(int);              // 0x80315160
    void InitFixCam();           // 0x803151d4
    void InitFixSearchCam();     // 0x803152d0
    void InitFixChaseCam();      // 0x80315364
    void InitChaseCam();         // 0x803154b4
    void InitFixPathCam();       // 0x803155a8
    void InitChasePathCam();     // 0x803156b0
    void InitFixSplCam();        // 0x80315780
    void InitChaseSplCam();      // 0x803158d8
    void InitFixPalaCam();       // 0x80315a00
    void InitDemoPathCam();      // 0x80315cfc
    void InitDemoParallelView(); // 0x80315dbc
    void InitID();               // 0x80315e3c
    // Inline/Unused
    void ChaseFovy();
    void DoFixCam();
    void DoFixSearchCam();
    void DoFixChaseCam();
    void DoChaseCam();
    void DoFixPathCam();
    void DoChasePathCam();
    void DoFixSplCam();
    void DoChaseSplCam();
    void DoFixPalaCam();
    void DoDemoPathCam();
    void DemoParallelView();
    void InitDemoGoalView();
    void DemoGoalView();
    void DemoView();
private:
    KartBody *mBody;          //
    Mtx mPalaCamMtx;          //
    Spline *_34;              //
    CrsArea mArea;            // 14
    u16 _4c;                  //
    u8 _4e;                   //
    u8 _4f;                   //
    u8 _50;                   //
    u8 _51[0x52 - 0x51];      //
    u16 _52;                  //
    u16 _54;                  //
    u8 _56[0x58 - 0x56];      //
    int mCamId;               // 58
    f32 mFovY;                // 5c
    u8 _60[0x88 - 0x60];      //
    f32 _88;                  // length of vector b8
    f32 _8c;                  //
    u8 _90[0x94 - 0x90];      //
    JGeometry::TVec3f _94;    //
    JGeometry::TVec3f _a0;    //
    JGeometry::TVec3f mPos;   // ac
    JGeometry::TVec3f _b8;    //
    JGeometry::TVec3f _c4;    //
    JGeometry::TVec3f _d0[4]; //
    u8 _100[0x160 - 0x100];   //
};
#endif // KARTPERCAM_H