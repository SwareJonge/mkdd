#ifndef OSAGE_H
#define OSAGE_H

#include <JSystem/J3D/J3DJoint.h>
#include "JSystem/JKernel/JKRHeap.h"

#include "Kawano/driver.h"

typedef void (*HairCallBack)(J3DJoint *joint, int);

class Osage {
    Osage(DriverModel *);
    void update(u16);
    void createModel(JKRSolidHeap *, u32, u32);
    void setCurrentViewNo(u32);
    void kHairRootCallBack(J3DJoint *joint, int);
    void kHairL1CallBack(J3DJoint *joint, int);
    void kHairL2CallBack(J3DJoint *joint, int);
    void kHairL3CallBack(J3DJoint *joint, int);
    void kHairR1CallBack(J3DJoint *joint, int);
    void kHairR2CallBack(J3DJoint *joint, int);
    void kHairR3CallBack(J3DJoint *joint, int);
    // Inline/Unused
    //void updateOffset();

    //static void sCurDriver;
    //static void sCurHair_L;
    //static void sCurHair_R;
    static Osage *sCurOsage;

}; 
// Outside class members

class OsageMgr {
public:
    OsageMgr() {}

    void init(DriverModel *);
    void setCallBack(int);
    void createModel(DriverModel *, JKRSolidHeap *, u32, u32);
    void setCurrentViewNo(DriverModel *, u32);
    void CheckNoIK();
    int IsOsageExist(DriverModel *);

    static OsageMgr *ptr();

    // Inline/Unused
    void reset();
    void update();
    void update(DriverModel *);

    static const char* mscaaJntName[7];
    static HairCallBack mscaCallBackTbl[7];
    static const char* mscaOsageBmdFileName;

    static u32 mNoIKFlag;
    static OsageMgr *mspOsageMgr;
};

 #endif // OSAGE_H