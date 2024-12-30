#ifndef JAUDIO_JASTRACK_H
#define JAUDIO_JASTRACK_H

#include "types.h"

#include "JSystem/JAudio/System/JASBankTable.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASOscillator.h"
#include "JSystem/JAudio/System/JASRegisterParam.h"
#include "JSystem/JAudio/System/JASSeqCtrl.h"
#include "JSystem/JAudio/System/JASSoundParams.h"

// REDO OFFSETS

class JASChannel;

class JASTrackPort
{
public:
    static const u32 MAX_PORTS = 16;

    void init();
    u16 readImport(u32 port_num);
    u16 readExport(u32 port_num);
    void writeImport(u32 port_num, u16);
    void writeExport(u32 port_num, u16);
    bool checkImport(u32 port_num) const;
    bool checkExport(u32 port_num) const;

    u16 get(u32 port_num) { return mPortData[port_num]; }
    void set(u32 port_num, u16 data) { mPortData[port_num] = data; }

    u16 mImportFlags;
    u16 mExportFlags;
    u16 mPortData[MAX_PORTS];
};

class JASTrack : public JASPoolAllocObject_MultiThreaded<JASTrack>
{
public:
    static const u32 MAX_CHILDREN = 16;
    static const s32 STATUS_FREE = 0;
    static const s32 STATUS_RUN = 1;
    static const s32 STATUS_STOPPED = 2;

    class MoveParam_
    {
        MoveParam_(); // 0x800a0270

        f32 mValue;  // 00
        f32 mTarget; // 04
        u32 mCount;  // 08
    };
    class TChannelMgr : public JASPoolAllocObject_MultiThreaded<TChannelMgr>
    {
    public:
        static const int CHANNEL_MAX = 8;

        TChannelMgr(JASTrack *); // 0x800a3e4c
        void init();             // 0x800a3ec0
        void releaseAll();       // 0x800a3f30
        void noteOff(u32, u16);  // 0x800a3fac
        void setPauseFlag(bool); // 0x800a4088

        JASChannel *mChannels[8];     // 00
        JASChannelParams mParams;     // 20
        s16 _38[8];                   // 38
        JASSoundParams *mSoundParams; // 48
        JASTrack *mTrack;             // 4c
    };
    class TList : JGadget::TLinkList<JASTrack, -0x240>
    {
        void append(JASTrack *); // 0x800a3c3c
        void seqMain();          // 0x800a3cf0
        virtual ~TList();        // 0x800a42d8
    };

    JASTrack *getChild(u32 index) const
    {
#line 115
        JUT_ASSERT(index < MAX_CHILDREN)
        return mChildren[index];
    }

    JASTrack();                                                                // 0x800a018c
    ~JASTrack();                                                               // 0x800a0288
    void setChannelMgrCount(u32);                                              // 0x800a0374
    void init();                                                               // 0x800a0544
    void initTimed();                                                          // 0x800a09e8
    void assignExtBuffer(u32, JASSoundParams *);                               // 0x800a0a5c
    void setSeqData(void *, u32);                                              // 0x800a0af4
    void startSeq();                                                           // 0x800a0b8c
    void stopSeq();                                                            // 0x800a0c64
    void start();                                                              // 0x800a0ca8
    void close();                                                              // 0x800a0d60
    void connectChild(u32, JASTrack *);                                        // 0x800a10e4
    void closeChild(u32);                                                      // 0x800a1190
    JASTrack *openChild(u32 trk_no);                                                       // 0x800a12ec
    void connectBus(int, int);                                                 // 0x800a1820
    void setLatestKey(u8);                                                     // 0x800a18b0
    s32 noteOn(u32, u32, u32);                                                // 0x800a18ec
    s32 gateOn(u32, u32, f32, u32);                                           // 0x800a1ac0
    s32 noteOff(u32, u16);                                                    // 0x800a1cf8
    void checkNoteStop(u32) const;                                             // 0x800a1db8
    void overwriteOsc(JASChannel *);                                           // 0x800a1e74
    void updateTimedParam();                                                   // 0x800a1f28
    void updateTrack(f32);                                                     // 0x800a2014
    void updateTempo();                                                        // 0x800a21b4
    void updateSeq(bool, f32);                                                 // 0x800a2250
    void seqTimeToDspTime(f32);                                                // 0x800a2578
    void setParam(u32, f32, u32);                                              // 0x800a2614
    void noteOffAll(u16);                                                      // 0x800a26d0
    void mute(bool);                                                           // 0x800a29a0
    void setOscTable(u32, const JASOscillator::Point *);                       // 0x800a29d8
    void setOscAdsr(s16, s16, s16, s16, u16);                                  // 0x800a2a74
    void setFIR(const s16 *);                                                  // 0x800a2acc
    void setIIR(const s16 *);                                                  // 0x800a2b2c
    u16 readPortSelf(u32);                                                    // 0x800a2b80
    void writePortSelf(u32, u16);                                              // 0x800a2ba4
    void writePort(u32, u16);                                                  // 0x800a2bc8
    u16 readPort(u32);                                                        // 0x800a2c2c
    void pause(bool);                                                          // 0x800a2c50
    void getTransposeTotal() const;                                            // 0x800a2d38
    void isMute() const;                                                       // 0x800a2ea8
    void setTempo(u16);                                                        // 0x800a3028
    void setTempoRate(f32);                                                    // 0x800a30b8
    void setTimebase(u16);                                                     // 0x800a3148
    void channelUpdateCallback(u32, JASChannel *, JASDsp::TChannel *, void *); // 0x800a31d8
    void getRootTrack();                                                       // 0x800a3350
    void tickProc();                                                           // 0x800a3368
    void seqMain();                                                            // 0x800a3afc

    // Inline/Unused
    // void JASThreadingModel::InterruptsDisable<JASMemPool_MultiThreaded<TChannelMgr>>::Lock::~InterruptsDisable();
    // void inherit(const JASTrack &);
    // void setAutoMixingMode();
    // void getVolume() const;
    // void getPitch() const;
    // void getPan() const;
    // void getFxmix() const;
    // void getDolby() const;
    // void channelStart(TChannelMgr *, unsigned long, unsigned long, unsigned long);
    // void setOscParam(unsigned long, const JASOscillator::Data &);
    // void setChannelPauseFlag(bool);
    // void updateChannel(JASChannel *, JASDsp::TChannel *);
    // void getChannelCount() const;
    // void JGadget::TLinkList<JASTrack, -584>::~TLinkList();
    // void JASGlobalInstance<JASDefaultBankTable>::~JASGlobalInstance();
    JASTrack *getParent() { return mParent; }
    JASSeqCtrl* getSeqCtrl() { return &mSeqCtrl; }
    s32 getStatus() const { return mStatus; }
    u32 getChannelMgrCount() const { return mChannelMgrCount; }

    u8 getBendSense() const { return mBendSense; }
    u16 getDirectRelease() const { return mDirectRelease; }
    u8 getGateRate() const { return mGateRate; }
    int getNoteOnPrio() const { return mNoteOnPrio; }
    f32 getPanPower() const { return mPanPower; }
    int getReleasePrio() const { return mReleasePrio; }
    u32 getSkipSample() const { return mSkipSample; }
    u16 getTimebase() const { return mTimebase; }
    s32 getTranspose() const { return mTranspose; }
    u16 getTremDelay() const { return mTremDelay; }
    f32 getTremDepth() const { return mTremDepth; }
    f32 getTremPitch() const { return mTremPitch; }
    u16 getVibDelay() const { return mVibDelay; }
    f32 getVibDepth() const { return mVibDepth; }
    f32 getVibPitch() const { return mVibPitch; }

    void setBendSense(u8 val) { mBendSense = val; }
    void setDirectRelease(u16 release) { mDirectRelease = release; }
    void setGateRate(u8 rate) { mGateRate = rate; }
    void setNoteOnPrio(u8 prio) { mNoteOnPrio = prio; }
    void setPanPower(f32 power) { mPanPower = power; }
    void setReleasePrio(u8 prio) { mReleasePrio = prio; }
    void setSkipSample(u32 skip) { mSkipSample = skip; }
    void setTranspose(s8 val) { mTranspose = val; }
    void setTremDelay(u16 delay) { mTremDelay = delay; }
    void setTremDepth(f32 depth) { mTremDepth = depth; }
    void setTremPitch(f32 pitch) { mTremPitch = pitch; }
    void setVibDelay(u16 delay) { mVibDelay = delay; }
    void setVibDepth(f32 depth) { mVibDepth = depth; }
    void setVibPitch(f32 pitch) { mVibPitch = pitch; }

    u16 getPort(u32 port) { return mTrackPort.get(port); }
    u16 getBankNumber() const { return mBankNumber; }    
    u16 getProgNumber() const { return mProgNumber; }
    void setPort(u32 port, u16 data) { mTrackPort.set(port, data); }
    void setBankNumber(u16 num) { mBankNumber = num; }
    void setProgNumber(u16 num) { mProgNumber = num; }
    u32 readReg(JASRegisterParam::RegID regId) { return mRegisterParam.read(regId); }
    void writeReg(JASRegisterParam::RegID regId, u32 p2) { mRegisterParam.write(regId, p2); }
    bool checkPort(u32 port) const { return mTrackPort.checkExport(port); }
    bool checkPortIn(u32 port) const { return mTrackPort.checkImport(port); }

    void setAutoDelete(bool del) {
        mFlags.autoDelete = del;
    }

    // Data
    static JASOscillator::Point sAdsTable[12];    // 0x80369df0
    static JASOscillator::Data sEnvOsc;           // 0x80369e08
    static JASOscillator::Data sPitchEnvOsc;      // 0x80369e20
    static JASDefaultBankTable sDefaultBankTable; // 0x803fac24
    static TList sTrackList;                      // 0x803fb070

    JASSeqCtrl mSeqCtrl;               // 000
    JASTrackPort mTrackPort;           // 05c
    JASRegisterParam mRegisterParam;   // 080
    MoveParam_ mMoveParam[6];          // 09c, volume, pitch, fxmix, pan, dolby, distFilter
    JASOscillator::Data mOscParam[2];  // 0e4
    JASOscillator::Point mOscPoint[4]; // 114
    JASTrack *mParent;                 // 12c
    JASTrack *mChildren[MAX_CHILDREN]; // 130
    TChannelMgr *mChannelMgrs[4];      // 170
    TChannelMgr mDefaultChannelMgr;    // 180
    int mChannelMgrCount;              // 1d0
    JASDefaultBankTable *mBankTable;   // 1d4
    f32 _1d8;                          // 1d8
    f32 _1dc;                          // 1dc
    f32 mVibDepth;                     // 1e0
    f32 mVibPitch;                     // 1e4
    f32 mTremDepth;                    // 1e8
    f32 mTremPitch;                    // 1ec
    u16 mVibDelay;                     // 1f0
    u16 mTremDelay;                    // 1f2
    s16 mFIR[8];                       // 1f4
    s16 mIIR[8];                       // 204
    u8 mFilterMode;                    // 214
    f32 mPanPower;                     // 218
    f32 mTempoRate;                    // 21c
    u32 mSkipSample;                   // 220
    u16 mDirectRelease;                // 224
    u16 mTempo;                        // 226
    u16 mTimebase;                     // 228
    s8 mTranspose;                     // 22a
    u8 _22b;                           // 22b
    u8 mBankNumber;                    // 22c
    u8 mProgNumber;                    // 22d
    u8 mBendSense;                     // 23e
    u8 mNoteOnPrio;                    // 22f
    u8 mReleasePrio;                   // 230
    u8 mGateRate;                      // 231
    u16 mMixConfig[6];                 // 234
    // TODO

    int mStatus; // 240, not volatile here?
    struct
    {
        bool pause : 1;
        bool mute : 1;
        bool flag2 : 1;
        bool autoDelete : 1;
        bool flag4 : 1;
        bool flag5 : 1;
        bool flag6 : 1;
        bool flag7 : 1;
    } mFlags;          // 216
    u8 _mkdd_pad[0x8]; //
}; // Size: 0x250

#endif
