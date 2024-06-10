#ifndef OBJUTILITY_H
#define OBJUTILITY_H

#include <JSystem/JGeometry.h>

#include "types.h"

namespace ObjUtility
{
    void *getPtrMainArc(char *);                                                             // 0x8025dc08
    void *getPtrCourseArc(char *);                                                          // 0x8025dc30
    void getPtrMainLocaleArc(char *);                                                       // 0x8025dc58
    void setRMtx(Mtx, const JGeometry::TVec3f &, const JGeometry::TVec3f &);                // 0x8025dc80
    void executeShakeCamera(const JGeometry::TVec3f &, f32, f32, f32);                      // 0x8025dcf4
    Mtx &getKartBodyMtx(int);                                                               // 0x8025de7c
    void getKartPos(int, JGeometry::TVec3f *);                                              // 0x8025df6c
    void getKartXdir(int, JGeometry::TVec3f *);                                             // 0x8025e0cc
    void getKartYdir(int, JGeometry::TVec3f *);                                             // 0x8025e234
    void getKartZdir(int, JGeometry::TVec3f *);                                             // 0x8025e39c
    void getKartVel(int, JGeometry::TVec3f *);                                              // 0x8025e504
    void getKartEfctVel(int, JGeometry::TVec3f *);                                          // 0x8025e5a0
    void findNearKart(const JGeometry::TVec3f &, f32, int);                                 // 0x8025e63c
    void findNearKartAngle(const JGeometry::TVec3f &, const JGeometry::TVec3f &, f32, f32); // 0x8025e868
    void getCamDependLightMtx(u32, const JGeometry::TVec3f &, Mtx);                         // 0x8025ec6c
    bool isPlayerDriver(const int &);                                                       // 0x8025edb0
    void getReflectVec(JGeometry::TVec3f, JGeometry::TVec3f, JGeometry::TVec3f *, bool);    // 0x8025eeb4
    void chkAvailableEnemyByDistande(bool *, f32);                                          // 0x8025f118
    void concatSimpleFileName(char *, u8);                                                  // 0x8025f514
}

#endif