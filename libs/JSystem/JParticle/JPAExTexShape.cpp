#include "JSystem/JParticle/JPAShape.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "dolphin/gx.h"

// TODO: replace GX functions with proper enumerated values

void JPALoadExTex(JPAEmitterWorkData *work) {
    GXTexCoordID result = GX_TEXCOORD1;

    JPAExTexShape *exTexShape = work->mResource->pEts;
    if (exTexShape->isUseIndirect()) {
        GXSetTexCoordGen2(result, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
        u8 texIdx = exTexShape->getIndTexIdx() & 0xFF;
        work->mResourceMgr->load(work->mResource->getTexIdx(texIdx), GX_TEXMAP2);
        result = GX_TEXCOORD2;
    }

    if (exTexShape->isUseSecTex()) {
        GXSetTexCoordGen2(result, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
        u8 texIdx = exTexShape->getSecTexIdx() & 0xFF;
        work->mResourceMgr->load(work->mResource->getTexIdx(texIdx), GX_TEXMAP3);
    }
}

JPAExTexShape::JPAExTexShape(const u8 *data) : mData((JPAExTexShapeData*)data) { }


void JPAExTexShape::init_jpa(const u8*, JKRHeap*)
{
    // UNUSED FUNCTION
}
