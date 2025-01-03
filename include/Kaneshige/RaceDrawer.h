#ifndef RACEDRAWER_H
#define RACEDRAWER_H

#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/Blurscreen.h"
#include "Kaneshige/DrawBuffer.h"
#include "Kaneshige/KartDrawer.h"
#include "Kaneshige/MaskScreen.h"
#include "types.h"

class RaceDrawer
{ // Autogenerated
public:
    // Global
    RaceDrawer(u16, bool);                     // 0x801c8f24
    void reset();                              // 0x801c9534
    void initJ3D();                            // 0x801c9660
    void drawSceneFirstStage(u32);             // 0x801c9698
    void drawMidScene();                       // 0x801c9dfc
    void drawSceneSecondStage(u32);            // 0x801c9f0c
    void drawPreScene();                       // 0x801ca29c
    void drawPostScene();                      // 0x801ca32c
    void drawGhost();                          // 0x801ca4b8
    void showAllGhost();                       // 0x801ca680
    static u8 sBlurA0;                         // 0x80414700
    static u8 sBlurDefaultDecrease;            // 0x80414701
    static float sBlurAddY;                    // 0x80414704
    static float sBlurAddX;                    // 0x80416488
    static JUtility::TColor sBlurDefaultColor; // 0x8041648c
    static int sForceGhostKind;                // 0x80416490
    // Inline
    void isGhostActive() const;    // 0x801c9650
    void isDrawJugem() const;      // 0x801c9cdc
    void isDrawGeoObj() const;     // 0x801c9ce8
    void isDrawItem() const;       // 0x801c9cf4
    void isDrawDriver() const;     // 0x801c9d24
    void isDrawKart() const;       // 0x801c9d54
    void isDrawCrs() const;        // 0x801c9dd0
    void isDrawSky() const;        // 0x801c9ddc
    void isDrawGhostScene() const; // 0x801c9ef8
    void isDrawCup() const;        // 0x801ca268
    void isBlurActive() const;     // 0x801ca31c
    MaskScreen *getMaskScreen();   // 0x801ca490
    void isMaskActive() const;     // 0x801ca4a0

    void setBlurDecrease(u8 a) { mBlurDecrease = a; }
    void setBlurColor(const JUTColor &color) { mBlurColor.set(color);  }

    DrawBuffer *getItemDrawBuffer(int kartNo) {
#line 163
        JUT_MINMAX_ASSERT(-1, kartNo, 8)
        return kartNo >= 0 ? mItmDrawBufs[kartNo] : mItmDrawBuf; // the day Kaneshige discovered ternaries
    }

    KartDrawer * getKartDrawer(int kartNo) {
#line 172
        JUT_MINMAX_ASSERT(0, kartNo, mKartNum);
        return &mKartDrawer[kartNo];
    }


    BlurScreen *getBlurScreen() { 
        return mBlurScreen;
    }
    // TODO: own class types
    DrawBuffer *getEffectDrawBuffer() {
        return mEfctDrawBuf;
    }
    DrawBuffer *getFeelDrawBuffer() {
        return mFeelDrawBuf;
    }
    DrawBuffer *getJugemDrawBuffer() {
        return mJugemDrawBuf;
    }
    DrawBuffer *getGeoDrawBuffer() {
        return mGeoDrawBuf;
    }
    DrawBuffer *getShadowGeoDrawBuffer() {
        return mShdwDrawBuf;
    }
private:
    u8 _0[6];
    DrawBuffer *mShdwDrawBuf;
    DrawBuffer *mGeoDrawBuf;
    DrawBuffer *mItmDrawBufs[8];
    DrawBuffer *mItmDrawBuf;
    DrawBuffer *mEfctDrawBuf;
    DrawBuffer *mJugemDrawBuf;
    DrawBuffer *mFeelDrawBuf;
    int mKartNum;
    KartDrawer *mKartDrawer; // double array?
    BlurScreen *mBlurScreen;
    u8 _4c[0x5c - 0x4c];
    u8 mBlurDecrease;
    u8 _5d[0x60 - 0x5d];
    JUTColor mBlurColor;
    // Size: 0x64
}; // class RaceDrawer
// void KartDrawer::lodOff() // KartDrawer.h; // 0x801c92f8
// EGhostKind KartInfo::getGhostKind() // KartInfo.h; // 0x801c9304
// virtual KartDrawer::~KartDrawer() // KartDrawer.h; // 0x801c930c
// void FeelDrawBuffer::FeelDrawBuffer(u32) // FeelDrawBuffer.h; // 0x801c93bc
// void JugemDrawBuffer::JugemDrawBuffer(u32) // JugemDrawBuffer.h; // 0x801c93f8
// void EffectDrawBuffer::EffectDrawBuffer(u32) // EffectDrawBuffer.h; // 0x801c9434
// void ItemDrawBuffer::setTargetKart(short) // ItemDrawBuffer.h; // 0x801c9470
// void ItemDrawBuffer::ItemDrawBuffer(u32) // ItemDrawBuffer.h; // 0x801c9478
// void GeoDrawBuffer::GeoDrawBuffer(u32) // GeoDrawBuffer.h; // 0x801c94bc
// void ShadowGeoDrawBuffer::ShadowGeoDrawBuffer(u32) // ShadowGeoDrawBuffer.h; // 0x801c94f8
// void GhostScreen::hide() // GhostScreen.h; // 0x801c9644
// void Course::drawHigh(u32) // Course.h; // 0x801c9cb0
// void KartDrawer::drawDriverFirstStage(u32) // KartDrawer.h; // 0x801c9d00
// void KartDrawer::drawKartFirstStage(u32) // KartDrawer.h; // 0x801c9d30
// void ShadowManager::drawShadowVolume(u32, ShadowModel::ShadowKind) // ShadowManager.h; // 0x801c9d60
// void Course::drawNormal(u32) // Course.h; // 0x801c9da4
// void LightMgr::setCurrentViewNo(u32) // LightMgr.h; // 0x801c9de8
// void RaceSceneLight::getSceneLightTagName(u32) // RaceSceneLight.h; // 0x801c9df0
// void GhostScreen::isHide() const // GhostScreen.h; // 0x801c9f04
// void KartDrawer::drawDriverSecondStage(u32) // KartDrawer.h; // 0x801ca220
// void KartDrawer::drawKartSecondStage(u32) // KartDrawer.h; // 0x801ca244
// void RaceCupLight::getTagName(u32) // RaceCupLight.h; // 0x801ca274
// void TLensFlare::getSupervisor() // TLensFlare.h; // 0x801ca280
// void KartDrawer::isSecondStage() const // KartDrawer.h; // 0x801ca288
// void RaceMgr::getAward2D() // RaceMgr.h; // 0x801ca488
// void RaceMgr::getStaffRoll2D() // RaceMgr.h; // 0x801ca498
// void GeoShimmer::getSupervisor() // GeoShimmer.h; // 0x801ca4b0
// void KartDrawer::drawDriverGhostStage(u32) // KartDrawer.h; // 0x801ca630
// void KartDrawer::drawKartGhostStage(u32) // KartDrawer.h; // 0x801ca654
// void KartDrawer::getGhostAlpha() const // KartDrawer.h; // 0x801ca678
// void GhostScreen::show() // GhostScreen.h; // 0x801ca6bc
#endif // RACEDRAWER_H
