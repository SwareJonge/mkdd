#ifndef _JSYSTEM_JPA_JPARESOURCE_H
#define _JSYSTEM_JPA_JPARESOURCE_H

#include "types.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPATexture.h"

typedef void JPAFunctionA(struct JPAEmitterWorkData *);
typedef void JPAFunctionB(struct JPAEmitterWorkData *, struct JPABaseParticle *);

/**
 * @size{0x48}
 */
struct JPAResource
{
	JPAResource();

	void init(JKRHeap *);
	void calc(JPAEmitterWorkData *, struct JPABaseEmitter *);
	void draw(JPAEmitterWorkData *, JPABaseEmitter *);
	void drawP(JPAEmitterWorkData *);
	void drawC(JPAEmitterWorkData *);
	void setPTev();
	void setCTev(JPAEmitterWorkData *);
	void calc_p(JPAEmitterWorkData *, JPABaseParticle *);
	void calc_c(JPAEmitterWorkData *, JPABaseParticle *);
	void calcField(JPAEmitterWorkData *, JPABaseParticle *);
	void calcKey(JPAEmitterWorkData *);
	void calcWorkData_c(JPAEmitterWorkData *);
	void calcWorkData_d(JPAEmitterWorkData *);

	u16 getUsrIdx() const {
		return usrIdx;
	}

	JPAFunctionA **_00;			   // _00
	JPAFunctionA **_04;			   // _04
	JPAFunctionA **_08;			   // _08
	JPAFunctionB **_0C;			   // _0C
	JPAFunctionB **_10;			   // _10
	JPAFunctionB **_14;			   // _14
	JPAFunctionB **_18;			   // _18
	struct JPABaseShape *pBsp;	   // _1C
	struct JPAExtraShape *pEsp;	   // _20
	struct JPAChildShape *pCsp;	   // _24
	struct JPAExTexShape *pEts;	   // _28
	struct JPADynamicsBlock *pDyn; // _2C
	struct JPAFieldBlock **ppFld;  // _30
	struct JPAKeyBlock **ppKey;	   // _34
	u16 *texIdxTbl;				   // _38
	u16 usrIdx;					   // _3C
	u8 fldNum;					   // _3E
	u8 keyNum;					   // _3F
	u8 texNum;					   // _40
	u8 _41;						   // _41
	u8 _42;						   // _42
	u8 _43;						   // _43
	u8 _44;						   // _44
	u8 _45;						   // _45
	u8 _46;						   // _46
	u8 _47;						   // _47
};

/**
 * @size{0x14}
 */
struct JPAResourceManager
{
	JPAResourceManager(u16, u16, JKRHeap *); // unused/inlined
	JPAResourceManager(const void *, JKRHeap *);

	JPAResource *getResource(u16) const;
	ResTIMG *swapTexture(const ResTIMG *, const char *);
	void registRes(JPAResource *);
	void registTex(JPATexture *);
	void *getResUserWork(u16) const;

	// unused/inlined:
	void load(const char *, u16);
	void load(const void *, u16);
	bool checkUserIndexDuplication(u16) const;
	void registTexDupCheck(const u8 *, JKRHeap *);

	JKRHeap *pHeap;				 // _00
	JPAResource **pResAry;		 // _04
	struct JPATexture **pTexAry; // _08
	u16 resMaxNum;				 // _0C
	u16 resRegNum;				 // _0E
	u16 texMaxNum;				 // _10
	u16 texRegNum;				 // _12
};

struct JPAResourceLoader
{
	JPAResourceLoader(const u8 *, JPAResourceManager *, u16); // unused/inlined
	JPAResourceLoader(const u8 *, JPAResourceManager *);

	void load_jpc(const u8 *, JPAResourceManager *);

	// unused/inlined:
	~JPAResourceLoader() {} // this is inlined for sure, however it doesn't show in TP, so ???
	void load_jpa(const u8 *, JPAResourceManager *p_res_mgr, u16);
};

#endif
