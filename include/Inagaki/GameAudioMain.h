#ifndef GAMEAUDIOMAIN_H
#define GAMEAUDIOMAIN_H

#include <JSystem/JKernel/JKRHeap.h>
#include <JSystem/JAudio/JAUSectionHeap.h>

#include "kartEnums.h"
#include "Inagaki/GameAudioFxConfig.h"
#include "Inagaki/GameSoundTable.h"
#include "types.h"

class CrsArea;

namespace GameAudio
{
    class CustomMgr;
    class CameraMgr {
    public:
        void setCameraInfo(u32, Vec, Mtx,  CrsArea *);
    };

    class Main
    {
    public:
        enum WS_ID // Data from GCKart.daa, might not be correct
        {
            STREAM0, // NINTENDO_LOGO_MARIO_WS_START?
            STREAM1, // SE_WS_START?
            STREAM2, // not used
            STREAM3  // TANAKA_BGM_WS_START?
        };

        enum PAUSE_STATUS
        {
            // TODO
        };

        void init(JKRSolidHeap *, u32, void *, void *, u32);
        void bootDSP();
        bool isActive();
        void initRaceSound();
        void frameWork();
        void changeSection(u32);
        void checkSection();
        bool isWaveLoaded(WS_ID);
        void checkCourceOnRace();
        void setPause(PAUSE_STATUS);
        void startSequenceBgm(u32);
        void stopSequenceBgm(u32);
        void getPlayingSequenceID();
        void prepareStreamBgm(u32);
        void startStreamBgm(u32);
        void stopStreamBgm(u32);
        u32 getPlayingStreamID();
        void setInvincibleBgm(u8, u8);
        void clearInvincibleBgm(u8, u8);
        void startInvincibleBgm(u8, bool);
        void checkBgm();
        void startSystemSe(u32 id);
        void stopSystemSe(u32 id);
        void startRaceSystemSe(u32);
        void startKartSystemSe(u8, u32);
        void checkThunderMode();
        void setThunderMode(u8);
        void clearThunderMode(u8);
        void startBombPointSe(u8, u8);
        void startBombReachSe(u8, u8);
        void setMasterVolume(s8);
        f32 getMasterVolumeValue();
        void setBgmVolume(f32);
        void updateBgmVolume(u32);
        void fadeOutAll(u32);
        void setRaceSeVolume(f32, u32);
        void setOutputMode(u32);
        void resetAudio(u32);
        void resumeAudio();
        void checkResetInner();
        void setGoalProcess();
        void processSeqCallback(JASTrack *, u16);

        f32 getTHPOptionVolume() { return getMasterVolumeValue() / 2; };
        JAUSectionHeap *getSectionHeap() { return mSectionHeap; }
        CustomSoundTable *getSoundTable() { return mTable; }

        // Inline/Unused
        // Main();
        // void clearWork();
        // void changeSection(char *);
        // void changeSection(void *);
        // void muteStreamBgm(f32, u32);
        // void changeStreamPlayTrack(u8);
        // void setSeVolume(f32);
        // void checkReset();



        CameraMgr *getCamera() {
#line 157
            JUT_ASSERT_MSG(mCamera, "GameAudioMain : カメラが初期化されていません。\n");
            return mCamera;
        }
        // static sJAudio; // TODO: type
        static Main *getAudio() { return msBasic; };

    private:
        static Main *msBasic;

        CustomMgr *mMgr;              // 00
        u8 _04[0xC - 0x4];            //
        CustomSoundTable *mTable;     // 0C
        u8 _10[4];                    //
        JAUSectionHeap *mSectionHeap; // 14
        u8 _18[0x44 - 0x18];          //
        FxLineConfig *mConfig;        // 44
        u8 _48[4];                    //
        PAUSE_STATUS mPauseStatus;    // 4C
        u8 _50[0x64 - 0x50];          //
        CameraMgr *mCamera;           // 64
        u8 _68[0xa8 - 0x68];          //

    }; // Size: 0xa8

    namespace Parameters
    {
        void setPlayerMode(u8);
        void setRaceMode(u8);
        void setDemoMode(u8);
        void setKillSwAllObject(bool);
        void setRaceCourse(ECourseID);
        void setKartType(u8, EKartWeight);
        void setNetworkMode(u8, u8);
        void setChrType(u8, EKartID);
        void setChibiFlag(u8, bool);
        void setMirrorSwitch(bool);
        ERaceMode getRaceMode();
        u8 getPlayerMode();
        int getEngineType(u8);
        int getCharacterType(u8);
        int getRaceCourse();
        int getChibiPitch(u32);
        int getNetworkCubes();
        int getChibiFlag(u8);
        u8 getDemoMode();
        int getMirrorSwitch();
        extern f32 sChibiPitch[4];
        extern u8 sDemoMode;
        extern u8 sMirrorSwitch;
        extern u8 sKartType[8];
        extern u8 sChrType[8];
        extern u8 sEngType[8];
        extern u8 sKartChibiFlag[8];
        extern u8 sPlayerMode;
        extern u8 sRaceMode;
        extern u8 sNetworkCubes;
        extern u8 sNetworkNumber;
        extern ECourseID sRaceCourse;
    }

    extern int GoalFanfareDelayFrame;
    extern int ParadeBgmMuteFrame;
    extern f32 ParadeBgmMuteVolume;
}

#define GetGameAudioMain() GameAudio::Main::getAudio()
#define GameAudioMain GameAudio::Main
#define GameAudioParameters GameAudio::Parameters

#endif