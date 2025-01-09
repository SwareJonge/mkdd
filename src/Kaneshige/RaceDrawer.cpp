#include "Kaneshige/RaceDrawer.h"

#include "Kameda/J2DManager.h"
#include "Kameda/PauseManager.h"
#include "Kaneshige/Blurscreen.h"
#include "Kaneshige/GhostScreen.h"
#include "Kaneshige/KartDrawer.h"
#include "Kaneshige/LightMgr.h"
#include "Kaneshige/MaskScreen.h"
#include "Kaneshige/RaceLight.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Objects/GeoShimmer.h"
#include "Kaneshige/Objects/GeoVision.h"
#include "Kawano/accessory.h"
#include "Osako/shadowMgr.h"
#include "Osako/shadowModel.h"
#include "Osako/shadowScr.h"
#include "Osako/system.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/EffectScreen.h"
#include "Sato/stEffectMgr.h"
#include "Shiraiwa/Balloon.h"
#include "Shiraiwa/LensFlare.h"
#include "Shiraiwa/MapObjAward.h"
#include "Shiraiwa/ZCaptureMgr.h"
#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXPixel.h"
#include "kartEnums.h"
#include "types.h"

u8 RaceDrawer::sBlurA0 = 0x80;
u8 RaceDrawer::sBlurDefaultDecrease = 0xfa; // 250
f32 RaceDrawer::sBlurAddY = -0.5f;
f32 RaceDrawer::sBlurAddX = 0.0f;
JUTColor RaceDrawer::sBlurDefaultColor(0xff,0xfa,0xff,0xff);
EGhostKind RaceDrawer::sForceGhostKind = KIND_0;

#include "JSystem/JAudio/JASFakeMatch2.h"

RaceDrawer::RaceDrawer(u16 numLevels, bool staffRoll) {
    RaceMgr *rcm = RaceMgr::getManager(); // Kaneshige saving the RaceMgr instance? Am i dreaming??
    
    mOrtho = new J2DOrthoGraph(System::getOrthoL(), System::getOrthoT(), System::getOrthoR(), System::getOrthoB(), -1000.0f, 1000.0f);

    _4 = -1; 
    mDrawFlags = 0xffff;
    mShdwDrawBuf = new ShadowGeoDrawBuffer();
    mGeoDrawBuf = new GeoDrawBuffer();

    for (int i = 0; i < 8; i++) {
        if (i < rcm->getKartNumber()) {
            mItmDrawBufs[i] = new ItemDrawBuffer();
            mItmDrawBufs[i]->setTargetKart(i);
        }
        else {
            mItmDrawBufs[i] = nullptr;
        }
    }

    mItmDrawBuf = new ItemDrawBuffer();
    mEfctDrawBuf = new EffectDrawBuffer();
    mJugemDrawBuf = new JugemDrawBuffer();
    mFeelDrawBuf = new FeelDrawBuffer();

    new DarkAnmMgr();
    mKartNum = rcm->getKartNumber();
    mKartDrawer = new KartDrawer[mKartNum];
    for (int i = 0; i < mKartNum; i++) {
        EGhostKind ghostKind = rcm->getKartInfo(i)->getGhostKind();
        if (sForceGhostKind != KIND_0) {
            ghostKind = sForceGhostKind; 
        }
        
        mKartDrawer[i].create(i, rcm->getCameraNumber(), numLevels, ghostKind);

        if (rcm->isCrsDemoMode()) {
            mKartDrawer[i].lodOff();
        }

    }

    if (staffRoll) {
        mBlurScreen = new BlurScreen();
    }
    else {
        mBlurScreen = nullptr;
    }
    
    if (rcm->getRaceMode() == TIME_ATTACK) {
        mGhostScreen = new GhostScreen();
    }
    else {
        mGhostScreen = nullptr;
    }
    
    mShadowScreen = new ShadowScreen();
    mCaptureMgr = new ZCaptureMgr(RaceMgr::getManager()->getConsoleNumber()); // hey, you forgot to use the copy!

    if (staffRoll) {
        mMaskScreen = new MaskScreen();
    }
    else {
        mMaskScreen = nullptr;
    }

    mBlurDecrease = sBlurDefaultDecrease;
    mBlurColor.set(sBlurDefaultColor);
}

void RaceDrawer::reset() {
    RaceMgr *rcm = RaceMgr::getManager(); // again??

    mDrawFlags = 0xffff;

    mShdwDrawBuf->reset();
    mGeoDrawBuf->reset();

    for (int i = 0; i < rcm->getKartNumber(); i++) {
        mItmDrawBufs[i]->reset();
    }

    mItmDrawBuf->reset();
    mEfctDrawBuf->reset();
    mJugemDrawBuf->reset();
    mFeelDrawBuf->reset();

    for (int i = 0; i < rcm->getKartNumber(); i++) {
        mKartDrawer[i].reset();
    }

    if (isGhostActive()) {
        mGhostScreen->reset();
        mGhostScreen->hide();
    }
    mCaptureMgr->reset();

    mBlurDecrease = sBlurDefaultDecrease;
    mBlurColor.set(sBlurDefaultColor);
}   

void RaceDrawer::initJ3D() {
    j3dSys.drawInit();
    j3dSys.setTexCacheRegion(GX_TEXCACHE_128K);
}

void RaceDrawer::drawSceneFirstStage(u32 viewNo) {
    RaceMgr *rcm = RaceMgr::getManager();
    u32 tagName = RaceSceneLight::getSceneLightTagName(viewNo);
    RaceSceneLight *sceneLight = (RaceSceneLight*)LightMgr::getManager()->searchLight(tagName);
    j3dSys.setViewMtx(rcm->getCamera(viewNo)->GetMtx());
    LightMgr::getManager()->setCurrentViewNo(viewNo);
    LightMgr::getManager()->draw();
    KartCtrl::getKartCtrl()->DrawKartInit(viewNo);
    initJ3D();

    rcm->beginProcTime(0);
    if (isDrawSky()) {
        rcm->getCourse()->drawSky(viewNo);
    }
    rcm->endProcTime(0);

    rcm->beginProcTime(1);
    if (isDrawCrs()) {
        rcm->getCourse()->drawNormal(viewNo);
    }
    rcm->endProcTime(1);

    getShadowGeoDrawBuffer()->draw(viewNo);
    GetGeoObjMgr()->drawSimpleModel_groundObj(viewNo);

    rcm->beginProcTime(13);
    mCaptureMgr->capture(viewNo);
    rcm->endProcTime(13);

    rcm->beginProcTime(3);
    ShadowManager::ptr()->drawShadowVolume(viewNo, ShadowModel::cShadowKind_Kart);
    rcm->endProcTime(3);

    rcm->beginProcTime(5);
    if (isDrawKart()) {
        for (int i = 0; i < rcm->getKartNumber(); i++) {
            getKartDrawer(i)->drawKartFirstStage(viewNo);
        }
    }

    ShadowManager::ptr()->drawShadowVolume(viewNo, ShadowModel::cShadowKind_Geo);
    
    if (isDrawDriver()) {
        for (int i = 0; i < rcm->getKartNumber(); i++) {
            getKartDrawer(i)->drawDriverFirstStage(viewNo);
        }
    }
    rcm->endProcTime(5);

    LightMgr::getManager()->searchLight(0x414d4249)->draw(); // AMBI

    rcm->beginProcTime(6);
    ShadowManager::ptr()->drawShadowVolume(viewNo, ShadowModel::cShadowKind_2);
    rcm->endProcTime(6);

    rcm->beginProcTime(7);
    if (isDrawItem()) {
        GXSetAlphaUpdate(GX_TRUE);
        sceneLight->load(GX_LIGHT1);
        getItemDrawBuffer(-1)->drawOpa(viewNo);
        if (RaceMgr::getManager()->isActiveAreaLight()) {
            for (int i = 0; i < rcm->getKartNumber(); i++) {
                getKartDrawer(i)->getLight(viewNo)->draw();
                getItemDrawBuffer(i)->drawOpa(viewNo);
            }
        }

        GXSetAlphaUpdate(GX_FALSE);
        if (RaceMgr::getManager()->isActiveAreaLight()) {
            for (int i = 0; i < rcm->getKartNumber(); i++) {
                getKartDrawer(i)->getLight(viewNo)->draw();
                getItemDrawBuffer(i)->drawXlu(viewNo);
            }
        }

        sceneLight->load(GX_LIGHT1);
        getItemDrawBuffer(-1)->drawXlu(viewNo);
        GXSetAlphaUpdate(GX_TRUE);
        if (RaceMgr::getManager()->isActiveAreaLight()) {
            for (int i = 0; i < rcm->getKartNumber(); i++) {
                if (!rcm->getKartInfo(i)->isGhostKart()) {
                    LightObj *lightObj = getKartDrawer(i)->getLight(viewNo);
                    lightObj->draw();
                    GetItemObjMgr()->drawSimpleModel(viewNo, ItemObjMgr::ItemType_1, i, lightObj);
                }
            }
        }

        sceneLight->load(GX_LIGHT1);
        LightMgr::getManager()->searchLight(0x414d4249)->draw(); //AMBI
        if (RaceMgr::getManager()->isActiveAreaLight()) {
            GetItemObjMgr()->drawSimpleModel(viewNo, ItemObjMgr::ItemType_2, -1, sceneLight);
        }
        else {
            GetItemObjMgr()->drawSimpleModel(viewNo, ItemObjMgr::ItemType_0, -1, sceneLight);
        }
    }
    rcm->endProcTime(7);

    rcm->beginProcTime(2);
    if (isDrawGeoObj()) {
        getGeoDrawBuffer()->drawOpa(viewNo);
        GetGeoObjMgr()->drawSimpleModel(viewNo);
        GXSetAlphaUpdate(GX_FALSE);
        getGeoDrawBuffer()->drawXlu(viewNo);

        if (TBalloonString::getSupervisor()) {
            TBalloonString::getSupervisor()->drawBalloon(viewNo);
        }
        GXSetAlphaUpdate(GX_TRUE);
    }
    rcm->endProcTime(2);

    if (isDrawJugem()) {
        getJugemDrawBuffer()->drawOpa(viewNo);
    }

    GXSetAlphaUpdate(GX_FALSE);

    if (isDrawJugem()) {
        getJugemDrawBuffer()->drawXlu(viewNo);
    }

    if (isDrawCrs()) {
        rcm->getCourse()->drawHigh(viewNo);
    }

    GXSetAlphaUpdate(GX_TRUE);

    ShadowManager::ptr()->drawShadowVolume(viewNo, ShadowModel::cShadowKind_SkyShip);
    rcm->beginProcTime(8);
    getEffectDrawBuffer()->draw(viewNo);
    rcm->endProcTime(8);

    getFeelDrawBuffer()->draw(viewNo);
    j3dSys.reinitGX();
    GetGeoObjMgr()->drawGXObj(viewNo);
    GetGeoObjMgr()->drawPrim(viewNo);
    GetItemObjMgr()->drawColModel(viewNo);
    rcm->getCourse()->drawModelForDebug(viewNo);
}

void RaceDrawer::drawMidScene() {
    RaceMgr *rcm = RaceMgr::getManager();
    rcm->beginProcTime(10);
    mShadowScreen->draw();
    rcm->endProcTime(10);

    if (isGhostActive() && !mGhostScreen->isHide() && isDrawGhostScene()) {
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        mGhostScreen->captureScene();
        GXSetColorUpdate(GX_TRUE);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        drawGhost();
        j3dSys.reinitGX();
        GXSetAlphaUpdate(GX_FALSE);
        mGhostScreen->captureGhost();
        mOrtho->setPort();
        mGhostScreen->draw();
    }
}

void RaceDrawer::drawSceneSecondStage(u32 viewNo) {
    RaceMgr *rcm = RaceMgr::getManager();
    bool isSecondStage = false;

    for (int i = 0; i < rcm->getKartNumber(); i++) {
        if (getKartDrawer(i)->isSecondStage()) {
            isSecondStage = true;
            break;
        }
    }
    GetEfctScreenMgr()->draw_demoScreen(viewNo);
    KartCtrl::getKartCtrl()->DrawKartInit(viewNo);

    rcm->beginProcTime(9);
    GetStEfctMgr()->draw(viewNo);
    GetJPAMgr()->draw(viewNo);
    rcm->endProcTime(9);

    if (!isSecondStage) {
        j3dSys.setViewMtx(rcm->getCamera(viewNo)->GetMtx());
        LightMgr::getManager()->setCurrentViewNo(viewNo);
        LightMgr::getManager()->draw();
        initJ3D();
        AccessoryMgr::ptr()->setCurrentViewNo(viewNo);
        AccessoryMgr::ptr()->simpleDraw(viewNo);
        j3dSys.reinitGX();
    }

    if (TLensFlare::getSupervisor()) {
        TLensFlare::getSupervisor()->draw((u8)viewNo);
    }
    rcm->beginProcTime(11);
    GetEfctScreenMgr()->draw_middle(viewNo);
    GetEfctScreenMgr()->draw(viewNo);
    rcm->endProcTime(11);

    bool secondStageOrAward = false;
    bool awardMode = false;
    if (RaceMgr::getManager()->isAwardDemoMode()) {
        j3dSys.setViewMtx(rcm->getCamera(viewNo)->GetMtx());
        LightMgr::getManager()->searchLight(RaceCupLight::getTagName(viewNo))->draw();
        KartCtrl::getKartCtrl()->DrawKartInit(viewNo);
        initJ3D();

        if (isDrawCup()) {
            TMapObjAwardCup::sSupervisor->draw(viewNo);
        }
        LightMgr::getManager()->searchLight(0x414d4249)->draw(); // AMBI
        awardMode = true;
        secondStageOrAward = true;
    }
    if (isSecondStage) {
        if (!awardMode) {
            j3dSys.setViewMtx(rcm->getCamera(viewNo)->GetMtx());
            LightMgr::getManager()->setCurrentViewNo(viewNo);
            LightMgr::getManager()->draw();
            KartCtrl::getKartCtrl()->DrawKartInit(viewNo);
            initJ3D();
        }
        for (int i = 0; i < rcm->getKartNumber(); i++) {
            getKartDrawer(i)->drawKartSecondStage(viewNo);
        }
        for (int i = 0; i < rcm->getKartNumber(); i++) {
            getKartDrawer(i)->drawDriverSecondStage(viewNo);
        }
        secondStageOrAward = true;
    }
    if (secondStageOrAward) {
        j3dSys.reinitGX();
    }
    if (!secondStageOrAward) {
        KartCtrl::getKartCtrl()->DrawKartInit(viewNo);
    }
    GetJPAMgr()->draw_thunder(viewNo);
}

bool RaceDrawer::drawPreScene() {
    if (isBlurActive()) {
        drawSceneFirstStage(0);
        mBlurScreen->captureScene();
        mBlurScreen->draw(mBlurColor, sBlurA0, mBlurDecrease, sBlurAddX, sBlurAddY);
        mBlurScreen->captureBlur();
    }
    GXSetAlphaUpdate(FALSE);
    ShadowManager::ptr()->setDstAlpha();
    return false;
}

void RaceDrawer::drawPostScene() {
    if (GeoVision::getGeoVisionSupervisor()) {
        GeoVision::getGeoVisionSupervisor()->capture();
    }
    mOrtho->setPort();

    if (GeoShimmer::getSupervisor()) {
        GeoShimmer::getSupervisor()->drawScreen();
    }

    if (isMaskActive()) {
        mMaskScreen->capture();
    }
    mOrtho->setPort();

    GetJPAMgr()->draw_forBefore2D();
    for (u8 i = 0; i < RaceMgr::getManager()->getCameraNumber(); i++) {
        GetJPAMgr()->draw_forBefore2D(i);
    }

    RaceMgr::getManager()->beginProcTime(12);
    J2DManager::getManager()->draw();
    RaceMgr::getManager()->endProcTime(12);

    if (RaceMgr::getManager()->getStaffRoll2D()) {
        RaceMgr::getManager()->getStaffRoll2D()->draw();
    }

    if (isMaskActive()) {
        getMaskScreen()->draw();
    }
    mOrtho->setPort();

    GetJPAMgr()->draw_for2D();
    PauseManager::getManager()->draw();

    if (RaceMgr::getManager()->getAward2D()) {
        RaceMgr::getManager()->getAward2D()->draw();
    }
}

void RaceDrawer::drawGhost() {
    RaceMgr *rcm = RaceMgr::getManager();
    j3dSys.setViewMtx(rcm->getCamera(0)->GetMtx());
    LightMgr::getManager()->setCurrentViewNo(0);
    LightMgr::getManager()->draw();

    KartCtrl::getKartCtrl()->DrawKartInit(0);
    initJ3D();

    for (int i = 0; i < rcm->getKartNumber(); i++) {
        GXSetDstAlpha(GX_TRUE, 0xff - getKartDrawer(i)->getGhostAlpha());
        getKartDrawer(i)->drawKartGhostStage(0);
        getKartDrawer(i)->drawDriverGhostStage(0);
    }

    for (int i = 0; i < rcm->getKartNumber(); i++) {
        if (rcm->getKartInfo(i)->isGhostKart()) {
            GXSetDstAlpha(GX_TRUE, 0xff - getKartDrawer(i)->getGhostAlpha());
            RaceKartLight *light = getKartDrawer(i)->getLight(0);
            light->draw();
            GetItemObjMgr()->drawSimpleModel(0, ItemObjMgr::ItemType_1, i, light);
        }
    }
        
    GXSetDstAlpha(GX_TRUE, 0);
}

void RaceDrawer::showAllGhost() {
    if (isGhostActive()) {
        mGhostScreen->show();
    }
}
