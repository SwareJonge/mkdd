#ifndef EXMODEL_H
#define EXMODEL_H

#include "JSystem/J3DU/J3DUClipper.h"
#include "JSystem/JUtility/TColor.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"

#include "JSystem/J3D/J3DModel.h"

#include "types.h"

class ExModel
{
public:
    // Global
    ExModel(u16 id = 1);                                                                // 0x801a4450
    ~ExModel();                                                                         // 0x801a4538
    void setModelData(J3DModelData *);                                                  // 0x801a4598
    void setModelData(u16, J3DModelData *);                                             // 0x801a45c0
    static void patchModelData(J3DModelData *);                                         // 0x801a46ec
    void setLODBias(f32);                                                               // 0x801a4724
    static void setLODBias(J3DModelData *, f32);                                               // 0x801a4790
    static void disableTexLOD(J3DModelData *);                                                 // 0x801a483c
    static void setFogInfo(J3DModelData *, u8, f32, f32, f32, f32, JUtility::TColor *); // 0x801a48ac
    static void reverseCullMode(J3DModelData *);                                               // 0x801a49f0
    static void setLightMask(J3DModelData *, _GXLightID);                                      // 0x801a4abc
    static void setTevColor(J3DModelData *, u32, const J3DGXColorS10 &);                       // 0x801a4b88
    void setTevColor(u32, const J3DGXColorS10 &);                                       // 0x801a4c34
    void makeSharedDL();                                                                // 0x801a4f08
    void lock();                                                                        // 0x801a4f90
    void setShapePktControl(u16);                                                       // 0x801a4fec
    bool isAllShapePacketHidding(u16 level, u32 viewNo);                                             // 0x801a5110
    void setCurMtx();                                                                   // 0x801a523c
    static void calcBBoardMtx(Mtx, Mtx);                                                       // 0x801a56f8
    void simpleDraw(u32, Mtx, u32);                                                     // 0x801a58ac
    void show();                                                                        // 0x801a5d6c
    void hide();                                                                        // 0x801a5e98
    void clipAll(u32, bool);                                                            // 0x801a5fc8
    void clipBySphere(u32, J3DUClipper *, const Mtx, f32);                              // 0x801a6040
    void clipJoint(u32, u16, bool);                                                     // 0x801a6254
    void setBaseTRMtx(Mtx);                                                             // 0x801a63c8
    void setBaseScale(JGeometry::TVec3f &);                                             // 0x801a6474
    bool tstDiffTexMtxMode();                                                           // 0x801a6550
    void setEffectMtx(Mtx, u32);                                                        // 0x801a65a4

    // Vtable
    virtual bool createModel(JKRSolidHeap *, u32, u32);               // 0x801a4c70
    virtual bool createDifferedModel(JKRSolidHeap *, u32, u32, bool); // 0x801a4e6c
    virtual void calc();                                              // 0x801a5288
    virtual void calc(u16);                                           // 0x801a52b8
    virtual void update(u16);                                         // 0x801a5434
    virtual void viewCalc(u32) {}                                     // 0x801a6824
    virtual void setCurrentViewNo(u32);                               // 0x801a5614

    // Inlines
    const Mtx &getBaseTRMtx() { return mBaseTRMtx; }
    void hide(u32 p1) { clipAll(p1, false); }
    void show(u32 p1) { clipAll(p1, true); }
    bool isAllShapePacketHidding(u32 viewNo) { return isAllShapePacketHidding(0, viewNo); }
    J3DModelData *getModelData() const { return mModelData[0]; }
    J3DModel *getModel() const { return mModel[0]; }
    void setSimpleTevReg(u32 id) { mSimpleTevReg |= (1 << id); }
    void simpleDraw(u32 viewNo) { simpleDraw(viewNo, nullptr, 1); }

    bool isAvailable() const { return mModelData[0] != nullptr; }
    bool tstSimpleTevReg(u32 id) const { return (mSimpleTevReg & (1 << id)) != 0; }
    bool tstConcatViewMode() const { return mModelData[0]->checkFlag(0x10); }
    bool tstKartBBoardOn() { return (_18 & 2) != 0; }

    J3DModelData *getModelData(u16 level) const {
#line 188
        JUT_MINMAX_ASSERT(0, level, mLevelCnt)
        return mModelData[level];
    }

    static bool isMtxCombinationOn() { return sMtxCombinationOn; };
    static f32 getInvalidLODBias() { return sExModelInvalidLODBias; }

    static bool sClippingOn;           // 0x80414610
    static bool sMtxCombinationOn;     // 0x80416398
    static bool sDrawingOnlyUpdZMat;   // 0x80416399
    static const f32 sExModelInvalidLODBias; // 0x80419d90

protected:
    // Vtable 0x0
    u16 mLevelCnt;             // 04
    J3DModelData **mModelData; // 08
    J3DModel **mModel;         // 0C
    u32 mMagic;                // 10
    int _14;                   // 
    u16 _18;                   // 
    u16 _1a;                   //
    u16 mSimpleTevReg;         // 1C
    JGeometry::TVec3f mScale;  // 20
    Mtx mBaseTRMtx;            // 2C
    Mtx _5c;                   // 5C
}; // Size: 0x8c

class ExMDRecord {
public:
    ExMDRecord(J3DModelData *mdlData);

    void patch(bool mirror);

    JSULink<ExMDRecord> mRecordLink;
    J3DModelData *mModelData;
};

class ExMdlDataMgr : JKRDisposer
{
public:
    ExMdlDataMgr(bool);
    ~ExMdlDataMgr();

    void entryModelData(J3DModelData*);
    ExMDRecord *searchRecord(J3DModelData *);

    static ExMdlDataMgr *getManager() { return sManager; }
private:
    static ExMdlDataMgr *sManager;

    bool mIsMirror;
    JSUList<ExMDRecord> mRecordList;
};

#endif // EXMODEL_H
