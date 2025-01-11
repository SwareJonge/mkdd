#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAField.h"
#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JParticle/JPAShape.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "macros.h"

// this most likely is some struct
u8 jpa_pos[0x160] ALIGN(32)
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

u8 jpa_crd[32]  = { 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 2, 0, 2, 1, 0, 1, 0, 0, 1, 0, 1, 2, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2 };

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

 // pBsp->isGlblTexAnm(); is typed u32, which i guess is a fakematch
 // it's more likely some member or another temp is u32 instead of int
void JPAResource::init(JKRHeap *heap)
{
    BOOL is_glbl_clr_anm = pBsp->isGlblClrAnm();
    BOOL is_glbl_tex_anm = pBsp->isGlblTexAnm();
    BOOL is_prm_anm = pBsp->isPrmAnm();
    BOOL is_env_anm = pBsp->isEnvAnm();
    BOOL is_tex_anm = pBsp->isTexAnm();
    
    BOOL is_tex_crd_anm = pBsp->isTexCrdAnm();
    BOOL is_prj_tex = pBsp->isPrjTex();
    BOOL is_enable_scale_anm = pEsp && pEsp->isEnableScaleAnm();
    BOOL is_enable_alpha_anm = pEsp && pEsp->isEnableAlphaAnm();
    BOOL is_enable_alpha_flick = pEsp && pEsp->isEnableAlphaAnm()
                                                      && pEsp->isEnableAlphaFlick();
    BOOL is_enable_rotate_anm = pEsp && pEsp->isEnableRotateAnm();
    BOOL is_rotate_on = is_enable_rotate_anm
                        || (pCsp && pCsp->isRotateOn());
    BOOL base_type_5_6 = pBsp->getType() == 5 || pBsp->getType() == 6;
    BOOL base_type_0 = pBsp->getType() == 0;
    BOOL base_type_0_1 = pBsp->getType() == 0 || pBsp->getType() == 1;
    BOOL child_type_5_6 = pCsp
                          && (pCsp->getType() == 5 || pCsp->getType() == 6);
    BOOL child_type_0 = pCsp && pCsp->getType() == 0;
    BOOL child_type_0_1 = pCsp
                          && (pCsp->getType() == 0 || pCsp->getType() == 1);
    BOOL is_draw_parent = !pBsp->isNoDrawParent();
    BOOL is_draw_child = !pBsp->isNoDrawChild();

    if (is_glbl_tex_anm && is_tex_anm) {
        mCalcEmitterFuncListNum++;
    }

    if (is_glbl_clr_anm) {
        if (is_prm_anm) {
            mCalcEmitterFuncListNum++;
        }
        if (is_env_anm) {
            mCalcEmitterFuncListNum++;
        }
        if (is_prm_anm || is_env_anm) {
            mCalcEmitterFuncListNum++;
        }
    }

    if (mCalcEmitterFuncListNum != 0) {
        mCalcEmitterFuncList =
            (JPAEmitterFunc*)JKRAllocFromHeap(heap, mCalcEmitterFuncListNum * 4, 4);
    }

    int func_no = 0;

    if (is_glbl_tex_anm && is_tex_anm) {
        switch (pBsp->getTexAnmType()) {
        case 0:
            mCalcEmitterFuncList[func_no] = &JPACalcTexIdxNormal;
            break;
        case 1:
            mCalcEmitterFuncList[func_no] = &JPACalcTexIdxRepeat;
            break;
        case 2:
            mCalcEmitterFuncList[func_no] = &JPACalcTexIdxReverse;
            break;
        case 3:
            mCalcEmitterFuncList[func_no] = &JPACalcTexIdxMerge;
            break;
        case 4:
            mCalcEmitterFuncList[func_no] = &JPACalcTexIdxRandom;
            break;
        }
        func_no++;
    }

    if (is_glbl_clr_anm) {
        if (is_prm_anm) {
            mCalcEmitterFuncList[func_no] = &JPACalcPrm;
            func_no++;
        }
        if (is_env_anm) {
            mCalcEmitterFuncList[func_no] = &JPACalcEnv;
            func_no++;
        }
        if (is_prm_anm || is_env_anm) {
            switch (pBsp->getClrAnmType()) {
            case 0:
                mCalcEmitterFuncList[func_no] = &JPACalcClrIdxNormal;
                break;
            case 1:
                mCalcEmitterFuncList[func_no] = &JPACalcClrIdxRepeat;
                break;
            case 2:
                mCalcEmitterFuncList[func_no] = &JPACalcClrIdxReverse;
                break;
            case 3:
                mCalcEmitterFuncList[func_no] = &JPACalcClrIdxMerge;
                break;
            case 4:
                mCalcEmitterFuncList[func_no] = &JPACalcClrIdxRandom;
                break;
            }
        }
    }

    if (!is_glbl_tex_anm && is_tex_anm) {
        mCalcParticleFuncListNum++;
    }

    if (!base_type_5_6 && (is_enable_alpha_anm || is_enable_alpha_flick)) {
        mCalcParticleFuncListNum++;
    }

    if (!is_glbl_clr_anm) {
        if (is_prm_anm) {
            mCalcParticleFuncListNum++;
        }
        if (is_env_anm) {
            mCalcParticleFuncListNum++;
        }
        if (is_prm_anm || is_env_anm) {
            mCalcParticleFuncListNum++;
        }
    } else {
        mCalcParticleFuncListNum++;
    }

    if (is_enable_scale_anm) {
        if (pBsp->getType() != 0) {
            if (pEsp->isScaleXYDiff()) {
                if (pEsp->getScaleAnmTypeX() == 0 && pEsp->getScaleAnmTypeY() == 0) {
                    mCalcParticleFuncListNum++;
                } else {
                    mCalcParticleFuncListNum++;
                    mCalcParticleFuncListNum++;
                }
            } else {
                mCalcParticleFuncListNum++;
            }
        }
        mCalcParticleFuncListNum++;
        mCalcParticleFuncListNum++;
    }

    if (mCalcParticleFuncListNum != 0) {
        mCalcParticleFuncList =
            (JPAParticleFunc*)JKRAllocFromHeap(heap, mCalcParticleFuncListNum * 4, 4);
    }

    func_no = 0;

    if (!is_glbl_tex_anm && is_tex_anm) {
        switch (pBsp->getTexAnmType()) {
        case 0:
            mCalcParticleFuncList[func_no] = &JPACalcTexIdxNormal;
            break;
        case 1:
            mCalcParticleFuncList[func_no] = &JPACalcTexIdxRepeat;
            break;
        case 2:
            mCalcParticleFuncList[func_no] = &JPACalcTexIdxReverse;
            break;
        case 3:
            mCalcParticleFuncList[func_no] = &JPACalcTexIdxMerge;
            break;
        case 4:
            mCalcParticleFuncList[func_no] = &JPACalcTexIdxRandom;
            break;
        }
        func_no++;
    }

    if (!base_type_5_6 && (is_enable_alpha_anm || is_enable_alpha_flick)) {
        if (is_enable_alpha_flick) {
            mCalcParticleFuncList[func_no] = &JPACalcAlphaFlickAnm;
            func_no++;
        } else {
            mCalcParticleFuncList[func_no] = &JPACalcAlphaAnm;
            func_no++;
        }
    }

    if (!is_glbl_clr_anm) {
        if (is_prm_anm) {
            mCalcParticleFuncList[func_no] = &JPACalcPrm;
            func_no++;
        }
        if (is_env_anm) {
            mCalcParticleFuncList[func_no] = &JPACalcEnv;
            func_no++;
        }
        if (is_prm_anm || is_env_anm) {
            switch (pBsp->getClrAnmType()) {
            case 0:
                mCalcParticleFuncList[func_no] = &JPACalcClrIdxNormal;
                break;
            case 1:
                mCalcParticleFuncList[func_no] = &JPACalcClrIdxRepeat;
                break;
            case 2:
                mCalcParticleFuncList[func_no] = &JPACalcClrIdxReverse;
                break;
            case 3:
                mCalcParticleFuncList[func_no] = &JPACalcClrIdxMerge;
                break;
            case 4:
                mCalcParticleFuncList[func_no] = &JPACalcClrIdxRandom;
                break;
            }
            func_no++;
        }
    } else {
        mCalcParticleFuncList[func_no] = &JPACalcColorCopy;
        func_no++;
    }

    if (is_enable_scale_anm) {
        if (pBsp->getType() != 0) {
            if (pEsp->isScaleXYDiff()) {
                mCalcParticleFuncList[func_no] = &JPACalcScaleY;
                func_no++;
                if (pEsp->getScaleAnmTypeY() != 0 || pEsp->getScaleAnmTypeX() != 0) {
                    switch (pEsp->getScaleAnmTypeY()) {
                    case 0:
                        mCalcParticleFuncList[func_no] = &JPACalcScaleAnmNormal;
                        break;
                    case 1:
                        mCalcParticleFuncList[func_no] = &JPACalcScaleAnmRepeatY;
                        break;
                    case 2:
                        mCalcParticleFuncList[func_no] = &JPACalcScaleAnmReverseY;
                        break;
                    }
                    func_no++;
                }
            } else {
                mCalcParticleFuncList[func_no] = &JPACalcScaleCopy;
                func_no++;
            }
        }
        mCalcParticleFuncList[func_no] = &JPACalcScaleX;
        func_no++;
        switch (pEsp->getScaleAnmTypeX()) {
        case 0:
            mCalcParticleFuncList[func_no] = &JPACalcScaleAnmNormal;
            break;
        case 1:
            mCalcParticleFuncList[func_no] = &JPACalcScaleAnmRepeatX;
            break;
        case 2:
            mCalcParticleFuncList[func_no] = &JPACalcScaleAnmReverseX;
            break;
        }
    }

    if (pCsp && pCsp->isScaleOutOn()) {
        mCalcParticleChildFuncListNum++;
    }

    if (pCsp && pCsp->isAlphaOutOn()) {
        mCalcParticleChildFuncListNum++;
    }

    if (mCalcParticleChildFuncListNum != 0) {
        mCalcParticleChildFuncList =
            (JPAParticleFunc*)JKRAllocFromHeap(heap, mCalcParticleChildFuncListNum * 4, 4);
    }

    func_no = 0;

    if (pCsp && pCsp->isScaleOutOn()) {
        mCalcParticleChildFuncList[func_no] = &JPACalcChildScaleOut;
        func_no++;
    }

    if (pCsp && pCsp->isAlphaOutOn()) {
        mCalcParticleChildFuncList[func_no] = &JPACalcChildAlphaOut;
    }

    if (is_draw_parent && base_type_5_6) {
        mDrawEmitterFuncListNum++;
    }

    mDrawEmitterFuncListNum++;

    if (pEts) {
        mDrawEmitterFuncListNum++;
    }

    if (is_glbl_tex_anm || !is_tex_anm) {
        mDrawEmitterFuncListNum++;
    }

    mDrawEmitterFuncListNum++;

    if (base_type_0_1) {
        mDrawEmitterFuncListNum++;
    }
    
    if (base_type_0_1 && !is_enable_scale_anm) {
        mDrawEmitterFuncListNum++;
    }

    if (is_glbl_clr_anm || (!is_prm_anm && !is_enable_alpha_anm) || !is_env_anm) {
        mDrawEmitterFuncListNum++;
    }

    if (mDrawEmitterFuncListNum != 0) {
        mDrawEmitterFuncList =
            (JPAEmitterFunc*)JKRAllocFromHeap(heap, mDrawEmitterFuncListNum * 4, 4);
    }

    func_no = 0;

    if (is_draw_parent && base_type_5_6) {
        if (pBsp->getType() == 5) {
            mDrawEmitterFuncList[func_no] = &JPADrawStripe;
            func_no++;
        } else {
            mDrawEmitterFuncList[func_no] = &JPADrawStripeX;
            func_no++;
        }
    }

    mDrawEmitterFuncList[func_no] = &JPADrawEmitterCallBackB;
    func_no++;

    if (pEts) {
        mDrawEmitterFuncList[func_no] = &JPALoadExTex;
        func_no++;
    }

    if (!is_tex_anm) {
        mDrawEmitterFuncList[func_no] = &JPALoadTex;
        func_no++;
    } else if (is_glbl_tex_anm) {
        mDrawEmitterFuncList[func_no] = &JPALoadTexAnm;
        func_no++;
    }

    if (base_type_0_1) {
        mDrawEmitterFuncList[func_no] = &JPAGenTexCrdMtxIdt;
        func_no++;
    } else if (is_prj_tex) {
        mDrawEmitterFuncList[func_no] = &JPAGenTexCrdMtxPrj;
        func_no++;
    } else if (is_tex_crd_anm) {
        if (base_type_5_6) {
            mDrawEmitterFuncList[func_no] = &JPAGenCalcTexCrdMtxAnm;
            func_no++;
        } else {
            mDrawEmitterFuncList[func_no] = &JPAGenTexCrdMtxAnm;
            func_no++;
        }
    } else {
        mDrawEmitterFuncList[func_no] = &JPAGenTexCrdMtxIdt;
        func_no++;
    }

    if (base_type_0_1) {
        mDrawEmitterFuncList[func_no] = &JPALoadPosMtxCam;
        func_no++;
    }

    if (base_type_0_1 && !is_enable_scale_anm) {
        if (base_type_0) {
            mDrawEmitterFuncList[func_no] = &JPASetPointSize;
            func_no++;
        } else {
            mDrawEmitterFuncList[func_no] = &JPASetLineWidth;
            func_no++;
        }
    }

    if (is_glbl_clr_anm) {
        if (base_type_5_6 || !is_enable_alpha_anm) {
            mDrawEmitterFuncList[func_no] = &JPARegistPrmEnv;
        } else if (is_enable_alpha_anm) {
            mDrawEmitterFuncList[func_no] = &JPARegistEnv;
        }
    } else if (!is_prm_anm && !is_enable_alpha_anm) {
        if (!is_env_anm) {
            mDrawEmitterFuncList[func_no] = &JPARegistPrmEnv;
        } else {
            mDrawEmitterFuncList[func_no] = &JPARegistPrm;
        }
    } else if (!is_env_anm) {
        mDrawEmitterFuncList[func_no] = &JPARegistEnv;
    }

    if (is_draw_child && child_type_5_6) {
        mDrawEmitterChildFuncListNum++;
    }

    mDrawEmitterChildFuncListNum++;

    if (child_type_0_1) {
        mDrawEmitterChildFuncListNum++;
    }

    if (pCsp && !pCsp->isAlphaOutOn() && !pCsp->isAlphaInherited()
                             && !pCsp->isColorInherited()) {
        mDrawEmitterChildFuncListNum++;
    }

    if (mDrawEmitterChildFuncListNum != 0) {
        mDrawEmitterChildFuncList =
            (JPAEmitterFunc*)JKRAllocFromHeap(heap, mDrawEmitterChildFuncListNum * 4, 4);
    }

    func_no = 0;

    if (is_draw_child && child_type_5_6) {
        if (pCsp->getType() == 5) {
            mDrawEmitterChildFuncList[func_no] = &JPADrawStripe;
            func_no++;
        } else {
            mDrawEmitterChildFuncList[func_no] = &JPADrawStripeX;
            func_no++;
        }
    }

    mDrawEmitterChildFuncList[func_no] = &JPADrawEmitterCallBackB;
    func_no++;

    if (child_type_0_1) {
        mDrawEmitterChildFuncList[func_no] = &JPALoadPosMtxCam;
        func_no++;
    }

    if (pCsp && !pCsp->isAlphaOutOn() && !pCsp->isAlphaInherited()
                             && !pCsp->isColorInherited()) {
        mDrawEmitterChildFuncList[func_no] = &JPARegistChildPrmEnv;
    }

    if (is_draw_parent && !base_type_5_6) {
        mDrawParticleFuncListNum++;
    }

    mDrawParticleFuncListNum++;

    if (!is_glbl_tex_anm && is_tex_anm) {
        mDrawParticleFuncListNum++;
    }

    if ((base_type_0_1 && is_enable_scale_anm) || (is_tex_crd_anm && !is_prj_tex)) {
        mDrawParticleFuncListNum++;
    }

    if ((!is_glbl_clr_anm && (is_prm_anm || is_env_anm || is_enable_alpha_anm))
                || (is_glbl_clr_anm && is_enable_alpha_anm && !base_type_5_6)) {
        mDrawParticleFuncListNum++;
    }

    if (mDrawParticleFuncListNum != 0) {
        mDrawParticleFuncList =
            (JPAParticleFunc*)JKRAllocFromHeap(heap, mDrawParticleFuncListNum * 4, 4);
    }

    func_no = 0;

    if (is_draw_parent && !base_type_5_6) {
        switch (pBsp->getType()) {
        case 2:
            if (is_enable_rotate_anm) {
                mDrawParticleFuncList[func_no] = &JPADrawRotBillboard;
            } else {
                mDrawParticleFuncList[func_no] = &JPADrawBillboard;
            }
            break;
        case 10:
            if (is_enable_rotate_anm) {
                mDrawParticleFuncList[func_no] = &JPADrawRotYBillboard;
            } else {
                mDrawParticleFuncList[func_no] = &JPADrawYBillboard;
            }
            break;
        case 3:
        case 4:
            if (is_enable_rotate_anm) {
                mDrawParticleFuncList[func_no] = &JPADrawRotDirection;
            } else {
                mDrawParticleFuncList[func_no] = &JPADrawDirection;
            }
            break;
        case 9:
            mDrawParticleFuncList[func_no] = &JPADrawDBillboard;
            break;
        case 7:
        case 8:
            mDrawParticleFuncList[func_no] = &JPADrawRotation;
            break;
        case 0:
            mDrawParticleFuncList[func_no] = &JPADrawPoint;
            break;
        case 1:
            mDrawParticleFuncList[func_no] = &JPADrawLine;
            break;
        }
        func_no++;
    }

    mDrawParticleFuncList[func_no] = &JPADrawParticleCallBack;
    func_no++;

    if (!is_glbl_tex_anm && is_tex_anm) {
        mDrawParticleFuncList[func_no] = &JPALoadTexAnm;
        func_no++;
    }

    if (base_type_0_1 && is_enable_scale_anm) {
        if (base_type_0) {
            mDrawParticleFuncList[func_no] = &JPASetPointSize;
            func_no++;
        } else {
            mDrawParticleFuncList[func_no] = &JPASetLineWidth;
            func_no++;
        }
    } else if (is_tex_crd_anm && !is_prj_tex) {
        mDrawParticleFuncList[func_no] = &JPALoadCalcTexCrdMtxAnm;
        func_no++;
    }

    if (!is_glbl_clr_anm) {
        if (is_prm_anm) {
            if (is_env_anm) {
                mDrawParticleFuncList[func_no] = &JPARegistPrmAlphaEnv;
            } else {
                mDrawParticleFuncList[func_no] = &JPARegistPrmAlpha;
            }
        } else if (is_enable_alpha_anm) {
            if (is_env_anm) {
                mDrawParticleFuncList[func_no] = &JPARegistAlphaEnv;
            } else {
                mDrawParticleFuncList[func_no] = &JPARegistAlpha;
            }
        } else if (is_env_anm) {
            mDrawParticleFuncList[func_no] = &JPARegistEnv;
        }
    } else if (is_enable_alpha_anm && !base_type_5_6) {
        mDrawParticleFuncList[func_no] = &JPARegistAlpha;
    }

    if (is_draw_child && pCsp && !child_type_5_6) {
        mDrawParticleChildFuncListNum++;
    }

    mDrawParticleChildFuncListNum++;

    if (child_type_0_1) {
        mDrawParticleChildFuncListNum++;
    }

    if (pCsp && (pCsp->isAlphaOutOn() || pCsp->isAlphaInherited()
                                || pCsp->isColorInherited())) {
        mDrawParticleChildFuncListNum++;
    }

    if (mDrawParticleChildFuncListNum != 0) {
        mDrawParticleChildFuncList =
            (JPAParticleFunc*)JKRAllocFromHeap(heap, mDrawParticleChildFuncListNum * 4, 4);
    }

    func_no = 0;

    if (is_draw_child && pCsp && !child_type_5_6) {
        switch (pCsp->getType()) {
        case 2:
            if (is_rotate_on) {
                mDrawParticleChildFuncList[func_no] = &JPADrawRotBillboard;
            } else {
                mDrawParticleChildFuncList[func_no] = &JPADrawBillboard;
            }
            break;
        case 10:
            if (is_rotate_on) {
                mDrawParticleChildFuncList[func_no] = &JPADrawRotYBillboard;
            } else {
                mDrawParticleChildFuncList[func_no] = &JPADrawYBillboard;
            }
            break;
        case 3:
        case 4:
            if (is_rotate_on) {
                mDrawParticleChildFuncList[func_no] = &JPADrawRotDirection;
            } else {
                mDrawParticleChildFuncList[func_no] = &JPADrawDirection;
            }
            break;
        case 9:
            mDrawParticleChildFuncList[func_no] = &JPADrawDBillboard;
            break;
        case 7:
        case 8:
            mDrawParticleChildFuncList[func_no] = &JPADrawRotation;
            break;
        case 0:
            mDrawParticleChildFuncList[func_no] = &JPADrawPoint;
            break;
        case 1:
            mDrawParticleChildFuncList[func_no] = &JPADrawLine;
            break;
        }
        func_no++;
    }

    mDrawParticleChildFuncList[func_no] = &JPADrawParticleCallBack;
    func_no++;

    if (child_type_0_1) {
        if (child_type_0) {
            mDrawParticleChildFuncList[func_no] = &JPASetPointSize;
            func_no++;
        } else {
            mDrawParticleChildFuncList[func_no] = &JPASetLineWidth;
            func_no++;
        }
    }

    if (pCsp && (pCsp->isAlphaOutOn() || pCsp->isAlphaInherited()
                                || pCsp->isColorInherited())) {
        mDrawParticleChildFuncList[func_no] = &JPARegistPrmAlphaEnv;
    }
}

bool JPAResource::calc(JPAEmitterWorkData *data, JPABaseEmitter *emitter)
{
    data->mEmitter      = emitter;
    data->mResource     = this;
    data->mCreateNumber = 0;

    if (!emitter->processTillStartFrame()) {
        return false;
    }

    if (emitter->processTermination()) {
        return true;
    }

    if (emitter->checkStatus(JPAEMIT_StopCalc)) {
        if (emitter->mEmitterCallback) {
            emitter->mEmitterCallback->execute(emitter);
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

        for (int i = fldNum - 1; i >= 0; i--) {
            ppFld[i]->initOpParam();
        }

        if (emitter->mEmitterCallback) {
            emitter->mEmitterCallback->execute(emitter);
            if (emitter->checkStatus(JPAEMIT_ForceDelete)) {
                return true;
            }
        }

        calcWorkData_c(data);

        for (int i = mCalcEmitterFuncListNum - 1; i >= 0; i--) {
            (*mCalcEmitterFuncList[i])(data);
        }

        for (int i = fldNum - 1; i >= 0; i--) {
            ppFld[i]->prepare(data);
        }

        if (!emitter->checkStatus(JPAEMIT_EnableDeleteEmitter)) {
            pDyn->create(data);
        }

        if (emitter->mEmitterCallback) {
            emitter->mEmitterCallback->executeAfter(emitter);
            if (emitter->checkStatus(JPAEMIT_ForceDelete)) {
                return true;
            }
        }

        JPANode<JPABaseParticle>* node = emitter->mAlivePtclBase.getFirst();
        JPANode<JPABaseParticle>* next;
        while (node != emitter->mAlivePtclBase.getEnd()) {
            next = node->getNext();
            if (node->getObject()->calc_p(data)) {
                emitter->mPtclPool->push_front(emitter->mAlivePtclBase.erase(node));
            }
            node = next;
        }

        node = emitter->mAlivePtclChld.getFirst();
        while (node != emitter->mAlivePtclChld.getEnd()) {
            next = node->getNext();
            if (node->getObject()->calc_c(data)) {
                emitter->mPtclPool->push_front(emitter->mAlivePtclChld.erase(node));
            }
            node = next;
        }

        emitter->mCurrentFrame++;
    }

    return false;
}

void JPAResource::draw(JPAEmitterWorkData *work, JPABaseEmitter *emitter)
{
    work->mEmitter   = emitter;
    work->mResource  = this;
    work->mDrawCount = 0;
    calcWorkData_d(work);
    pBsp->setGX(work);
    for (s32 i = 1; i <= emitter->getDrawTimes(); i++) {
        work->mDrawCount++;
        if (getBsp()->isDrawPrntAhead() && pCsp != nullptr)
            drawC(work);
        drawP(work);
        if (!getBsp()->isDrawPrntAhead() && pCsp != nullptr)
            drawC(work);
    }
}

void JPAResource::drawP(JPAEmitterWorkData *data)
{
    data->mEmitter->clearStatus(JPAEMIT_DrawChild);
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

    data->mProjectionType = (pBsp->isPrjTex()) ? (pBsp->isTexCrdAnm() ? 2 : 1) : 0;
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

void JPAResource::drawC(JPAEmitterWorkData *data)
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
    GXTexCoordID coord = GX_TEXCOORD1;
    u8 nStages         = 1;
    u8 nTexGens        = 1;
    u8 nIndStages      = 0;
    
    int base_plane_type = (pBsp->getType() == 3 || pBsp->getType() == 7) ?
        pBsp->getBasePlaneType() : 0;
    int center_offset;
    if (pEsp) {
        center_offset =
            (pEsp->getScaleCenterX() + 3 * pEsp->getScaleCenterY()) * 0xC;
    } else {
        center_offset = 0x30;
    }
    int pos_offset = center_offset + base_plane_type * 0x6C;
    int crd_offset = (pBsp->getTilingS() + 2 * pBsp->getTilingT()) * 8;
    GXSetArray(GX_VA_POS, jpa_pos + pos_offset, 3);
    GXSetArray(GX_VA_TEX0, jpa_crd + crd_offset, 2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

    if (pEts) {
        if (pEts->isUseIndirect()) {
            GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
            GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
            GXSetIndTexMtx(GX_ITM_0, pEts->getIndTexMtx(), pEts->getExpScale());
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

    if (pBsp->isClipOn()) {
        GXSetMisc(GX_MT_XF_FLUSH, 8);
        GXSetClipMode(GX_CLIP_ENABLE);
    } else {
        GXSetClipMode(GX_CLIP_DISABLE);
    }
    GXSetNumTexGens(nTexGens);
}

void JPAResource::setCTev(JPAEmitterWorkData *data)
{
    int base_plane_type = (pCsp->getType() == 3 || pCsp->getType() == 7) ?
        pCsp->getBasePlaneType() : 0;
    int pos_offset = 0x30 + base_plane_type * 0x6C;
    GXSetArray(GX_VA_POS, jpa_pos + pos_offset, 3);
    GXSetArray(GX_VA_TEX0, jpa_crd, 2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, GX_FALSE, 0x7d);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);

    if (pCsp->isClipOn()) {
        GXSetMisc(GX_MT_XF_FLUSH, 8);
        GXSetClipMode(GX_CLIP_ENABLE);
    } else {
        GXSetClipMode(GX_CLIP_DISABLE);
    }
    GXSetNumTexGens(1);

    data->mResourceMgr->pTexAry[data->mResource->texIdxTbl[pCsp->mData->mTexIdx]]->load(GX_TEXMAP1);
}

void JPAResource::calc_p(JPAEmitterWorkData *workData, JPABaseParticle* particle)
{
    if (mCalcParticleFuncList == nullptr) {
        return;
    }
    for (int i = mCalcParticleFuncListNum - 1; 0 <= i; i--) {
        mCalcParticleFuncList[i](workData, particle);
    }
}

void JPAResource::calc_c(JPAEmitterWorkData *workData, JPABaseParticle* particle)
{
    if (!mCalcParticleChildFuncList) {
        return;
    }

    for (int i = mCalcParticleChildFuncListNum - 1; 0 <= i; i--) {
        mCalcParticleChildFuncList[i](workData, particle);
    }
}

void JPAResource::calcField(JPAEmitterWorkData *workData, JPABaseParticle* particle)
{
    for (int i = fldNum - 1; 0 <= i; i--) {
        ppFld[i]->pFld->calc(workData, ppFld[i], particle);
    }
}

void JPAResource::calcKey(JPAEmitterWorkData *data)
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

void JPAResource::calcWorkData_c(JPAEmitterWorkData *data)
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

void JPAResource::calcWorkData_d(JPAEmitterWorkData *work)
{
    Mtx mtx;
    JPAGetXYZRotateMtx(work->mEmitter->mLocalRot.x * 0xB6, work->mEmitter->mLocalRot.y * 0xB6, work->mEmitter->mLocalRot.z * 0xB6, mtx);
    PSMTXConcat(work->mEmitter->mGlobalRot, mtx, work->mGlobalRot);
    PSMTXMultVecSR(work->mGlobalRot, (Vec*)&work->mEmitter->mLocalDir, (Vec*)&work->mGlobalEmtrDir);
}
