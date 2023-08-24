#ifndef JAUDIO_TCHANNELMGR_H
#define JAUDIO_TCHANNELMGR_H

#include "types.h"
#include "JSystem/JAudio/System/JASSoundParams.h"

class JASChannel;

class JASTrack
{
public:
    class MoveParam_
    {
        MoveParam_(); // 0x800a0270
    };
    class TChannelMgr
    {
        TChannelMgr(JASTrack *); // 0x800a3e4c
        void init();             // 0x800a3ec0
        void releaseAll();       // 0x800a3f30
        void noteOff(u32, u16);  // 0x800a3fac
        void setPauseFlag(bool); // 0x800a4088
    };
    class TList
    {
        void append(JASTrack *); // 0x800a3c3c
        void seqMain();          // 0x800a3cf0
        virtual ~TList(); // 0x800a42d8
    };

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
    void openChild(u32);                                                       // 0x800a12ec
    void connectBus(int, int);                                                 // 0x800a1820
    void setLatestKey(u8);                                                     // 0x800a18b0
    void noteOn(u32, u32, u32);                                                // 0x800a18ec
    void gateOn(u32, u32, f32, u32);                                           // 0x800a1ac0
    void noteOff(u32, u16);                                                    // 0x800a1cf8
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
    //void setOscTable(u32, const JASOscillator::Point *);                       // 0x800a29d8
    void setOscAdsr(s16, s16, s16, s16, u16);                                  // 0x800a2a74
    void setFIR(const s16 *);                                                  // 0x800a2acc
    void setIIR(const s16 *);                                                  // 0x800a2b2c
    void readPortSelf(u32);                                                    // 0x800a2b80
    void writePortSelf(u32, u16);                                              // 0x800a2ba4
    void writePort(u32, u16);                                                  // 0x800a2bc8
    void readPort(u32);                                                        // 0x800a2c2c
    void pause(bool);                                                          // 0x800a2c50
    void getTransposeTotal() const;                                            // 0x800a2d38
    void isMute() const;                                                       // 0x800a2ea8
    void setTempo(u16);                                                        // 0x800a3028
    void setTempoRate(f32);                                                    // 0x800a30b8
    void setTimebase(u16);                                                     // 0x800a3148
    //void channelUpdateCallback(u32, JASChannel *, JASDsp::TChannel *, void *); // 0x800a31d8
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

    // Data
    static s16 sAdsTable[12];         // 0x80369df0
    //static sEnvOsc;            // 0x80369e08, combination of u32 and f32
    //void sPitchEnvOsc;      // 0x80369e20
    //void sDefaultBankTable; // 0x803fac24
    //void sTrackList;        // 0x803fb070
};

#endif
