#ifndef SHADOWMODEL_H
#define SHADOWMODEL_H

#include "JSystem/JKernel/JKRHeap.h"
#include "Kaneshige/ExModel.h"

class ShadowModel : ExModel
{
public:
    enum ShadowKind
    {
        // TODO
    };

    ShadowModel();
    virtual void createModel(JKRSolidHeap *, u32, u32); // override
};

class KartShadowModel : ShadowModel {
    virtual void createModel(JKRSolidHeap *, u32, u32); // override
};

#endif