#ifndef KARTGAME_H
#define KARTGAME_H

#include "Sato/ItemObj.h"
#include "Yamamoto/kartSus.h"
#include "types.h"

class KartGame
{
public:
    void Init(int);
    void GetGorundTireNum();
    void WatchEffectAcceleration();
    void WatchAcceleration();
    void DoItmCancel();
    void DoStopItm();
    void DoChange();
    void DoSlide();
    void DoDriftTurboSterr();
    void SetDriftTurboSterr();
    void CheckDriftTurbo();
    void DoWarmUpRoll();
    void DoRollAnim();
    void DoDriftClear();
    void DoRoll();
    void DoTestPitch();
    void DoLiftTurbo();
    void DoTurbo();
    void DoRollThrow();
    void DoRollOver();
    void DoWanWan();
    void DoPushStart();
    void DoBalance(f32 *, f32);
    void MakeClear();
    void MakeBoardDash();
    void MakeJumpDash();
    void MakeSpJumpDash();
    void MakeMashDash();
    void MakeGoldenMashDash();
    void MakeStartDash();
    void MakeCrashDash();
    void MakeWheelSpin();
    void MakeJump();
    void DoAirCheck();
    void DoRearSlidePower();
    void DoRearSlideBody();
    void DoCorner();
    void FrameWork(f32, KartSus *, KartSus *);
    void DoBodyAction();
    void DoElementForce();
    void CheckBalloon();
    void SetRank();
    void RankWatchMan();
    void ItemWatchMan(ItemObj *);
    void AfterItemWatchMan();
    void DoFlagCtrl();
    void KeepWatch();
    void DoActionMgr();
    void DoActionCtrl();
    void DoStatus();
    // Inline/Unused
    // void DoParamChange();
    // void DoJump();
    // void CheckTeamDriftTurbo();
    // void DoTeamWarmUpRoll();
    // void DoTeamRollAnim();
    // void DoPushBody();
    // void FrameWorkR(f32, f32, KartSus *);
    // void FrameWorkL(f32, f32, KartSus *);
    // void DoTurboPower();
    // void CheckBalloonPlayer();
private:
    // TODO
};

#endif KARTGAME_H