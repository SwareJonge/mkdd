#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASDsp.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JUtility/JUTAssert.h"

JASDSPChannel *JASDSPChannel::sDspChannels;

JASDSPChannel::JASDSPChannel() {
    mStatus = 1;
    mPriority = -1;
    mFlags = 0;
    _0C = 0;
    mCb = NULL;
    mCbData = NULL;
    mHandle = NULL;
}

void JASDSPChannel::free() {
    mCb = NULL;
    mCbData = NULL;
}

void JASDSPChannel::start() {
    mFlags |= 1;
}

void JASDSPChannel::drop() {
    if (mCb) {
        mCb(3, NULL, mCbData);
    }
    mCb = NULL;
    mCbData = NULL;
    mPriority = -1;
    mFlags &= ~1;
    if (mStatus == STATUS_ACTIVE) {
        mFlags |= 2;
        mStatus = 2;
    }
}

void JASDSPChannel::initAll() {
#line 101
    sDspChannels = new (JASDram, 0x20) JASDSPChannel[0x40];
    JUT_ASSERT(sDspChannels);

    for (int i = 0; i < 0x40; i++) {
        sDspChannels[i].mHandle = JASDsp::getDSPHandle(i);
    }
}

JASDSPChannel *JASDSPChannel::alloc(u8 prio, JASDSPChannel::Callback cb, void *data) {
    JASDSPChannel *channel = getLowestChannel(prio);
    if (!channel) {
        return NULL;
    }

    channel->drop();
    channel->mPriority = prio;
    channel->_0C = 0;
    channel->mCb = cb;
    channel->mCbData = data;
    return channel;
}

JASDSPChannel *JASDSPChannel::allocForce(u8 prio, JASDSPChannel::Callback cb, void *data) {
    JASDSPChannel *channel = getLowestChannel(prio);
    if (!channel) {
        return NULL;
    }

    channel->mStatus = 1;
    channel->drop();
    channel->mPriority = prio;
    channel->_0C = 0;
    channel->mCb = cb;
    channel->mCbData = data;
    return channel;
}

void JASDSPChannel::setPriority(u8 prio) {
    mPriority = prio;
}

JASDSPChannel *JASDSPChannel::getLowestChannel(int i_prio) {
    s16 best_prio= 0xff;
    int best_idx = -1;
    int best_unk = 0;
    for (int i = 0; i < 0x40; i++) {
        JASDSPChannel *channel = &sDspChannels[i];
        s16 prio = channel->mPriority;
        if (prio < 0) {
            return &sDspChannels[i];
        }
        if (prio <= i_prio && prio <= best_prio) {
            if (prio != best_prio || channel->_0C > best_unk) {
                best_unk = channel->_0C;
                best_idx = i;
                best_prio = prio;                
            }
        }
    }
    if (best_idx < 0) {
        return NULL;
    }
    return &sDspChannels[best_idx];
}

JASDSPChannel *JASDSPChannel::getLowestActiveChannel() {
    s16 best_prio = 0xff;
    int best_idx = -1;
    u32 best_unk = 0;
    for (int i = 0; i < 0x40; i++) {
        JASDSPChannel *channel = &sDspChannels[i];
        if (channel->mStatus == 0) {
            s16 prio = channel->mPriority;
            if (prio < 0x7f && prio <= best_prio && (prio != best_prio || channel->_0C > best_unk)) {
                best_unk = channel->_0C;
                best_idx = i;
                best_prio = prio;
            }
        }
    }

    if (best_idx < 0) {
        return NULL;
    }
    return &sDspChannels[best_idx];
}

void JASDSPChannel::updateProc() {
    int ret;
    if (mHandle->isFinish()) {
        mFlags &= ~2;
        if (mStatus == STATUS_ACTIVE) {
            if (mCb != NULL) {
                ret = mCb(CB_STOP, NULL, mCbData);
            } else {
                ret = -1;
            }
            if (ret < 0) {
                mPriority = -1;
            }
        }
        mStatus = STATUS_INACTIVE;
        mHandle->replyFinishRequest();
        mHandle->flush();
    } else if (mFlags & 2) {
        mFlags &= ~2;
        mHandle->forceStop();
        mHandle->flush();
    } else if (mStatus != STATUS_RELEASE) {
        if ((mFlags & 1) && mStatus == STATUS_INACTIVE) {
            mFlags &= ~1;
            mStatus = STATUS_ACTIVE;
            mHandle->init();
            if (mCb != NULL) {
                ret = mCb(CB_START, mHandle, mCbData);
            } else {
                ret = 0;
            }
            if (ret < 0) {
                mStatus = STATUS_INACTIVE;
                if (mCb != NULL) {
                    ret = mCb(CB_STOP, NULL, mCbData);
                } else {
                    ret = -1;
                }
                if (ret < 0) {
                    mPriority = -1;
                }
                mHandle->flush();
            } else {
                mHandle->playStart();
                mHandle->flush();
            }
        } else if (mStatus != STATUS_INACTIVE) {
            bool flush = false;
            if (mCb != NULL) {
                ret = mCb(CB_PLAY, mHandle, mCbData);
                flush = true;
            } else {
                ret = 0;
            }
            if (ret < 0) {
                mStatus = STATUS_INACTIVE;
                if (mCb != NULL) {
                    ret = mCb(CB_STOP, NULL, mCbData);
                } else {
                    ret = -1;
                }
                if (ret < 0) {
                    mPriority = -1;
                }
                mHandle->playStop();
                mHandle->flush();
            } else {
                _0C++;
                if (flush) {
                    mHandle->flush();
                }
            }
        }
    }
}

void JASDSPChannel::updateAll() {
    for (u32 i = 0; i < 0x40; i++) {
        if ((i & 0xf) == 0 && i != 0) {
            JASDsp::releaseHalt((i - 1) >> 4);
        }
        sDspChannels[i].updateProc();
    }
    JASDsp::releaseHalt(3);
}

bool JASDSPChannel::killActiveChannel() {
    JASDSPChannel *channel = getLowestActiveChannel();
    if (!channel) {
        return false;
    }
    channel->drop();
    return true;
}

JASDSPChannel *JASDSPChannel::getHandle(u32 index) {
    return &sDspChannels[index];
}

int JASDSPChannel::getNumUse() {}

int JASDSPChannel::getNumFree() {}

int JASDSPChannel::getNumBreak() {}

u16 JASDSPChannel::getBlockCounter() const {}

u16 JASDSPChannel::getRemainSamples() const {}
