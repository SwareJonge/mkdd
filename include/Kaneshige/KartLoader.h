#ifndef KARTLOADER_H
#define KARTLOADER_H

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "Kaneshige/ExModel.h"
#include "types.h"

class KartLoader {
public:
    KartLoader(int, const KartInfo *, u16, bool, bool);
    void reset();                                                              // 0x801a6f08
    void load();                                                               // 0x801a6f58
    //void patchTevRegKey(J3DModelData *, J3DAnmTevRegKey *);                    // 0x801a73e0
    void loadTevAnm(int);                                                      // 0x801a747c
    //void createMaterialAnmInUpdateMaterial(J3DModelData *, J3DAnmTevRegKey *); // 0x801a7774
    //void createMaterialAnm(J3DMaterial *);                                     // 0x801a78b8
    void entryTevRegAnimator(int);                                             // 0x801a7a10
    void removeTevRegAnimator(int);                                            // 0x801a83b8
    void createModel(JKRSolidHeap *, u32);                                     // 0x801a8598
    void isCommonWheelModelData();                                             // 0x801a8624

    // Inline/Unused
    // void KartShadowModel::~KartShadowModel();
    // void ShadowModel::~ShadowModel();
    // void ViewCtrlModel::~ViewCtrlModel();
    // ~KartLoader();
    //  Inline
    bool isShockExist() const;                              // 0x801a7720
    void getShockTevRegKey(int);                            // 0x801a7bcc
    void getExModelShock(int);                              // 0x801a7c64
    void getWheelRTevRegKey(int);                           // 0x801a7cfc
    void getWheelLTevRegKey(int);                           // 0x801a7d94
    void getExModelWheel(int);                              // 0x801a7e2c
    void getArmTevRegKey(int);                              // 0x801a7ec4
    void getExModelArm(int);                                // 0x801a7f5c
    void getBodyTevRegKey(int);                             // 0x801a7ff4
    ExModel *getExModelBody(); /* { return mModelBody;  }*/ // 0x801a808c
    bool isKartTevAnmEnable(int);                           // 0x801a8094
    void getAccessoryTevRegKey(int, int);                   // 0x801a80cc
    void getExModelDriver(int);                             // 0x801a81e8
    void getDriverTevRegKey(int, int);                      // 0x801a8280
    bool isDriverTevAnmEnable(int);                         // 0x801a8378
    void setDemoBodyBmd(void *);

private:
    u8 _0[0x14];
    ExModel mModelBody;
};

#endif // KARTLOADER_H