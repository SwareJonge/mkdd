#ifndef _JSYSTEM_JPA_JPABLOCK_H
#define _JSYSTEM_JPA_JPABLOCK_H

#include "JSystem/JGeometry.h"
#include "types.h"

struct JKRHeap;
struct JPAEmitterWorkData;
struct JPAFieldBase;

typedef void JPAVolumeFunc(JPAEmitterWorkData *);

/**
 * @size{0x8}
 */
struct JPADynamicsBlock
{
	/**
	 * @fabricated
	 */
	struct Data
	{
		u8 _00[0x8];  // _00
		u32 _08;	  // _08
		u8 _0C[0x48]; // _0C
		f32 _54;	  // _54
		u8 _58[0x18]; // _58
		s16 _70;	  // _70
	};

	JPADynamicsBlock(const u8 *);
	void init();
	void create(JPAEmitterWorkData *);

	/**
	 * @fabricated
	 */
	inline const Data *castData() const { return reinterpret_cast<const Data *>(mData); }

	// unused/inlined:
	void init_jpa(const u8 *, JKRHeap *);

	u8 *mData;						// _00
	JPAVolumeFunc *mVolumeFunction; // _04
};

/**
 * @size{0x2C}
 */
struct JPAFieldBlock
{
	/** @fabricated */
	struct Data
	{
		u8 _00[4];				   // _00
		u32 _04;				   // _04
		u32 flags;				   // _08
		JGeometry::TVec3f posOrig; // _0C
		JGeometry::TVec3f dirOrig; // _18
		f32 magOrig;			   // _24
		f32 magRndm;			   // _28
		f32 val1;				   // _2C
		f32 fadeInTime;			   // _30
		f32 fadeOutTime;		   // _34
		f32 enTime;				   // _38
		f32 disTime;			   // _3C
		u8 cycle;				   // _4C
	};

	JPAFieldBlock(const u8 *, JKRHeap *);

	void init(JKRHeap *);

	// unused/inlined:
	void init_jpa(const u8 *, JKRHeap *);

	const u8 *pData;	   // _00
	JPAFieldBase *pFld;	   // _04
	f32 fadeInRate;		   // _08
	f32 fadeOutRate;	   // _0C
	JGeometry::TVec3f pos; // _10
	JGeometry::TVec3f dir; // _1C
	f32 opParam;		   // _28
};

/**
 * @size{0x8}
 */
struct JPAKeyBlock
{
	JPAKeyBlock(const u8 *);

	void calc(f32);

	// Unused/inlined:
	void init_jpa(const u8 *, JKRHeap *);

	const u8 *mDataStart; // _00
	const f32 *mKeyData;  // _04
};

#endif
