#ifndef GEOVISION_H
#define GEOVISION_H

#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "types.h"

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

class GeoVision;

class GeoVisionSupervisor : public GeoObjSupervisor, JKRDisposer {
public:
    GeoVisionSupervisor();
    void reset();
    void calc();
    void entry(GeoVision *);
    void capture();

    virtual ~GeoVisionSupervisor() {}
    JUTTexture *getCaptureTexture() { return mCaptureTex; }
    bool isCaptureEnable() { return mCaptureTex != nullptr; }

    // Unused
    //void cCaptureWidth;
    //void cCaptureHeight;
private: // TODO
    JUTTexture *mCaptureTex;
};

class GeoVision : public GeographyObj, StateObserver  { // Autogenerated
public:
    GeoVision(const CrsData::SObject &);   // 0x801c5e1c
    void reset();                          // 0x801c5f6c
    void createModel(JKRSolidHeap *, u32); // 0x801c6070
    void calc();                           // 0x801c6324
    void initFuncWait();                   // 0x801c64d0
    void doFuncWait();                     // 0x801c64d4

    // Unused
    //void sMonTime;
    //void sCreateNum;

    // Inline
    static GeoVisionSupervisor *getGeoVisionSupervisor() { return sSupervisor; }
    void InitExec() { Observer_FindAndInit(GeoVision, 1); }
    void MoveExec() { Observer_FindAndExec(GeoVision, 1); }
    virtual ~GeoVision() {} 
    virtual const char * getBmdFileName() { return "/Objects/Wl_Screen1.bmd"; }
    virtual GeoVisionSupervisor *getSupervisor() { return sSupervisor; }
    void initClassCreateNum() { sSupervisorCreateNum = 0; }

    friend class GeoVisionSupervisor;
private:
    static StateFuncSet<GeoVision> sTable[1]; // 0x80397180
    static u16 sSupervisorCreateNum;          // 0x80416470
    static GeoVisionSupervisor *sSupervisor;  // 0x80416474

    // TODO
}; // class GeoVision

#endif // GEOVISION_H
