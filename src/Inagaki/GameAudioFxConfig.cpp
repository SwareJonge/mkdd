
#include "Inagaki/GameAudioFxConfig.h"

namespace GameAudio {
    JASDsp::FxlineConfig_ Non[4] = {
        {1, 0, 0x6000, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0x6000, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0x0000, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
    };

    JASDsp::FxlineConfig_ Hall[4] = {
        {1, 0, 0x6000, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0x6000, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0x6000, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0x6000, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
    };

    JASDsp::FxlineConfig_ Room[4] = {
        {1, 0, 0x6000, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0x6000, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0x6000, 1, 0, 0x20,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0x6000, 0x20,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
    };

    JASDsp::FxlineConfig_ Canyon[4] = {
        {1, 0, 0x6000, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0x6000, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0x6000, 1, 0, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
        {1, 0, 0, 1, 0x6000, 0x40,
         0, 0, 0, 0, 0, 0, 0x3fff, 0},
    };

    FxLineConfig::FxLineConfig(JKRHeap * heap)
    {
        mConfig[0] = Non;
        mConfig[1] = Hall;
        mConfig[2] = Room;
        mConfig[3] = Canyon;

        for (u8 i = 0; i < 4; i++)
        {
            u32 bufCnt = 0;
            for(u8 j = 0; j < 4; j++)
            {
                u32 configBufCnt = mConfig[j][i].mBufCount;
                if (configBufCnt > bufCnt)
                    bufCnt = configBufCnt;
            } // maybe this is inline?
            mBuf[i] = new (heap, 32) s16[(bufCnt * 0xa0 / 2)];
        }

        set(0);
    }

    void FxLineConfig::set(u8 lineNum)
    {   
    #line 200
        JUT_ASSERT_F(lineNum < 4, "%s", "FxLineConfig::set ラインナンバーが不正です。\n") // line number is invalid
        mNum = lineNum;

        for(u8 i = 0; i < 4; i++)
            JASDsp::setFXLine(i, mBuf[i], &mConfig[lineNum][i]);
    }
}