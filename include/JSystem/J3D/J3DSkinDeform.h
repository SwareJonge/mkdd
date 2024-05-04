#ifndef _JSYSTEM_J3D_J3DSKINDEFORM_H
#define _JSYSTEM_J3D_J3DSKINDEFORM_H

#include "JSystem/J3D/J3DModel.h"
#include "JSystem/J3D/J3DCluster.h"

struct J3DAnmCluster;
class J3DClusterVertex;

enum J3DSkinDeformFlags {
	J3DSKIN_Unk1 = 0x1,
	J3DSKIN_Unk2 = 0x2,
};

class J3DDeformData
{
public:
	J3DDeformData();
	void offAllFlag(u32);
	void deform(J3DVertexBuffer *);
	void deform(J3DModel *);
	void setAnm(J3DAnmCluster *);

	J3DCluster *getClusterPointer(u16 index) { return &mClusterPointer[index]; }
	u16 getClusterNum() const { return mClusterNum; }
	u16 getClusterKeyNum() const { return mClusterKeyNum; }
	J3DClusterKey *getClusterKeyPointer(u16 i) { return &mClusterKeyPointer[i]; }
	f32 *getVtxPos() { return mVtxPos; }
	f32 *getVtxNrm() { return mVtxNrm; }

	u16 mClusterNum;				   // 00
	u16 mClusterKeyNum;				   // 02
	u16 mClusterVertexNum;			   // 04
	J3DCluster *mClusterPointer;	   // 08
	J3DClusterKey *mClusterKeyPointer; // 0c
	J3DClusterVertex *mClusterVertex;  // 10
	u16 mVtxPosNum;					   // 14
	u16 mVtxNrmNum;					   // 16
	f32 *mVtxPos;					   // 18
	f32 *mVtxNrm;					   // 1C
	JUTNameTab *mClusterName;		   // 20
	JUTNameTab *mClusterKeyName;	   // 24
}; // Size: 0x28

struct J3DDeformer {
	J3DDeformer(J3DDeformData*);

	void deform(J3DVertexBuffer*, u16, f32*);
	void deform(J3DVertexBuffer*, u16);
	void deform_VtxPosF32(J3DVertexBuffer*, J3DCluster*, J3DClusterKey*, f32*);
	void deform_VtxNrmF32(J3DVertexBuffer*, J3DCluster*, J3DClusterKey*, f32*);
	void normalizeWeight(int, f32*);

	J3DDeformData* mDeformData; // _00
	J3DAnmCluster* mAnmCluster; // _04
	int _08;                    // _08
	int _0C;                    // _0C
	u32 mFlags;                 // _10
};

/*struct J3DSkinDeform {
	J3DSkinDeform();

	virtual void deform(J3DVertexBuffer*, J3DMtxBuffer*); // _08
	virtual ~J3DSkinDeform() { }                          // _0C

	void deform(J3DModel* model);

	void setFlag(u32 flag) { mFlags.typeView |= flag; }
	void resetFlag(u32 flag) { mFlags.typeView &= ~flag; }
	bool checkFlag(u32 flag) const { return mFlags.typeView & flag; }

	// _00 VTBL
	void* mPosData;      // _04
	//BitFlag<u32> _08;    // _08 // bitfield of some sort
	unknown _0C;         // _0C
	unknown _10;         // _10
	BitFlag<u32> mFlags; // _14
};*/

#endif
