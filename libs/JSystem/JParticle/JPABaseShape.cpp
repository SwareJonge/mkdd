#include "JSystem/JParticle/JPAShape.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JUtility/JUTAssert.h"

// forward declared local functions
static void noLoadPrj(const JPAEmitterWorkData *workData, const Mtx mtx);
static void loadPrj(const JPAEmitterWorkData *workData, const Mtx p2);
static void loadPrjAnm(const JPAEmitterWorkData *workData, const Mtx p2);
static void dirTypeVel(const JPAEmitterWorkData *, const JPABaseParticle *, JGeometry::TVec3f *);
static void dirTypePos(const JPAEmitterWorkData *, const JPABaseParticle *, JGeometry::TVec3f *);
static void dirTypePosInv(const JPAEmitterWorkData *, const JPABaseParticle *, JGeometry::TVec3f *);
static void dirTypeEmtrDir(const JPAEmitterWorkData *, const JPABaseParticle *, JGeometry::TVec3f *);
static void dirTypePrevPtcl(const JPAEmitterWorkData *, const JPABaseParticle *, JGeometry::TVec3f *);
static void rotTypeX(f32, f32, Mtx &);
static void rotTypeY(f32, f32, Mtx &);
static void rotTypeZ(f32, f32, Mtx &);
static void rotTypeXYZ(f32, f32, Mtx &);
static void basePlaneTypeXY(Mtx, f32, f32);
static void basePlaneTypeXZ(Mtx, f32, f32);
static void basePlaneTypeX(Mtx, f32, f32);

static u8 jpa_dl[32] ALIGN(32) = {
    0x80,
    0x00,
    0x04,
    0x00,
    0x00,
    0x01,
    0x01,
    0x02,
    0x02,
    0x03,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

static u8 jpa_dl_x[32] ALIGN(32) = {
    0x80,
    0x00,
    0x08,
    0x00,
    0x00,
    0x01,
    0x01,
    0x02,
    0x02,
    0x03,
    0x03,
    0x48,
    0x00,
    0x49,
    0x01,
    0x4A,
    0x02,
    0x4B,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

typedef void (*projectionFunc)(JPAEmitterWorkData const *, const Mtx);
static projectionFunc p_prj[3] = {
    noLoadPrj,
    loadPrj,
    loadPrjAnm,
};

typedef void (*dirTypeFunc)(const JPAEmitterWorkData *, const JPABaseParticle *, JGeometry::TVec3f *);
static dirTypeFunc p_direction[5] = {
    dirTypeVel,
    dirTypePos,
    dirTypePosInv,
    dirTypeEmtrDir,
    dirTypePrevPtcl,
};

typedef void (*rotTypeFunc)(f32, f32, Mtx &);
static rotTypeFunc p_rot[5] = {
    rotTypeY,
    rotTypeX,
    rotTypeZ,
    rotTypeXYZ,
    rotTypeY,
};

typedef void (*planeFunc)(MtxPtr, f32, f32);
static planeFunc p_plane[3] = {
    basePlaneTypeXY,
    basePlaneTypeXZ,
    basePlaneTypeX,
};

static u8 *p_dl[2] = {
    jpa_dl,
    jpa_dl_x,
};

void JPASetPointSize(JPAEmitterWorkData *workData) { GXSetPointSize(workData->mGlobalPtclScl.x * 25.0f, GX_TO_ONE); }

void JPASetLineWidth(JPAEmitterWorkData *workData) { GXSetLineWidth(workData->mGlobalPtclScl.x * 25.0f, GX_TO_ONE); }

void JPASetPointSize(JPAEmitterWorkData *workData, JPABaseParticle *particle)
{
    GXSetPointSize((particle->mParticleScaleX * (25.0f * workData->mGlobalPtclScl.x)), GX_TO_ONE);
}

void JPASetLineWidth(JPAEmitterWorkData *workData, JPABaseParticle *particle)
{
    GXSetLineWidth((particle->mParticleScaleX * (25.0f * workData->mGlobalPtclScl.x)), GX_TO_ONE);
}

void JPARegistPrm(JPAEmitterWorkData *work)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor prm = emtr->mPrmClr;
    prm.r = COLOR_MULTI(prm.r, emtr->mGlobalPrmClr.r);
    prm.g = COLOR_MULTI(prm.g, emtr->mGlobalPrmClr.g);
    prm.b = COLOR_MULTI(prm.b, emtr->mGlobalPrmClr.b);
    prm.a = COLOR_MULTI(prm.a, emtr->mGlobalPrmClr.a);
    GXSetTevColor(GX_TEVREG0, prm);
}

void JPARegistEnv(JPAEmitterWorkData *work)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor env = emtr->mEnvClr;
    env.r = COLOR_MULTI(env.r, emtr->mGlobalEnvClr.r);
    env.g = COLOR_MULTI(env.g, emtr->mGlobalEnvClr.g);
    env.b = COLOR_MULTI(env.b, emtr->mGlobalEnvClr.b);
    GXSetTevColor(GX_TEVREG1, env);
}

void JPARegistPrmEnv(JPAEmitterWorkData *work)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor prm = emtr->mPrmClr;
    GXColor env = emtr->mEnvClr;
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

void JPARegistAlpha(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor prm = emtr->mPrmClr;
    prm.r = COLOR_MULTI(prm.r, emtr->mGlobalPrmClr.r);
    prm.g = COLOR_MULTI(prm.g, emtr->mGlobalPrmClr.g);
    prm.b = COLOR_MULTI(prm.b, emtr->mGlobalPrmClr.b);
    prm.a = COLOR_MULTI(prm.a, emtr->mGlobalPrmClr.a);
    prm.a = COLOR_MULTI(prm.a, ptcl->mPrmColorAlphaAnm);
    GXSetTevColor(GX_TEVREG0, prm);
}

void JPARegistPrmAlpha(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor prm = ptcl->mPrmClr;
    prm.r = COLOR_MULTI(prm.r, emtr->mGlobalPrmClr.r);
    prm.g = COLOR_MULTI(prm.g, emtr->mGlobalPrmClr.g);
    prm.b = COLOR_MULTI(prm.b, emtr->mGlobalPrmClr.b);
    prm.a = COLOR_MULTI(prm.a, emtr->mGlobalPrmClr.a);
    prm.a = COLOR_MULTI(prm.a, ptcl->mPrmColorAlphaAnm);
    GXSetTevColor(GX_TEVREG0, prm);
}

void JPARegistPrmAlphaEnv(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor prm = ptcl->mPrmClr;
    GXColor env = ptcl->mEnvClr;
    prm.r = COLOR_MULTI(prm.r, emtr->mGlobalPrmClr.r);
    prm.g = COLOR_MULTI(prm.g, emtr->mGlobalPrmClr.g);
    prm.b = COLOR_MULTI(prm.b, emtr->mGlobalPrmClr.b);
    prm.a = COLOR_MULTI(prm.a, emtr->mGlobalPrmClr.a);
    prm.a = COLOR_MULTI(prm.a, ptcl->mPrmColorAlphaAnm);
    env.r = COLOR_MULTI(env.r, emtr->mGlobalEnvClr.r);
    env.g = COLOR_MULTI(env.g, emtr->mGlobalEnvClr.g);
    env.b = COLOR_MULTI(env.b, emtr->mGlobalEnvClr.b);
    GXSetTevColor(GX_TEVREG0, prm);
    GXSetTevColor(GX_TEVREG1, env);
}

void JPARegistAlphaEnv(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor prm = emtr->mPrmClr;
    GXColor env = ptcl->mEnvClr;
    prm.r = COLOR_MULTI(prm.r, emtr->mGlobalPrmClr.r);
    prm.g = COLOR_MULTI(prm.g, emtr->mGlobalPrmClr.g);
    prm.b = COLOR_MULTI(prm.b, emtr->mGlobalPrmClr.b);
    prm.a = COLOR_MULTI(prm.a, emtr->mGlobalPrmClr.a);
    prm.a = COLOR_MULTI(prm.a, ptcl->mPrmColorAlphaAnm);
    env.r = COLOR_MULTI(env.r, emtr->mGlobalEnvClr.r);
    env.g = COLOR_MULTI(env.g, emtr->mGlobalEnvClr.g);
    env.b = COLOR_MULTI(env.b, emtr->mGlobalEnvClr.b);
    GXSetTevColor(GX_TEVREG0, prm);
    GXSetTevColor(GX_TEVREG1, env);
}

void JPARegistEnv(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseEmitter *emtr = work->mEmitter;
    GXColor env = ptcl->mEnvClr;
    env.r = COLOR_MULTI(env.r, emtr->mGlobalEnvClr.r);
    env.g = COLOR_MULTI(env.g, emtr->mGlobalEnvClr.g);
    env.b = COLOR_MULTI(env.b, emtr->mGlobalEnvClr.b);
    GXSetTevColor(GX_TEVREG1, env);
}

void JPACalcClrIdxNormal(JPAEmitterWorkData *work)
{
    JPABaseShape *bsp = work->mResource->getBsp();
    work->mClrKeyFrame = work->mEmitter->mCurrentFrame < bsp->getClrAnmMaxFrm() ? (s16)work->mEmitter->mCurrentFrame : bsp->getClrAnmMaxFrm();
}

void JPACalcClrIdxNormal(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape *bsp = work->mResource->getBsp();
    work->mClrKeyFrame = ptcl->mAge < bsp->getClrAnmMaxFrm() ? ptcl->mAge : bsp->getClrAnmMaxFrm();
}

void JPACalcClrIdxRepeat(JPAEmitterWorkData *work)
{
    JPABaseShape *shape = work->mResource->getBsp();
    work->mClrKeyFrame = work->mEmitter->mCurrentFrame % (shape->getClrAnmMaxFrm() + 1);
}

void JPACalcClrIdxRepeat(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape *shape = work->mResource->getBsp();
    work->mClrKeyFrame = (ptcl->mAge + shape->getClrLoopOfst(ptcl->mAnmRandom)) % (shape->getClrAnmMaxFrm() + 1);
}

void JPACalcClrIdxReverse(JPAEmitterWorkData *work)
{
    JPABaseShape *shape = work->mResource->getBsp();
    u32 maxFrameCount = shape->getClrAnmMaxFrm();

    // Progress is 0 or 1
    s32 colourAnimationProgress = (work->mEmitter->mCurrentFrame / maxFrameCount) & 1;
    u32 remainder = (work->mEmitter->mCurrentFrame % maxFrameCount);

    work->mClrKeyFrame = remainder + (colourAnimationProgress) * (maxFrameCount - remainder * 2);
}

void JPACalcClrIdxReverse(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape *baseShape = work->mResource->getBsp();
    int particleAgeOffset = ptcl->mAge + baseShape->getClrLoopOfst(ptcl->mAnmRandom);    
    int maxFrameCount = baseShape->getClrAnmMaxFrm();
    int progress = ((particleAgeOffset / maxFrameCount) & 1);
    int remainder = particleAgeOffset % maxFrameCount;
    work->mClrKeyFrame = remainder + progress * (maxFrameCount - remainder * 2);
}

void JPACalcClrIdxMerge(JPAEmitterWorkData *workData) { workData->mClrKeyFrame = 0; }

void JPACalcClrIdxMerge(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape *shape = work->mResource->getBsp();
    s32 maxFrm = shape->getClrAnmMaxFrm() + 1;
    work->mClrKeyFrame = ((s32)(ptcl->mTime * maxFrm) + shape->getClrLoopOfst(ptcl->mAnmRandom)) % maxFrm;
}

void JPACalcClrIdxRandom(JPAEmitterWorkData *workData) { workData->mClrKeyFrame = 0; }

void JPACalcClrIdxRandom(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape *shape = work->mResource->getBsp();
    work->mClrKeyFrame = (shape->getClrLoopOfst(ptcl->mAnmRandom)) % (shape->getClrAnmMaxFrm() + 1);
}

void JPACalcPrm(JPAEmitterWorkData *work) { work->mResource->getBsp()->getPrmClr(work->mClrKeyFrame, &work->mEmitter->mPrmClr); }

void JPACalcPrm(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    work->mResource->getBsp()->getPrmClr(work->mClrKeyFrame, &ptcl->mPrmClr);
}

void JPACalcEnv(JPAEmitterWorkData *work) { work->mResource->getBsp()->getEnvClr(work->mClrKeyFrame, &work->mEmitter->mEnvClr); }

void JPACalcEnv(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    work->mResource->getBsp()->getEnvClr(work->mClrKeyFrame, &ptcl->mEnvClr);
}

void JPACalcColorCopy(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseEmitter *emtr = work->mEmitter;
    ptcl->mPrmClr = emtr->mPrmClr;
    ptcl->mEnvClr = emtr->mEnvClr;
}

void JPAGenTexCrdMtxIdt(JPAEmitterWorkData *)
{
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
}

void JPAGenTexCrdMtxAnm(JPAEmitterWorkData *)
{
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
}

void JPAGenTexCrdMtxPrj(JPAEmitterWorkData *)
{
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
}

// lots of instruction swaps
void JPAGenCalcTexCrdMtxAnm(JPAEmitterWorkData *workData)
{
    // Get the base shape from the resource
    JPABaseShape *baseShape = workData->mResource->getBsp();

    // Get the current tick count from the emitter
    f32 tickCount = workData->mEmitter->mCurrentFrame;

    // Calculate half of the tiling for S and T
    f32 halfTilingS = 0.5f * (1.0f + baseShape->getTilingS());
    f32 halfTilingT = 0.5f * (1.0f + baseShape->getTilingT());

    // Calculate the X and Y translations
    f32 transX = (tickCount * baseShape->getIncTransX()) + baseShape->getInitTransX();
    f32 transY = (tickCount * baseShape->getIncTransY()) + baseShape->getInitTransY();

    // Calculate the X and Y scales
    f32 scaleX = (tickCount * baseShape->getIncScaleX()) + baseShape->getInitScaleX();
    f32 scaleY = (tickCount * baseShape->getIncScaleY()) + baseShape->getInitScaleY();

    // Calculate the rotation
    s16 rotation = (tickCount * baseShape->getIncRot()) + baseShape->getInitRot();

    // Calculate the sine and cosine of the rotation
    f32 sinRotation = JMASSin(rotation);
    f32 cosRotation = JMASCos(rotation);

    // Initialize the transformation matrix
    Mtx transformationMatrix;

    // Fill the transformation matrix with calculated values
    transformationMatrix[0][0] = scaleX * cosRotation;
    transformationMatrix[0][1] = -scaleX * sinRotation;
    transformationMatrix[0][2] = 0.0f;
    transformationMatrix[0][3] = (halfTilingS + (scaleX * ((sinRotation * (halfTilingT + transY)) - (cosRotation * (halfTilingS + transX)))));
    transformationMatrix[1][0] = scaleY * sinRotation;
    transformationMatrix[1][1] = scaleY * cosRotation;
    transformationMatrix[1][2] = 0.0f;
    transformationMatrix[1][3] = (halfTilingT + (-scaleY * ((sinRotation * (halfTilingS + transX)) + (cosRotation * (halfTilingT + transY)))));
    transformationMatrix[2][0] = 0.0f;
    transformationMatrix[2][1] = 0.0f;
    transformationMatrix[2][2] = 1.0f;
    transformationMatrix[2][3] = 0.0f;

    // Load the transformation matrix into the texture matrix
    GXLoadTexMtxImm(transformationMatrix, GX_TEXMTX0, GX_MTX2x4);

    // Set the texture coordinate generation parameters
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
}

// instruction swaps
void JPALoadCalcTexCrdMtxAnm(JPAEmitterWorkData *workData, JPABaseParticle *particle)
{
    // Get the base shape from the resource
    JPABaseShape *baseShape = workData->mResource->getBsp();

    // Get the age of the particle
    f32 particleAge = particle->mAge;

    // Calculate half of the tiling for S and T
    f32 halfTilingS = 0.5f * (1.0f + baseShape->getTilingS());
    f32 halfTilingT = 0.5f * (1.0f + baseShape->getTilingT());

    // Calculate the X and Y translations
    f32 translationX = (particleAge * baseShape->getIncTransX()) + baseShape->getInitTransX();
    f32 translationY = (particleAge * baseShape->getIncTransY()) + baseShape->getInitTransY();

    // Calculate the X and Y scales
    f32 scaleX = (particleAge * baseShape->getIncScaleX()) + baseShape->getInitScaleX();
    f32 scaleY = (particleAge * baseShape->getIncScaleY()) + baseShape->getInitScaleY();

    // Calculate the rotation
    s16 rotation = (particleAge * baseShape->getIncRot()) + baseShape->getInitRot();

    // Calculate the sine and cosine of the rotation
    f32 sinRotation = JMASSin(rotation);
    f32 cosRotation = JMASCos(rotation);

    // Initialize the transformation matrix
    Mtx transformationMatrix;

    // Fill the transformation matrix with calculated values
    transformationMatrix[0][0] = scaleX * cosRotation;
    transformationMatrix[0][1] = -scaleX * sinRotation;
    transformationMatrix[0][2] = 0.0f;
    transformationMatrix[0][3] = (halfTilingS + (scaleX * ((sinRotation * (halfTilingT + translationY)) - (cosRotation * (halfTilingS + translationX)))));
    transformationMatrix[1][0] = scaleY * sinRotation;
    transformationMatrix[1][1] = scaleY * cosRotation;
    transformationMatrix[1][2] = 0.0f;
    transformationMatrix[1][3] = (halfTilingT + (-scaleY * ((sinRotation * (halfTilingS + translationX)) + (cosRotation * (halfTilingT + translationY)))));
    transformationMatrix[2][0] = 0.0f;
    transformationMatrix[2][1] = 0.0f;
    transformationMatrix[2][2] = 1.0f;
    transformationMatrix[2][3] = 0.0f;

    // Load the transformation matrix into the texture matrix
    GXLoadTexMtxImm(transformationMatrix, 0x1e, GX_MTX2x4);
}

void JPALoadTex(JPAEmitterWorkData *work)
{
    work->mResourceMgr->load(work->mResource->getTexIdx(work->mResource->getBsp()->getTexIdx()), GX_TEXMAP0);
}

void JPALoadTexAnm(JPAEmitterWorkData *work)
{
    work->mResourceMgr->load(work->mResource->getTexIdx(work->mEmitter->mTexAnmIdx), GX_TEXMAP0);
}

void JPALoadTexAnm(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    work->mResourceMgr->load(work->mResource->getTexIdx(ptcl->mTexAnmIdx), GX_TEXMAP0);
}

void JPACalcTexIdxNormal(JPAEmitterWorkData *work)
{
    JPABaseShape * shape = work->mResource->getBsp();
    work->mEmitter->mTexAnmIdx = shape->getTexIdx(((shape->getTexAnmKeyNum() - 1) < work->mEmitter->mCurrentFrame ? (shape->getTexAnmKeyNum() - 1) : work->mEmitter->mCurrentFrame));
}

void JPACalcTexIdxNormal(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape * shape = work->mResource->getBsp();
    ptcl->mTexAnmIdx = shape->getTexIdx(shape->getTexAnmKeyNum() - 1 < ptcl->mAge ? shape->getTexAnmKeyNum() - 1 : ptcl->mAge);
}

void JPACalcTexIdxRepeat(JPAEmitterWorkData *work)
{
    JPABaseShape * shape = work->mResource->getBsp();
    work->mEmitter->mTexAnmIdx = shape->getTexIdx(work->mEmitter->mCurrentFrame % shape->getTexAnmKeyNum());
}

void JPACalcTexIdxRepeat(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape * shape = work->mResource->getBsp();
    ptcl->mTexAnmIdx = shape->getTexIdx(((int)shape->getTexLoopOfst(ptcl->mAnmRandom) + ptcl->mAge) % shape->getTexAnmKeyNum());
}

void JPACalcTexIdxReverse(JPAEmitterWorkData *workData)
{
    // Get the base shape from the resource
    JPABaseShape *baseShape = workData->mResource->getBsp();

    // Get the current tick from the emitter
    int currentTick = workData->mEmitter->mCurrentFrame;

    // Calculate the number of keys
    int totalKeys = (int)baseShape->getTexAnmKeyNum() - 1;

    // Calculate the quotient and remainder of the current tick divided by the total keys
    int quotient = (currentTick / totalKeys) & 1;
    int remainder = currentTick % totalKeys;

    // Calculate the texture animation index
    workData->mEmitter->mTexAnmIdx = baseShape->getTexIdx(remainder + (quotient) * (totalKeys - remainder * 2));
}

void JPACalcTexIdxReverse(JPAEmitterWorkData *workData, JPABaseParticle *particle)
{
    // Get the base shape from the resource
    JPABaseShape * baseShape = workData->mResource->getBsp();

    // Calculate the current tick based on the particle's age and the texture loop offset
    s32 currentTick = baseShape->getTexLoopOfst(particle->mAnmRandom) + particle->mAge;

    // Calculate the total number of keys
    int totalKeys = (int)baseShape->getTexAnmKeyNum() - 1;

    // Calculate the quotient and remainder of the current tick divided by the total keys
    int quotient = (currentTick / totalKeys) & 1;
    int remainder = currentTick % totalKeys;

    // Calculate the texture animation index
    particle->mTexAnmIdx = baseShape->getTexIdx(remainder + (quotient ) * (totalKeys - remainder * 2));
}

void JPACalcTexIdxMerge(JPAEmitterWorkData *workData) { 
    workData->mEmitter->mTexAnmIdx = workData->mResource->getBsp()->getTexIdx(); 
}

void JPACalcTexIdxMerge(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape * shape = work->mResource->getBsp();
    s32 maxFrm = shape->getTexAnmKeyNum();
    //int tick = (int)(maxFrm * ptcl->mTime) + shape->getTexLoopOfst(ptcl->mAnmRandom);
    //ptcl->mTexAnmIdx = shape->getTexIdx(tick % maxFrm);
    ptcl->mTexAnmIdx = shape->getTexIdx(((int)(maxFrm * ptcl->mTime) + (int)shape->getTexLoopOfst(ptcl->mAnmRandom)) % maxFrm);
}

void JPACalcTexIdxRandom(JPAEmitterWorkData *work) { work->mEmitter->mTexAnmIdx = work->mResource->getBsp()->getTexIdx(); }

void JPACalcTexIdxRandom(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    JPABaseShape *shape = work->mResource->getBsp();
    ptcl->mTexAnmIdx = shape->getTexIdx(((int)shape->getTexLoopOfst(ptcl->mAnmRandom)) % shape->getTexAnmKeyNum());
}

void JPALoadPosMtxCam(JPAEmitterWorkData *work)
{
    GXLoadPosMtxImm(work->mPosCamMtx, GX_PNMTX0);
}

void noLoadPrj(const JPAEmitterWorkData *workData, const Mtx mtx) {}

void loadPrj(const JPAEmitterWorkData *workData, const Mtx p2)
{
    Mtx v1;
    PSMTXConcat(workData->mPrjMtx, p2, v1);
    GXLoadTexMtxImm(v1, 0x1E, GX_MTX3x4);
}

// regswaps
void loadPrjAnm(const JPAEmitterWorkData *workData, const Mtx transformationMatrix)
{
    // Get the base shape from the resource
    JPABaseShape *baseShape = workData->mResource->getBsp();

    // Get the age of the emitter
    f32 emitterAge = workData->mEmitter->getAge();

    // Calculate half of the tiling for S and T
    f32 halfTilingS = 0.5f * (1.0f + baseShape->getTilingS());
    f32 halfTilingT = 0.5f * (1.0f + baseShape->getTilingT());

    // Calculate the X and Y translations
    f32 translationX = (emitterAge * baseShape->getIncTransX()) + baseShape->getInitTransX();
    f32 translationY = (emitterAge * baseShape->getIncTransY()) + baseShape->getInitTransY();

    // Calculate the X and Y scales
    f32 scaleX = (emitterAge * baseShape->getIncScaleX()) + baseShape->getInitScaleX();
    f32 scaleY = (emitterAge * baseShape->getIncScaleY()) + baseShape->getInitScaleY();

    // Calculate the rotation
    s16 rotation = (emitterAge * baseShape->getIncRot()) + baseShape->getInitRot();

    // Calculate the sine and cosine of the rotation
    f32 sinRotation = JMASSin(rotation);
    f32 cosRotation = JMASCos(rotation);

    // Initialize the transformation matrix
    Mtx localTransformationMatrix;

    // Fill the transformation matrix with calculated values
    localTransformationMatrix[0][0] = scaleX * cosRotation;
    localTransformationMatrix[0][1] = -scaleX * sinRotation;
    localTransformationMatrix[0][2] = (halfTilingS + (scaleX * ((sinRotation * (halfTilingT + translationY)) - (cosRotation * (halfTilingS + translationX)))));
    localTransformationMatrix[0][3] = 0.0f;
    localTransformationMatrix[1][0] = scaleY * sinRotation;
    localTransformationMatrix[1][1] = scaleY * cosRotation;
    localTransformationMatrix[1][2] = (halfTilingT + (-scaleY * ((sinRotation * (halfTilingS + translationX)) + (cosRotation * (halfTilingT + translationY)))));
    localTransformationMatrix[1][3] = 0.0f;
    localTransformationMatrix[2][0] = 0.0f;
    localTransformationMatrix[2][1] = 0.0f;
    localTransformationMatrix[2][2] = 1.0f;
    localTransformationMatrix[2][3] = 0.0f;

    // Concatenate the local transformation matrix with the projection matrix
    PSMTXConcat(localTransformationMatrix, workData->mPrjMtx, localTransformationMatrix);

    // Concatenate the local transformation matrix with the passed transformation matrix
    PSMTXConcat(localTransformationMatrix, transformationMatrix, localTransformationMatrix);

    // Load the local transformation matrix into the texture matrix
    GXLoadTexMtxImm(localTransformationMatrix, 0x1e, GX_MTX3x4);
}

void JPADrawBillboard(JPAEmitterWorkData *work, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
        return;
    
    JGeometry::TVec3f position;
    PSMTXMultVec(work->mPosCamMtx, (Vec *)&particle->mPosition, (Vec *)&position);
    Mtx mtx;
    mtx[0][0] = work->mGlobalPtclScl.x * particle->mParticleScaleX;
    mtx[0][3] = position.x;
    mtx[1][1] = work->mGlobalPtclScl.y * particle->mParticleScaleY;
    mtx[1][3] = position.y;
    mtx[2][2] = 1.0f;
    mtx[2][3] = position.z;
    mtx[0][1] = mtx[0][2] = mtx[1][0] = mtx[1][2] = mtx[2][0] = mtx[2][1] = 0.0f;
    GXLoadPosMtxImm(mtx, 0);
    p_prj[work->mProjectionType](work, mtx);
    GXCallDisplayList(jpa_dl, sizeof(jpa_dl));
    
}

void JPADrawRotBillboard(JPAEmitterWorkData *work, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
    return;
    
    JGeometry::TVec3f position;
    PSMTXMultVec(work->mPosCamMtx, (Vec *)&particle->mPosition, (Vec *)&position);
    f32 sinRot = JMASSin(particle->mRotateAngle);
    f32 cosRot = JMASCos(particle->mRotateAngle);
    f32 particleX = work->mGlobalPtclScl.x * particle->mParticleScaleX;
    f32 particleY = work->mGlobalPtclScl.y * particle->mParticleScaleY;

    Mtx mtx;
    mtx[0][0] = cosRot * particleX;
    mtx[0][1] = -sinRot * particleY;
    mtx[0][3] = position.x;
    mtx[1][0] = sinRot * particleX;
    mtx[1][1] = cosRot * particleY;
    mtx[1][3] = position.y;
    mtx[2][2] = 1.0f;
    mtx[2][3] = position.z;
    mtx[0][2] = mtx[1][2] = mtx[2][0] = mtx[2][1] = 0.0f;
    GXLoadPosMtxImm(mtx, 0);
    p_prj[work->mProjectionType](work, mtx);
    GXCallDisplayList(jpa_dl, sizeof(jpa_dl));
    
}

void JPADrawYBillboard(JPAEmitterWorkData *work, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
        return;

        JGeometry::TVec3f position;
        PSMTXMultVec(work->mPosCamMtx, (Vec *)&particle->mPosition, (Vec *)&position);
        Mtx mtx;
        f32 particleX = work->mGlobalPtclScl.x * particle->mParticleScaleX;
        f32 particleY = work->mGlobalPtclScl.y * particle->mParticleScaleY;        
        mtx[0][0] = particleX;
        mtx[0][3] = position.x;
        mtx[1][1] = work->mYBBCamMtx[1][1] * particleY;
        mtx[1][2] = work->mYBBCamMtx[1][2];
        mtx[1][3] = position.y;
        mtx[2][1] = work->mYBBCamMtx[2][1] * particleY;
        mtx[2][2] = work->mYBBCamMtx[2][2];
        mtx[2][3] = position.z;
        mtx[0][1] = mtx[0][2] = mtx[1][0] = mtx[2][0] = 0.0f;
        GXLoadPosMtxImm(mtx, 0);
        p_prj[work->mProjectionType](work, mtx);
        GXCallDisplayList(jpa_dl, sizeof(jpa_dl));
    
}

void JPADrawRotYBillboard(JPAEmitterWorkData *work, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
        return;

    JGeometry::TVec3f position;
    PSMTXMultVec(work->mPosCamMtx, (Vec *)&particle->mPosition, (Vec *)&position);
    f32 sinRot = JMASSin(particle->mRotateAngle);
    f32 cosRot = JMASCos(particle->mRotateAngle);
    Mtx mtx;
    f32 scaleX = work->mGlobalPtclScl.x * particle->mParticleScaleX;
    f32 scaleY = work->mGlobalPtclScl.y * particle->mParticleScaleY;
    f32 transformedWidth = (f32)(sinRot * scaleX);
    f32 transformedHeight = (f32)(cosRot * scaleY);

    f32 boundsY11 = work->mYBBCamMtx[1][1];
    f32 boundsY21 = work->mYBBCamMtx[2][1];

    mtx[0][0] = (f32)(cosRot * scaleX);
    mtx[0][1] = (f32)(-sinRot * scaleY);
    mtx[0][2] = 0.0f;
    mtx[0][3] = position.x;
    mtx[1][0] = transformedWidth * boundsY11;
    mtx[1][1] = transformedHeight * boundsY11;
    mtx[1][2] = -boundsY21;
    mtx[1][3] = position.y;
    mtx[2][0] = transformedWidth * boundsY21;
    mtx[2][1] = transformedHeight * boundsY21;
    mtx[2][2] = boundsY11;
    mtx[2][3] = position.z;
    GXLoadPosMtxImm(mtx, 0);
    p_prj[work->mProjectionType](work, mtx);
    GXCallDisplayList(jpa_dl, sizeof(jpa_dl));
    
}

void dirTypeVel(const JPAEmitterWorkData *workData, const JPABaseParticle *particle, JGeometry::TVec3f *direction)
{
    particle->getVelVec(direction);
}

void dirTypePos(const JPAEmitterWorkData *workData, const JPABaseParticle *particle, JGeometry::TVec3f *direction)
{
    particle->getLocalPosition(direction);
}

void dirTypePosInv(const JPAEmitterWorkData *workData, const JPABaseParticle *particle, JGeometry::TVec3f *direction)
{
    particle->getLocalPosition(direction);
    direction->negate();
}

void dirTypeEmtrDir(const JPAEmitterWorkData *workData, const JPABaseParticle *particle, JGeometry::TVec3f *direction)
{
    direction->set(workData->mGlobalEmtrDir);
}

void dirTypePrevPtcl(const JPAEmitterWorkData *work, const JPABaseParticle *ptcl, JGeometry::TVec3f *direction)
{
    JGeometry::TVec3f vec;
    ptcl->getGlobalPosition(&vec);

    if (work->mpCurNode->getPrev() != work->mpAlivePtcl->getEnd())
    {
        work->mpCurNode->getPrev()->getObject()->getGlobalPosition(direction);
    }
    else
    {
        work->mEmitter->calcEmitterGlobalPosition(direction);
    }
    direction->sub(vec);
}

void rotTypeY(f32 p1, f32 p2, Mtx &mtx)
{
    mtx[0][0] = p2;
    mtx[0][1] = 0.0f;
    mtx[0][2] = -p1;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = p1;
    mtx[2][1] = 0.0f;
    mtx[2][2] = p2;
    mtx[2][3] = 0.0f;
}

void rotTypeX(f32 p1, f32 p2, Mtx &mtx)
{
    mtx[0][0] = 1.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = p2;
    mtx[1][2] = -p1;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = p1;
    mtx[2][2] = p2;
    mtx[2][3] = 0.0f;
}

void rotTypeZ(f32 p1, f32 p2, Mtx &mtx)
{
    mtx[0][0] = p2;
    mtx[0][1] = -p1;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = p1;
    mtx[1][1] = p2;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    mtx[2][3] = 0.0f;
}

void rotTypeXYZ(f32 p1, f32 p2, Mtx &mtx)
{
    f32 diag = 0.33333299f * (1.0f - p2);
    f32 off1 = 0.57735f * p1;
    f32 off2 = diag + off1;
    off1 = diag - off1;
    diag += p2;
    mtx[0][0] = diag;
    mtx[0][1] = off1;
    mtx[0][2] = off2;
    mtx[0][3] = 0.0f;
    mtx[1][0] = off2;
    mtx[1][1] = diag;
    mtx[1][2] = off1;
    mtx[1][3] = 0.0f;
    mtx[2][0] = off1;
    mtx[2][1] = off2;
    mtx[2][2] = diag;
    mtx[2][3] = 0.0f;
}

void basePlaneTypeXY(Mtx mtx, f32 x, f32 y)
{
    mtx[0][0] *= x;
    mtx[1][0] *= x;
    mtx[2][0] *= x;
    mtx[0][1] *= y;
    mtx[1][1] *= y;
    mtx[2][1] *= y;
}

void basePlaneTypeXZ(Mtx mtx, f32 x, f32 z)
{
    mtx[0][0] *= x;
    mtx[1][0] *= x;
    mtx[2][0] *= x;
    mtx[0][2] *= z;
    mtx[1][2] *= z;
    mtx[2][2] *= z;
}

void basePlaneTypeX(Mtx mtx, f32 xz, f32 y)
{
    mtx[0][0] *= xz;
    mtx[1][0] *= xz;
    mtx[2][0] *= xz;
    mtx[0][1] *= y;
    mtx[1][1] *= y;
    mtx[2][1] *= y;
    mtx[0][2] *= xz;
    mtx[1][2] *= xz;
    mtx[2][2] *= xz;
}

void JPADrawDirection(JPAEmitterWorkData *emitterData, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
        return;
    
    JGeometry::TVec3f directionVector;
    JGeometry::TVec3f crossProductVector;
    p_direction[emitterData->mDirType](emitterData, particle, &directionVector);
    if (directionVector.isZero())
        return;
    
    directionVector.normalize();
    crossProductVector.cross(particle->mBaseAxis, directionVector);
    if (crossProductVector.isZero())
        return;
    
    crossProductVector.normalize();
    particle->mBaseAxis.cross(directionVector, crossProductVector);
    particle->mBaseAxis.normalize();

    Mtx transformationMatrix;

    f32 scaleX = emitterData->mGlobalPtclScl.x * particle->mParticleScaleX;
    f32 scaleY = emitterData->mGlobalPtclScl.y * particle->mParticleScaleY;

    transformationMatrix[0][0] = particle->mBaseAxis.x;
    transformationMatrix[0][1] = directionVector.x;
    transformationMatrix[0][2] = crossProductVector.x;
    transformationMatrix[0][3] = particle->mPosition.x;
    transformationMatrix[1][0] = particle->mBaseAxis.y;
    transformationMatrix[1][1] = directionVector.y;
    transformationMatrix[1][2] = crossProductVector.y;
    transformationMatrix[1][3] = particle->mPosition.y;
    transformationMatrix[2][0] = particle->mBaseAxis.z;
    transformationMatrix[2][1] = directionVector.z;
    transformationMatrix[2][2] = crossProductVector.z;
    transformationMatrix[2][3] = particle->mPosition.z;

    p_plane[emitterData->mPlaneType](transformationMatrix, scaleX, scaleY);
    PSMTXConcat(emitterData->mPosCamMtx, transformationMatrix, transformationMatrix);
    GXLoadPosMtxImm(transformationMatrix, 0);
    p_prj[emitterData->mProjectionType](emitterData, transformationMatrix);
    GXCallDisplayList(p_dl[emitterData->mDLType], sizeof(jpa_dl));
}

void JPADrawRotDirection(JPAEmitterWorkData *work, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
        return;
    
    f32 sinRot = JMASSin(particle->mRotateAngle);
    f32 cosRot = JMASCos(particle->mRotateAngle);
    JGeometry::TVec3<f32> direction;
    JGeometry::TVec3<f32> crossProduct;
    p_direction[work->mDirType](work, particle, &direction);
    if (direction.isZero())
        return;
    
    direction.normalize();
    crossProduct.cross(particle->mBaseAxis, direction);
    if (crossProduct.isZero())
        return;
        
    crossProduct.normalize();
    particle->mBaseAxis.cross(direction, crossProduct);
    particle->mBaseAxis.normalize();
    f32 particleX = work->mGlobalPtclScl.x * particle->mParticleScaleX;
    f32 particleY = work->mGlobalPtclScl.y * particle->mParticleScaleY;
    Mtx rotationMtx;
    Mtx transformationMtx;
    p_rot[work->mRotType](sinRot, cosRot, rotationMtx);
    p_plane[work->mPlaneType](rotationMtx, particleX, particleY);
    transformationMtx[0][0] = particle->mBaseAxis.x;
    transformationMtx[0][1] = direction.x;
    transformationMtx[0][2] = crossProduct.x;
    transformationMtx[0][3] = particle->mPosition.x;
    transformationMtx[1][0] = particle->mBaseAxis.y;
    transformationMtx[1][1] = direction.y;
    transformationMtx[1][2] = crossProduct.y;
    transformationMtx[1][3] = particle->mPosition.y;
    transformationMtx[2][0] = particle->mBaseAxis.z;
    transformationMtx[2][1] = direction.z;
    transformationMtx[2][2] = crossProduct.z;
    transformationMtx[2][3] = particle->mPosition.z;
    PSMTXConcat(transformationMtx, rotationMtx, rotationMtx);
    PSMTXConcat(work->mPosCamMtx, rotationMtx, transformationMtx);
    GXLoadPosMtxImm(transformationMtx, 0);
    p_prj[work->mProjectionType](work, transformationMtx);
    GXCallDisplayList(p_dl[work->mDLType], sizeof(jpa_dl));
            

}

void JPADrawDBillboard(JPAEmitterWorkData *work, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
        return;
    
        JGeometry::TVec3<f32> direction;
        p_direction[work->mDirType](work, particle, &direction);
        JGeometry::TVec3<f32> cameraPos(work->mPosCamMtx[2][0], work->mPosCamMtx[2][1], work->mPosCamMtx[2][2]);
        direction.cross(direction, cameraPos);
        if (direction.isZero())
            return;
        
        direction.normalize();
        PSMTXMultVecSR(work->mPosCamMtx, (Vec *)&direction, (Vec *)&direction);
        JGeometry::TVec3<f32> particlePos;
        PSMTXMultVec(work->mPosCamMtx, (Vec *)&particle->mPosition, (Vec *)&particlePos);
        f32 particleX = work->mGlobalPtclScl.x * particle->mParticleScaleX;
        f32 particleY = work->mGlobalPtclScl.y * particle->mParticleScaleY;
        Mtx transformMtx;
        transformMtx[0][0] = direction.x * particleX;
        transformMtx[0][1] = -direction.y * particleY;
        transformMtx[0][3] = particlePos.x;
        transformMtx[1][0] = direction.y * particleX;
        transformMtx[1][1] = direction.x * particleY;
        transformMtx[1][3] = particlePos.y;
        transformMtx[2][2] = 1.0f;
        transformMtx[2][3] = particlePos.z;
        transformMtx[0][2] = transformMtx[2][0] = transformMtx[2][1] = 0.0f;
        GXLoadPosMtxImm(transformMtx, 0);
        p_prj[work->mProjectionType](work, transformMtx);
        GXCallDisplayList(jpa_dl, sizeof(jpa_dl));
}

void JPADrawRotation(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    if (ptcl->checkStatus(8) != 0)
        return;
    
    f32 sinRot = JMASSin(ptcl->mRotateAngle);
    f32 cosRot = JMASCos(ptcl->mRotateAngle);
    f32 particleX = work->mGlobalPtclScl.x * ptcl->mParticleScaleX;
    f32 particleY = work->mGlobalPtclScl.y * ptcl->mParticleScaleY;
    Mtx mtx;
    p_rot[work->mRotType](sinRot, cosRot, mtx);
    p_plane[work->mPlaneType](mtx, particleX, particleY);
    mtx[0][3] = ptcl->mPosition.x;
    mtx[1][3] = ptcl->mPosition.y;
    mtx[2][3] = ptcl->mPosition.z;
    PSMTXConcat(work->mPosCamMtx, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    p_prj[work->mProjectionType](work, mtx);
    GXCallDisplayList(p_dl[work->mDLType], sizeof(jpa_dl));
}

void JPADrawPoint(JPAEmitterWorkData *work, JPABaseParticle *ptcl)
{
    if (ptcl->checkStatus(8) != 0)
        return;
    
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXBegin(GX_POINTS, GX_VTXFMT1, 1);
    GXPosition3f32(ptcl->mPosition.x, ptcl->mPosition.y, ptcl->mPosition.z);
    GXTexCoord2f32(0.0f, 0.0f);
    GXEnd();

    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
}

// incorrect scheduling and regswaps for the GXPosition stuff
void JPADrawLine(JPAEmitterWorkData *work, JPABaseParticle *particle)
{
    if (particle->checkStatus(8) != 0)
        return;

    JGeometry::TVec3f position(particle->mPosition);
    JGeometry::TVec3f direction;
    particle->getVelVec(&direction);
    if (direction.isZero())
        return;

    direction.setLength(work->mGlobalPtclScl.y * (25.0f * particle->mParticleScaleY));
    direction.sub(position, direction);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXBegin(GX_LINES, GX_VTXFMT1, 2);

    GXPosition3f32(position.x, position.y, position.z);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(direction.x, direction.y, direction.z);
    GXTexCoord2f32(0.0f, 1.0f);

    GXEnd();

    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
}

static JPANode<JPABaseParticle> *getNext(JPANode<JPABaseParticle> *node) { return node->getNext(); }

static JPANode<JPABaseParticle> *getPrev(JPANode<JPABaseParticle> *node) { return node->getPrev(); }

void JPADrawStripe(JPAEmitterWorkData *work)
{
    JPABaseShape *shape = work->mResource->getBsp();
    u32 ptcl_num = work->mpAlivePtcl->getNum();
    if (ptcl_num < 2)
    {
        return;
    }

    f32 coord = 0.0f;
    f32 step = 1.0f / (ptcl_num - 1.0f);
    f32 dVar14 = (1.0f + work->mPivot.x) * (25.0f * work->mGlobalPtclScl.x);
    f32 dVar13 = (1.0f - work->mPivot.x) * (25.0f * work->mGlobalPtclScl.x);
    Mtx matrix;
    f32 sin;
    f32 cos;
    JGeometry::TVec3f local_ec;
    JGeometry::TVec3f local_e0[2];
    JGeometry::TVec3f direction;
    JGeometry::TVec3f local_104;
    JPANode<JPABaseParticle> *startNode = nullptr;
    getNodeFunc node_func = nullptr;    
    if (shape->isDrawFwdAhead())
    {
        startNode = work->mpAlivePtcl->getLast();
        node_func = getPrev;
        coord = 1.0f;
        step = -step;
    }
    else
    {
        startNode = work->mpAlivePtcl->getFirst();
        node_func = getNext;
    }

    GXLoadPosMtxImm(work->mPosCamMtx, 0);
    p_prj[work->mProjectionType](work, work->mPosCamMtx);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, ptcl_num << 1);

    for (JPANode<JPABaseParticle> *node = startNode; node != work->mpAlivePtcl->getEnd(); node = node_func(node), coord += step)
    {
        work->mpCurNode = node;
        JPABaseParticle *particle = node->getObject();
        local_ec.set(particle->mPosition);
        sin = JMASSin(particle->mRotateAngle);
        cos = JMASCos(particle->mRotateAngle);
        local_e0[0].set(-particle->mParticleScaleX * dVar14, 0.0f, 0.0f);
        local_e0[0].set(local_e0[0].x * cos, 0.0f, local_e0[0].x * sin);
        local_e0[1].set(particle->mParticleScaleX * dVar13, 0.0f, 0.0f);
        local_e0[1].set(local_e0[1].x * cos, 0.0f, local_e0[1].x * sin);
        p_direction[work->mDirType](work, particle, &direction);
        if (direction.isZero())
        {
            direction.set(0.0f, 1.0f, 0.0f);
        }
        else
        {
            direction.normalize();
        }
        local_104.cross(particle->mBaseAxis, direction);
        if (local_104.isZero())
        {
            local_104.set(1.0f, 0.0f, 0.0f);
        }
        else
        {
            local_104.normalize();
        }
        particle->mBaseAxis.cross(direction, local_104);
        particle->mBaseAxis.normalize();

        matrix[0][0] = local_104.x;
        matrix[0][1] = direction.x;
        matrix[0][2] = particle->mBaseAxis.x;
        matrix[0][3] = 0.0f;

        matrix[1][0] = local_104.y;
        matrix[1][1] = direction.y;
        matrix[1][2] = particle->mBaseAxis.y;
        matrix[1][3] = 0.0f;

        matrix[2][0] = local_104.z;
        matrix[2][1] = direction.z;
        matrix[2][2] = particle->mBaseAxis.z;
        matrix[2][3] = 0.0f;

        PSMTXMultVecArraySR(matrix, (Vec *)local_e0, (Vec *)local_e0, 2); // ???
        GXPosition3f32(local_e0[0].x + local_ec.x, local_e0[0].y + local_ec.y, local_e0[0].z + local_ec.z);


        GXTexCoord2f32(0.0f, coord);
        GXPosition3f32(local_e0[1].x + local_ec.x, local_e0[1].y + local_ec.y, local_e0[1].z + local_ec.z);
        GXTexCoord2f32(1.0f, coord);        
    }
    GXEnd();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
}

void JPADrawStripeX(JPAEmitterWorkData *work)
{
    JPABaseShape *shape = work->mResource->getBsp();
    u32 ptcl_num = work->mpAlivePtcl->getNum();
    if (ptcl_num < 2)
    {
        return;
    }

    f32 start_coord = 0.0f;
    f32 coord = 0.0f;
    f32 step = 1.0f / (ptcl_num - 1.0f);
    f32 local_154 = (1.0f + work->mPivot.x) * (25.0f * work->mGlobalPtclScl.x);
    f32 local_158 = (1.0f - work->mPivot.x) * (25.0f * work->mGlobalPtclScl.x);
    f32 local_15c = (1.0f + work->mPivot.y) * (25.0f * work->mGlobalPtclScl.y);
    f32 local_160 = (1.0f - work->mPivot.y) * (25.0f * work->mGlobalPtclScl.y);
    Mtx matrix;
    f32 sin;
    f32 cos;
    
    JGeometry::TVec3f position;
    JGeometry::TVec3f local_a8[2];
    JGeometry::TVec3f direction;
    JGeometry::TVec3f local_cc;
    JPANode<JPABaseParticle> *startNode = nullptr;
    getNodeFunc node_func = nullptr;
    
    
    if (shape->isDrawFwdAhead())
    {
        startNode = work->mpAlivePtcl->getLast();
        node_func = getPrev;
        start_coord = coord = 1.0f;
        step = -step;
    }
    else
    {
        startNode = work->mpAlivePtcl->getFirst();
        node_func = getNext;
    }

    GXLoadPosMtxImm(work->mPosCamMtx, 0);
    p_prj[work->mProjectionType](work, work->mPosCamMtx);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, ptcl_num << 1);
    for (JPANode<JPABaseParticle> *node = startNode; node != work->mpAlivePtcl->getEnd(); node = node_func(node), coord += step)
    {
        work->mpCurNode = node;
        JPABaseParticle *particle = node->getObject();
        position.set(particle->mPosition);
        sin = JMASSin(particle->mRotateAngle);
        cos = JMASCos(particle->mRotateAngle);
        local_a8[0].set(-particle->mParticleScaleX * local_154, 0.0f, 0.0f);
        local_a8[0].set(local_a8[0].x * cos, 0.0f, local_a8[0].x * sin);
        local_a8[1].set(particle->mParticleScaleX * local_158, 0.0f, 0.0f);
        local_a8[1].set(local_a8[1].x * cos, 0.0f, local_a8[1].x * sin);
        p_direction[work->mDirType](work, particle, &direction);
        if (direction.isZero())
        {
            direction.set(0.0f, 1.0f, 0.0f);
        }
        else
        {
            direction.normalize();
        }
        local_cc.cross(particle->mBaseAxis, direction);
        if (local_cc.isZero())
        {
            local_cc.set(1.0f, 0.0f, 0.0f);
        }
        else
        {
            local_cc.normalize();
        }
        particle->mBaseAxis.cross(direction, local_cc);
        particle->mBaseAxis.normalize();

        matrix[0][0] = local_cc.x;
        matrix[0][1] = direction.x;
        matrix[0][2] = particle->mBaseAxis.x;
        matrix[0][3] = 0.0f;

        matrix[1][0] = local_cc.y;
        matrix[1][1] = direction.y;
        matrix[1][2] = particle->mBaseAxis.y;
        matrix[1][3] = 0.0f;

        matrix[2][0] = local_cc.z;
        matrix[2][1] = direction.z;
        matrix[2][2] = particle->mBaseAxis.z;
        matrix[2][3] = 0.0f;
        PSMTXMultVecArraySR(matrix, (Vec *)local_a8, (Vec *)local_a8, 2); // ???
        GXPosition3f32(local_a8[0].x + position.x, local_a8[0].y + position.y, local_a8[0].z + position.z);
        GXTexCoord2f32( 0.0f, coord);
        GXPosition3f32(local_a8[1].x + position.x, local_a8[1].y + position.y, local_a8[1].z + position.z);
        GXTexCoord2f32( 1.0f, coord);
    }
    GXEnd();

    coord = start_coord;
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, ptcl_num << 1);
    for (JPANode<JPABaseParticle> *node = startNode; node != work->mpAlivePtcl->getEnd(); node = node_func(node), coord += step)
    {
        work->mpCurNode = node;
        JPABaseParticle *particle = node->getObject();
        position.set(particle->mPosition);
        // rename these variables?
        sin = JMASCos(particle->mRotateAngle);
        cos = -JMASSin(particle->mRotateAngle);

        local_a8[0].set(-particle->mParticleScaleY * local_15c, 0.0f, 0.0f);
        local_a8[0].set(local_a8[0].x * cos, 0.0f, local_a8[0].x * sin);
        local_a8[1].set(particle->mParticleScaleY * local_160, 0.0f, 0.0f);
        local_a8[1].set(local_a8[1].x * cos, 0.0f, local_a8[1].x * sin);

        p_direction[work->mDirType](work, particle, &direction);
        if (direction.isZero())
        {
            direction.set(0.0f, 1.0f, 0.0f);
        }
        else
        {
            direction.normalize();
        }
        local_cc.cross(particle->mBaseAxis, direction);
        if (local_cc.isZero())
        {
            local_cc.set(1.0f, 0.0f, 0.0f);
        }
        else
        {
            local_cc.normalize();
        }
        particle->mBaseAxis.cross(direction, local_cc);
        particle->mBaseAxis.normalize();

        matrix[0][0] = local_cc.x;
        matrix[0][1] = direction.x;
        matrix[0][2] = particle->mBaseAxis.x;
        matrix[0][3] = 0.0f;

        matrix[1][0] = local_cc.y;
        matrix[1][1] = direction.y;
        matrix[1][2] = particle->mBaseAxis.y;
        matrix[1][3] = 0.0f;

        matrix[2][0] = local_cc.z;
        matrix[2][1] = direction.z;
        matrix[2][2] = particle->mBaseAxis.z;
        matrix[2][3] = 0.0f;
        PSMTXMultVecArraySR(matrix, (Vec *)local_a8, (Vec *)local_a8, 2); // ???
        GXPosition3f32(local_a8[0].x + position.x, local_a8[0].y + position.y, local_a8[0].z + position.z);

        //const f32 zero = 0.0f;
        //const f32 one = 1.0f;
        GXTexCoord2f32(0.0f, coord);
        GXPosition3f32(local_a8[1].x + position.x, local_a8[1].y + position.y, local_a8[1].z + position.z);
        GXTexCoord2f32(1.0f, coord);
    }
    GXEnd();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
}

void JPADrawEmitterCallBackB(JPAEmitterWorkData *workData)
{
    JPABaseEmitter *emt = workData->mEmitter;
    if (emt->mEmitterCallback == nullptr)
        return;
    emt->mEmitterCallback->draw(emt);
}

void JPADrawParticleCallBack(JPAEmitterWorkData *workData, JPABaseParticle *particle)
{
    JPABaseEmitter *emt = workData->mEmitter;
    if (emt->mParticleCallback == nullptr)
        return;
    
    emt->mParticleCallback->draw(emt, particle);
}

// incorrect scheduling?
void makeColorTable(GXColor **colorTable, const JPAClrAnmKeyData *data, u8 a2, s16 size, JKRHeap *heap)
{
    GXColor *p_clr_tbl = (GXColor *)JKRAllocFromHeap(heap, (size + 1) * sizeof(GXColor), 4);
#line 1523
    JUT_ASSERT(p_clr_tbl);

    f32 r_step, g_step, b_step, a_step;
    r_step = g_step = b_step = a_step = 0.0f;

    f32 diff_r, diff_g, diff_b, diff_a;
    
    f32 r, g, b, a;
    diff_r = r = data[0].color.r;
    diff_g = g = data[0].color.g;
    diff_b = b = data[0].color.b;
    diff_a = a = data[0].color.a;
    f32 base_step = 0.0f;
    int j = 0;

    for (s16 i = 0; i < size + 1; i++)
    {
        if (i == data[j].index)
        {
            p_clr_tbl[i] = data[j].color;
            r = data[j].color.r;
            g = data[j].color.g;
            b = data[j].color.b;
            a = data[j].color.a;
            j++;
            if (j < a2)
            {
                diff_r = (data[j].color.r);
                diff_g = (data[j].color.g);
                diff_b = (data[j].color.b);
                diff_a = (data[j].color.a);
                base_step = (1.0f / (data[j].index - (data + j - 1)->index));

                r_step = base_step * (diff_r - r);
                g_step = base_step * (diff_g - g);
                b_step = base_step * (diff_b - b);
                a_step = base_step * (diff_a - a);
            }
            else
            {
                r_step = g_step = b_step = a_step = 0.0f;
            }
        }
        else
        {
            r += r_step;
            p_clr_tbl[i].r = r;
            g += g_step;
            p_clr_tbl[i].g = g;
            b += b_step;
            p_clr_tbl[i].b = b;
            a += a_step;
            p_clr_tbl[i].a = a;
        }
    }
    *colorTable = p_clr_tbl;
}

GXBlendMode JPABaseShape::st_bm[3] = {GX_BM_NONE, GX_BM_BLEND, GX_BM_LOGIC};

GXBlendFactor JPABaseShape::st_bf[10] = {GX_BL_ZERO, GX_BL_ONE, GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_BL_DSTCLR,
                                         GX_BL_INVDSTCLR, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA};

GXLogicOp JPABaseShape::st_lo[16] = {GX_LO_CLEAR, GX_LO_SET, GX_LO_COPY, GX_LO_INVCOPY, GX_LO_NOOP, GX_LO_INV, GX_LO_AND, GX_LO_NAND,
                                     GX_LO_OR, GX_LO_NOR, GX_LO_XOR, GX_LO_EQUIV, GX_LO_REVAND, GX_LO_INVAND, GX_LO_REVOR, GX_LO_INVOR};

GXCompare JPABaseShape::st_c[8] = {GX_NEVER, GX_LESS, GX_LEQUAL, GX_EQUAL, GX_NEQUAL, GX_GEQUAL, GX_GREATER, GX_ALWAYS};

GXAlphaOp JPABaseShape::st_ao[4] = {GX_AOP_AND, GX_AOP_OR, GX_AOP_XOR, GX_AOP_XNOR};

GXTevColorArg JPABaseShape::st_ca[6][4] = {
    {GX_CC_ZERO, GX_CC_TEXC, GX_CC_ONE, GX_CC_ZERO}, //
    {GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO},  //
    {GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO},   //
    {GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO},    //
    {GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1},    //
    {GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0},  //
};

GXTevAlphaArg JPABaseShape::st_aa[2][4] = {
    {GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO}, //
    {GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0}, //
};

JPABaseShape::JPABaseShape(const u8 *data, JKRHeap *heap)
{
    pBsd = (const JPABaseShapeData *)data;

    if (isTexCrdAnm())
    {
        mTexCrdMtxAnmTbl = (const void *)(data + sizeof(JPABaseShapeData));
    }
    else
    {
        mTexCrdMtxAnmTbl = nullptr;
    }

    if (isTexAnm())
    {
#line 1673
        JUT_ASSERT(pBsd->texAnmKeyNum != 0);
        u32 offs = sizeof(JPABaseShapeData);
        if (isTexCrdAnm())
            offs += 0x28;
        mTexIdxAnimTbl = (const u8 *)(data + offs);
    } else {
        mTexIdxAnimTbl = nullptr;
    }
    if (isPrmAnm()) {
        JUT_ASSERT(pBsd->prmAnmKeyNum != 0);
        JPAClrAnmKeyData * keyData =(JPAClrAnmKeyData *)(data + pBsd->mClrPrmAnmOffset);
        makeColorTable(&mPrmClrAnmTbl, keyData, pBsd->prmAnmKeyNum, pBsd->mClrAnmFrmMax,
                       heap);
    } else {
        mPrmClrAnmTbl = nullptr;
    }
    if (isEnvAnm())
    {
        JUT_ASSERT(pBsd->envAnmKeyNum != 0);
        JPAClrAnmKeyData *keyData = (JPAClrAnmKeyData *)(data + pBsd->mClrEnvAnmOffset);
        makeColorTable(&mEnvClrAnmTbl, keyData, pBsd->envAnmKeyNum, pBsd->mClrAnmFrmMax,
                       heap);
    }
    else
    {
        mEnvClrAnmTbl = nullptr;
    }
}

void JPABaseShape::init_jpa(const u8 *, JKRHeap *)
{
    // UNUSED FUNCTION
}

// instruction + regswap
void JPABaseShape::setGX(JPAEmitterWorkData *work) const
{
    JPAExtraShape *esp = work->mResource->getEsp();
    GXTevColorArg *colorArg = st_ca[(pBsd->mFlags >> 0x0F) & 0x07];
    GXTevAlphaArg *alphaArg = st_aa[(pBsd->mFlags >> 0x12) & 0x01];
    GXSetBlendMode(st_bm[pBsd->mBlendModeCfg & 0x03], st_bf[(pBsd->mBlendModeCfg >> 2) & 0x0F], st_bf[(pBsd->mBlendModeCfg >> 6) & 0x0F], st_lo[(pBsd->mBlendModeCfg >> 10) & 0x0F]);
    GXSetZMode((pBsd->mZModeCfg & 0x01), st_c[(pBsd->mZModeCfg >> 1) & 0x07], ((pBsd->mZModeCfg >> 4) & 0x01));
    GXSetAlphaCompare(st_c[pBsd->mAlphaCompareCfg & 0x07], pBsd->mAlphaRef0, st_ao[(pBsd->mAlphaCompareCfg >> 3) & 0x03], st_c[(pBsd->mAlphaCompareCfg >> 5) & 0x07], pBsd->mAlphaRef1);
    
    GXSetTevColorIn(GX_TEVSTAGE0, colorArg[0], colorArg[1], colorArg[2], colorArg[3]);
    
    GXSetTevAlphaIn(GX_TEVSTAGE0, alphaArg[0], alphaArg[1], alphaArg[2], alphaArg[3]);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetZCompLoc(((pBsd->mZModeCfg >> 5) & 0x01));
}