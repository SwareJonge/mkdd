#ifndef J3DEFFECTMGR
#define J3DEFFECTMGR

#include "Kaneshige/ExModel.h"

class J3DEffectObj
{
public:
    J3DEffectObj() : mLink(this) {}

    template <class T>
    T *New();

    template <class T>
    const char *GetBmdName();

    virtual ~J3DEffectObj() {}                  // 08, weak
    virtual void onlyinit() = 0;                // 0c
    virtual void init() = 0;                    // 10
    virtual void calc() = 0;                    // 14
    virtual void update() { mModel.update(0); } // 18, weak
    virtual void setTrigger() {}                // 1c, weak

    ExModel mModel;              // 4
    u8 _90[0xc8 - 0x90];         // 90
    JSULink<J3DEffectObj> mLink; // c8
};

class J3DEffectMgr
{
public:
    J3DEffectMgr();
    static void createMgr() { sJ3DMgr = new J3DEffectMgr(); }
    void createModel(JKRSolidHeap *, u32);         // 0x8025657c
    void reset();                                  // 0x80256618
    void loadJ3DdataAll();                         // 0x802566a4
    void createAll();                              // 0x80256758
    void remove(J3DEffectObj *);                   // 0x802567e4
    J3DEffectObj *setEffectKart(int, int, Mtx m);  // 0x8025681c
    void setEffectSplash(CrsGround *, int, Mtx m); // 0x80256920
    void setEffect(int, Mtx m, int);               // 0x80256a5c
    void setModelDataAppend(J3DEffectObj *, int);  // 0x80256ae8
    void calc();                                   // 0x80256b40
    void update();                                 // 0x80256bc0
    void viewCalc(u32);                            // 0x80256c3c
    void setCurrentViewNo(u32);                    // 0x80256c90

    static J3DEffectMgr *getJ3DEffectMgr() { return sJ3DMgr; }
    static J3DEffectMgr *sJ3DMgr;

private:
    u8 _00[0x78];
};

class J3DEfctKartAnmMgr
{
public:
    J3DEfctKartAnmMgr();
    static void createMgr() { sJ3DKartAnmMgr = new J3DEfctKartAnmMgr(); };
    static J3DEfctKartAnmMgr *sJ3DKartAnmMgr;

private:
    u8 _00[0x18];
};

inline void CreateJ3DEfctKarAnmMgr() { J3DEfctKartAnmMgr::createMgr(); };
inline void CreateJ3DEfctMgr() { J3DEffectMgr::createMgr(); };
inline J3DEffectMgr *GetJ3DEfctMgr() { return J3DEffectMgr::getJ3DEffectMgr(); }

#endif