#include "Inagaki/GameSoundMgr.h"

namespace GameAudio {

// TODO

u32 BoundSe[0x19];
u32 WheelSpinSe[0x19];
u32 SpinSe[0x19];
u32 SpinTurnSe[0x19];

f32 GA_ENEMY_VOLUME_DOWN_VALUE = 0.85f;
const f32 EngineKeisuuRaceUp[] = {
    0.006f, 0.006f, 0.005f, 0.003f, 
    0.015f, 0.015f, 0.013f, 0.011f, 
    0.02f,
};

const f32 EngineKeisuuRaceDown[] = {
    0.005f, 0.003f, 0.005f, 0.002f, 
    0.012f, 0.012f, 0.012f, 0.009f, 
    0.019f,
};

u8 KartSoundMgr::smKartCount;
u8 KartSoundMgr::smEntryKartCount;
u8 KartSoundMgr::smGoalKartCount;

u8 KartSoundMgr::smKartRankClassMem[7] = {};

KartSoundMgr::KartSoundMgr(Vec *pos, JKRHeap *heap, u8 p3, u8 p4) : SoundMgr(pos, heap, 12) {

}

KartSoundMgr::~KartSoundMgr() {}

void KartSoundMgr::startSoundHandleNumber(u8, u32, u32) {}

void KartSoundMgr::dispose() {}

void KartSoundMgr::init() {}

void KartSoundMgr::changeDriver(bool) {}

void KartSoundMgr::frameWork(u8) {}

void KartSoundMgr::checkAfterGoalVolume() {}

void KartSoundMgr::setWaterDepth(u8, f32) {}

void KartSoundMgr::setSlip(u8, u8, u8, f32) {}

void KartSoundMgr::setConductStatus(f32, f32, bool, bool, bool, u8, CrsArea *) {}

void KartSoundMgr::setWaterCutoffPort(u16) {}

void KartSoundMgr::countGoalKart() {}

void KartSoundMgr::setConductLocomotiveAccel() {}

void KartSoundMgr::setConductLocomotiveSpeed(bool) {}

void KartSoundMgr::setConductPressed() {}

void KartSoundMgr::setConductSignal() {}

void KartSoundMgr::setConductOutOfCourse(u8) {}

void KartSoundMgr::setConductTrouble(f32, u8) {}

void KartSoundMgr::setConductRace(bool) {}

void KartSoundMgr::setConductAfterGoal(bool) {}

void KartSoundMgr::setCrushSe(CrsGround::EMat, f32) {}

void KartSoundMgr::setCrushSe(u32, f32) {}

void KartSoundMgr::setBrakeSe(u32) {}

void KartSoundMgr::setDashSe(u32) {}

void KartSoundMgr::setMiniturboSe(u32) {}

void KartSoundMgr::setJumpUpSe(u32) {}

void KartSoundMgr::setBoundSe(f32) {}

void KartSoundMgr::setWheelSpinSe() {}

void KartSoundMgr::setSpinSe() {}

void KartSoundMgr::setSpinTurnSe() {}

void KartSoundMgr::setSe(u32) {}

void KartSoundMgr::setChibiPitch(JAISoundHandle *) {}

void KartSoundMgr::adjustEngine() {}

void KartSoundMgr::crushRenzokuTaisaku() {}

void KartSoundMgr::slipParamSet() {}

void KartSoundMgr::checkEcho() {}

void KartSoundMgr::setInvincibleBgm(u8) {}

void KartSoundMgr::clearInvincibleBgm(u8) {}

void KartSoundMgr::setChibiFlag(bool, bool) {}

}
