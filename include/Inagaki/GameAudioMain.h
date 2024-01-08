#ifndef GAMEAUDIOMAIN_H
#define GAMEAUDIOMAIN_H

#include <JSystem/JKernel/JKRHeap.h>
#include <JSystem/JAudio/JAUSectionHeap.h>

#include "Inagaki/GameAudioFxConfig.h"
#include "Inagaki/GameSoundTable.h"
#include "types.h"

namespace GameAudio
{
    class CustomMgr;
    class CameraMgr;

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
        void getPlayingStreamID();
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
        extern u8 getDemoMode();
        extern void setDemoMode(u8);
    }

    extern int ParadeBgmMuteFrame;
    extern f32 ParadeBgmMuteVolume;
}

#define GetGameAudioMain() GameAudio::Main::getAudio()
#define GameAudioMain GameAudio::Main
#define GameAudioParameters GameAudio::Parameters

#endif