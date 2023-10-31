#ifndef GEOSIGNAL_H
#define GEOSIGNAL_H

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

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
    // void sTable;                                     // 0x80396438
    // Inline/Unused
    // void GeoSignalSupervisor::searchSameGroupSignal(GeoSignal *);
    // void JSUListIterator<GeoSignal>::operator!= (const JSULink<GeoSignal> *) const;
    // void JSUList<GeoSignal>::getEnd() const;
    // void JSUListIterator<GeoSignal>::operator++ (int);
    // void JSULink<GeoSignal>::getNext() const;
    // void JSUListIterator<GeoSignal>::JSUListIterator(const JSUListIterator<GeoSignal> &);
    // void isSameGroup(GeoSignal *) const;
    // void JSUListIterator<GeoSignal>::operator-> () const;
    // void JSULink<GeoSignal>::getObject() const;
    // void JSUListIterator<GeoSignal>::getObject() const;
    // void JSUListIterator<GeoSignal>::JSUListIterator(JSULink<GeoSignal> *);
    // void JSUList<GeoSignal>::getFirst() const;
    // void GeoSignalSupervisor::searchSignal(long);
    // void isSameID(long) const;
    // void sGreenTime;
    // void sLagTime;
    // void sRedAnmFrame;
    // void sG2RAnmRate;
    // void sR2GAnmRate;
    // void sCreateNum;
    // void sGreenAnmFrame;

    virtual void InitExec() // 0x801be7a8, overide
    {
        /*__ptmf_scall(this, FindStateFunc(getState()), &sTable, 1);*/
    }
    void MoveExec() // 0x801be8bc, overide
    {
        /*__ptmf_scall(this, FindStateFunc(getState()), &sTable, 1);*/
    }
    virtual ~GeoSignal() {}                                         // 0x801be918, overide
    virtual char *getBmdFileName() { "/Objects/Signal1.bmd"; }      // 0x801be9a0, overide
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; } // 0x801be9ac, overide
private:
    static u32 sSupervisorCreateNum;      // 0x80416408
    static GeoObjSupervisor *sSupervisor; // 0x8041640c
    static J3DAnmTevRegKey *sTevRegKey;   // 0x80416410
    // TODO
}; // class GeoSignal

class GeoSignalSupervisor : public GeoObjSupervisor, JKRDisposer
{
public:
    GeoSignalSupervisor();

    void entrySignal(GeoSignal *);

    virtual void reset();
    virtual void calc();
    virtual ~GeoSignalSupervisor() {} // 0x801be2a8, overide
private:
    // TODO
}; // class GeoSignalSupervisor

// Outside class members

// 0x// void JSUList<GeoSignal>::initiate() // JSUList<GeoSignal>.h; // 0x801be20c
// virtual JSUList<GeoSignal>::~JSUList() // JSUList<GeoSignal>.h; // 0x801be22c
// void JSUList<GeoSignal>::JSUList() // JSUList<GeoSignal>.h; // 0x801be278
// void JSUList<GeoSignal>::append(JSULink<GeoSignal> *) // JSUList<GeoSignal>.h; // 0x801be35c
// virtual JSULink<GeoSignal>::~JSULink() // JSULink<GeoSignal>.h; // 0x801be500
// void JSULink<GeoSignal>::JSULink(GeoSignal *) // JSULink<GeoSignal>.h; // 0x801be54c
#endif // GEOSIGNAL_H