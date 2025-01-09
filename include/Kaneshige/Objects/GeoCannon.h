#ifndef GEOCANNON_H
#define GEOCANNON_H

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

struct J3DAnmObjTrans; // TODO

class GeoCannon : public GeographyObj, StateObserver
{
public:
    GeoCannon(const CrsData::SObject &);   // 0x801bf6a8
    virtual void reset();                  // 0x801bf850
    virtual void resetAnimation();         // 0x801bfa68
    virtual void loadAnimation();          // 0x801bfb9c
    virtual void createModel(JKRSolidHeap *, u32); // 0x801bfc0c
    virtual void calc();                           // 0x801c009c
    virtual void update();                         // 0x801c00e8
    void shot();                           // 0x801c04c4
    void initFuncWait();                   // 0x801c0574
    void doFuncWait();                     // 0x801c0578

    // Inline
    bool isSameID(long) const; // 0x801bf644
    // Inline/Unused
    // static u32 sCreateNum;

    virtual void InitExec() // 0x801c035c
    {
        /*__ptmf_scall(this, StateObserver::FindStateFunc(uVar2, sTable, 1));*/
    }
    virtual void MoveExec() // 0x801c0470
    {
        /*__ptmf_scall(this, StateObserver::FindStateFunc(uVar2, sTable, 1));*/
    }
    virtual ~GeoCannon() {}                                           // 0x801c05d4
    virtual const char *getBmdFileName() { return "/Objects/Cannon1.bmd"; } // 0x801c065c
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; }   // 0x801c0668
private:
    // void sTable;                                     // 0x80396640
    static char *sEmitterName[4];         // 0x8039665c
    static u32 sSupervisorCreateNum;      // 0x80416420
    static GeoObjSupervisor *sSupervisor; // 0x80416424
    static J3DAnmTevRegKey *sTevRegKey;   // 0x80416428
    static J3DAnmObjTrans *sBcaAnmTrans;  // 0x8041642c
    static J3DAnmTransform *sBcaMtxCalc;  // 0x80416430
};                                        // class GeoCannon
// Outside class members

class GeoCannonSupervisor : public GeoObjSupervisor
{
public:
    GeoCannonSupervisor();

    void entryCannon(GeoCannon *);
    GeoCannon *searchCannon(long);

    virtual ~GeoCannonSupervisor() {} // 0x801bf428
    virtual void reset();             // overide
    virtual void calc();              // overide
private:
    // TODO
};

// 0x// void JSUList<GeoCannon>::initiate() // JSUList<GeoCannon>.h; // 0x801bf38c
// virtual JSUList<GeoCannon>::~JSUList() // JSUList<GeoCannon>.h; // 0x801bf3ac
// void JSUList<GeoCannon>::JSUList() // JSUList<GeoCannon>.h; // 0x801bf3f8
// void JSUList<GeoCannon>::append(JSULink<GeoCannon> *) // JSUList<GeoCannon>.h; // 0x801bf4dc
// void JSUListIterator<GeoCannon>::operator!= (const JSULink<GeoCannon> *) const // JSUListIterator<GeoCannon>.h; // 0x801bf59c
// void JSUList<GeoCannon>::getEnd() const // JSUList<GeoCannon>.h; // 0x801bf5b0
// void JSUListIterator<GeoCannon>::operator++ (int) // JSUListIterator<GeoCannon>.h; // 0x801bf5b8
// void JSULink<GeoCannon>::getNext() const // JSULink<GeoCannon>.h; // 0x801bf604
// void JSUListIterator<GeoCannon>::JSUListIterator(const JSUListIterator<GeoCannon> &) // JSUListIterator<GeoCannon>.h; // 0x801bf60c
// void JSUListIterator<GeoCannon>::getObject() const // JSUListIterator<GeoCannon>.h; // 0x801bf618
// void JSULink<GeoCannon>::getObject() const // JSULink<GeoCannon>.h; // 0x801bf63c
// void JSUListIterator<GeoCannon>::operator-> () const // JSUListIterator<GeoCannon>.h; // 0x801bf65c
// void JSUListIterator<GeoCannon>::JSUListIterator(JSULink<GeoCannon> *) // JSUListIterator<GeoCannon>.h; // 0x801bf680
// void JSUList<GeoCannon>::getFirst() const // JSUList<GeoCannon>.h; // 0x801bf688
// void GeographyObj::NewAnmCtrl() // GeographyObj.h; // 0x801bf784
// virtual JSULink<GeoCannon>::~JSULink() // JSULink<GeoCannon>.h; // 0x801bf7d4
// void JSULink<GeoCannon>::JSULink(GeoCannon *) // JSULink<GeoCannon>.h; // 0x801bf820
// void GeographyObj::clrObjFlagHidding() // GeographyObj.h; // 0x801bfa48
// void GeographyObj::clrObjFlagCheckGeoHitting() // GeographyObj.h; // 0x801bfa58
// void AnmController::getFrameCtrl(unsigned char) // AnmController.h; // 0x801bfacc
// void TAnmControlBase<J3DAnmObjTrans>::getFrameCtrl(unsigned char) // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801bfaf0
// void AnmController::IsAvailableTrans() const // AnmController.h; // 0x801bfb84
// void GeographyObj::getAnmCtrl() const // GeographyObj.h; // 0x801bfb94
// void AnmController::RegisterTrans(unsigned char, J3DAnmTransform *, J3DMtxCalc *) // AnmController.h; // 0x801bfcc4
// void AnmController::InitRegistration(unsigned char, ExModel *) // AnmController.h; // 0x801bfce8
// void TAnmControlBase<J3DAnmObjTrans>::initAnm(unsigned char, ExModel *) // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801bfd50
// virtual J3DAnmObjTrans::~J3DAnmObjTrans() // J3DAnmObjTrans.h; // 0x801bfdf8
// void J3DAnmObjTrans::J3DAnmObjTrans() // J3DAnmObjTrans.h; // 0x801bfe50
// void AnmControlTrans::AnmControlTrans() // AnmControlTrans.h; // 0x801bfe98
// virtual TAnmControlBase<J3DAnmObjTrans>::~TAnmControlBase() // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801bfed4
// virtual AnmControlBase::~AnmControlBase() // AnmControlBase.h; // 0x801bff2c
// virtual JSULink<AnmControlBase>::~JSULink() // JSULink<AnmControlBase>.h; // 0x801bff88
// void TAnmControlBase<J3DAnmObjTrans>::TAnmControlBase() // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801bffd4
// void AnmControlBase::AnmControlBase() // AnmControlBase.h; // 0x801c0018
// void JSULink<AnmControlBase>::JSULink(AnmControlBase *) // JSULink<AnmControlBase>.h; // 0x801c006c
// void JPABaseEmitter::setGlobalTranslation(const JGeometry::TVec3<float> &) // JPABaseEmitter.h; // 0x801c01d0
// void JPABaseEmitter::setGlobalRMatrix(const float (*) [4]) // JPABaseEmitter.h; // 0x801c0250
// void JPABaseEmitter::isEnableDeleteEmitter() const // JPABaseEmitter.h; // 0x801c027c
// void JPABaseEmitter::getParticleNumber() const // JPABaseEmitter.h; // 0x801c02d0
// void JPAList<JPABaseParticle>::getNum() const // JPAList<JPABaseParticle>.h; // 0x801c0310
// void JPABaseEmitter::checkStatus(u32) const // JPABaseEmitter.h; // 0x801c0318
// void ExModel::getModel() const // ExModel.h; // 0x801c0324
// void J3DModelData::getJointName() const // J3DModelData.h; // 0x801c0330
// void J3DJointTree::getJointName() const // J3DJointTree.h; // 0x801c0354
// void GeographyObj::tstObjFlagHidding() const // GeographyObj.h; // 0x801c0568
// void AnmControlBase::changeAnm(unsigned char, bool) // AnmControlBase.h; // 0x801c057c
// void AnmControlBase::changeMatAnm(unsigned char, bool) // AnmControlBase.h; // 0x801c0580
// void TAnmControlBase<J3DAnmObjTrans>::resetAnm() // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801c0584
// void TAnmControlBase<J3DAnmObjTrans>::resetMatAnm() // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801c0674
// void TAnmControlBase<J3DAnmObjTrans>::setAnmProc() // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801c06bc
// void TAnmControlBase<J3DAnmObjTrans>::frameProc() // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801c06f8
// void TAnmControlBase<J3DAnmObjTrans>::doframe(unsigned char) // TAnmControlBase<J3DAnmObjTrans>.h; // 0x801c071c
// void AnmControlBase::tstFlgAnmStop() const // AnmControlBase.h; // 0x801c07c0
#endif // GEOCANNON_H
