#ifndef _JSYSTEM_JPA_JPARESOURCE_H
#define _JSYSTEM_JPA_JPARESOURCE_H

#include "types.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPATexture.h"

typedef void (*JPAEmitterFunc)(struct JPAEmitterWorkData *);
typedef void (*JPAParticleFunc)(struct JPAEmitterWorkData *, struct JPABaseParticle *);

struct JPABaseShape;
struct JPAExtraShape;
struct JPAChildShape;
struct JPAExTexShape;
struct JPADynamicsBlock;
struct JPAFieldBlock;
struct JPAKeyBlock;

/**
 * @size{0x48}
 */
struct JPAResource
{
    JPAResource();

    void init(JKRHeap *);
    bool calc(JPAEmitterWorkData *, struct JPABaseEmitter *);
    void draw(JPAEmitterWorkData *, JPABaseEmitter *);
    void drawP(JPAEmitterWorkData *); // Draw parent
    void drawC(JPAEmitterWorkData *); // Draw child
    void setPTev();
    void setCTev(JPAEmitterWorkData *);
    void calc_p(JPAEmitterWorkData *, JPABaseParticle *);
    void calc_c(JPAEmitterWorkData *, JPABaseParticle *);
    void calcField(JPAEmitterWorkData *, JPABaseParticle *);
    void calcKey(JPAEmitterWorkData *);
    void calcWorkData_c(JPAEmitterWorkData *);
    void calcWorkData_d(JPAEmitterWorkData *);

    JPABaseShape *const getBsp() const { return pBsp; }
    JPAExtraShape *getEsp() const { return pEsp; }
    JPAChildShape *getCsp() const { return pCsp; }
    JPAExTexShape *getEts() const { return pEts; }
    JPADynamicsBlock *getDyn() const { return pDyn; }

    u16 getTexIdx(u8 idx) { return texIdxTbl[idx]; }
    u16 getUsrIdx() const { return mUsrIdx; }

    JPAEmitterFunc *mCalcEmitterFuncList;        // _00
    JPAEmitterFunc *mDrawEmitterFuncList;        // _04
    JPAEmitterFunc *mDrawEmitterChildFuncList;   // _08
    JPAParticleFunc *mCalcParticleFuncList;      // _0C
    JPAParticleFunc *mDrawParticleFuncList;      // _10
    JPAParticleFunc *mCalcParticleChildFuncList; // _14
    JPAParticleFunc *mDrawParticleChildFuncList; // _18
    JPABaseShape *pBsp;                          // _1C
    JPAExtraShape *pEsp;                         // _20
    JPAChildShape *pCsp;                         // _24
    JPAExTexShape *pEts;                         // _28
    JPADynamicsBlock *pDyn;                      // _2C
    JPAFieldBlock **ppFld;                       // _30
    JPAKeyBlock **ppKey;                         // _34
    u16 *texIdxTbl     ;                         // _38
    u16 mUsrIdx;                                 // _3C
    u8 fldNum;                                   // _3E
    u8 keyNum;                                   // _3F
    u8 texNum;                                   // _40
    u8 mCalcEmitterFuncListNum;                  // _41
    u8 mDrawEmitterFuncListNum;                  // _42
    u8 mDrawEmitterChildFuncListNum;             // _43
    u8 mCalcParticleFuncListNum;                 // _44
    u8 mDrawParticleFuncListNum;                 // _45
    u8 mCalcParticleChildFuncListNum;            // _46
    u8 mDrawParticleChildFuncListNum;            // _47
};

/**
 * @size{0x14}
 */
struct JPAResourceManager
{
    JPAResourceManager(u16, u16, JKRHeap *); // unused/inlined
    JPAResourceManager(const void *, JKRHeap *);

    JPAResource *getResource(u16) const;
    const ResTIMG *swapTexture(const ResTIMG *, const char *);
    void registRes(JPAResource *);
    void registTex(JPATexture *);
    u32 getResUserWork(u16) const;

    // unused/inlined:
    void load(const char *, u16);
    void load(const void *, u16);
    bool checkUserIndexDuplication(u16) const;
    void registTexDupCheck(const u8 *, JKRHeap *);

    void load(u16 idx, GXTexMapID texMapID) { pTexAry[idx]->load(texMapID); }

    JKRHeap *pHeap;              // _00
    JPAResource **pResAry;       // _04
    struct JPATexture **pTexAry; // _08
    u16 resMaxNum;               // _0C
    u16 resRegNum;               // _0E
    u16 texMaxNum;               // _10
    u16 texRegNum;               // _12
};

struct JPAResourceLoader
{
    JPAResourceLoader(const u8 *, JPAResourceManager *, u16); // unused/inlined
    JPAResourceLoader(const u8 *, JPAResourceManager *);

    void load_jpc(const u8 *, JPAResourceManager *);

    // unused/inlined:
    // ~JPAResourceLoader();
    void load_jpa(const u8 *, JPAResourceManager *, u16);
};

#endif
