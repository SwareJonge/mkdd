#include "JSystem/JAudio/System/JASAramStream.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASDvd.h"
#include "JSystem/JAudio/System/JASTaskThread.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRCompression.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/dvd/dvdfs.h"

#ifdef MATCHING
template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;
#endif

JASTaskThread *JASAramStream::sLoadThread;
u8 *JASAramStream::sReadBuffer;
u32 JASAramStream::sBlockSize;
u32 JASAramStream::sChannelMax;
bool JASAramStream::sSystemPauseFlag;
bool JASAramStream::sFatalErrorFlag;

static const JASOscillator::Point OSC_RELEASE_TABLE[2] = {
    {0x0000, 0x0002, 0x0000},
    {0x000F, 0x0000, 0x0000},
};

static const JASOscillator::Data OSC_ENV = {
    0, 1.0f, NULL, OSC_RELEASE_TABLE, 1.0f, 0.0f
};

void JASAramStream::initSystem(u32 block_size, u32 channel_max) {
#line 66
    JUT_ASSERT(block_size % 32 == 0);
    JUT_ASSERT(block_size % 9 == 0);
    JUT_ASSERT(channel_max > 0 && channel_max <= CHANNEL_MAX);
    JUT_ASSERT(sReadBuffer == 0);

    if (!JASDriver::registerSubFrameCallback(dvdErrorCheck, NULL)) {
        JUT_WARNING_F2("%s", "registerSubFrameCallback Failed");
        return;
    }
    if (sLoadThread == NULL) {
        sLoadThread = JASDvd::getThreadPointer();
    }
    sReadBuffer = new (JASDram, 0x20) u8[(block_size + 0x20) * channel_max];
    JUT_ASSERT(sReadBuffer);
    sBlockSize = block_size;
    sChannelMax = channel_max;
    sSystemPauseFlag = false;
    sFatalErrorFlag = false;
}

void JASAramStream::setLoadThread(JASTaskThread *taskThread) {
    // UNUSED
}

JASAramStream::JASAramStream() {
    _0a8 = NULL;
    _0ac = false;
    _0ad = false;
    _0ae = 0;
    _0b0 = 0;
    _0b4 = 0;
    mReadSample = 0;
    _0bc = 0;
    _0c0 = false;
    _0c4 = 0;
    _0c8 = 0.0f;
    mAramBlockCount = 0;
    mAramBlock = 0;
    mBlock = 0;
    mCancelled = 0;
    mFreeBlockCount = 0;
    _12c = 0;
    _148 = 0;
    mAramBufSize = 0;
    mCb = NULL;
    mCbData = NULL;
    mFormat = 0;
    mChannelNum = 0;
    mBufCount = 0;
    mBlockCount = 0;
    mSampleRate = 0;
    mLoop = false;
    mLoopStart = 0;
    mLoopEnd = 0;
    mVolume = 1.0f;
    mPitch = 1.0f;
    for (int i = 0; i < 6; i++) {
        mChannels[i] = NULL;
        _130[i] = 0;
        _13c[i] = 0;
        mChannelVolume[i] = 1.0f;
        mChannelPan[i] = 0.5f;
        mChannelFxMix[i] = 0.0f;
        mChannelDolby[i] = 0.0f;
    }
    for (int i = 0; i < 6; i++) {
        _1dc[i] = 0;
    }
}

void JASAramStream::init(u32 aramAddr, u32 aramSize, StreamCallback cb, void *cbData) {
#line 153
    JUT_ASSERT(sReadBuffer != 0);
    _148 = aramAddr;
    mAramBufSize = aramSize;
    _0c8 = 0.0f;
    _0ae = 0;
    _0ac = false;
    _0ad = false;
    mCancelled = 0;
    mChannelNum = 0;
    for (int i = 0; i < 6; i++) {
        mChannelVolume[i] = 1.0f;
        mChannelPan[i] = 0.5f;
        mChannelFxMix[i] = 0.0f;
        mChannelDolby[i] = 0.0f;
    }
    mVolume = 1.0f;
    mPitch = 1.0f;
    _1dc[0] = 0xffff;
    mCb = cb;
    mCbData = cbData;
    OSInitMessageQueue(&_000, _040, 0x10);
    OSInitMessageQueue(&_020, _080, 4);
}

void JASAramStream::setBusSetting(u32 line, u16 bus) {
    // UNUSED
    JUT_ASSERT(line < JASChannel::BUSOUT_CPUCH);
}

bool JASAramStream::prepare(const char *fileName, int p2) {
    // UNUSED, size mismatch, not sure if this called DVDPathToEntryNum instead
    if (!DVDOpen(fileName, &mDvdFileInfo)) {
        JUT_WARNING_F2("Cannot open file %s", fileName);
        return false;
    }

    return true;
}

bool JASAramStream::prepare(s32 entryNum, int p2) {
    if (!DVDFastOpen(entryNum, &mDvdFileInfo)) {
#line 240
        JUT_WARNING_F2("%s", "DVDFastOpen Failed");
        return false;
    }

    if (!JASDriver::registerSubFrameCallback(channelProcCallback, this)) {
        JUT_WARNING_F2("%s", "registerSubFrameCallback Failed");
        return false;
    }

    TaskData data;
    data.stream = this;
    data._4 = mAramBufSize;
    data._8 = p2;
    if (!sLoadThread->sendCmdMsg(headerLoadTask, &data, 0xc)) {
        JUT_WARNING_F2("%s", "sendCmdMsg headerLoadTask Failed");
        JASDriver::rejectCallback(channelProcCallback, this);
        return false;
    }
    return true;
}

bool JASAramStream::start() {
    if (!OSSendMessage(&_000, (OSMessage)0, OS_MESSAGE_NOBLOCK)) {
#line 273
        JUT_WARNING_F2("%s", "OSSendMessage Failed");
        return false;
    }
    return true;
}

bool JASAramStream::stop(u16 p1) {
    if (!OSSendMessage(&_000, (OSMessage)(p1 << 0x10 | 1), OS_MESSAGE_NOBLOCK)) {
#line 290
        JUT_WARNING_F2("%s", "OSSendMessage Failed");
        return false;
    }
    return true;
}

bool JASAramStream::pause(bool p1) {
    OSMessage msg;
    if (p1) {
        msg = (OSMessage)2;
    } else {
        msg = (OSMessage)3;
    }
    if (!OSSendMessage(&_000, msg, OS_MESSAGE_NOBLOCK)) {
#line 308
        JUT_WARNING_F2("%s", "OSSendMessage Failed");
        return false;
    }
    return true;
}

bool JASAramStream::cancel() {
    mCancelled = 1;
    if (!sLoadThread->sendCmdMsg(finishTask, this)) {
#line 326
        JUT_WARNING_F2("%s", "sendCmdMsg finishTask Failed");
        return false;
    }
    return true;
}

u32 JASAramStream::getBlockSamples() const {
    if (mFormat == 0) {
        return (sBlockSize << 4) / 9;
    }
    return sBlockSize >> 1;
}

void JASAramStream::headerLoadTask(void *arg) {
    TaskData *data = (TaskData*)arg;
    data->stream->headerLoad(data->_4, data->_8);
}

void JASAramStream::firstLoadTask(void *arg) {
    TaskData *data = (TaskData*)arg;
    JASAramStream *stream = data->stream;
    if (!stream->load()) {
        return;
    }
    if (data->_8 > 0) {
        data->_8--;
        if (data->_8 == 0 && !sLoadThread->sendCmdMsg(prepareFinishTask, stream)) {
#line 364
            JUT_WARNING_F2("%s", "sendCmdMsg prepareFinishTask Failed");
            sFatalErrorFlag = true;
        }
    }

    if (data->_4 != 0) {
        data->_4--;
        if (!sLoadThread->sendCmdMsg(firstLoadTask, data, 0xc)) {
            JUT_WARNING_F2("%s", "sendCmdMsg firstLoadTask Failed");
            sFatalErrorFlag = true;
        }

        JAS_CS_START
        stream->mFreeBlockCount++;
        JAS_CS_END
    }
}

void JASAramStream::loadToAramTask(void *arg) {
    ((JASAramStream*)arg)->load();
}

void JASAramStream::finishTask(void *arg) {
    JASAramStream *stream = (JASAramStream*)arg;
    if (!JASDriver::rejectCallback(channelProcCallback, stream)) {
#line 392
        JUT_WARNING_F2("%s", "rejectSubFrameCallback Failed");
    }
    if (stream->mCb) {
        stream->mCb(CB_START, stream, stream->mCbData);
        stream->mCb = NULL;
    }
}

void JASAramStream::prepareFinishTask(void *arg) {
    JASAramStream *stream = (JASAramStream*)arg;
    OSSendMessage(&stream->_020, (OSMessage)4, OS_MESSAGE_BLOCK);
    if (stream->mCb) {
        stream->mCb(CB_STOP, stream, stream->mCbData);
    }
}

bool JASAramStream::headerLoad(u32 p1, int p2) {
    if (sFatalErrorFlag) {
        return false;
    }
    if (mCancelled) {
        return false;
    }
    if (DVDReadPrio(&mDvdFileInfo, sReadBuffer, sizeof(Header), 0, 1) < 0) {
#line 420
        JUT_WARNING_F2("%s", "DVDReadPrio Failed");
        sFatalErrorFlag = true;
        return false;
    }

    Header *header = (Header*)sReadBuffer;
    JUT_ASSERT(header->tag == 'STRM');
    JUT_ASSERT(header->format <= 1);
    JUT_ASSERT(header->bits == 16);
    JUT_ASSERT(header->channels <= sChannelMax);
    JUT_ASSERT(header->block_size == sBlockSize);
    mFormat = header->format;
    mChannelNum = header->channels;
    mSampleRate = header->_10;
    mLoop = header->loop != 0;
    mLoopStart = header->loop_start;
    mLoopEnd = header->loop_end;
    mVolume = header->_28 / 127.0f;
    mFreeBlockCount = 0;
    mBlock = 0;
    mAramBlock = 0;
    mBlockCount = (p1 / sBlockSize) / header->channels;
    mBufCount = mBlockCount;
    //JUT_ASSERT(mBufCount > 0);
    mBufCount--;
    if (mBufCount < 3) {
#line 448
        JUT_WARNING_F2("%s", "Too few Buffer-Size");
    }
    mAramBlockCount = mBufCount;
    u32 samples = (mLoopEnd - 1U) / getBlockSamples();
    if (samples <= mBufCount && mLoop) {
#line 457
        JUT_WARNING_F2("%s", "Too few samples for Loop-buffer");
    }
    if (p2 < 0 || p2 > mAramBlockCount) {
        p2 = mAramBlockCount;
    }
    if (mCancelled != 0) {
        return false;
    }
    TaskData data;
    data.stream = this;
    data._4 = mAramBlockCount - 1;
    data._8 = p2;
    if (!sLoadThread->sendCmdMsg(firstLoadTask, &data, 0xc)) {
#line 472
        JUT_WARNING_F2("%s", "sendCmdMsg firstLoadTask Failed");
        sFatalErrorFlag = true;
        return false;
    }
    JAS_CS_START
    mFreeBlockCount++;
    JAS_CS_END
    return true;
}

bool JASAramStream::load() {
    JAS_CS_START
    mFreeBlockCount--;
    JAS_CS_END
    
    if (sFatalErrorFlag) {
        return false;
    }
    if (mCancelled != 0) {
        return false;
    }
    u32 loop_end_block = (mLoopEnd - 1) / getBlockSamples();
    u32 loop_start_block = mLoopStart / getBlockSamples();
    u32 block = mBlock;
    if (block > loop_end_block) {
        return false;
    }
    u32 size = sBlockSize * mChannelNum + sizeof(BlockHeader);
    u32 offset = block * size + sizeof(Header);
    u32 size2 = size;
    if (block == loop_end_block) {
        size2 = mDvdFileInfo.length - offset;
    }
    if (DVDReadPrio(&mDvdFileInfo, sReadBuffer, size2, offset, 1) < 0) {
#line 506
        JUT_WARNING_F2("%s", "DVDReadPrio Failed");
        sFatalErrorFlag = true;
        return false;
    }
    BlockHeader *bhead = (BlockHeader*)sReadBuffer;
    JUT_ASSERT(bhead->tag == 'BLCK');
    if (mCancelled != 0) {
        return false;
    }

    u32 uvar2 = _148 + mAramBlock * sBlockSize;
    for (int i = 0; i < mChannelNum; i++) {
        if (!JKRMainRamToAram(sReadBuffer + bhead->_4 * i + sizeof(BlockHeader),
                                    uvar2 + sBlockSize * mBlockCount * i,
                                    bhead->_4, EXPAND_SWITCH_DEFAULT, 0, NULL, -1, NULL)) {
            JUT_WARNING_F2("%s", "JKRMainRamToAram Failed");
            sFatalErrorFlag = true;
            return false;
        }
    }
    mAramBlock++;
    if (mAramBlock >= mAramBlockCount) {
        u32 block = mBlock;
        block += mAramBlockCount - 1;
        if (mLoop) {
#line 536
            JUT_ASSERT(loop_start_block < loop_end_block);
            while (block > loop_end_block) {
                block -= loop_end_block;
                block += loop_start_block;
            }
        }
        if (block == loop_end_block || block + 2 == loop_end_block) {
            mAramBlockCount = mBlockCount;
            OSSendMessage(&_020, (OSMessage)5, OS_MESSAGE_BLOCK);
        } else {
            mAramBlockCount = mBlockCount - 1;
        }
        for (int i = 0; i < mChannelNum; i++) {
            _130[i] = bhead->_8[i]._0;
            _13c[i] = bhead->_8[i]._2;
        }
        mAramBlock = 0;
    }
    mBlock++;
    if (mBlock > loop_end_block && mLoop) {
        mBlock = loop_start_block;
    }
    return true;
}

s32 JASAramStream::channelProcCallback(void *stream) {
    return ((JASAramStream*)stream)->channelProc();
}

s32 JASAramStream::dvdErrorCheck(void *) {
    switch (DVDGetDriveStatus()) {
    case DVD_STATE_END:
        sSystemPauseFlag = false;
        break;
    case DVD_STATE_BUSY:
        break;
    case DVD_STATE_WAITING:
    case DVD_STATE_COVER_CLOSED:
    case DVD_STATE_NO_DISK:
    case DVD_STATE_WRONG_DISK:
    case DVD_STATE_MOTOR_STOPPED:
    case DVD_STATE_IGNORED:
    case DVD_STATE_CANCELED:
    case DVD_STATE_RETRY:
    case DVD_STATE_FATAL_ERROR:
    default:
        sSystemPauseFlag = true;
        break;
    }
    return 0;
}

void JASAramStream::channelCallback(u32 callbackType, JASChannel *channel, JASDsp::TChannel *dspChannel, void *arg) {
     ((JASAramStream*)arg)->updateChannel(callbackType, channel, dspChannel);
}

void JASAramStream::updateChannel(u32 callbackType, JASChannel *channel, JASDsp::TChannel *dspChannel) {
    u32 block_samples = getBlockSamples();
    switch (callbackType) {
    case JASChannel::CB_START:
        if (_0a8 == NULL) {
            _0a8 = channel;
            _0b4 = block_samples * mBufCount;
            mReadSample = 0;
            _0b0 = 0;
            _0bc = (mLoopEnd - 1) / block_samples;
            _0c0 = 0;
            _0c4 = 0;
            _12c = 0;
        }
        break;
    case JASChannel::CB_PLAY:
        if (dspChannel->_08 == 0) {
            if (channel == _0a8) {
                _12c = 0;
                u32 uvar8 = dspChannel->_74 + dspChannel->_64;
                u32 waveCnt = _0b4;
                if (uvar8 <= waveCnt) {
                    mReadSample += waveCnt - uvar8;
                } else {
                    if (!_0c0) {
                        mReadSample += waveCnt;
                        mReadSample += block_samples * mBufCount - uvar8;
                    } else {
                        mReadSample += waveCnt;
                        mReadSample += block_samples * mBufCount - uvar8
                                       - dspChannel->_110;
                        mReadSample -= mLoopEnd;
                        mReadSample += mLoopStart;
                        dspChannel->_110 = 0;
                        _120 = 0;
                        _12c |= 2;
                        if (_0c4 < 0xffffffff) {
                            _0c4++;
                        }
                        _0c0 = false;
                    }
                }
                if (mReadSample > mLoopEnd) {
#line 685
                    JUT_WARNING_F2("%s", "mReadSample > mLoopEnd")
                    sFatalErrorFlag = true;
                }
                f32 fvar1 = _0c4;
                fvar1 *= mLoopEnd - mLoopStart;
                if (_0c4 < 0xffffffff) {
                    fvar1 += mReadSample;
                }
                fvar1 /= mSampleRate;
                _0c8 = fvar1;
                if (mReadSample + 400 >= mLoopEnd && !_0c0) {
                    if (mLoop) {
                        u32 waveCnt = _0bc + 1;
                        if (waveCnt >= mBufCount) {
                            waveCnt = 0;
                        }
                        dspChannel->_110 = mLoopStart % block_samples
                                                    + waveCnt * block_samples;
                        _120 = dspChannel->_110;
                        _12c |= 2;
                    } else {
                        dspChannel->_102 = 0;
                        _128 = 0;
                        _12c |= 8;
                    }
                    dspChannel->_74 -= block_samples * mBufCount
                                        - (_0bc * block_samples + mLoopEnd % block_samples);
                    _11c = dspChannel->_74;
                    _12c |= 1;
                    _0bc += (mLoopEnd - 1) / block_samples - mLoopStart / block_samples + 1;
                    _0c0 = true;
                }
                u32 waveSize = dspChannel->_70 - channel->mWaveData.mPtr;
                if (waveSize != 0) {
                    waveSize--;
                }
                waveCnt = waveSize / sBlockSize;
                getBlockSamples();
                if (waveCnt != _0b0) {
                    bool cmp = waveCnt < _0b0;
                    while (waveCnt != _0b0) {
                        if (!sLoadThread->sendCmdMsg(loadToAramTask, this)) {
#line 740
                            JUT_WARNING_F2("sendCmdMsg Failed %d %d (%d %d)", dspChannel->_70, channel->mWaveData.mPtr, waveCnt, _0b0);
                            sFatalErrorFlag = true;
                            break;
                        }
                        
                        JAS_CS_START
                        mFreeBlockCount++;
                        JAS_CS_END
                        _0b0++;
                        if (_0b0 >= mBufCount) {
                            _0b0 = 0;
                        }
                    }
                    if (cmp) {
                        _0bc -= mBufCount;
                        if (_0ad) {
                            if (!_0c0) {
                                dspChannel->_74 += block_samples;
                                _11c = dspChannel->_74;
                                _12c |= 1;
                            }
                            dspChannel->_114 += block_samples;
                            _124 = dspChannel->_114;
                            _12c |= 4;
                            mBufCount = mBlockCount;
                            _0ad = false;
                        } else {
                            if (mBufCount != mBlockCount - 1) {
                                mBufCount = mBlockCount - 1;
                                dspChannel->_114 -= block_samples;
                                _124 = dspChannel->_114;
                                _12c |= 4;
                                if (!_0c0) {
                                    dspChannel->_74 -= block_samples;
                                    _11c = dspChannel->_74;
                                    _12c |= 1;
                                }
                            }
                        }
                    }
                } else {
                    if (mFreeBlockCount == 0 && !sSystemPauseFlag) {
                        _0ae &= ~2;
                        _0ae &= ~4;
                    }
                }
                _0b4 = dspChannel->_74 + dspChannel->_64;
                if (mFreeBlockCount >= mBlockCount - 2) {
#line 809
                    JUT_WARNING_F1("%s", "buffer under error")
                    _0ae |= 4;
                }
            } else {
                if (_12c & 1) {
                    dspChannel->_74 = _11c;
                }
                if (_12c & 2) {
                    dspChannel->_110 = _120;
                }
                if (_12c & 4) {
                    dspChannel->_114 = _124;
                }
                if (_12c & 8) {
                    dspChannel->_102 = _128;
                }
            }
            int ch = 0;
            for (; ch < 6l; ch++) {
                if (channel == mChannels[ch]) {
                    break;
                }
            }
#line 833
            JUT_ASSERT(ch < CHANNEL_MAX);
            dspChannel->_104 = _130[ch];
            dspChannel->_106 = _13c[ch];
        }
        break;
    case JASChannel::CB_STOP:
        bool open_channel = false;
        for (int i = 0; i < 6; i++) {
            if (channel == mChannels[i]) {
                mChannels[i] = NULL;
            } else if (mChannels[i] != NULL) {
                open_channel = true;
            }
        }
        if (!open_channel) {
            mCancelled = 1;
            if (!sLoadThread->sendCmdMsg(finishTask, this)) {
#line 853
                JUT_WARNING_F2("%s", "sendCmdMsg finishTask Failed");
                sFatalErrorFlag = true;
                return;
            }
        }
        break;
    }
    channel->mPauseFlag = _0ae != 0;
}

s32 JASAramStream::channelProc() {
    OSMessage msg;
    while (OSReceiveMessage(&_020, &msg, OS_MESSAGE_NOBLOCK)) {
        switch ((u32)msg) {
        case 4:
            _0ac = true;
            break;
        case 5:
            _0ad = true;
            break;
        }
    }
    
    if (!_0ac) {
        return 0;
    }

    while (OSReceiveMessage(&_000, &msg, OS_MESSAGE_NOBLOCK)) {
        switch ((u32)msg & 0xff) {
        case 0:
            channelStart();
            break;
        case 1:
            channelStop((u32)msg >> 0x10);
            break;
        case 2:
            _0ae |= 1;
            break;
        case 3:
            _0ae &= ~1;
            break;
        }
    }

    if (sFatalErrorFlag) {
        _0ae |= 8;
    }
    if (sSystemPauseFlag) {
        _0ae |= 2;
    }

    for (int i = 0; i < mChannelNum; i++) {
        JASChannel *channel = mChannels[i];
        if (channel != NULL) {
            JASChannelParams params;
            params.mVolume = mVolume * mChannelVolume[i];
            params.mPitch = mPitch;
            params._8 = 0.0f;
            params.mPan = mChannelPan[i];
            params.mFxMix = mChannelFxMix[i];
            params.mDolby = mChannelDolby[i];
            channel->setParams(params);
        }
    }
    
    return 0;
}

void JASAramStream::channelStart() {
    u8 fmt;
    switch (mFormat) {
    case 0:
        fmt = 0;
        break;
    case 1:
        fmt = 3;
        break;
    }
    for (int i = 0; i < mChannelNum; i++) {
        JASWaveInfo wave_info;
        wave_info._00 = fmt;
        wave_info._01 = 60;
        wave_info._10 = -1;
        wave_info._14 = 0;
        wave_info._18 = mBufCount * getBlockSamples();
        wave_info._1c = wave_info._18;
        wave_info._20 = 0;
        wave_info._22 = 0;
        static u32 const one = 1;
        wave_info._24 = &one;
        
        JASChannel *jc = new JASChannel(channelCallback, this);
#line 958
        JUT_ASSERT(jc);
        jc->setPriority(0x7f7f);
        for (u32 j = 0; j < 6; j++) {
            jc->setMixConfig(j, _1dc[j]);
        }
        jc->setInitPitch(mSampleRate / JASDriver::getDacRate());
        jc->setOscInit(0, &OSC_ENV);
        jc->mWaveData.mInfo = wave_info;
        jc->mWaveData.mPtr = _148 + sBlockSize * mBlockCount * i;
        jc->mWaveData._0 = 0;
        int ret = jc->playForce();
#line 972
        JUT_ASSERT(ret);
#line 974
        JUT_ASSERT_MSG(!mChannels[i], "channelStart for already playing channel");
        mChannels[i] = jc;
    }
    _0a8 = NULL;
}

void JASAramStream::channelStop(u16 directRelease) {
    for (int i = 0; i < mChannelNum; i++) {
        if (mChannels[i] != NULL) {
            mChannels[i]->release(directRelease);
        }
    }
}
