#ifndef JAUDIO_JASARAMSTREAM_H
#define JAUDIO_JASARAMSTREAM_H

#include "JSystem/JAudio/System/JASTaskThread.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/dvd.h"

class JASChannel;

namespace JASDsp
{
    class TChannel;
}

class JASAramStream
{
public:
    typedef void (*StreamCallback)(u32, JASAramStream *, void *);
    static const u32 CHANNEL_MAX = 6;
    enum CallbackType
    {
        CB_START, // 0
        CB_STOP, // 1
    };

    // Used internally for passing data to task functions
    struct TaskData
    {
        JASAramStream *stream; // 0
        u32 _4;                // 4
        int _8;                // 8
    };

    struct Header
    {
        u32 tag;        // 00
        u8 _4[5];       // 04
        u8 format;      // 09
        u8 bits;        // 0A
        u16 channels;   // 0C
        u16 loop;       // 0E
        int _10;        // 10
        u8 _14[4];      // 14
        int loop_start; // 18
        int loop_end;   // 1C
        u32 block_size; // 20
        u8 _24[4];      // 24
        u8 _28;         // 28
        u8 _29[0x17];   // 29

    }; // Size: 0x40

    struct BlockHeader
    {
        u32 tag; // 00
        u32 _4;  // 04
        struct   // 08
        {
            s16 _0;
            s16 _2;
        } _8[6];
    }; // Size: 0x20

    static void initSystem(u32, u32);
    JASAramStream();
    void init(u32, u32, StreamCallback, void *);
    bool prepare(s32, int);
    bool start();
    bool stop(u16);
    bool pause(bool);
    bool cancel();
    u32 getBlockSamples() const;
    static void headerLoadTask(void *);
    static void firstLoadTask(void *);
    static void loadToAramTask(void *);
    static void finishTask(void *);
    static void prepareFinishTask(void *);
    bool headerLoad(u32, int);
    bool load();
    static s32 channelProcCallback(void *);
    static s32 dvdErrorCheck(void *);
    static void channelCallback(u32, JASChannel *, JASDsp::TChannel *, void *);
    void updateChannel(u32, JASChannel *, JASDsp::TChannel *);
    s32 channelProc();
    void channelStart();
    void channelStop(u16);

    void setPitch(f32 pitch) { mPitch = pitch; }
    void setVolume(f32 volume)
    {
        for (int i = 0; i < CHANNEL_MAX; i++)
        {
            mChannelVolume[i] = volume;
        }
    }

    void setPan(f32 pan)
    {
        for (int i = 0; i < CHANNEL_MAX; i++)
        {
            mChannelPan[i] = pan;
        }
    }

    void setFxmix(f32 fxMix)
    {
        for (int i = 0; i < CHANNEL_MAX; i++)
        {
            mChannelFxMix[i] = fxMix;
        }
    }

    void setDolby(f32 dolby)
    {
        for (int i = 0; i < CHANNEL_MAX; i++)
        {
            mChannelDolby[i] = dolby;
        }
    }

    void setChannelVolume(int channel, f32 volume)
    {
#line 288
        JUT_ASSERT(channel < CHANNEL_MAX);
        mChannelVolume[channel] = volume;
    }

    void setChannelPan(int channel, f32 pan)
    {
        JUT_ASSERT(channel < CHANNEL_MAX);
        mChannelPan[channel] = pan;
    }

    void setChannelFxmix(int channel, f32 fxMix)
    {
        JUT_ASSERT(channel < CHANNEL_MAX);
        mChannelFxMix[channel] = fxMix;
    }

    void setChannelDolby(int channel, f32 dolby)
    {
        JUT_ASSERT(channel < CHANNEL_MAX);
        mChannelDolby[channel] = dolby;
    }

    static u32 getBlockSize() { return sBlockSize; }

    OSMessageQueue _000;                // 000
    OSMessageQueue _020;                // 020
    void *_040[16];                     // 040
    void *_080[4];                      // 080
    JASChannel *mChannels[CHANNEL_MAX]; // 090
    JASChannel *_0a8;                   // 0A8
    bool _0ac;                          // 0AC
    bool _0ad;                          // 0AD
    u8 _0ae;                            // 0AE
    int _0b0;                           // 0B0
    int _0b4;                           // 0B4
    u32 _0b8;                           // 0B8
    int _0bc;                           // 0BC
    bool _0c0;                          // 0C0
    u32 _0c4;                           // 0C4
    f32 _0c8;                           // 0C8
    DVDFileInfo mDvdFileInfo;           // 0CC
    u32 _108;                           // 108
    int _10c;                           // 10C
    u32 mBlock;                         // 110
    u8 _114;                            // 114
    u32 _118;                           // 118
    int _11c;                           // 11C
    int _120;                           // 120
    int _124;                           // 124
    u16 _128;                           // 128
    int _12c;                           // 12C
    s16 _130[CHANNEL_MAX];              // 130
    s16 _13c[CHANNEL_MAX];              // 13C
    int _148;                           // 148
    u32 _14c;                           // 14C
    StreamCallback mCallback;           // 150
    void *mCallbackData;                // 154
    u16 _158;                           // 158
    u16 mChannelNum;                    // 15A
    u32 mBufCount;                      // 15C
    u32 _160;                           // 160
    u32 _164;                           // 164
    bool mLoop;                         // 168
    u32 mLoopStart;                     // 16C
    u32 mLoopEnd;                       // 170
    f32 mVolume;                        // 174
    f32 mPitch;                         // 178
    f32 mChannelVolume[CHANNEL_MAX];    // 17C
    f32 mChannelPan[CHANNEL_MAX];       // 194
    f32 mChannelFxMix[CHANNEL_MAX];     // 1AC
    f32 mChannelDolby[CHANNEL_MAX];     // 1C4
    u16 _1dc[CHANNEL_MAX];              // 1DC

    static JASTaskThread *sLoadThread;
    static u8 *sReadBuffer;
    static u32 sBlockSize;
    static u32 sChannelMax;
};

#endif