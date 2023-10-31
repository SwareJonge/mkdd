#ifndef KARTCTRL_H
#define KARTCTRL_H

#include "types.h"
#include "Inagaki/KartSoundMgr.h"
#include "Sato/RivalKart.h"

#include "Yamamoto/kartCamera.h"
#include "Yamamoto/kartPad.h"
#include "Yamamoto/kartSus.h"
#include "Yamamoto/KartAnime.h"
#include "Yamamoto/KartAppendix.h"
#include "Yamamoto/KartDisp.h"
#include "Yamamoto/KartSound.h"
#include "Yamamoto/KartTarget.h"

#include "Yamamoto/kartBody.h"

class KartCtrl
{
public:
    KartCtrl() {}

    static void makeKartCtrl();
    void Init(KartBody *, KartSus **, KartAppendix **, KartSound *, KartTarget *, KartDisp *, int);
    void DynamicsInit(bool);
    void DynamicsReset();
    void DynamicsStage1();
    void DynamicsStage2();
    void DrawKartInit(int);
    void SetGrafPort();
    void DrwaBody(int, u32, Mtx44);
    void DrwaParts(int, u32, Mtx44);

    // KartCtrlInfo
    void SetPadClear(KartGamePad *);
    void SetWanWanPadClear(KartGamePad *);
    void SetGhostPadClear(KartGamePad *);
    void PadAllClear(int);
    void DoContCtl(int);
    void DoContCopy(int);
    void DoContPaste(int);
    f32 GetItemStickY(int);
    f32 GetItemStickX(int);
    void GetItemButton(int);
    void DoLod();
    void GetPortPtr(int);
    void GetCamFovy(int);
    void GetCamAspect(int);
    void GetBodyGround(int);
    void GetRRTireGround(int);
    void GetRLTireGround(int);
    void GetBodyPos(int idx, JGeometry::TVec3f *ret);
    void GetBodyVel(int, JGeometry::TVec3f *);
    void GetLeftFTirePos(int, Vec *);
    void GetRightFTirePos(int, Vec *);
    void GetLeftTirePos(int, Vec *);
    void GetRightTirePos(int, Vec *);
    void GetTirePos(int, int, Vec *);
    void GeTireG(int);
    void GetCarSpeed(int);
    void GetCarRpm(int);
    void GetKartRpm(int);
    void GetDriveCont(int);
    void GetCoDriveCont(int);
    void GetCarStatus(int);
    void GetGameStatus(int);
    void SetTireDispRound(KartBody *, KartSus *, f32);
    void SetKartRpm(KartBody *, f32, f32);
    void WhichDriver(int);
    void WhichNowDriver(int);
    void CheckCamera(int);
    void GetCameraNum(int);
    void CheckItem(int);
    void GetMaxSpeed(int);
    void GetDownSlopeAcc(int);
    void GetDownSlopeSpeed(int);
    void GetTireAngle(int);
    void GetTandemDir(int);
    void GetWaterHeight(int);
    void CheckJugemuSignal();
    GameAudio::KartSoundMgr *GetKartSoundMgr(int idx); /*{ return getKartSound(idx)->mKartSoundMgr; }*/
    f32 GetKartBodyOffset(int);
    void MakeChangePossible(int);
    void CheckTandemItmGet(int);
    void CheckTandemItmRollingGet(int);
    void SetObjectPos(int, JGeometry::TVec3f);
    void CheckThunderBolt(int);
    void GetTireRadius(int);
    void IsBurn(int);
    void IsWallReact(int);
    void HaveBalloon(int);
    int GetDriftCnt(int);
    bool IsMiniGame();
    bool IsMiniGameEnd();
    void CheckWinner();
    void GetKartEffctVel(int, JGeometry::TVec3f *);
    void CheckChange(int);
    void CheckMatchless(int);
    void CheckReverse(int);
    f32 GetKartScale(int);
    RivalKart *getKartEnemy(int);
    void GetKartCenterPtr(int);
    void EnemyInit(int);
    void EnemyResetInit(int);
    void CheckZoomWinConsole();
    void SetUpMiniTurbo(int, u8);
    // Inline/Unused
    // void SetPadKeyClear(KartGamePad *);
    // void DiscriminationCont(int);
    // void IsHandleCont(int);
    // void GetCamNear(int);
    // void GetCamFar(int);
    // void GetFRTireGround(int);
    // void GetFLTireGround(int);
    // void GetFRTireStatus(int);
    // void GetFLTireStatus(int);
    // void GetRRTireStatus(int);
    // void GetRLTireStatus(int);
    // void GetBodyAngle(int);
    // void GetBodyMove(int);
    // void GetCarType(int);
    // void DispSpeed(J2DPrint *);
    // void MakeRnd();
    // void CheckDamage(int);
    // void GetTireMtx(int, Mtx44);
    // void GetCharID(int);
    // void GetCharacterSoundMgr(int);
    // void GetKartTopPos(int);
    // void CheckPushStart(int);
    // void GetCenterLineNum(int);
    // void RivalKart::GetTargetcnt();

    bool HaveRabbit();

    int GetKartNumber() { return mKartCount; }
    KartBody *getKartBody(int idx){return mKartBodies[idx]; } 
    KartCam *getKartCam(int idx) { return mKartCams[idx]; }
    KartSound *getKartSound(int idx) { return mKartSounds[idx]; }

    static KartCtrl *getKartCtrl() { return mCtrl; }

private:
    static KartCtrl *mCtrl;

    KartLoader *mKartLoaders[8];
    KartGamePad *mGamePads[8][2]; // might not be a double sided array
    KartPad *mKartPads[8];
    KartAnime *mKartAnimes[8];
    KartBody *mKartBodies[8];
    KartSus *mKartSus[8][4];
    KartAppendix *mKartAppendix[8][2];
    RivalKart *mRivalKarts[8];
    KartSound *mKartSounds[8];
    KartDisp *mKartDisps[8];
    KartTarget *mKartTargets[8];
    KartCam *mKartCams[8];
    u8 _220[4]; // unused
    u32 mBitfield;
    u32 _228;
    int mKartCount;
}; // size: 0x230

inline KartCtrl *GetKartCtrl() {
    return KartCtrl::getKartCtrl();
}

#endif