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
        void startSystemSe(u32 id);
        void startRaceSystemSe(u32 id);
        void initRaceSound();
        void bootDSP();
        void setBgmVolume(f32);
        bool isActive();
        bool isWaveLoaded(WS_ID);
        void frameWork();
        void setMasterVolume(s8);
        void setOutputMode(u32);
        void resetAudio(u32);
        void resumeAudio();
        f32 getMasterVolumeValue();
        f32 getTHPOptionVolume() { return getMasterVolumeValue() / 2; };

        JAUSectionHeap *getSectionHeap() { return mSectionHeap; }
        CustomSoundTable *getSoundTable() { return mTable; }

        static Main *getAudio() { return msBasic; };

        static Main *msBasic;

    private:
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
}

#define GetGameAudioMain() GameAudio::Main::getAudio()
#define GameAudioMain GameAudio::Main
#define GameAudioParameters GameAudio::Parameters

#endif