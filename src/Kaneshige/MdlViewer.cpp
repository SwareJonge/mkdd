#include "Kaneshige/MdlViewer.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JSupport/JSUList.h"
#include "Kaneshige/DrawBuffer.h"
#include "Osako/system.h"
#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXLighting.h"
#include "dolphin/mtx.h"

MVGrafPort::MVGrafPort(f32 x, f32 y, f32 w, f32 h, f32 fovY, f32 near, f32 far) : J2DPerspGraph(x, y, w, h, fovY, near, far) {}

void MVGrafPort::setPort() {
    J2DGrafContext::setPort();
    // check in release wheter or not this requires a temp
    f32 aspect = mBounds.getWidth() / mBounds.getHeight();
    C_MTXPerspective(mMtx44, mFovY, aspect,  mNear, mFar);
    GXSetProjection(mMtx44, GX_PERSPECTIVE);
}

MVCamera::MVCamera(MdlViewer *viewer) {
    mTargetPos.set(0.0f, 0.0f, 0.0f);

    _18 = 0;
    _1a = 0;
    _1c = 500.0f;

    mCamPos.x = _1c * JMASCos(_18) * JMASSin(_1a) + mTargetPos.x;
    mCamPos.y = _1c * JMASSin(_18) + mTargetPos.y;
    mCamPos.z = _1c * JMASCos(_18) * JMASCos(_1a) + mTargetPos.z;

    mFrDir.sub(mTargetPos, mCamPos);
    mFrDir.normalize();

    mUpDir.set(0.0f, 1.0f, 0.0f);
    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();

    mFovY = 60.0f;
    mNear = 50.0f;
    mFar = 5000.0f;

    C_MTXLookAt(mViewMtx, &mCamPos, &mUpDir, &mTargetPos);
}

void MVCamera::reset() {}

void MVCamera::calc() {
    mCamPos.x = _1c * JMASCos(_18) * JMASSin(_1a) + mTargetPos.x;
    mCamPos.y = _1c * JMASSin(_18) + mTargetPos.y;
    mCamPos.z = _1c * JMASCos(_18) * JMASCos(_1a) + mTargetPos.z;

    mFrDir.sub(mTargetPos, mCamPos);
    mFrDir.normalize();

    mUpDir.set(0.0f, 1.0f, 0.0f);
    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
    C_MTXLookAt(mViewMtx, &mCamPos, &mUpDir, &mTargetPos);
}

void MVCamera::setPort() {
    MVGrafPort grafPort(System::get3DVpX(), System::get3DVpY(),System::get3DVpW(), System::get3DVpH(), mFovY, mNear, mFar);
    grafPort.scissor(System::get3DScisX(), System::get3DScisY(),System::get3DScisW(), System::get3DScisH());
    grafPort.setPort();
}

MVAmbient::MVAmbient(MdlViewer *) : mColor (0x64, 0x64, 0x64, 0) {}

void MVAmbient::reset() {}

void MVAmbient::calc() {}

void MVAmbient::draw() {
    GXSetChanAmbColor(GX_COLOR0A0, mColor);
    GXSetChanAmbColor(GX_COLOR1A1, mColor);
}

MVLight::MVLight(MdlViewer *viewer) : mPos(0.0f, 0.0f, 0.0f), mColor(0xff, 0xff, 0xff, 0xff)  {
    mViewer = viewer;
    setGXObj();
}

void MVLight::reset() {}

void MVLight::calc() { setGXObj(); }

void MVLight::setGXObj() {
    JGeometry::TVec3f lightPos;
    PSMTXMultVec(mViewer->getCamera()->getViewMtx(), &mPos, &lightPos);
    GXInitLightPos(&mLightObj, lightPos.x, lightPos.y, lightPos.z);
    GXInitLightAttn(&mLightObj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    GXInitLightColor(&mLightObj, mColor);
}


void MVLight::draw() {
    GXLoadLightObjImm(&mLightObj, GX_LIGHT0);
}

MVChaseLight::MVChaseLight(MdlViewer *viewer) : MVLight(viewer), _58(-20000.0f, 50000.0f, 30000.0f) {
    PSMTXIdentity(mEfctMtx);
}

void MVChaseLight::reset() {}

void MVChaseLight::calc() {
    MVCamera *camera = getViewer()->getCamera();
    JGeometry::TVec3f targetPos;
    camera->getTargetPosition(&targetPos);

    JGeometry::TVec3f cameraPos;
    camera->getCameraPosition(&cameraPos);

    JGeometry::TVec3f camFrDir;
    camera->getFrontDirection(&camFrDir);

    JGeometry::TVec3f frDir(camFrDir);
    JGeometry::TVec3f upDir(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f lfDir;

    lfDir.cross(upDir, frDir);
    lfDir.normalize();

    frDir.cross(lfDir, upDir);
    frDir.normalize();

    mPos.x = ((cameraPos.x - _58.x * lfDir.x) + _58.y * upDir.x) - (_58.z * frDir.x);
    mPos.y = ((cameraPos.y - _58.x * lfDir.y) + _58.y * upDir.y) - (_58.z * frDir.y);
    mPos.z = ((cameraPos.z - _58.x * lfDir.z) + _58.y * upDir.z) - (_58.z * frDir.z);
    
    JGeometry::TVec3f posDiff;
    posDiff.sub(targetPos, mPos);
    posDiff.normalize();
    
    JGeometry::TVec3f target;
    target.add(posDiff, camFrDir);
    target.normalize();

    JGeometry::TVec3f camLookPos(0.0f, 0.0f, 0.0f);
    C_MTXLookAt(mEfctMtx, &camLookPos, &upDir, &target);
    setGXObj();
}

MVDrawBuffer::MVDrawBuffer(MdlViewer *viewer, u32 size) : DrawBuffer(size) {
    mViewer = viewer;
}

void MVDrawBuffer::drawIn(u32 viewNo) {
    mViewer->setCurViewNoActor(viewNo);
}

void MVDrawBuffer::update() {
    DrawBuffer::update();
    mViewer->updateActor();
}

void MVDrawBuffer::viewCalc(u32 viewNo) {
    mViewer->viewCalcActor(viewNo);
}

MVActor::MVActor() : mLink(this) {}

MdlViewer::MdlViewer() : _0(0), mActorList() {
    _0 = 0;
    mCamera = new MVCamera(this);
    mAmbient = new MVAmbient(this);
    mChaseLight = new MVChaseLight(this);
    mDrawBuffer = new MVDrawBuffer(this, 0x20);
    reset();
}

void MdlViewer::reset() {
    mCamera->reset();
    mAmbient->reset();
    mChaseLight->reset();
    mDrawBuffer->reset();
}

void MdlViewer::calc() {
    mCamera->calc();
    mAmbient->calc();
    mChaseLight->calc();
    calcActor();
    mDrawBuffer->update();
    mDrawBuffer->viewCalc(0);
}

void MdlViewer::draw() {
    mCamera->setPort();
    j3dSys.setViewMtx(mCamera->getViewMtx());
    mAmbient->draw();    
    mChaseLight->draw();
    mDrawBuffer->draw(0);
}

void MdlViewer::append(MVActor *actor) {
    mActorList.append(&actor->mLink);
}

void MdlViewer::setCurViewNoActor(u32 viewNo) {
    for (JSUListIterator<MVActor> it(mActorList.getFirst()); it != mActorList.getEnd(); it++) {
        it->setCurViewNo(viewNo);
    }
}

void MdlViewer::calcActor() {
    for (JSUListIterator<MVActor> it(mActorList.getFirst()); it != mActorList.getEnd(); it++) {
        it->calc();
    }
}

void MdlViewer::updateActor() {
    for (JSUListIterator<MVActor> it(mActorList.getFirst()); it != mActorList.getEnd(); it++) {
        it->update();
    }
}

void MdlViewer::viewCalcActor(u32 viewNo) {
    for (JSUListIterator<MVActor> it(mActorList.getFirst()); it != mActorList.getEnd(); it++) {
        it->viewCalc(viewNo);
    }
}
