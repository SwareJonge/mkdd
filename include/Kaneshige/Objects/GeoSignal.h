#ifndef GEOSIGNAL_H
#define GEOSIGNAL_H

#include "JSystem/JSupport/JSUList.h"
#include "Sato/GeographyObj.h"
#include "Sato/J3DAnmObject.h"
#include "Sato/StateObserver.h"

class GeoSignal;

class GeoSignalSupervisor : public GeoObjSupervisor, JKRDisposer
{
public:
    GeoSignalSupervisor();

    void entrySignal(GeoSignal *);
    void searchSameGroupSignal(GeoSignal *);
    GeoSignal *searchSignal(s32);

    virtual void reset();
    virtual void calc();
    virtual ~GeoSignalSupervisor(); // 0x801be2a8, overide
private:
    JSUList<GeoSignal> mList;
}; // class GeoSignalSupervisor

class GeoSignal : public GeographyObj, StateObserver
{
public:
    GeoSignal(const CrsData::SObject &);           // 0x801be37c
    virtual void reset();                          // 0x801be57c
    virtual void loadAnimation();                  // 0x801be5ec
    virtual void createColModel(J3DModelData *);   // 0x801be63c
    virtual void createModel(JKRSolidHeap *, u32); // 0x801be664
    virtual void simpleDraw(u32);                  // 0x801be6cc
    virtual void calc();                           // 0x801be718
    virtual void update();                         // 0x801be764
    void initFuncWait();                           // 0x801be910
    void doFuncWait();                             // 0x801be914
    
    // Inline/Unused
    bool isSameGroup(GeoSignal *) const;
    bool isSameID(s32) const;
    // void sGreenTime;
    // void sLagTime;
    // void sRedAnmFrame;
    // void sG2RAnmRate;
    // void sR2GAnmRate;
    // void sCreateNum;
    // void sGreenAnmFrame;

    virtual void InitExec(); // 0x801be7a8, overide
    void MoveExec(); // 0x801be8bc, overide
    //virtual ~GeoSignal() {}                                         // 0x801be918, overide
    virtual const char *getBmdFileName() { return "/Objects/Signal1.bmd"; }      // 0x801be9a0, overide
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; } // 0x801be9ac, overide
    virtual GeoSignalSupervisor *getSupervisor() { return sSupervisor; }
    static GeoSignalSupervisor *getSupervisor() { return sSupervisor; }
private:
    friend class GeoSignalSupervisor;

    static StateFuncSet<GeoSignal> sTable[1]; // 0x80396438
    static u16 sSupervisorCreateNum;          // 0x80416408
    static GeoSignalSupervisor *sSupervisor;  // 0x8041640c
    static J3DAnmTevRegKey *sTevRegKey;       // 0x80416410
    
    JSULink<GeoSignal> mLink;
    bool _168;
    u8 _169[0x174 - 0x169];
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mLfDir;
    J3DAnmObjMaterial mMat;
}; // class GeoSignal

#endif // GEOSIGNAL_H
