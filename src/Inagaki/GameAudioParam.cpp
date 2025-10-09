#include "Bando/EngineSound.h"
#include "Inagaki/GameAudioCamera.h"
#include "Inagaki/GameAudioMain.h"
#include "Inagaki/GameMapSoundMgr.h"
#include "Inagaki/GameSoundMgr.h"
#include "Inagaki/GameSoundTable.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/mtx.h"
#include "kartEnums.h"
#include "types.h"

namespace GameAudio {
const f32 GA_PLAY_SE_VOLUME_1P = 1.0f;
const f32 GA_PLAY_SE_VOLUME_2P = 0.8f;
const f32 GA_PLAY_SE_VOLUME_3P = 0.7f;
const f32 GA_PLAY_SE_VOLUME_4P = 0.65f;

namespace Parameters {

u8 sDemoMode;
bool sMirrorSwitch;
u8 sKartType[8];
u8 sChrType[8];
u8 sEngType[8];
bool sKartChibiFlag[8];
u8 sPlayerMode;
u8 sRaceMode;
u8 sNetworkCubes;
u8 sNetworkNumber;
ECourseID sRaceCourse;
f32 sChibiPitch[4] = { 1.0f, 1.7f, 2.3f, 4.5f };

void setPlayerMode(u8 playerMode) {
    if (playerMode < 3) {
        setKillSwAllObject(true);
        sPlayerMode = 4;
        return;
    }
    sPlayerMode = playerMode;
    int activeSe = JAISeMgr::getInstance()->getNumActiveSe();

    CameraMgr *cameraMgr = Main::getAudio()->getCamera();
    cameraMgr->setSceneMax(playerMode - 1);
}

void setRaceMode(u8) {}

void setDemoMode(u8 demoMode) {
    bool kill = !!demoMode;
    sDemoMode = demoMode;

    if (Main::getAudio()->getPlayingSequenceID() != 0x1000000) {
        if (kill) {
            Main::getAudio()->set_50(5);
        }
        else {
            Main::getAudio()->set_50(3);
        }
    }

    if (sPlayerMode != 4 || kill) {
        setKillSwAllObject(kill);
    }
}

void setKillSwAllObject(bool killSw) {
    KartSoundMgr::setKillSwAll(killSw);
    ObjectSoundMgr::setKillSwAll(killSw);
    CharacterSoundMgr::setKillSwAll(killSw);
    RockSoundMgr::setKillSwAll(killSw);
    NpcCarSoundMgr::setKillSwAll(killSw);
    ShoreSoundMgr::setKillSwAll(killSw);
    CircleSoundMgr::setKillSwAll(killSw);
    CoasterSoundMgr::setKillSwAll(killSw);
    MapSoundObjectMgr::setKillSwAll(killSw);
}

void setRaceCourse(ECourseID courseID) {
    sRaceCourse = courseID;
    switch (courseID) {
    case MINI_3:
    case MINI_8:
        Main::getAudio()->setFxLine(0);
        break;
    case DK_MOUNTAIN:
    case BOWSER_CASTLE: 
        Main::getAudio()->setFxLine(3);
        break;
    case DINO_DINO_JUNGLE:
        Main::getAudio()->setFxLine(1);
        break;
    default:
        Main::getAudio()->setFxLine(2);
        break;
    }
}

void setKartType(u8 idx, EKartWeight w) {
    sKartType[idx] = w;
}

void setNetworkMode(u8 numCubes, u8 num) {
    sNetworkCubes = numCubes;
    sNetworkNumber = num;
}

void setChrType(u8 idx, EKartID kartID) {
    sEngType[idx] = EngineSound::getEngineType(kartID);
    sChrType[idx] = kartID;
}

void setChibiFlag(u8 idx, const bool isChibi) { // fake?
    u8 i = 2;
    sKartChibiFlag[idx] = isChibi;
    
    CharacterSoundMgr *soundMgr = CharacterSoundMgr::smStart;
    while (soundMgr) {
        if (idx == soundMgr->_64) {
            if (isChibi) {
                soundMgr->setVoicePitch(true);
            }
            else {
                soundMgr->setVoicePitch(false);
            }
            i--;
            if (i == 0) {
                break;
            }
        }
        soundMgr = soundMgr->mNext;
    }
}

void setMirrorSwitch(bool sw) {
    sMirrorSwitch = sw;
}

ERaceMode getRaceMode() {
    return (ERaceMode)sRaceMode;
}

u8 getPlayerMode() {
    return sPlayerMode;
}

int getEngineType(u8 kart) {
    // I wonder if this was copy pasted...
#line 399
    JUT_ASSERT_MSG(kart < 8, "getCharactorType : kartの値が8以上になっています！\n");
    return sEngType[kart];
}

int getCharacterType(u8 kart) {
    // Value of kart is now 8 or higher
#line 406
    JUT_ASSERT_MSG(kart < 8, "getCharactorType : kartの値が8以上になっています！\n");
    return sChrType[kart];
}

ECourseID getRaceCourse() {
    return sRaceCourse;
}

f32 getChibiPitch(u32 soundID) {
    CustomSoundTable *soundTable = Main::getAudio()->getSoundTable();
    u32 swbit = soundTable->getSwBit(soundID);
    u32 idx = (swbit >> 20) & 0x3;
    return sChibiPitch[idx];
}

u8 getNetworkCubes() {
    return sNetworkCubes;
}

bool getChibiFlag(u8 idx) {
    return !!sKartChibiFlag[idx];
}

u8 getDemoMode() {
    return sDemoMode;
}

bool getMirrorSwitch() {
    return sMirrorSwitch;
}

} // namespace Parameters
} // namespace GameAudio
