#include "JSystem/JAudio/DSP.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASDsp.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASWaveInfo.h"

#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/dsp.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/types.h"
#include "macros.h"

u32 JASWaveInfo::one = 1;

namespace JASDsp {
static TChannel *CH_BUF;
static Fxline *FX_BUF;
static f32 sDSPVolume;

static u16 SEND_TABLE[12] = {
    0x0D00, 0x0D60, 0x0DC0, 0x0E20, 
    0x0E80, 0x0EE0, 0x0CA0, 0x0F40, 
    0x0FA0, 0x0B00, 0x09A0, 0x0000,
};

static const u8 DSPADPCM_FILTER[64] ALIGN(32) = {
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x04, 0x00, 
    0x10, 0x00, 0xF8, 0x00, 0x0E, 0x00, 0xFA, 0x00,
    0x0C, 0x00, 0xFC, 0x00, 0x12, 0x00, 0xF6, 0x00,
    0x10, 0x68, 0xF7, 0x38, 0x12, 0xC0, 0xF7, 0x04,
    0x14, 0x00, 0xF4, 0x00, 0x08, 0x00, 0xF8, 0x00,
    0x04, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x04, 0x00,
    0xFC, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00,
};

static const u32 DSPRES_FILTER[320] ALIGN(32)  = {
    0x0C3966AD, 0x0D46FFDF, 0x0B396696, 0x0E5FFFD8, 
    0x0A446669, 0x0F83FFD0, 0x095A6626, 0x10B4FFC8, 
    0x087D65CD, 0x11F0FFBF, 0x07AB655E, 0x1338FFB6,
    0x06E464D9, 0x148CFFAC, 0x0628643F, 0x15EBFFA1,
    0x0577638F, 0x1756FF96, 0x04D162CB, 0x18CBFF8A,
    0x043561F3, 0x1A4CFF7E, 0x03A46106, 0x1BD7FF71,
    0x031C6007, 0x1D6CFF64, 0x029F5EF5, 0x1F0BFF56,
    0x022A5DD0, 0x20B3FF48, 0x01BE5C9A, 0x2264FF3A,
    0x015B5B53, 0x241EFF2C, 0x010159FC, 0x25E0FF1E,
    0x00AE5896, 0x27A9FF10, 0x00635720, 0x297AFF02,
    0x001F559D, 0x2B50FEF4, 0xFFE2540D, 0x2D2CFEE8,
    0xFFAC5270, 0x2F0DFEDB, 0xFF7C50C7, 0x30F3FED0,
    0xFF534F14, 0x32DCFEC6, 0xFF2E4D57, 0x34C8FEBD,
    0xFF0F4B91, 0x36B6FEB6, 0xFEF549C2, 0x38A5FEB0,
    0xFEDF47ED, 0x3A95FEAC, 0xFECE4611, 0x3C85FEAB,
    0xFEC04430, 0x3E74FEAC, 0xFEB6424A, 0x4060FEAF,
    0xFEAF4060, 0x424AFEB6, 0xFEAC3E74, 0x4430FEC0,
    0xFEAB3C85, 0x4611FECE, 0xFEAC3A95, 0x47EDFEDF,
    0xFEB038A5, 0x49C2FEF5, 0xFEB636B6, 0x4B91FF0F,
    0xFEBD34C8, 0x4D57FF2E, 0xFEC632DC, 0x4F14FF53,
    0xFED030F3, 0x50C7FF7C, 0xFEDB2F0D, 0x5270FFAC,
    0xFEE82D2C, 0x540DFFE2, 0xFEF42B50, 0x559D001F,
    0xFF02297A, 0x57200063, 0xFF1027A9, 0x589600AE,
    0xFF1E25E0, 0x59FC0101, 0xFF2C241E, 0x5B53015B,
    0xFF3A2264, 0x5C9A01BE, 0xFF4820B3, 0x5DD0022A,
    0xFF561F0B, 0x5EF5029F, 0xFF641D6C, 0x6007031C,
    0xFF711BD7, 0x610603A4, 0xFF7E1A4C, 0x61F30435,
    0xFF8A18CB, 0x62CB04D1, 0xFF961756, 0x638F0577,
    0xFFA115EB, 0x643F0628, 0xFFAC148C, 0x64D906E4,
    0xFFB61338, 0x655E07AB, 0xFFBF11F0, 0x65CD087D,
    0xFFC810B4, 0x6626095A, 0xFFD00F83, 0x66690A44,
    0xFFD80E5F, 0x66960B39, 0xFFDF0D46, 0x66AD0C39,
    0x00000C8B, 0x18F82527, 0x30FB3C56, 0x471C5133,
    0x5A8262F1, 0x6A6D70E2, 0x76417A7C, 0x7D897F61,
    0x7FFF7F61, 0x7D897A7C, 0x764170E2, 0x6A6D62F1,
    0x5A825133, 0x471C3C56, 0x30FB2527, 0x18F80C8B,
    0x0000F375, 0xE708DAD9, 0xCF05C3AA, 0xB8E4AECD,
    0xA57E9D0F, 0x95938F1E, 0x89BF8584, 0x8277809F,
    0x8001809F, 0x82778584, 0x89BF8F1E, 0x95939D0F,
    0xA57EAECD, 0xB8E4C3AA, 0xCF05DAD9, 0xE708F375,
    0x000007FF, 0x0FFF17FF, 0x1FFF27FF, 0x2FFF37FF,
    0x3FFF47FF, 0x4FFF57FF, 0x5FFF67FF, 0x6FFF77FF,
    0x7FFF7800, 0x70006800, 0x60005800, 0x50004800,
    0x40003800, 0x30002800, 0x20001800, 0x10000800,
    0x0000F801, 0xF001E801, 0xE001D801, 0xD001C801,
    0xC001B801, 0xB001A801, 0xA0019801, 0x90018801,
    0x80018800, 0x90009800, 0xA000A800, 0xB000B800, // according to objdiff the first entry should be 0x00000000 which is wrong(false positive from dtk?)
    0xC000C800, 0xD000D800, 0xE000E800, 0xF000F800,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x1FFF3FFF,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x1FFFC001,
    0x00000192, 0x032404B6, 0x064707D9, 0x096A0AFB,
    0x0C8B0E1B, 0x0FAB1139, 0x12C71455, 0x15E1176D,
    0x18F81A82, 0x1C0B1D93, 0x1F19209F, 0x222323A6,
    0x252726A7, 0x282629A3, 0x2B1F2C98, 0x2E102F87,
    0x30FB326E, 0x33DE354D, 0x36B93824, 0x398C3AF2,
    0x3C563DB7, 0x3F174073, 0x41CD4325, 0x447A45CC,
    0x471C4869, 0x49B34AFB, 0x4C3F4D81, 0x4EBF4FFB,
    0x51335268, 0x539A54C9, 0x55F5571D, 0x58425964,
    0x5A825B9C, 0x5CB35DC7, 0x5ED75FE3, 0x60EB61F0,
    0x62F163EE, 0x64E865DD, 0x66CF67BC, 0x68A6698B,
    0x6A6D6B4A, 0x6C236CF8, 0x6DC96E96, 0x6F5E7022,
    0x70E2719D, 0x72547307, 0x73B5745F, 0x750475A5,
    0x764176D8, 0x776B77FA, 0x78847909, 0x79897A05,
    0x7A7C7AEE, 0x7B5C7BC5, 0x7C297C88, 0x7CE37D39,
    0x7D897DD5, 0x7E1D7E5F, 0x7E9C7ED5, 0x7F097F37,
    0x7F617F86, 0x7FA67FC1, 0x7FD87FE9, 0x7FF57FFD,
};

void boot(DSPCallback cb) {
    static bool first = true;
    if (first) {
        DspBoot(cb);
        first = false;
    }
}

void releaseHalt(u32 msg) {
    DSPReleaseHalt2(msg);
}

void finishWork(u16 p1) {
    DspFinishWork(p1);
}

void syncFrame(u32 p1, u32 p2, u32 p3) {
    DsyncFrame2(p1, p2, p3);
}

void setDSPMixerLevel(f32 dsp_level) {
    sDSPVolume = dsp_level;
    dsp_level *= 4.0f;
#line 269
    JUT_ASSERT((0.0 <= dsp_level) && (dsp_level <= 8.0));
    DsetMixerLevel(dsp_level);
}

f32 getDSPMixerLevel() {
    return sDSPVolume;
}

TChannel *getDSPHandle(int index) {
    return &CH_BUF[index];
}

TChannel *getDSPHandleNc(int) {
    // UNUSED
}

void setFilterTable(s16 *dst, s16 *src, u32 n) {
    for (int  i = 0; i < n; i++) {
        *dst++ = *src++;
    }
}

void flushBuffer() {
    DCFlushRange(CH_BUF, sizeof(TChannel) * 0x40);
    DCFlushRange(FX_BUF, sizeof(Fxline) * 4);
}

void flushChannelAll() {
    // UNUSED
}

void cacheChannelAll() {
    // UNUSED
}

void invalChannelAll() {
    DCInvalidateRange(CH_BUF, sizeof(TChannel) * 0x40);
}

void initBuffer() {
#line 345
    CH_BUF = new(JASDram, 0x20) TChannel[0x40];
    JUT_ASSERT(CH_BUF);
    FX_BUF = new(JASDram, 0x20) Fxline[4];
    JUT_ASSERT(FX_BUF);
    JASCalc::bzero(CH_BUF, sizeof(TChannel) * 0x40);
    JASCalc::bzero(FX_BUF, sizeof(Fxline) * 4);
    for (u8 i = 0; i < 4; i++) {
        setFXLine(i, NULL, NULL);
    }
    DsetupTable(0x40, u32(CH_BUF), u32(&DSPRES_FILTER), u32(&DSPADPCM_FILTER), u32(FX_BUF));
    flushBuffer();
}

Fxline &getFXHandle(u8) {
    // UNUSED
}

Fxline &getFXHandleNc(u8) {
    // UNUSED
}

bool setFXLine(u8 p1, s16 *buffer, FxlineConfig_ *config) {
    Fxline *fxLine = FX_BUF + p1;
    JAS_CS_START
    fxLine->_00 = 0;
    if (config != NULL) {
        fxLine->_0A = config->_04;
        fxLine->_08 = SEND_TABLE[config->_02];
        fxLine->_0E = config->_08;
        fxLine->_0C = SEND_TABLE[config->_06];
        fxLine->_02 = config->mBufCount;
        setFilterTable(fxLine->mFilterTable, config->mFilterTable, 8);
    }
    if (buffer != NULL && config != NULL) {
        u32 bufsize = config->mBufCount * 0xa0;
        fxLine->_04 = buffer;
        JASCalc::bzero(buffer, bufsize);
#line 412
        JUT_ASSERT((reinterpret_cast<u32>(buffer) & 0x1f) == 0);
        JUT_ASSERT((bufsize & 0x1f) == 0)
        DCFlushRange(buffer, bufsize);
    } else if (config == NULL || buffer != NULL) {
        fxLine->_04 = buffer;
    }

    if (fxLine->_04 != NULL) {
        fxLine->_00 = config->_00;
    } else {
        fxLine->_00 = 0;
    }
    DCFlushRange(fxLine, sizeof(Fxline));
    JAS_CS_END
    return 1;
}

void changeFXLineParam(u8, u8, u32) {
    // UNUSED
}

void TChannel::init() {
    mPauseFlag = 0;
    mIsFinished = 0;
    mForcedStop = 0;
    mIsActive = 0;
    _58 = 0;
    _68 = 0;
    initFilter();
}

void TChannel::playStart() {
    _10C = 0;
    _60 = 0;
    _08 = 1;
    _66 = 0;

    for (int i = 0; i < 4l; i++) {
        _78[i] = 0;
        _A8[i] = 0;
    }

    for (int i = 0; i < 0x14l; i++) {
        _80[i] = 0;
    }
    mIsActive = 1;
}

void TChannel::playStop() {
    mIsActive = 0;
}

void TChannel::replyFinishRequest() {
    mIsFinished = 0;
    mIsActive = 0;
}

void TChannel::forceStop() {
    mForcedStop = 1;
}

bool TChannel::isActive() const {
    return mIsActive != 0;
}

bool TChannel::isFinish() const {
    return mIsFinished != 0;
}

u16 TChannel::getBlockCounter() const {
    // UNUSED
}

u16 TChannel::getRemainSamples() const {
    // UNUSED
}

void TChannel::setWaveInfo(const JASWaveInfo &waveInfo, u32 p2, u32 p3) {
    _118 = p2;
    static const u8 COMP_BLOCKSAMPLES[8] = {0x10, 0x10, 0x01, 0x01, 0x01, 0x10, 0x10, 0x01};
    _64 = COMP_BLOCKSAMPLES[waveInfo._00];
    static const u8 COMP_BLOCKBYTES[8] = {0x09, 0x05, 0x08, 0x10, 0x01, 0x01, 0x01, 0x01};
    _100 = COMP_BLOCKBYTES[waveInfo._00];
    _68 = 0;
    if (_100 >= 4) {
        _11C = waveInfo._1c;
        _102 = waveInfo._10;
        if (_102) {
            if (p3 == 1) {
                p3 = waveInfo._14;
            }
            _110 = waveInfo._14;
            _114 = waveInfo._18;
            _104 = waveInfo._20;
            _106 = waveInfo._22;
        } else {
            _114 = _11C;
        }
        if (p3 && _114 > p3) {
            switch (waveInfo._00) {
            case 0:
            case 1:
                _68 = p3;
                _118 += p3 * _100 >> 4;
                _110 -= p3;
                _114 -= p3;
                break;
            case 2:
            case 3:
                _68 = p3;
                break;
            }
        }
        for (int i = 0; i < 0x10l; i++) {
            _B0[i] = 0;
        }
    }
}

void TChannel::setOscInfo(u32 p1) {
    _118 = 0;
    _64 = 0x10;
    _100 = p1;
}

void TChannel::initAutoMixer() {
    if (_58) {
        _54 = _56;
    }
    else {
        _54 = 0;
        _58 = 1;
    }
}

void TChannel::setAutoMixer(u16 p1, u8 p2, u8 p3, u8 p4, u8 p5) {
    _50 = (p2 << 8) | p3;
    _52 = (p4 << 8) | (p4 << 1); // should've been p5 i guess?
    _56 = p1;
    _58 = 1;
}

void TChannel::updateAMVolume(u16) {
    // UNUSED
}

void TChannel::updateAMPan(u8, u8) {
    // UNUSED
}

void TChannel::updateAMFX(u8) {

}

void TChannel::setPitch(u16 pitch) {
    if (pitch >= 0x7fff) {
        pitch = 0x7fff;
    }
    mPitch = pitch;
}

void TChannel::setMixerInitDelayMax(u8) {
    // UNUSED
}

void TChannel::setMixerInitVolume(u8 index, s16 p2) {
    Mixer *mixer = &_10[index];
    mixer->_04 = p2;
    mixer->_02 = p2;
    mixer->_06 = 0;
}

void TChannel::setMixerInitDelaySamples(u8, u8) {
    // UNUSED
}

void TChannel::setMixerDelaySamples(u8, u8) {
    // UNUSED
}

void TChannel::setMixerVolume(u8 index, s16 p2) {
    if (mForcedStop == 0) {
        Mixer *mixer = &_10[index];
        mixer->_02 = p2;
        mixer->_06 &= 0xff;
    }   
}

void TChannel::setPauseFlag(u8 flag) {
    mPauseFlag = flag;
}

void TChannel::flush() {
    DCFlushRange(this, sizeof(TChannel));
}

void TChannel::initFilter() {
    for (int i = 0; i < 8; i++) {
        mFir8FilterParam[i] = 0;
    }
    mFir8FilterParam[0] = 0x7fff;
    for (int i = 0; i < 8; i++) {
        mIirFilterParam[i] = 0;
    }
    mIirFilterParam[0] = 0x7fff;
    mIirFilterParam[4] = 0; // clueless
}

void TChannel::setFilterMode(u16 mode) {
    u8 r30 = mode & 0x20;
    u8 r31 = mode & 0x1f;
    if (r30) {
        if (r31 > 0x14) {
            r31 = 0x14;
        }
    } else {
        if (r31 > 0x18) {
            r31 = 0x18;
        }
    }
    mFilterMode = r30 + r31;    
}

void TChannel::setIIRFilterParam(s16 *src) {
    setFilterTable(mIirFilterParam, src, 8);
}

void TChannel::setFIR8FilterParam(s16 *src) {
    setFilterTable(mFir8FilterParam, src, 8);
}

void TChannel::setDistFilter(s16 filter) {
    mIirFilterParam[4] = filter;
}

void TChannel::setBusConnect(u8 idx, u8 tableIdx) {
    Mixer *mixer = &_10[idx];
    static u16 const connect_table[12] = {
        0x0000, 0x0D00, 0x0D60, 0x0DC0, 
        0x0E20, 0x0E80, 0x0EE0, 0x0CA0, 
        0x0F40, 0x0FA0, 0x0B00, 0x09A0,
    };
    mixer->busConnect = connect_table[tableIdx];
}
}

u16 DSP_CreateMap2(u32 p1) {
    u16 ret = 0;
    JASDsp::TChannel *channel = &JASDsp::CH_BUF[p1 << 4];
    for (int i = 0; i < 16; i++) {
        ret <<= 1;
        if (channel->isActive()) {
            ret |= 1;
        }
        channel++;
    }
    return ret;
}

void DSP_CreateMap() {
    // UNUSED
}
