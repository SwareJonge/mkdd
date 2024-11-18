#ifndef SIUTIL_H
#define SIUTIL_H

#include <JSystem/JGeometry.h>
#include "Sato/GeographyObj.h"
#include "types.h"

// Screen Interface?
namespace SiUtil
{ // Autogenerated
    // Global
    bool checkPassFrame(f32, f32, f32);                                                                                  // 0x802a7038
    void getViewport(JGeometry::TBox2f *, u8, u8);                                                                       // 0x802a709c
    void getViewport(JGeometry::TBox2f *, u8);                                                                           // 0x802a71ec
    int getScreenPosCheckMirror(int, const JGeometry::TVec3f &, JGeometry::TVec2f *);                                   // 0x802a7290
    int getScreenPos(int, const JGeometry::TVec3f &, JGeometry::TVec2f *, bool);                                        // 0x802a72cc
    void setItemReaction(const void *, GeographyObj *);                                                                  // 0x802a7474
    void clampVec(const JGeometry::TVec3f &, JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &); // 0x802a7644
    f32 getNormalRange(f32, f32, f32);                                                                                  // 0x802a76cc
    u8 getConsoleNum();                                                                                                // 0x802a76f8
    u8 getDrawConsoleNum();                                                                                            // 0x802a7790
    f32 getCourseHeight(JGeometry::TVec3f &);                                                                           // 0x802a7828
    s16 searchNearKartBall(const JGeometry::TVec3f &, f32);                                                             // 0x802a7874
    s16 searchNearKartCircle(const JGeometry::TVec3f &, f32);                                                           // 0x802a7950
    s16 setRandomStartFrame(J3DFrameCtrl *frameCtrl, stRandom *rand);                                                            // 0x802a7a30
    // Inline/Unused
    void getMultiple(u16, u16, bool);
    void getBufferImage(u8 *, u16, u16, u16, u16, bool);
    void drawBufferImage(_GXTexObj *, short, short, u16, u16);
    void initGX(JUtility::TColor);
    f32 getWaterHeight(JGeometry::TVec3f &);
    void searchNearKartCylinder(const JGeometry::TVec3f &, f32, f32);
}; // namespace SiUtil

#endif // SIUTIL_H
