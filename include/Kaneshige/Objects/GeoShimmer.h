#ifndef GEOSHIMMER_H
#define GEOSHIMMER_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "types.h"

#include "Sato/GeographyObj.h"

class GeoShimmer;

class GeoShimmerSupervisor : public GeoObjSupervisor, JKRDisposer {
public:
    GeoShimmerSupervisor();
    void reset();
    void calc();
    void entry(GeoShimmer *);
    void drawScreen();
    virtual ~GeoShimmerSupervisor() {}
};

class GeoShimmer : public GeographyObj { // Autogenerated
public:
    GeoShimmer(const CrsData::SObject &); // 0x801d4434
    void setCrsOfsZ0();                   // 0x801d4798
    void setCrsOfsZ1();                   // 0x801d47c8
    void reset();                         // 0x801d47f8
    void createColModel(J3DModelData *);  // 0x801d483c
    void calc();                          // 0x801d4840
    void drawScreen();                    // 0x801d493c

    // Inline
    bool isShimmer() const; // 0x801d4784
    virtual ~GeoShimmer() {}
    virtual GeoShimmerSupervisor *getSupervisor() { return sSupervisor;  }
    static GeoShimmerSupervisor *getSupervisor() { return sSupervisor;  }

    friend class GeoShimmerSupervisor;
private:
    static GeoShimmerSupervisor *sSupervisor; // 0x80416588
    static u16  sCreateNum;                   // UNUSED
}; // class GeoShimmer

#endif // GEOSHIMMER_H
