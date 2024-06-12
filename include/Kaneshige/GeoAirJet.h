#ifndef GEOAIRJET_H
#define GEOAIRJET_H

#include <JSystem/JParticle/JPAEmitter.h>

#include "Kaneshige/Course/CrsData.h"
#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

class GeoAirJet : public GeographyObj, public StateObserver
{
public:
    GeoAirJet(const CrsData::SObject &); // 0x801cca0c
    void initFuncWait();                 // 0x801ccdbc
    void doFuncWait();                   // 0x801ccdc0

    // Vtable
    virtual ~GeoAirJet() {}                      // 0x801ccdc4
    virtual void reset();                        // 0x801cca84
    virtual void calc();                         // 0x801ccbcc
    virtual void createColModel(J3DModelData *); // 0x801ccb90
    virtual void InitExec();                     // 0x801ccc54
    virtual void MoveExec();                     // 0x801ccd68

    static StateObserver::StateFuncSet<GeoAirJet> sTable[1]; // 0x803978d8

    // Why is this swapped? rpy, ypr thing?
    JGeometry::TVec3f mDirZ;
    JGeometry::TVec3f mDirY;
    JGeometry::TVec3f mDirX;
    JPABaseEmitter *mEmt;
}; // Size: 0x180

#endif // GEOAIRJET_H