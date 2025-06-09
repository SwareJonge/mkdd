#ifndef GEOSOUNDGEN_H
#define GEOSOUNDGEN_H

#include "Inagaki/GameMapSoundMgr.h"
#include "JSystem/JSupport/JSUList.h"
#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

class GeoSoundGen;

class GeoSoundGenSupervisor : public GeoObjSupervisor, JKRDisposer {
public:
    GeoSoundGenSupervisor();
    virtual ~GeoSoundGenSupervisor(); // 0x801d4ba4
    void reset();
    void calc();
    void entry(GeoSoundGen *);

   
private:
    JSUList<GeoSoundGen> mList;
};

class GeoSoundGen : public GeographyObj {
public:
    GeoSoundGen(const CrsData::SObject &); // 0x801d4c78
    void reset(); // 0x801d4f00
    void createColModel(J3DModelData *); // 0x801d4f44
    void calc(); // 0x801d4f6c
    
    // Unused
    static u16 sCreateNum;
    // Inline
    //virtual ~GeoSoundGen(); // 0x801d4f98
    virtual GeoSoundGenSupervisor *getSupervisor() { return sSupervisor; } // 0x801d5000
private:
    friend class GeoSoundGenSupervisor;
    static GeoSoundGenSupervisor *sSupervisor; // 0x80416590
    
    JSULink<GeoSoundGen> mLink;
    GameAudio::MapSoundObjectMgr *mMapSoundMgr;
}; // class GeoSoundGen

#endif // GEOSOUNDGEN_H
