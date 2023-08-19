#ifndef KARTSOUND_H
#define KARTSOUND_H

#include "Kaneshige/Course/CrsGround.h"
#include "Inagaki/KartSoundMgr.h"

class KartSound
{
public:
    KartSound() {}

    void Init(int);                            // 0x802fec88
    void DoInfo(int);                          // 0x802fed84
    void DoFinishSound(int);                   // 0x802fefec
    void DoSlipSound(int);                     // 0x802ff03c
    void DoWallHitSound(f32, CrsGround::EMat); // 0x802ff18c
    void DoHorn();                             // 0x802ff1b0
    void DoKartHitSound(f32);                  // 0x802ff30c
    void DoMashDashSound();                    // 0x802ff338
    void DoDashSound();                        // 0x802ff364
    void DoRollCrashStartSound();              // 0x802ff390
    void DoRollCrashSound();                   // 0x802ff3c8
    void DoRollCrashEndSound();                // 0x802ff400
    void DoBrakeSound();                       // 0x802ff438
    void DoStrikeSound();                      // 0x802ff464
    void DoStarStartSound();                   // 0x802ff49c
    void DoStarStopSound();                    // 0x802ff4c4
    void DoWanWanStartSound();                 // 0x802ff4ec
    void DoWanWanStopSound();                  // 0x802ff514
    void DoChangeStarSound();                  // 0x802ff554
    void DoChibiSound(bool);                   // 0x802ff57c
    void DoGoalSound();                        // 0x802ff5a4
    void DoLapSound();                         // 0x802ff5dc
    void DoThunderSound();                     // 0x802ff614
    void DoShootSound();                       // 0x802ff640
    void DoFireBall();                         // 0x802ff66c
    void DoHitFireBall();                      // 0x802ff6a4
    void DoKartDiveSound();                    // 0x802ff6dc
    void DoKartMiniTuroboSound();              // 0x802ff714
    void DoKartsetSeSound(u32);                // 0x802ff740
    void DoLandingSound(f32);                  // 0x802ff770
    void DoWheelSpin();                        // 0x802ff794
    void DoShineStartBGM();                    // 0x802ff7b8
    void DoShineStopBGM();                     // 0x802ff7e4
    void DoWallObjectSound(u32);               // 0x802ff81c
    void DoItemAlarm();                        // 0x802ff89c
    void DoSpinSound();                        // 0x802ff918
    void DoSpinTurnSound();                    // 0x802ff93c
    void DoStrikeVoice();                      // 0x802ff960
    void DoStaggerVoice();                     // 0x802ff990
    void DoCannonVoice();                      // 0x802ff9c0
    void DoDragVoice();                        // 0x802ffa00
    void DoChampionVoice();                    // 0x802ffa30
    void DoPrizeVoice();                       // 0x802ffa60
    void DoOutsideVoice();                     // 0x802ffa90
    void DoGoalVoice();                        // 0x802ffac0
    void DoDriverChampionVoice();              // 0x802ffc68
    void DoDriverPrizeVoice();                 // 0x802ffc98
    void DoDriverOutsideVoice();               // 0x802ffcc8
    void DoDriverGoalVoice();                  // 0x802ffcf8
    void DoSpinVoice();                        // 0x802ffea0
    void DoFallVoice();                        // 0x802ffed0
    void DoDashVoice();                        // 0x802fff18
    void DoGoldenDashVoice();                  // 0x802fff48
    void DoThrowVoice();                       // 0x802fff78
    void DoPutVoice();                         // 0x802fffa8
    void DoChangeVoice();                      // 0x802fffd8
    void DoWanwanVoice();                      // 0x80300008
    void DoItmHitVoice();                      // 0x80300038
    void DoPathVoice();                        // 0x80300068
    void DoShootVoice();                       // 0x80300098
    void DoRollCrashVoice();                   // 0x803000e0
    void DoHeartVoice();                       // 0x80300128
    void DoTandemVoice(u8);                    // 0x80300158
    void DoDriverVoice(u8);                    // 0x80300194
    void DoAllVoice(u8);                       // 0x803001d0
    // Inline/Unused
    // void DoWoodHitSound(float);
    // void DoBomCrashSound();
    // void DoStrikedSound();
    // void DoCannonSound();
    // void DoJumpVoice();
private:
    GameAudio::KartSoundMgr *mKartSoundMgr;
    KartBody *mKartBody;
    u8 _8;
    u8 _9;
};

#endif KARTSOUND_H
