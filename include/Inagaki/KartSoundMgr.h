#ifndef KARTSOUNDMGR_H
#define KARTSOUNDMGR_H

#include "types.h"
#include "Inagaki/SoundMgr.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/Course/CrsArea.h"

namespace GameAudio
{
class KartSoundMgr : public SoundMgr<KartSoundMgr>
{
public:
    KartSoundMgr(Vec *, JKRHeap *, u8, u8);                           // 0x80123118
    void startSoundHandleNumber(u8, u32, u32);                        // 0x801233fc
    virtual void dispose();                                           // 0x801236d0
    virtual void init();                                              // 0x80123708
    void changeDriver(bool);                                          // 0x80123ad4
    void frameWork(u8 rank);                                          // 0x80123ad8
    void checkAfterGoalVolume();                                      // 0x80123c50
    void setWaterDepth(u8, f32);                                      // 0x80123ee4
    void setSlip(u8, u8, u8, f32);                                    // 0x80123ef4
    void setConductStatus(f32, f32, bool, bool, bool, u8, CrsArea *); // 0x80124400
    void setWaterCutoffPort(unsigned short);                          // 0x8012485c
    void countGoalKart();                                             // 0x80124930
    void setConductLocomotiveAccel();                                 // 0x8012495c
    void setConductLocomotiveSpeed(bool);                             // 0x80124d9c
    void setConductPressed();                                         // 0x80125194
    void setConductSignal();                                          // 0x80125490
    void setConductOutOfCourse(u8);                                   // 0x80125b04
    void setConductTrouble(f32, u8);                                  // 0x80125f60
    void setConductRace(bool);                                        // 0x801263cc
    void setConductAfterGoal(bool);                                   // 0x80127188
    void setCrushSe(CrsGround::EMat, f32);                            // 0x801271a8
    void setCrushSe(u32, f32);                                        // 0x801272e4
    void setBrakeSe(u32);                                             // 0x80127474
    void setDashSe(u32);                                              // 0x8012763c
    void setMiniturboSe(u32);                                         // 0x801276e4
    void setJumpUpSe(u32);                                            // 0x80127770
    void setBoundSe(f32);                                             // 0x801277dc
    void setWheelSpinSe();                                            // 0x8012798c
    void setSpinSe();                                                 // 0x80127aa4
    void setSpinTurnSe();                                             // 0x80127bb4
    virtual void setSe(u32);                                          // 0x80127ccc
    void setChibiPitch(JAISoundHandle *);                             // 0x80127ec8
    void adjustEngine();                                              // 0x80127fb8
    void crushRenzokuTaisaku();                                       // 0x80128140
    void slipParamSet();                                              // 0x8012816c
    void checkEcho();                                                 // 0x801288f8
    void setInvincibleBgm(u8);                                        // 0x80128aac
    void clearInvincibleBgm(u8);                                      // 0x80128c30
    void setChibiFlag(bool, bool);                                    // 0x80128e98

    virtual ~KartSoundMgr() {}

private:
    u8 _5c[0x134 - 0x5c];
};
}

#endif
