#ifndef GEOTIDESENSOR_H
#define GEOTIDESENSOR_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "Sato/GeographyObj.h"

class GeoTideSensor;

#define MAX_NUM_SENSORS 2

class GeoTideSensorSupervisor : public GeoObjSupervisor, JKRDisposer {
public:
    GeoTideSensorSupervisor();
    virtual ~GeoTideSensorSupervisor(); // 0x801d2420

    void reset();
    void calc();
    void entry(GeoTideSensor *);
    GeoTideSensor *searchNearSensor(int, const JGeometry::TVec3f &, f32);
private:
    JSUList<GeoTideSensor> mList;
    GameAudio::ShoreSoundMgr *mShoreSoundMgr;
    s16 _4c;
    GeoTideSensor *mSensors[MAX_NUM_SENSORS];
};

class GeoTideSensor : public GeographyObj {
public:
    GeoTideSensor(const CrsData::SObject &); // 0x801d2884
    void reset(); // 0x801d2990
    void createColModel(J3DModelData *); // 0x801d29d4
    void calc(); // 0x801d29d8

    // Inline/Unused
    void drawDebugModel(unsigned long);
    static s16 sCreateNum;
    static bool sDrawDummyModel;
    // Inline
    bool isInside(f32 h) const { return mPos.y <= h; } // 0x801d2804
    bool isSameGroup(s16 groupId) const { return mObjData->mParam1 == groupId; } // 0x801d281c
    //virtual ~GeoTideSensor(); // 0x801d29dc
    virtual GeoTideSensorSupervisor *getSupervisor() { return sSupervisor; } // 0x801d2a44
private:
    friend class GeoTideSensorSupervisor;

    static f32 sSearchDistance; // 0x80414798
    static GeoTideSensorSupervisor * sSupervisor; // 0x80416568

    JSULink<GeoTideSensor> mLink;
};

#endif // GEOTIDESENSOR_H
