#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAField.h"
#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JParticle/JPAShape.h"
#include "JSystem/JUtility/JUTAssert.h"

// this might be a group of smaller arrays
// it for sure is
u8 jpa_pos[]
    = { 00,   00,   00,   0x32, 00,   00,   0x32, 0xCE, 00,   00,   0xCE, 00,   0xE7, 00,   00,   0x19, 00,   00,   0x19, 0xCE, 00,   0xE7,
	    0xCE, 00,   0xCE, 00,   00,   00,   00,   00,   00,   0xCE, 00,   0xCE, 0xCE, 00,   00,   0x19, 00,   0x32, 0x19, 00,   0x32, 0xE7,
	    00,   00,   0xE7, 00,   0xE7, 0x19, 00,   0x19, 0x19, 00,   0x19, 0xE7, 00,   0xE7, 0xE7, 00,   0xCE, 0x19, 00,   00,   0x19, 00,
	    00,   0xE7, 00,   0xCE, 0xE7, 00,   00,   0x32, 00,   0x32, 0x32, 00,   0x32, 00,   00,   00,   00,   00,   0xE7, 0x32, 00,   0x19,
	    0x32, 00,   0x19, 00,   00,   0xE7, 00,   00,   0xCE, 0x32, 00,   00,   0x32, 00,   00,   00,   00,   0xCE, 00,   00,   00,   00,
	    00,   0x32, 00,   00,   0x32, 00,   0x32, 00,   00,   0x32, 0xE7, 00,   00,   0x19, 00,   00,   0x19, 00,   0x32, 0xE7, 00,   0x32,
	    0xCE, 00,   00,   00,   00,   00,   00,   00,   0x32, 0xCE, 00,   0x32, 00,   00,   0xE7, 0x32, 00,   0xE7, 0x32, 00,   0x19, 00,
	    00,   0x19, 0xE7, 00,   0xE7, 0x19, 00,   0xE7, 0x19, 00,   0x19, 0xE7, 00,   0x19, 0xCE, 00,   0xE7, 00,   00,   0xE7, 00,   00,
	    0x19, 0xCE, 00,   0x19, 00,   00,   0xCE, 0x32, 00,   0xCE, 0x32, 00,   00,   00,   00,   00,   0xE7, 00,   0xCE, 0x19, 00,   0xCE,
	    0x19, 00,   00,   0xE7, 00,   00,   0xCE, 00,   0xCE, 00,   00,   0xCE, 00,   00,   00,   0xCE, 00,   00,   00,   00,   00,   00,
	    00,   0x32, 00,   0xCE, 0x32, 00,   0xCE, 00,   00,   00,   0xE7, 00,   00,   0x19, 00,   0xCE, 0x19, 00,   0xCE, 0xE7, 00,   00,
	    0xCE, 00,   00,   00,   00,   0xCE, 00,   00,   0xCE, 0xCE, 00,   0x19, 00,   00,   0x19, 0x32, 00,   0xE7, 0x32, 00,   0xE7, 00,
	    00,   0x19, 0xE7, 00,   0x19, 0x19, 00,   0xE7, 0x19, 00,   0xE7, 0xE7, 00,   0x19, 0xCE, 00,   0x19, 00,   00,   0xE7, 00,   00,
	    0xE7, 0xCE, 00,   0x32, 00,   00,   0x32, 0x32, 00,   00,   0x32, 00,   00,   00,   00,   0x32, 0xE7, 00,   0x32, 0x19, 00,   00,
	    0x19, 00,   00,   0xE7, 00,   0x32, 0xCE, 00,   0x32, 00,   00,   00,   00,   00,   00,   0xCE, 00,   00,   00,   00,   00,   00,
	    00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00 };

u8 jpa_crd[] = { 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 2, 0, 2, 1, 0, 1, 0, 0, 1, 0, 1, 2, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2 };

JPAResource::JPAResource()
{
	mDrawEmitterChildFuncList     = nullptr;
	mDrawEmitterFuncList          = nullptr;
	mCalcEmitterFuncList          = nullptr;
	mDrawParticleChildFuncList    = nullptr;
	mCalcParticleChildFuncList    = nullptr;
	mDrawParticleFuncList         = nullptr;
	mCalcParticleFuncList         = nullptr;
	pBsp                          = nullptr;
	pEsp                          = nullptr;
	pCsp                          = nullptr;
	pEts                          = nullptr;
	pDyn                          = nullptr;
	ppFld                         = nullptr;
	ppKey                         = nullptr;
	texIdxTbl                     = nullptr;
	mDrawParticleChildFuncListNum = 0;
	mCalcParticleChildFuncListNum = 0;
	mDrawParticleFuncListNum      = 0;
	mCalcParticleFuncListNum      = 0;
	mDrawEmitterChildFuncListNum  = 0;
	mDrawEmitterFuncListNum       = 0;
	mCalcEmitterFuncListNum       = 0;
	texNum                        = 0;
	keyNum                        = 0;
	fldNum                        = 0;
	mUsrIdx                       = 0;
}

/**
 * @note Address: 0x8009565C
 * @note Size: 0x1540
 */
void JPAResource::init(JKRHeap* heap)
{
	JPAExtraShape* exShape          = pEsp;
	const JPABaseShapeData* shpData = pBsp->pBsd;

	u32 shpFlags    = shpData->mFlags;
	u8 clrFlag      = shpData->mClrFlg;
	u8 texFlag      = shpData->mTexFlg;
	u8 isGlblClrAnm = shpFlags & 0x1000;
	u8 isGlblTexAnm = shpFlags & 0x4000;
	u8 isTexCrdAnm  = shpFlags & 0x1000000;
	u8 isPrjTex     = shpFlags & 0x100000;

	bool exShapeSet1 = false;
	if (exShape && exShape->mData->mFlags & 1) {
		exShapeSet1 = true;
	}

	bool exShapeSet2 = false;
	if (exShape && exShape->mData->mFlags & 0x10000) {
		exShapeSet2 = true;
	}

	bool exShapeSet3 = false;
	if (exShape && (exShape->mData->mFlags & 0x10000) && (exShape->mData->mFlags & 0x20000)) {
		exShapeSet3 = true;
	}

	bool exShapeSet4 = false;
	if (exShape && exShape->mData->mFlags & 0x1000000) {
		exShapeSet4 = true;
	}

	bool exShapeSet5 = false;
	if (exShapeSet4 || (pCsp && pCsp->mData->mFlags & 0x1000000)) {
		exShapeSet5 = true;
	}

	u8 shpType = shpFlags & 0xf;
	bool test  = true;
	if (shpType != 5 && shpType != 6) {
		test = false;
	}
	// skipping some dumb stuff for now

	if (isGlblTexAnm && (texFlag & 1)) {
		mCalcEmitterFuncListNum++;
	}
	if (isGlblClrAnm) {
		if (clrFlag & 2) {
			mCalcEmitterFuncListNum++;
		}
		if (clrFlag & 8) {
			mCalcEmitterFuncListNum++;
		}
		if ((clrFlag & 2) || (clrFlag & 8)) {
			mCalcEmitterFuncListNum++;
		}
	}
	if (mCalcParticleFuncListNum) {
		mCalcEmitterFuncList = (JPAFunctionA**)heap->alloc(mCalcParticleChildFuncListNum * 4, 4);
	}

	int idx = 0;
	if (isGlblTexAnm && (texFlag & 1)) {
		switch (pBsp->getTexAnmType()) {
		case 0:
			mCalcEmitterFuncList[0] = JPACalcTexIdxNormal;
			break;
		case 1:
			mCalcEmitterFuncList[0] = JPACalcTexIdxRepeat;
			break;
		case 2:
			mCalcEmitterFuncList[0] = JPACalcTexIdxReverse;
			break;
		case 3:
			mCalcEmitterFuncList[0] = JPACalcTexIdxMerge;
			break;
		case 4:
			mCalcEmitterFuncList[0] = JPACalcTexIdxRandom;
			break;
		}
		idx = 1;
	}

	if (isGlblClrAnm) {
		int i = idx;
		if (clrFlag & 2) {
			i                               = idx + 1;
			mCalcParticleChildFuncList[idx] = JPACalcPrm;
		}
		idx = i;
		if (clrFlag & 8) {
			idx                             = i + 1;
			mCalcParticleChildFuncList[idx] = JPACalcEnv;
		}
		if ((clrFlag & 2) || (clrFlag & 8)) {
			switch (pBsp->getTexAnmType()) {
			case 0:
				mCalcEmitterFuncList[idx] = JPACalcTexIdxNormal;
				break;
			case 1:
				mCalcEmitterFuncList[idx] = JPACalcTexIdxRepeat;
				break;
			case 2:
				mCalcEmitterFuncList[idx] = JPACalcTexIdxReverse;
				break;
			case 3:
				mCalcEmitterFuncList[idx] = JPACalcTexIdxMerge;
				break;
			case 4:
				mCalcEmitterFuncList[idx] = JPACalcTexIdxRandom;
				break;
			}
		}
	}

	// determin number of particle callbacks
	if (!isGlblTexAnm && (texFlag & 1)) {
		mCalcParticleFuncListNum++;
	}
	if ((!test) && (exShapeSet2 || exShapeSet3)) {
		mCalcParticleFuncListNum++;
	}
	if (isGlblClrAnm) {
		if (clrFlag & 2) {
			mCalcParticleFuncListNum++;
		}
		if (clrFlag & 8) {
			mCalcParticleFuncListNum++;
		}
		if (clrFlag & 2 || clrFlag & 8) {
			mCalcParticleFuncListNum++;
		}
	} else {
		mCalcParticleFuncListNum++;
	}
	if (exShapeSet1) {
		if (pBsp->getType() != 0) {
			u32 type = pBsp->getType();
			if (!(type & 2)) {
				mCalcParticleFuncListNum++;
			} else if (!(type >> 8 & 3) && !(type >> 10 & 3)) {
				mCalcParticleFuncListNum++;
			} else {
				mCalcParticleFuncListNum++;
				mCalcParticleFuncListNum++;
			}
		}
		mCalcParticleFuncListNum++;
		mCalcParticleFuncListNum++;
	}
	// allocate particle callbacks
	if (mCalcParticleChildFuncListNum != 0) {
		mCalcParticleFuncList = (JPAFunctionB**)heap->alloc(mCalcParticleFuncListNum * 4, 4);
	}

	// set particle callback functions
	idx = 0;
	if (!isGlblTexAnm && (texFlag & 1)) {
		switch (pBsp->getTexAnmType()) {
		case 0:
			mCalcParticleFuncList[0] = JPACalcTexIdxNormal;
			break;
		case 1:
			mCalcParticleFuncList[0] = JPACalcTexIdxRepeat;
			break;
		case 2:
			mCalcParticleFuncList[0] = JPACalcTexIdxReverse;
			break;
		case 3:
			mCalcParticleFuncList[0] = JPACalcTexIdxMerge;
			break;
		case 4:
			mCalcParticleFuncList[0] = JPACalcTexIdxRandom;
			break;
		}
		idx = 1;
	}

	{
		int i = idx;
		if (!test && (exShapeSet2 || exShapeSet3)) {
			if (exShapeSet3) {
				i                          = idx + 1;
				mCalcParticleFuncList[idx] = JPACalcAlphaFlickAnm;
			} else {
				idx                        = i + 1;
				mCalcParticleFuncList[idx] = JPACalcAlphaAnm;
			}
		}

		if (!isGlblClrAnm) {
			int i = idx;
			if (clrFlag & 2) {
				i                          = idx + 1;
				mCalcParticleFuncList[idx] = JPACalcPrm;
			}
			idx = i;
			if (clrFlag & 8) {
				idx                        = i + 1;
				mCalcParticleFuncList[idx] = JPACalcEnv;
			}
			if ((clrFlag & 2) || (clrFlag & 8)) {
				switch (pBsp->getTexAnmType()) {
				case 0:
					mCalcParticleFuncList[idx] = JPACalcClrIdxNormal;
					break;
				case 1:
					mCalcParticleFuncList[idx] = JPACalcClrIdxRepeat;
					break;
				case 2:
					mCalcParticleFuncList[idx] = JPACalcClrIdxReverse;
					break;
				case 3:
					mCalcParticleFuncList[idx] = JPACalcClrIdxMerge;
					break;
				case 4:
					mCalcParticleFuncList[idx] = JPACalcClrIdxRandom;
					break;
				}
			}
		} else {
			mCalcParticleFuncList[idx] = JPACalcColorCopy;
		}
	}

	if (exShapeSet1) {
		if (pBsp->getType()) {
			if (pBsp->pBsd->mFlags & 2) {
				mCalcParticleFuncList[idx] = JPACalcScaleY;
				switch (pBsp->getTexAnmType()) {
				case 0:
					mCalcParticleFuncList[idx] = JPACalcScaleAnmNormal;
					break;
				case 1:
					mCalcParticleFuncList[idx] = JPACalcScaleAnmRepeatY;
					break;
				case 2:
					mCalcParticleFuncList[idx] = JPACalcScaleAnmReverseY;
					break;
				}
			} else {
				mCalcParticleFuncList[idx] = JPACalcScaleCopy;
			}
			idx += 2;
		}
		mCalcParticleFuncList[idx] = JPACalcScaleX;
		switch (pBsp->getTexAnmType()) {
		case 0:
			mCalcParticleFuncList[idx] = JPACalcScaleAnmNormal;
			break;
		case 1:
			mCalcParticleFuncList[idx] = JPACalcScaleAnmRepeatX;
			break;
		case 2:
			mCalcParticleFuncList[idx] = JPACalcScaleAnmReverseX;
			break;
		}
	}

	if (pCsp && pCsp->mData->mFlags & 0x40000) {
		mCalcParticleChildFuncListNum++;
	}
	if (pCsp && pCsp->mData->mFlags & 0x80000) {
		mCalcParticleChildFuncListNum++;
	}
	if (mCalcParticleChildFuncListNum) {
		mCalcParticleChildFuncList = (JPAFunctionB**)heap->alloc(mCalcParticleChildFuncListNum * 4, 4);
	}
	idx = 0;
	if (pCsp && pCsp->mData->mFlags & 0x40000) {
		idx                           = 1;
		mCalcParticleChildFuncList[0] = JPACalcChildScaleOut;
	}
	if (pCsp && pCsp->mData->mFlags & 0x80000) {
		idx                           = 1;
		mCalcParticleChildFuncList[0] = JPACalcChildAlphaOut;
	}

	if (test) {
		mDrawEmitterFuncListNum++;
	}
	mDrawEmitterFuncListNum++;
	if (pEts) {
		mDrawEmitterFuncListNum++;
	}
	if (isGlblTexAnm || !(texFlag & 1)) {
		mDrawEmitterFuncListNum++;
	}
	mDrawEmitterFuncListNum++;
	if (!test) {
		mDrawEmitterFuncListNum++;
		if (!exShapeSet1) {
			mDrawEmitterFuncListNum++;
		}
	}
	if (isGlblClrAnm || (!(clrFlag & 2) && !exShapeSet2) || !(clrFlag & 8)) {
		mDrawEmitterFuncListNum++;
	}
	if (mDrawEmitterFuncListNum) {
		mDrawEmitterFuncList = (JPAFunctionA**)heap->alloc(mDrawEmitterFuncListNum * 4, 4);
	}

	if (test) {
		if (pBsp->getType() == 5) {
			idx                     = 1;
			mDrawEmitterFuncList[0] = JPADrawStripe;
		} else {
			idx                     = 1;
			mDrawEmitterFuncList[0] = JPADrawStripeX;
		}
	}
	mDrawEmitterFuncList[0] = JPADrawEmitterCallBackB;
	if (pEts) {
		mDrawEmitterFuncList[idx + 1] = JPALoadExTex;
	}
	if (!(texFlag & 1)) {
		mDrawEmitterFuncList[idx + 1] = JPALoadTex;
	} else if (isGlblTexAnm) {
		mDrawEmitterFuncList[idx + 1] = JPALoadTexAnm;
	}

	if (!test) {
		mDrawEmitterFuncList[idx] = JPAGenTexCrdMtxIdt;
	} else if (isPrjTex) {
		mDrawEmitterFuncList[idx] = JPAGenTexCrdMtxPrj;
	} else if (test) {
		mDrawEmitterFuncList[idx] = JPAGenCalcTexCrdMtxAnm;
	} else if (isTexCrdAnm) {
		mDrawEmitterFuncList[idx] = JPAGenTexCrdMtxAnm;
	} else {
		mDrawEmitterFuncList[idx] = JPAGenTexCrdMtxIdt;
	}

	if (!test) {
		mDrawEmitterFuncList[idx + 1] = JPALoadPosMtxCam;
		if (!exShapeSet1) {
			if (exShapeSet2) {
				mDrawEmitterFuncList[idx] = JPASetPointSize;
			} else {
				mDrawEmitterFuncList[idx] = JPASetLineWidth;
			}
		}
	}

	if (!test) {
		mDrawEmitterFuncList[idx + 1] = JPALoadPosMtxCam;
		if (!exShapeSet1) {
			if (exShapeSet2) {
				mDrawEmitterFuncList[idx] = JPASetPointSize;
			} else {
				mDrawEmitterFuncList[idx] = JPASetLineWidth;
			}
		}
	}

	if (isGlblClrAnm == 0) {
		if ((clrFlag & 2) || exShapeSet2) {
			if (!(clrFlag & 8)) {
				mDrawEmitterFuncList[idx] = JPARegistEnv;
			}
		} else if (!(clrFlag & 8)) {
			mDrawEmitterFuncList[idx] = JPARegistPrmEnv;
		} else {
			mDrawEmitterFuncList[idx] = JPARegistPrm;
		}
	} else if (test || !exShapeSet2) {
		mDrawEmitterFuncList[idx] = JPARegistPrmEnv;
	} else if (exShapeSet2) {
		mDrawEmitterFuncList[idx] = JPARegistEnv;
	}

	if (shpFlags && test) {
		mDrawEmitterChildFuncListNum++;
	}
	mDrawEmitterChildFuncListNum++;
	if (test) {
		mDrawEmitterChildFuncListNum++;
	}
	if (pCsp && !(pCsp->mData->mFlags & 0x80000) && !(pCsp->mData->mFlags & 0x20000)
	    && !(pCsp->mData->mFlags & 0x40000)) {
		mDrawEmitterChildFuncListNum++;
	}
	if (mDrawEmitterChildFuncListNum) {
		mDrawEmitterChildFuncList = (JPAFunctionA**)heap->alloc(mDrawEmitterChildFuncListNum * 4, 4);
	}
	idx = 0;
	if (shpFlags && test) {
		if ((pCsp->mData->mFlags & 0xf) == 5) {
			idx                          = 1;
			mDrawEmitterChildFuncList[0] = JPADrawStripe;
		} else {
			idx                          = 1;
			mDrawEmitterChildFuncList[0] = JPADrawStripeX;
		}
	}
	mDrawEmitterChildFuncList[idx] = JPADrawEmitterCallBackB;
	if (test) {
		mDrawEmitterChildFuncList[idx] = JPALoadPosMtxCam;
	}
	if (pCsp && !(pCsp->mData->mFlags & 0x800000) && !(pCsp->mData->mFlags & 0x200000)
	    && !(pCsp->mData->mFlags & 0x400000)) {
		mDrawEmitterChildFuncList[idx] = JPARegistChildPrmEnv;
	}

	if (test) {
		mDrawParticleFuncListNum++;
	}
	mDrawParticleFuncListNum++;
	if (!isGlblTexAnm && (texFlag & 1)) {
		mDrawParticleFuncListNum++;
	}
	if ((!test && exShapeSet1) || (isTexCrdAnm && !isPrjTex)) {
		mDrawParticleFuncListNum++;
	}
	if (!isGlblClrAnm && (clrFlag & 2) || (clrFlag & 8) || exShapeSet2 || (isGlblClrAnm && exShapeSet2) && !test) {
		mDrawParticleFuncListNum++;
	}
	if (mDrawParticleFuncListNum) {
		mDrawParticleFuncList = (JPAFunctionB**)heap->alloc(mDrawParticleFuncListNum * 4, 4);
	}
	if (test) {
		switch (pBsp->getType()) {
		case 0:
			mDrawParticleFuncList[0] = JPADrawPoint;
			break;
		case 1:
			mDrawParticleFuncList[0] = JPADrawLine;
			break;
		case 2:
			if (exShapeSet4)
				mDrawParticleFuncList[0] = JPADrawRotBillboard;
			else
				mDrawParticleFuncList[0] = JPADrawBillboard;
			break;
		case 3:
		case 4:
			if (exShapeSet4) {
				mDrawParticleFuncList[0] = JPADrawRotDirection;
			} else {
				mDrawParticleFuncList[0] = JPADrawDirection;
			}
			break;
		case 7:
		case 8:
			mDrawParticleFuncList[0] = JPADrawRotation;
			break;
		case 9:
			mDrawParticleFuncList[0] = JPADrawDBillboard;
			break;
		case 10:
			if (exShapeSet4) {
				mDrawParticleFuncList[0] = JPADrawRotYBillboard;
			} else {
				mDrawParticleFuncList[0] = JPADrawYBillboard;
			}
			break;
		}
	}
	mDrawParticleFuncList[idx] = JPADrawParticleCallBack;
	if (!isGlblTexAnm && (texFlag & 1)) {
		mDrawParticleFuncList[idx] = JPALoadTexAnm;
	}
	if ((test) || (!exShapeSet1)) {
		if ((isTexCrdAnm != 0) && (isPrjTex == 0)) {
			mDrawParticleFuncList[idx] = JPALoadCalcTexCrdMtxAnm;
		}
	} else if (test == 0) {
		mDrawParticleFuncList[idx] = JPASetLineWidth;
	} else {
		mDrawParticleFuncList[idx] = JPASetPointSize;
	}

	if (!isGlblClrAnm) {
		if (!(clrFlag & 2)) {
			if (exShapeSet2) {
				if (!(clrFlag & 8)) {
					mDrawParticleFuncList[idx] = JPARegistAlpha;
				} else {
					mDrawParticleFuncList[idx] = JPARegistAlphaEnv;
				}
			} else if (clrFlag & 8) {
				mDrawParticleFuncList[idx] = JPARegistEnv;
			}
		} else if (!(clrFlag & 8)) {
			mDrawParticleFuncList[idx] = JPARegistPrmAlpha;
		} else {
			mDrawParticleFuncList[idx] = JPARegistPrmAlphaEnv;
		}
	} else if (exShapeSet2 && !test) {
		mDrawParticleFuncList[idx] = JPARegistAlpha;
	}

	if (shpFlags && pCsp && !test) {
		mDrawParticleChildFuncListNum++;
	}
	mDrawParticleChildFuncListNum++;
	if (test) {
		mDrawParticleChildFuncListNum++;
	}
	if (pCsp && !(pCsp->mData->mFlags & 0x800000) && !(pCsp->mData->mFlags & 0x200000)
	    && !(pCsp->mData->mFlags & 0x400000)) {
		mDrawParticleChildFuncListNum++;
	}
	if (mDrawParticleChildFuncListNum) {
		mDrawParticleChildFuncList = (JPAFunctionB**)heap->alloc(mDrawParticleChildFuncListNum * 4, 4);
	}
	// my sanity is all gone by this point
	if (shpFlags && pCsp && !test) {
		switch (pBsp->getType()) {
		case 0:
			mDrawParticleChildFuncList[0] = JPADrawPoint;
			break;
		case 1:
			mDrawParticleChildFuncList[0] = JPADrawLine;
			break;
		case 2:
			if (exShapeSet4)
				mDrawParticleChildFuncList[0] = JPADrawRotBillboard;
			else
				mDrawParticleChildFuncList[0] = JPADrawBillboard;
			break;
		case 3:
		case 4:
			if (exShapeSet4) {
				mDrawParticleChildFuncList[0] = JPADrawRotDirection;
			} else {
				mDrawParticleChildFuncList[0] = JPADrawDirection;
			}
			break;
		case 7:
		case 8:
			mDrawParticleChildFuncList[0] = JPADrawRotation;
			break;
		case 9:
			mDrawParticleChildFuncList[0] = JPADrawDBillboard;
			break;
		case 10:
			if (exShapeSet4) {
				mDrawParticleChildFuncList[0] = JPADrawRotYBillboard;
			} else {
				mDrawParticleChildFuncList[0] = JPADrawYBillboard;
			}
			break;
		}
		idx = 1;
	}

	mDrawParticleChildFuncList[idx] = JPADrawParticleCallBack;
	if (test) {
		mDrawParticleChildFuncList[idx] = JPASetPointSize;
	} else {
		mDrawParticleChildFuncList[idx] = JPASetLineWidth;
	}
	if (pCsp && !(pCsp->mData->mFlags & 0x800000) && !(pCsp->mData->mFlags & 0x200000)
	    && (pCsp->mData->mFlags & 0x400000)) {
		mDrawParticleChildFuncList[idx] = JPARegistPrmAlphaEnv;
	}
}

bool JPAResource::calc(JPAEmitterWorkData* data, JPABaseEmitter* emitter)
{
	data->mEmitter      = emitter;
	data->mResource     = this;
	data->mCreateNumber = 0;

	if (!emitter->processTillStartFrame()) {
		return false;
	}

	if (emitter->processTermination()) {
		return false;
	}

	if (emitter->checkStatus(JPAEMIT_StopCalc)) {
		JPAEmitterCallBack* cback = emitter->mEmitterCallback;
		if (cback) {
            cback->execute(emitter);
            if (emitter->checkStatus(JPAEMIT_ForceDelete)) {
                return true;
            }

            emitter->mEmitterCallback->executeAfter(emitter);
            if (emitter->checkStatus(JPAEMIT_ForceDelete)) {
                return true;
            }
		}
	} else {
        calcKey(data);

        for (int i = fldNum - 1; 0 <= i; i--) {
            ppFld[i]->initOpParam();
        }

        if (emitter->mEmitterCallback) {
            emitter->mEmitterCallback->execute(emitter);
            if (emitter->checkStatus(JPAEMIT_ForceDelete)) {
                return true;
            }
        }

        calcWorkData_c(data);

        for (int i = mCalcEmitterFuncListNum - 1; 0 <= i; i--) {
            mCalcEmitterFuncList[i](data);
        }

        for (int i = fldNum - 1; 0 <= i; i--) {
            ppFld[i]->pFld->prepare(data, ppFld[i]);
        }

        if (emitter->checkStatus(JPAEMIT_EnableDeleteEmitter)) {
            pDyn->create(data);
        }

        if (emitter->mEmitterCallback) {
            emitter->mEmitterCallback->executeAfter(emitter);
            if (emitter->checkStatus(JPAEMIT_ForceDelete)) {
                return true;
            }
        }

        FOREACH_NODE(JPANode<JPABaseParticle>, data->mEmitter->mAlivePtclBase.getLast(), node)
        {
            if (node->getObject()->calc_p(data)) {
                // something like this
                emitter->mAlivePtclBase.push_back(node);
                emitter->mAlivePtclBase.pop_front();
            }
        }

        FOREACH_NODE(JPANode<JPABaseParticle>, data->mEmitter->mAlivePtclChld.getLast(), node)
        {
            if (node->getObject()->calc_c(data)) {
                emitter->mAlivePtclChld.push_back(node);
                emitter->mAlivePtclChld.pop_front();
            }
        }

        emitter->mCurrentFrame++;
    }


	return false;
}

void JPAResource::draw(JPAEmitterWorkData* work, JPABaseEmitter* emtr)
{
	work->mEmitter   = emtr;
	work->mResource  = this;
	work->mDrawCount = 0;
	calcWorkData_d(work);
	pBsp->setGX(work);
	for (s32 i = 1; i <= emtr->getDrawTimes(); i++) {
		work->mDrawCount++;
		if (getBsp()->isDrawPrntAhead() && pCsp != nullptr)
			drawC(work);
		drawP(work);
		if (!getBsp()->isDrawPrntAhead() && pCsp != nullptr)
			drawC(work);
	}
}

void JPAResource::drawP(JPAEmitterWorkData* data)
{
	data->mEmitter->resetFlag(JPAEMIT_DrawChild);
	data->mGlobalPtclScl.x = data->mEmitter->mGlobalPScl.x * pBsp->pBsd->mBaseSizeX;
	data->mGlobalPtclScl.y = data->mEmitter->mGlobalPScl.y * pBsp->pBsd->mBaseSizeY;
	u32 flag               = pBsp->getType();
	if (flag == 0) {
		data->mGlobalPtclScl.x *= 1.02f;
	} else if (flag == 1) {
		data->mGlobalPtclScl.x *= 1.02f;
		data->mGlobalPtclScl.y *= 0.4f;
	}

	if (pEsp && pEsp->isEnableScaleAnm()) {
		data->mPivot.x = (pEsp->mData->mFlags >> 0xc & 3) - 1.0f;
		data->mPivot.y = (pEsp->mData->mFlags >> 0xe & 3) - 1.0f;
	} else {
		data->mPivot.y = 0.0f;
		data->mPivot.x = 0.0f;
	}
	bool test      = true;
	data->mDirType = pBsp->getDirType();
	data->mRotType = pBsp->getRotType();

	flag = pBsp->pBsd->mFlags & 15;
	if (flag != 4 && flag != 8) {
		test = false;
	}
	data->mDLType    = test;
	data->mPlaneType = ((u32)data->mDLType) ? 2 : pBsp->getBasePlaneType();
	// int projType;
	// if (pBsp->isPrjTex()) {
	// 	projType = pBsp->getProjType() + 1;
	// } else {
	// 	projType = 0;
	// }
	// data->mProjectionType = projType;
	data->mProjectionType = (!pBsp->isPrjTex()) ? 0 : pBsp->getProjType(); // need this to not optimise
	data->mpAlivePtcl     = &data->mEmitter->mAlivePtclBase;
	setPTev();

	for (int i = mDrawEmitterFuncListNum - 1; 0 <= i; i--) {
		mDrawEmitterFuncList[i](data);
	}

	if (pBsp->pBsd->mFlags & 0x200000) {
		for (JPANode<JPABaseParticle>* node = data->mEmitter->mAlivePtclBase.getLast(); node; node = node->getPrev()) {
			data->mpCurNode = node;
			if (mDrawParticleFuncList) {
				for (int i = mDrawParticleFuncListNum - 1; 0 <= i; i--) {
					mDrawParticleFuncList[i](data, node->getObject());
				}
			}
		}
	} else {
		FOREACH_NODE(JPANode<JPABaseParticle>, data->mEmitter->mAlivePtclBase.getFirst(), node)
		{
			data->mpCurNode = node;
			if (mDrawParticleFuncList) {
				for (int i = mDrawParticleFuncListNum - 1; 0 <= i; i--) {
					mDrawParticleFuncList[i](data, node->getObject());
				}
			}
		}
	}
	GXSetMisc(GX_MT_XF_FLUSH, 0);
	if (data->mEmitter->mEmitterCallback) {
		data->mEmitter->mEmitterCallback->drawAfter(data->mEmitter);
	}
}

void JPAResource::drawC(JPAEmitterWorkData* data)
{
	data->mEmitter->setFlag(JPAEMIT_DrawChild);
	if (pCsp->isScaleInherited()) {
		data->mGlobalPtclScl.x = data->mEmitter->mGlobalPScl.x * pBsp->pBsd->mBaseSizeX;
		data->mGlobalPtclScl.y = data->mEmitter->mGlobalPScl.y * pBsp->pBsd->mBaseSizeY;
	} else {
		data->mGlobalPtclScl.x = data->mEmitter->mGlobalPScl.x * pCsp->mData->mScaleX;
		data->mGlobalPtclScl.y = data->mEmitter->mGlobalPScl.y * pCsp->mData->mScaleY;
	}

	u32 flag = pCsp->mData->mFlags & 0xf;
	if (flag == 0) {
		data->mGlobalPtclScl.x *= 1.02f;
	} else if (flag == 1) {
		data->mGlobalPtclScl.x *= 1.02f;
		data->mGlobalPtclScl.y *= 0.4f;
	}

	bool test      = true;
	data->mPivot.y = 0.0f;
	data->mPivot.x = 0.0f;
	data->mDirType = pCsp->mData->mFlags >> 4 & 7;
	data->mRotType = pCsp->mData->mFlags >> 7 & 7;

	flag = pCsp->mData->mFlags & 15;
	if (flag != 4 && flag != 8) {
		test = false;
	}
	data->mDLType         = test;
	data->mPlaneType      = ((u32)data->mDLType) ? 2 : pCsp->mData->mFlags >> 10 & 1;
	data->mProjectionType = 0;
	data->mpAlivePtcl     = &data->mEmitter->mAlivePtclChld;
	setCTev(data);

	for (int i = mDrawEmitterChildFuncListNum - 1; 0 <= i; i--) {
		mDrawEmitterChildFuncList[i](data);
	}

	if (pBsp->pBsd->mFlags & 0x200000) {
		for (JPANode<JPABaseParticle>* node = data->mEmitter->mAlivePtclChld.getLast(); node; node = node->getPrev()) {
			data->mpCurNode = node;
			if (mDrawParticleChildFuncList) {
				for (int i = mDrawParticleChildFuncListNum - 1; 0 <= i; i--) {
					mDrawParticleChildFuncList[i](data, node->getObject());
				}
			}
		}
	} else {
		FOREACH_NODE(JPANode<JPABaseParticle>, data->mEmitter->mAlivePtclChld.getFirst(), node)
		{
			data->mpCurNode = node;
			if (mDrawParticleChildFuncList) {
				for (int i = mDrawParticleChildFuncListNum - 1; 0 <= i; i--) {
					mDrawParticleChildFuncList[i](data, node->getObject());
				}
			}
		}
	}
	GXSetMisc(GX_MT_XF_FLUSH, 0);
	if (data->mEmitter->mEmitterCallback) {
		data->mEmitter->mEmitterCallback->drawAfter(data->mEmitter);
	}
}

void JPAResource::setPTev()
{
	u8 nStages         = 1;
	u8 flag            = 1;
	GXTexCoordID coord = GX_TEXCOORD1;
	u8 nIndStages      = 1;
	u8 nTexGens        = 0;

	u32 childflag = pCsp->mData->mFlags & 0xf;
	if (childflag != 3 && childflag != 7) {
		flag = 0;
	}

	int offs;
	if (pEsp) {
		u32 shpflag = pEsp->mData->mFlags & 0xf;
		offs        = ((shpflag >> 0xc & 3) + (shpflag >> 0xe & 3) * 3) * 0xc;
	} else {
		offs = 0x30;
	} // some nonsense here
	GXSetArray(GX_VA_POS, &jpa_pos[offs + flag], 3);
	GXSetArray(GX_VA_TEX0, &jpa_crd[offs], 2);

	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

	if (pEts) {
		if (pEts->mData->mFlags & 0x1) {
			nIndStages = 1;
		}

		if (nIndStages) {
			GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
			GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
			const JPAExTexShapeData* data = pEts->mData;
			GXSetIndTexMtx(GX_ITM_0, data->mIndTexMtx, data->mExpScale);
			GXSetTevIndirect(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_STU, GX_ITM_0, GX_ITW_OFF, GX_ITW_OFF, GX_FALSE, GX_FALSE,
			                 GX_ITBA_OFF);
			coord      = GX_TEXCOORD2;
			nIndStages = 1;
			nTexGens   = 2;
		}

		if ((pEts->mData->mFlags & 0x100) != 0) {
			GXSetTevOrder(GX_TEVSTAGE1, coord, GX_TEXMAP3, GX_COLOR_NULL);
			GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
			GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
			GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			nStages = 2;
			nTexGens += 1;
		}
	}
	GXSetNumTevStages(nStages);
	GXSetNumIndStages(nIndStages);

	if (pBsp->pBsd->mFlags & 0x800000) {
		GXSetMisc(GX_MT_XF_FLUSH, 8);
		GXSetClipMode(GX_CLIP_ENABLE);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}
	GXSetNumTexGens(nTexGens);
}

void JPAResource::setCTev(JPAEmitterWorkData* data)
{
	u32 flag = pCsp->mData->mFlags & 0xf;
	bool set = true;
	if (flag != 3 && flag != 7) {
		set = false;
	}
	// some nonsense here
	GXSetArray(GX_VA_POS, &jpa_pos[flag >> 10 & 1 & (set ? 1 : 0)], 3);

	GXSetArray(GX_VA_TEX0, jpa_crd, 2);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTexCoordGen2(GX_TEXCOORD_NULL, GX_TG_MTX3x4, GX_TG_TEX0, 0x3c, GX_FALSE, 0x7d);
	GXSetTevDirect(GX_TEVSTAGE0);
	GXSetNumTevStages(1);
	GXSetNumIndStages(0);

	if (pCsp->mData->mFlags & 0x100000) {
		GXSetMisc(GX_MT_XF_FLUSH, 8);
		GXSetClipMode(GX_CLIP_ENABLE);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}
	GXSetNumTexGens(1);

	data->mResourceMgr->pTexAry[data->mResource->texIdxTbl[pCsp->mData->mTexIdx]]->load(GX_TEXMAP1);
}

void JPAResource::calc_p(JPAEmitterWorkData* workData, JPABaseParticle* particle)
{
	if (mCalcParticleFuncList == nullptr) {
		return;
	}
	for (int i = mCalcParticleFuncListNum - 1; 0 <= i; i--) {
		mCalcParticleFuncList[i](workData, particle);
	}
}

void JPAResource::calc_c(JPAEmitterWorkData* workData, JPABaseParticle* particle)
{
	if (!mCalcParticleChildFuncList) {
		return;
	}

	for (int i = mCalcParticleChildFuncListNum - 1; 0 <= i; i--) {
		mCalcParticleChildFuncList[i](workData, particle);
	}
}

void JPAResource::calcField(JPAEmitterWorkData* workData, JPABaseParticle* particle)
{
	for (int i = fldNum - 1; 0 <= i; i--) {
		ppFld[i]->pFld->calc(workData, ppFld[i], particle);
	}
}

void JPAResource::calcKey(JPAEmitterWorkData* data)
{
	for (int i = keyNum - 1; i >= 0; i--) {
		f32 calc = ppKey[i]->calc(data->mEmitter->mCurrentFrame);

		switch (ppKey[i]->mDataStart->mFlag) {
		case 0:
			data->mEmitter->setRate(calc);
			break;
		case 1:
			data->mEmitter->setVolumeSize(calc);
			break;
		case 3:
			data->mEmitter->mVolumeMinRad = calc;
			break;
		case 4:
			data->mEmitter->setLifeTime(calc);
			break;
		case 6:
			data->mEmitter->mAwayFromCenterSpeed = calc;
			break;
		case 7:
			data->mEmitter->mAwayFromAxisSpeed = calc;
			break;
		case 8:
			data->mEmitter->mDirSpeed = calc;
			break;
		case 9:
			data->mEmitter->mSpread = calc;
			break;
		case 10:
			data->mEmitter->mScaleOut = calc;
			break;
        default:
#line 917
            JUT_WARNING_F2("%s", "JPA : WRONG ID in key data\n");
		}
	}
}

void JPAResource::calcWorkData_c(JPAEmitterWorkData* data)
{
	data->mVolumeSize     = data->mEmitter->mVolumeSize;
	data->mVolumeMinRad   = data->mEmitter->mVolumeMinRad;
	data->mVolumeSweep    = data->mEmitter->mVolumeSweep;
	data->mVolumeX        = 0;
	data->mVolumeAngleNum = 0;
	data->mVolumeAngleMax = 1;
	data->mDivNumber      = pDyn->mData->mDivNumber * 2 + 1;

	Mtx mtx, mtx2, mtx3;
	PSMTXScale(mtx, data->mEmitter->mLocalScl.x, data->mEmitter->mLocalScl.y, data->mEmitter->mLocalScl.z);
	JPAGetXYZRotateMtx(data->mEmitter->mLocalRot.x * 182, data->mEmitter->mLocalRot.y * 182, data->mEmitter->mLocalRot.z * 182, mtx2);
	PSMTXScale(mtx3, data->mEmitter->mGlobalScl.x, data->mEmitter->mGlobalScl.y, data->mEmitter->mGlobalScl.z);
	PSMTXConcat(data->mEmitter->mGlobalRot, mtx3, mtx3);
	mtx3[0][3] = data->mEmitter->mGlobalTrs.x;
	mtx3[1][3] = data->mEmitter->mGlobalTrs.y;
	mtx3[2][3] = data->mEmitter->mGlobalTrs.z;
	PSMTXCopy(data->mEmitter->mGlobalRot, data->mRotationMtx);
	PSMTXConcat(data->mRotationMtx, mtx2, data->mGlobalRot);
	PSMTXConcat(data->mGlobalRot, mtx, data->mGlobalSR);

    data->mEmitterPos.set(data->mEmitter->mLocalTrs);
    data->mGlobalScl.mul(data->mEmitter->mGlobalScl, data->mEmitter->mLocalScl);

	JPAGetDirMtx(data->mEmitter->mLocalDir, data->mDirectionMtx);
	data->mPublicScale.set(data->mEmitter->mGlobalScl);
	PSMTXMultVec(mtx3, &data->mEmitter->mLocalTrs, &data->mGlobalPos);
}

void JPAResource::calcWorkData_d(JPAEmitterWorkData* work)
{
	Mtx mtx;
	JPAGetXYZRotateMtx(work->mEmitter->mLocalRot.x * 0xB6, work->mEmitter->mLocalRot.y * 0xB6, work->mEmitter->mLocalRot.z * 0xB6, mtx);
	PSMTXConcat(work->mEmitter->mGlobalRot, mtx, work->mGlobalRot);
	PSMTXMultVecSR(work->mGlobalRot, (Vec*)&work->mEmitter->mLocalDir, (Vec*)&work->mGlobalEmtrDir);
}
