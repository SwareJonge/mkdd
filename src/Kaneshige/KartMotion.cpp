#include "Kaneshige/KartMotion.h"
#include "dolphin/mtx.h"

KartMtxCalcAdditiveAnm::KartMtxCalcAdditiveAnm() : J3DMtxCalcMayaAdditiveAnm(nullptr) {
    reset();
}

void KartMtxCalcAdditiveAnm::reset() {
    for (u8 i = 0; i < 4; i++) {
        setAnmTransform(i, nullptr);
        setWeight(i, 0.0f);
    }
    _24 = 0;
    _28 = 0.0f;
}

KartMtxCalcBlendAnm::KartMtxCalcBlendAnm() : J3DMtxCalcBlendAnm<J3DMtxCalcBlendAdaptorDefault<J3DMtxCalcBlend,J3DMtxCalcScaleBlendMaya>, J3DMtxCalcJ3DSysInitMaya>(nullptr) {
    reset();
}

void KartMtxCalcBlendAnm::reset() {
    for (u8 i = 0; i < 4; i++) {
        setAnmTransform(i, nullptr);
        setWeight(i, 0.0f);
    }
}

void KartMtxCalcBlendAnm::setAnm(J3DAnmTransform *anm1, J3DAnmTransform *anm2, J3DAnmTransform *anm3, J3DAnmTransform *anm4) {
    setAnmWeight(anm1, 1.0f, anm2, 0.0f, anm3, 0.0f, anm4, 0.0f);
}

void KartMtxCalcBlendAnm::setAnmWeight(J3DAnmTransform *anm1, f32 w1, J3DAnmTransform *anm2, f32 w2, J3DAnmTransform *anm3, f32 w3, J3DAnmTransform *anm4, f32 w4) {
    setAnmTransform(0, anm1);
    setWeight(0, w1);

    setAnmTransform(1, anm2);
    setWeight(1, w2);

    setAnmTransform(2, anm3);
    setWeight(2, w3);

    setAnmTransform(3, anm4);
    setWeight(3, w4);
}
