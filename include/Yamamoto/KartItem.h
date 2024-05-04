#ifndef KARTITEMSTRAT_H
#define KARTITEMSTRAT_H

#include "types.h"

class KartItem
{
public:
    KartItem() {}
    void Init(int);
    void DoRabbit();
    void SetSucMashItem();
    void CheckHitItem();
    void FallStealItem(int, int);
    void FallItem();
    void FallAllItem();
    void ReleseWanWan();
    void CompulsionReleseWanWan();
    void KillWanWan();
    void IsMyItem();
    void IsChanging();
    void DoTurtleReleaseDir(int, int);
    void DoBananaReleaseDir();
    void DoTurtleReleaseDynamicDir(u32);
    void DoBananaReleaseDynamicDir();
    void TeamRobItm();
    void SuspendItem();
    void CheckOtherAnime();
    void DoOtherAnime();
    void DoTandemItemAnime();
    void DoTandemItemRelease();
    void DoItem();
    // Inline/Unused
    // void DoDriverTurtleReleaseDir(int);
    // void DoDriverBananaReleaseDir();
    // void DoBarrelReleaseDir(int, int);
    // void DoJumperReleaseDir();
    // void DoTurtleSpReleaseDir(int);
    // void DoBananaSpReleaseDir();
    // void DoDriverTurtleReleaseDynamicDir();
    // void DoDriverBananaReleaseDynamicDir();
    // void DoBarrelReleaseDynamicDir();
    // void DoDriverItemAnime();
    // void DoDriverItemRelease();
private:
    u8 _0[0xc]; //

}; // 0xc

#endif // KARTITEMSTRAT_H