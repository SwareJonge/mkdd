#include "Inagaki/GameAudioMain.h"
#include "kartEnums.h"

namespace GameAudio {
const f32 GA_PLAY_SE_VOLUME_1P = 1.0f;
const f32 GA_PLAY_SE_VOLUME_2P = 0.8f;
const f32 GA_PLAY_SE_VOLUME_3P = 0.7f;
const f32 GA_PLAY_SE_VOLUME_4P = 0.65f;

namespace Parameters {
    u8 sDemoMode;
    u8 sMirrorSwitch;
    u8 sKartType[8];
    u8 sChrType[8];
    u8 sEngType[8];
    u8 sKartChibiFlag[8];
    u8 sPlayerMode;
    u8 sRaceMode;
    u8 sNetworkCubes;
    u8 sNetworkNumber;
    ECourseID sRaceCourse;
    f32 sChibiPitch[4] = { 1.0f, 1.7f, 2.3f, 4.5f };

    void setPlayerMode(u8) {}

    void setRaceMode(u8) {}

    void setDemoMode(u8) {}

    void setKillSwAllObject(bool) {}

    void setRaceCourse(ECourseID) {

    }

    void setKartType(u8, EKartWeight) {
        
    }

    void setNetworkMode(u8, u8) {}

    void setChrType(u8, EKartID) {}

    void setChibiFlag(u8, bool) {}

    void setMirrorSwitch(bool) {}

    ERaceMode getRaceMode() {}

    u8 getPlayerMode() {}

    int getEngineType(u8) {}

    int getCharacterType(u8) {}

    int getRaceCourse() {}

    int getChibiPitch(u32) {}

    int getNetworkCubes() {}

    int getChibiFlag(u8) {}

    u8 getDemoMode() {}

    int getMirrorSwitch() {}

} // namespace Parameters
} // namespace GameAudio
