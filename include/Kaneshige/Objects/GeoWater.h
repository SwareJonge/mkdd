#ifndef GEOWATER_H
#define GEOWATER_H

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

class GeoWater : public GeographyObj, StateObserver
{
public:
    GeoWater(const CrsData::SObject &);            // 0x801c45f8
    virtual void reset();                          // 0x801c4780
    virtual void loadAnimation();                  // 0x801c49b4
    virtual void createModel(JKRSolidHeap *, u32); // 0x801c4a08
    virtual void simpleDraw(u32);                  // 0x801c4a70
    virtual void update();                         // 0x801c4abc
    virtual void calc();                           // 0x801c4adc
    void getSurfaceHeight();                       // 0x801c4c90
    void initFuncWait();                           // 0x801c4c98
    void doFuncWait();                             // 0x801c4c9c

    // Inline/Unused
    // void sCreateNum;
    // Inline
    bool isSameID(long id) const {/*return d == _e9->_30; */}; // 0x801c4594
    void InitExec()
    { /*__ptmf_scall(this, FindStateFunc(getState()), &sTable, 1);*/
    } // 0x801c4b28
    void MoveExec()
    {                                                                  /*__ptmf_scall(this, FindStateFunc(getState()), &sTable, 1);*/
    }                                                                  // 0x801c4c3c
    virtual ~GeoWater() {}                                             // 0x801c4e94
    virtual char *getBmdFileName() { return "/Objects/sea1_spc.bmd"; } // 0x801c4f1c
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; }    // 0x801c4f28
private:
    // void sTable;                          // 0x80396f88
    static f32 sStartAngle;               // 0x804146d8
    static u16 sSupervisorCreateNum;      // 0x80416460
    static GeoObjSupervisor *sSupervisor; // 0x80416464
    static J3DAnmTextureSRTKey *sBtkAnm;  // 0x80416468
    // TODO
}; // class GeoWater

class GeoWaterSupervisor : public GeoObjSupervisor
{
public:
    GeoWaterSupervisor();
    virtual ~GeoWaterSupervisor(); // 0x801c4378
    virtual void reset();
    virtual void calc();
    virtual void entry(GeoWater *);
    virtual void searchWater(long);
};

class GeoSubWater : public GeographyObj
{
public:
    GeoSubWater(u32);                              // 0x801c66bc
    virtual void reset();                          // 0x801c671c
    virtual void createModel(JKRSolidHeap *, u32); // 0x801c6760
    virtual void setPosition(JGeometry::TVec3f &); // 0x801c6764
    virtual ~GeoSubWater() {}                      // 0x801c67e4
    void calc() {}                                 // 0x801c6be8
    // Inline
    void getPosition(JGeometry::TVec3f *out) const { /*out->set(mPos);*/ }

private:
    // TODO
};

class GeoWaterUp2 : public GeoSubWater
{
public:
    GeoWaterUp2();
    virtual void reset();
    virtual void loadAnimation();
    virtual void createModel(JKRSolidHeap *, u32);
    virtual void simpleDraw(u32);
    virtual void calc();
    virtual void simpleDraw(u32);
    virtual ~GeoWaterUp2() {}                                          // 0x801c6b10
    virtual char *getBmdFileName() { return "/Objects/sea2_tex.bmd"; } // 0x801c6bdc
private:
    // TODO
};

class GeoWaterUp : public GeoSubWater
{
public:
    GeoWaterUp();
    virtual void simpleDraw(u32);
    virtual ~GeoWaterUp() {}                                            // 0x801c6b84
    virtual char *getBmdFileName() { return "/Objects/sea3_dark.bmd"; } // 0x801c6bdc
private:
    // TODO
};

class GeoWaterMiddleD : public GeoSubWater
{
public:
    virtual void reset();
    virtual void loadAnimation();
    virtual void createModel(JKRSolidHeap *, u32);
    virtual void simpleDraw(u32);
    virtual void calc();
    virtual ~GeoWaterMiddleD() {} // 0x801c6c50
    virtual char *getBmdFileName() { return "/Objects/sea4_nami.bmd"; } // 0x801c6ca8
private:
    // TODO
};

class GeoWaterDown : public GeoSubWater
{
public:
    GeoWaterDown();
    virtual void simpleDraw(u32);
    virtual ~GeoWaterDown() {}            // 0x801c6bec
    virtual char *getBmdFileName() { return "/Objects/sea5_sand.bmd"; } // 0x801c6c44
private:
    // TODO
};

// 0x// void JSUList<GeoWater>::initiate() // JSUList<GeoWater>.h; // 0x801c42dc
// virtual JSUList<GeoWater>::~JSUList() // JSUList<GeoWater>.h; // 0x801c42fc
// void JSUList<GeoWater>::JSUList() // JSUList<GeoWater>.h; // 0x801c4348
// void JSUList<GeoWater>::append(JSULink<GeoWater> *) // JSUList<GeoWater>.h; // 0x801c442c
// void JSUListIterator<GeoWater>::operator!= (const JSULink<GeoWater> *) const // JSUListIterator<GeoWater>.h; // 0x801c44ec
// void JSUList<GeoWater>::getEnd() const // JSUList<GeoWater>.h; // 0x801c4500
// void JSUListIterator<GeoWater>::operator++ (int) // JSUListIterator<GeoWater>.h; // 0x801c4508
// void JSULink<GeoWater>::getNext() const // JSULink<GeoWater>.h; // 0x801c4554
// void JSUListIterator<GeoWater>::JSUListIterator(const JSUListIterator<GeoWater> &) // JSUListIterator<GeoWater>.h; // 0x801c455c
// void JSUListIterator<GeoWater>::getObject() const // JSUListIterator<GeoWater>.h; // 0x801c4568
// void JSULink<GeoWater>::getObject() const // JSULink<GeoWater>.h; // 0x801c458c
// void JSUListIterator<GeoWater>::operator-> () const // JSUListIterator<GeoWater>.h; // 0x801c45ac
// void JSUListIterator<GeoWater>::JSUListIterator(JSULink<GeoWater> *) // JSUListIterator<GeoWater>.h; // 0x801c45d0
// void JSUList<GeoWater>::getFirst() const // JSUList<GeoWater>.h; // 0x801c45d8
// virtual JSULink<GeoWater>::~JSULink() // JSULink<GeoWater>.h; // 0x801c4704
// void JSULink<GeoWater>::JSULink(GeoWater *) // JSULink<GeoWater>.h; // 0x801c4750
// void GeographyObj::clrObjFlagLODBias() // GeographyObj.h; // 0x801c49a4

#endif // GEOWATER_H