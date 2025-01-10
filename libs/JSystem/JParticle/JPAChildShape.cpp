#include "JSystem/JParticle/JPAShape.h"
#include "JSystem/JParticle/JPAEmitter.h"

void JPARegistChildPrmEnv(JPAEmitterWorkData *work) {
    JPAChildShape* csp   = work->mResource->getCsp();
    JPABaseEmitter* emtr = work->mEmitter;
    GXColor prm, env;
    csp->getPrmClr(&prm);
    csp->getEnvClr(&env);
    prm.r = COLOR_MULTI(prm.r, emtr->mGlobalPrmClr.r);
    prm.g = COLOR_MULTI(prm.g, emtr->mGlobalPrmClr.g);
    prm.b = COLOR_MULTI(prm.b, emtr->mGlobalPrmClr.b);
    prm.a = COLOR_MULTI(prm.a, emtr->mGlobalPrmClr.a);
    env.r = COLOR_MULTI(env.r, emtr->mGlobalEnvClr.r);
    env.g = COLOR_MULTI(env.g, emtr->mGlobalEnvClr.g);
    env.b = COLOR_MULTI(env.b, emtr->mGlobalEnvClr.b);
    GXSetTevColor(GX_TEVREG0, prm);
    GXSetTevColor(GX_TEVREG1, env);
}

void JPACalcChildAlphaOut(JPAEmitterWorkData *workData, JPABaseParticle *particle) {
    f32 anm = (1.0f - particle->mTime) * 255.0f;
    OSf32tou8(&anm, &particle->mPrmColorAlphaAnm);
}

void JPACalcChildScaleOut(JPAEmitterWorkData *workData, JPABaseParticle *particle) {
    particle->mParticleScaleX = particle->mScaleOut * (1.0f - particle->mTime);
    particle->mParticleScaleY = particle->mAlphaWaveRandom * (1.0f - particle->mTime);
}

JPAChildShape::JPAChildShape(const u8 *data) {
    pBsd = (JPAChildShapeData *)data;
}

void JPAChildShape::init_jpa(const u8 *, JKRHeap *) {
    // UNUSED
}
