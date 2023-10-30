#ifndef KARTLOADER_H
#define KARTLOADER_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/J3D/J3DModel.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/ExModel.h"
#include "types.h"

class KartLoader
{
public:
    KartLoader(int, const KartInfo *, u16, bool, bool);
    void reset(); // 0x801a6f08
    void load();  // 0x801a6f58
    void patchTevRegKey(J3DModelData *, J3DAnmTevRegKey *);                    // 0x801a73e0
    void loadTevAnm(int); // 0x801a747c
    void createMaterialAnmInUpdateMaterial(J3DModelData *, J3DAnmTevRegKey *); // 0x801a7774
    void createMaterialAnm(J3DMaterial *);                                     // 0x801a78b8
    void entryTevRegAnimator(int);         // 0x801a7a10
    void removeTevRegAnimator(int);        // 0x801a83b8
    void createModel(JKRSolidHeap *, u32); // 0x801a8598
    void isCommonWheelModelData();         // 0x801a8624

    // Inline/Unused
    // void KartShadowModel::~KartShadowModel();
    // void ShadowModel::~ShadowModel();
    // void ViewCtrlModel::~ViewCtrlModel();
    // ~KartLoader();
    //  Inline
    void setDemoBodyBmd(void *ptr) { mDemoBmd = ptr; }
    bool isShockExist() const; /* { return mShockModels[0].getModelData() != 0; } */ // 0x801a7720
    bool isKartTevAnmEnable(int wheel);                                              // 0x801a8094
    bool isDriverTevAnmEnable(int);                                                  // 0x801a8378
    ExModel *getExModelDriver(int driver)                                            // 0x801a81e8
    {
#line 80
        JUT_MINMAX_ASSERT(0, driver, 2); // line 80
        return mDriverModels[driver];
    }
    ExModel *getExModelBody() { return &mBodyModel; } // 0x801a808c
    int getWheelNumber() { return mWheelNum; }
    ExModel *getExModelWheel(int wheel) // 0x801a7e2c
    {
        JUT_MINMAX_ASSERT(0, wheel, 6); // line 87
        return &mWheelModels[wheel];
    };
    ExModel *getExModelArm(int wheel) // 0x801a7f5c
    {
        JUT_MINMAX_ASSERT(0, wheel, 6); // line 92
        return &mArmModels[wheel];
    }
    ExModel *getExModelShock(int wheel) // 0x801a7c64
    {
        JUT_MINMAX_ASSERT(0, wheel, 6); // line 97
        return &mShockModels[wheel];
    }

    void getBodyTevRegKey(int tevanmID);                    // 0x801a7ff4
    void getArmTevRegKey(int tevanmID);                     // 0x801a7ec4
    void getShockTevRegKey(int tevanmID);                   // 0x801a7bcc
    void getWheelLTevRegKey(int tevanmID);                  // 0x801a7d94
    void getWheelRTevRegKey(int tevanmID);                  // 0x801a7cfc
    void getDriverTevRegKey(int driverNo, int tevanmID);    // 0x801a8280
    void getAccessoryTevRegKey(int driverNo, int tevanmID); // 0x801a80cc

private:
    int mKartNum;
    const KartInfo *mKartInfo;
    void *mDemoBmd;
    ExModel *mDriverModels[2];
    ExModel mBodyModel;
    int mWheelNum;
    ExModel mWheelModels[6];
    ExModel mArmModels[6];
    ExModel mShockModels[6];
    u8 _a7c[0xb7c - 0xa7c];
}; // Size 0xb7c

#endif // KARTLOADER_H