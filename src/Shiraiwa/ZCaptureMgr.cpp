#include "Shiraiwa/ZCaptureMgr.h"
#include "JSystem/JSupport.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/gx/GXFrameBuffer.h"
#include "dolphin/gx/GXManage.h"
#include "JSystem/JUtility.h"

ZCaptureMgr::ZCaptureMgr(u32 numViews) {
    mNumViews = numViews;
    mScreens = new ZCaptureScreen*[mNumViews];

    for (u32 i = 0 ; i < mNumViews; i++) {
        mScreens[i] = new ZCaptureScreen();
    }
}

ZCaptureMgr::~ZCaptureMgr() {
    for (u32 i = 0 ; i < mNumViews; i++) {
        delete mScreens[i];
    }
    delete[] mScreens;
}

void ZCaptureMgr::reset() {
    for (u32 i = 0 ; i < mNumViews; i++) {
        mScreens[i]->reset();
    }
}

bool ZCaptureMgr::captureAll() {
    // UNUSED
}

void ZCaptureMgr::capture(u32 viewNo) {
#line 66
    JUT_MINMAX_ASSERT(0, viewNo, mNumViews);
    if (mScreens[viewNo]->captureAll()) {
        captureDone();
    }
}

void ZCaptureMgr::captureDone() {
    GXPixModeSync();
}

void ZCaptureMgr::setActive(u32) {
    // UNUSED
}

void ZCaptureMgr::setInactive(u32) {
    // UNUSED
}

bool ZCaptureMgr::isActive(u32) {
    // UNUSED
}

void ZCaptureMgr::setPosition(u32 id, u32 viewNo, s16 x, s16 y) {
#line 121
    JUT_MINMAX_ASSERT(0, viewNo, mNumViews);
    mScreens[viewNo]->setPosition(id, x, y);
}

u32 ZCaptureMgr::getZValue(u32 id, u32 viewNo) {
#line 132
    JUT_MINMAX_ASSERT(0, viewNo, mNumViews);
    return mScreens[viewNo]->getZValue(id);
}

ZCaptureScreen::ZCaptureScreen() {
    mNodes = new ZCaptureNode*[11];
    for (int i = 0; i < 11; i++) {
        mNodes[i] = new ZCaptureNode();
    }
    reset();
}

ZCaptureScreen::~ZCaptureScreen() {
    for (int i = 0; i < 11; i++) {
        delete mNodes[i];
    }
}

void ZCaptureScreen::reset() {
    for (int i = 0; i < 11; i++) {
        mNodes[i]->reset();
    }
}

bool ZCaptureScreen::captureAll() {
    bool ret = false;

    for (u32 i = 0; i < 11; i++) {
        ret |= capture(i);
    }
    return ret;
}

bool ZCaptureScreen::capture(u32 id) {
#line 192
    JUT_MINMAX_ASSERT(0, id, 11);
    return mNodes[id]->capture();
}

void ZCaptureScreen::setActive(u32) {
    // UNUSED
}

void ZCaptureScreen::setInactive(u32) {
    // UNUSED
}

bool ZCaptureScreen::isActive(u32) {
    // UNUSED
}

void ZCaptureScreen::setPosition(u32 id, s16 x, s16 y) {
#line 235
    JUT_MINMAX_ASSERT(0, id, 11);
    return mNodes[id]->setPosition(x, y);
}

u32 ZCaptureScreen::getZValue(u32 id) const {
#line 245
    JUT_MINMAX_ASSERT(0, id, 11);
    return mNodes[id]->getZValue();
}

const s16 ZCaptureScreen::ZCaptureNode::scCaptureSizeX = 2;
const s16 ZCaptureScreen::ZCaptureNode::scCaptureSizeY = 2; 
const int ZCaptureScreen::ZCaptureNode::scBufferSize = 64;

ZCaptureScreen::ZCaptureNode::ZCaptureNode() {
    mTex = new (32) u8[scBufferSize];
}

ZCaptureScreen::ZCaptureNode::~ZCaptureNode() {
    delete mTex;
}

void ZCaptureScreen::ZCaptureNode::reset() {
    mActive = true;
    setPosition(0, 0);
    for (int i = 0; i < scBufferSize; i++) {
        mTex[i] = 0xff;
    }
}

void ZCaptureScreen::ZCaptureNode::setPosition(s16 x, s16 y) {
    mPosX = x;
    mPosY = y;
    if (!mActive) {
        mActive = true;
    }
}

bool ZCaptureScreen::ZCaptureNode::capture() {
    bool ret = false;
    if (mActive) {
        GXSetTexCopySrc(mPosX, mPosY, scCaptureSizeX, scCaptureSizeY);
        GXSetTexCopyDst(scCaptureSizeX,scCaptureSizeY, GX_TF_Z24X8, GX_FALSE);
        GXCopyTex(mTex, GX_FALSE);
        ret = true;
    }
    return ret;
}

u32 ZCaptureScreen::ZCaptureNode::getZValue() {
    u16 x = mTex[32];
    u8 y = mTex[33];
    u8 z = mTex[1];
    x = (((x << 8) & 0xFF00) | (y & 0xFF));
    return ((z & 0xFF) << 16) | (x & 0xFFFF);
}
