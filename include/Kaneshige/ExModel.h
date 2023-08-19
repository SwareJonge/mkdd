#ifndef EXMODEL_H
#define EXMODEL_H

#include <dolphin/mtx.h>
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "types.h"

// TODO: JSystem
class J3DUClipper;
class J3DModelData;
class J3DGXColorS10;

class ExModel
{
public:
    // Global
    ExModel(s16);                                                                        // 0x801a4450
    ~ExModel();                                                                          // 0x801a4538
    void setModelData(J3DModelData *);                                                   // 0x801a4598
    void setModelData(s16, J3DModelData *);                                              // 0x801a45c0
    void patchModelData(J3DModelData *);                                                 // 0x801a46ec
    void setLODBias(float);                                                              // 0x801a4724
    void setLODBias(J3DModelData *, float);                                              // 0x801a4790
    void disableTexLOD(J3DModelData *);                                                  // 0x801a483c
    void setFogInfo(J3DModelData *, u8, float, float, float, float, JUtility::TColor *); // 0x801a48ac
    void reverseCullMode(J3DModelData *);                                                // 0x801a49f0
    void setLightMask(J3DModelData *, _GXLightID);                                       // 0x801a4abc
    void setTevColor(J3DModelData *, u32, const J3DGXColorS10 &);                        // 0x801a4b88
    void setTevColor(u32, const J3DGXColorS10 &);                                        // 0x801a4c34
    void makeSharedDL();                                                                 // 0x801a4f08
    void lock();                                                                         // 0x801a4f90
    void setShapePktControl(s16);                                                        // 0x801a4fec
    void isAllShapePacketHidding(s16, u32);                                              // 0x801a5110
    void setCurMtx();                                                                    // 0x801a523c
    void calcBBoardMtx(Mtx44, Mtx44);                                                    // 0x801a56f8
    void simpleDraw(u32, Mtx44, u32);                                                    // 0x801a58ac
    void show();                                                                         // 0x801a5d6c
    void hide();                                                                         // 0x801a5e98
    void clipAll(u32, bool);                                                             // 0x801a5fc8
    void clipBySphere(u32, J3DUClipper *, const Mtx44, float);                           // 0x801a6040
    void clipJoint(u32, s16, bool);                                                      // 0x801a6254
    void setBaseTRMtx(Mtx44);                                                            // 0x801a63c8
    void setBaseScale(JGeometry::TVec3f &);                                              // 0x801a6474
    void tstDiffTexMtxMode();                                                            // 0x801a6550
    void setEffectMtx(Mtx44, u32);                                                       // 0x801a65a4

    // Vtable
    virtual void createModel(JKRSolidHeap *, u32, u32);               // 0x801a4c70
    virtual void createDifferedModel(JKRSolidHeap *, u32, u32, bool); // 0x801a4e6c
    virtual void calc();                                              // 0x801a5288
    virtual void calc(s16);                                           // 0x801a52b8
    virtual void update(s16);                                         // 0x801a5434
    virtual void viewCalc(u32) {}                                     // 0x801a6824
    virtual void setCurrentViewNo(u32);                               // 0x801a5614

    static bool isMtxCombinationOn() { return sMtxCombinationOn; };
    Mtx *getBaseTRMtx()
    {
        return &mBaseTRMtx;
    }

    static bool sClippingOn;           // 0x80414610
    static bool sMtxCombinationOn;     // 0x80416398
    static bool sDrawingOnlyUpdZMat;   // 0x80416399
    static f32 sExModelInvalidLODBias; // 0x80419d90

private:
    u8 _4[0x2c - 0x4];
    Mtx mBaseTRMtx;
};

class ExMDRecord;

class ExMdlDataMgr : JKRDisposer
{
public:
    ExMdlDataMgr(bool);

private:
    JSUList<ExMDRecord> mList;
    bool _18;
};

#endif // EXMODEL_H
