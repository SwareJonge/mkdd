#ifndef KARTLOADER_H
#define KARTLOADER_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/KartInfo.h"
#include "Kawano/driver.h"
#include "Osako/shadowModel.h"
#include "types.h"

class KartLoader
{
public:
    KartLoader(int, const KartInfo *, u16, bool, bool);                               // 0x801a6c30
    void reset();                                                                     // 0x801a6f08
    void load();                                                                      // 0x801a6f58
    static void patchTevRegKey(J3DModelData *, J3DAnmTevRegKey *);                    // 0x801a73e0
    void loadTevAnm(int effNo);                                                       // 0x801a747c
    static void createMaterialAnmInUpdateMaterial(J3DModelData *, J3DAnmTevRegKey *); // 0x801a7774
    static void createMaterialAnm(J3DMaterial *mat);                                  // 0x801a78b8
    void entryTevRegAnimator(int);                                                    // 0x801a7a10
    void removeTevRegAnimator(int);                                                   // 0x801a83b8
    void createModel(JKRSolidHeap *, u32);                                            // 0x801a8598
    bool isCommonWheelModelData();                                                    // 0x801a8624

    // Inline/Unused
    // ~KartLoader();
    //  Inline
    void setDemoBodyBmd(void *ptr) { mDemoBmd = ptr; }
    bool isOsageExist(int driver) { return getExModelOsage(driver) != nullptr; }
    bool isShockExist() const { return mShockModels[0].getModelData() != 0; } // 0x801a7720
    bool isKartTevAnmEnable(int tevanmID) {
        if (tevanmID < 0)
            return false;
        return getBodyTevRegKey(tevanmID) != nullptr;    
    }
    bool isDriverTevAnmEnable(int tevanmID) {
        if (tevanmID < 0)
            return false;
        return getDriverTevRegKey(0, tevanmID) != nullptr;
    }
    
    DriverModel *getExModelDriver(int driver) {
#line 80
        JUT_MINMAX_ASSERT(0, driver, 2); // line 80
        return mDriverModels[driver];
    }
    
    ExModel *getExModelBody() { return &mBodyModel; }
    int getWheelNumber() { return mWheelNum; }
    ExModel *getExModelWheel(int wheel) {
        JUT_MINMAX_ASSERT(0, wheel, 6); // line 87
        return &mWheelModels[wheel];
    }

    ExModel *getExModelArm(int wheel) {
        JUT_MINMAX_ASSERT(0, wheel, 6); // line 92
        return &mArmModels[wheel];
    }

    ExModel *getExModelShock(int wheel) {
        JUT_MINMAX_ASSERT(0, wheel, 6); // line 97
        return &mShockModels[wheel];
    }

    ExModel *getExModelOsage(int driver) {
        JUT_MINMAX_ASSERT(0, driver, 2); // line 102
        return mOsageModel[driver];
    }

    J3DAnmTevRegKey *getDriverTevRegKey(int driverNo, int tevanmID)  {
#line 168
        JUT_MINMAX_ASSERT(0, driverNo, 2); // line 168
        JUT_MINMAX_ASSERT(0, tevanmID, 2); // line 169
        return mDriverTevRegKey[driverNo][tevanmID];
    }

    J3DAnmTevRegKey *getAccessoryTevRegKey(int driverNo, int tevanmID) {
        JUT_MINMAX_ASSERT(0, driverNo, 2); // line 174
        JUT_MINMAX_ASSERT(0, tevanmID, 2); // line 175
        return mAccessoryTevRegKey[driverNo][tevanmID];
    }

    J3DAnmTevRegKey *getBodyTevRegKey(int tevanmID) {
        JUT_MINMAX_ASSERT(0, tevanmID, 2); // line 180
        return mBodyTevRegKey[tevanmID];
    }

    J3DAnmTevRegKey *getArmTevRegKey(int tevanmID) {
        JUT_MINMAX_ASSERT(0, tevanmID, 2); // line 185
        return mArmTevRegKey[tevanmID];
    }

    J3DAnmTevRegKey *getShockTevRegKey(int tevanmID) {
        JUT_MINMAX_ASSERT(0, tevanmID, 2); // line 190
        return mShockTevRegKey[tevanmID];
    }

    J3DAnmTevRegKey *getWheelLTevRegKey(int tevanmID) {
        JUT_MINMAX_ASSERT(0, tevanmID, 2); // line 195
        return mWheelLTevRegKey[tevanmID];
    }

    J3DAnmTevRegKey *getWheelRTevRegKey(int tevanmID) {
        JUT_MINMAX_ASSERT(0, tevanmID, 2); // line 200
        return mWheelRTevRegKey[tevanmID];
    }
private:
    int mKartNum;                               // 0
    const KartInfo *mKartInfo;                  // 4
    void *mDemoBmd;                             // 8
    DriverModel *mDriverModels[2];              // 0c
    ExModel mBodyModel;                         // 14
    int mWheelNum;                              // a0
    ExModel mWheelModels[6];                    // a4
    ExModel mArmModels[6];                      // 3ec
    ExModel mShockModels[6];                    // 734
    ExModel *mOsageModel[2];                    // a7c
    KartShadowModel mShadowModel;               // a84
    J3DAnmTevRegKey *mDriverTevRegKey[2][2];    // b34
    J3DAnmTevRegKey *mAccessoryTevRegKey[2][2]; // b44
    J3DAnmTevRegKey *mBodyTevRegKey[2];         // b54
    J3DAnmTevRegKey *mArmTevRegKey[2];          // b5c
    J3DAnmTevRegKey *mShockTevRegKey[2];        // b64
    J3DAnmTevRegKey *mWheelLTevRegKey[2];       // b6c
    J3DAnmTevRegKey *mWheelRTevRegKey[2];       // b74
}; // Size 0xb7c

#endif // KARTLOADER_H
