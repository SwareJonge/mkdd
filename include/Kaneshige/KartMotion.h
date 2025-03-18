#ifndef KARTMOTION_H
#define KARTMOTION_H

#include "JSystem/J3D/J3DAnmTransform.h"
#include "JSystem/J3D/J3DMtxCalc.h"

class KartMtxCalcBlendAnm : public J3DMtxCalcBlendAnm<J3DMtxCalcBlendAdaptorDefault<J3DMtxCalcBlend,J3DMtxCalcScaleBlendMaya>, J3DMtxCalcJ3DSysInitMaya> {
public:
    KartMtxCalcBlendAnm();
    virtual ~KartMtxCalcBlendAnm() {}

    void reset();
    void setAnm(J3DAnmTransform *, J3DAnmTransform *, J3DAnmTransform *, J3DAnmTransform *);
    void setAnmWeight(J3DAnmTransform *, float, J3DAnmTransform *, float, J3DAnmTransform *, float, J3DAnmTransform *, float);

};

class KartMtxCalcAdditiveAnm : public J3DMtxCalcMayaAdditiveAnm {
public:
    KartMtxCalcAdditiveAnm();
    virtual ~KartMtxCalcAdditiveAnm() {}
    void reset();
};


#endif // KARTMOTION_H
