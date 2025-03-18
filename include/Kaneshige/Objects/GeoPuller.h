#ifndef GEOPULLER_H
#define GEOPULLER_H

#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "Kaneshige/Course/CrsData.h"
#include "Sato/GeographyObj.h"

class GeoPuller;

class GeoPullerSupervisor : public GeoObjSupervisor, JKRDisposer {
public:
    GeoPullerSupervisor();

    virtual ~GeoPullerSupervisor(); // 0x801d3ae0

    void reset();
    void calc();
    void entry(GeoPuller *link);
    GeoPuller *search(u32);

private:
    JSUList<GeoPuller> mList;
};

class GeoPuller : public GeographyObj { // Autogenerated
public:
    GeoPuller(const CrsData::SObject &); // 0x801d3da8
    void reset(); // 0x801d3f9c
    void createColModel(J3DModelData *); // 0x801d3fe0
    void calc(); // 0x801d4008
    void getVelocity(JGeometry::TVec3f *, const JGeometry::TVec3f &); // 0x801d40d0

    // Inline
    bool isSameID(u32 id) const { return mObjData->mParam1 == (s16)id; } // 0x801d3d40
    virtual ~GeoPuller() {} // 0x801d4170

    virtual GeoPullerSupervisor *getSupervisor() { return sSupervisor; }
    static GeoPullerSupervisor *getSupervisor() { return sSupervisor; }

    friend class GeoPullerSupervisor;
private:
    static GeoPullerSupervisor *sSupervisor; // 0x80416580
    // Unused
    static int sCreateNum;

    JSULink<GeoPuller> mPullerLink;
    f32 _15c;
    f32 _160;
    f32 _164;
}; // class GeoPuller

#endif // GEOPULLER_H
