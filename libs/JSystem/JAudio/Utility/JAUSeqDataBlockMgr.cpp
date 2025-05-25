#include "JSystem/JAudio/JAUSeqDataBlockMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/JAUSoundInfo.h"
#include "JSystem/JAudio/System/JASResArcLoader.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/types.h"

JAUSeqDataBlock::JAUSeqDataBlock() : link_(this) {}

u8 *JAUSeqDataBlocks::getSeqData(JAISoundID soundID) {
    if (soundID.isAnonymous()) {
        return NULL;
    }
    for (JSULink<JAUSeqDataBlock>* link = getFirst(); link != NULL; link = link->getNext()) {
        if ((u32)link->getObject()->soundID_ == (u32)soundID) {
            return link->getObject()->region.addr;
        }
    }
    return NULL;
}

JSULink<JAUSeqDataBlock> *JAUSeqDataBlocks::seekFreeBlock(u32 size) {
    for (JSULink<JAUSeqDataBlock>* link = getFirst(); link != NULL; link = link->getNext()) {
        if (link->getObject()->soundID_.isAnonymous()) {
            u32 blockSize = link->getObject()->region.size;
            if (blockSize >= size) {
                return link;
            }
        }
    }
    return NULL;
}

bool JAUSeqDataBlocks::append(JSULink<JAUSeqDataBlock>* link) {
    return JSUList::append(link);
}

bool JAUSeqDataBlocks::remove(JSULink<JAUSeqDataBlock> *link) {
    return JSUList::remove(link);
}

bool JAUSeqDataBlocks::hasFailedBlock(JAISoundID soundID) {
    for (JSULink<JAUSeqDataBlock>* link = getFirst(); link != NULL; link = link->getNext()) {
        if (link->getObject()->soundID_ == soundID) {
            link->getObject()->soundID_.setAnonymous();
            return true;
        }
    }
    return false;
}

JAUDynamicSeqDataBlocks::JAUDynamicSeqDataBlocks() {
    seqDataArchive_ = NULL;
}

void JAUDynamicSeqDataBlocks::setSeqDataArchive(JKRArchive* archive) {
#line 93
    JUT_ASSERT(seqDataArchive_ == 0);
    seqDataArchive_ = archive;
}

JAISeqDataResult JAUDynamicSeqDataBlocks::getSeqData(JAISoundID soundID, JAISeqDataUser* user,
                                        JAISeqData* data, bool param_3) {
    rearrangeLoadingSeqs_();
    if (freeBlocks_.hasFailedBlock(soundID)) {
        return JAI_ASYNC_RESULT_0;
    }

    if (_c.getSeqData(soundID)) {
        return JAI_ASYNC_RESULT_RETRY;
    }

    u8* seqData = loadedBlocks_.getSeqData(soundID);
    if (seqData != NULL) {
        data->_0 = seqData;
        data->_4 = 0;
        return JAI_ASYNC_RESULT_OK;
    }

    if (loadDynamicSeq(soundID, param_3, user)) {
        return JAI_ASYNC_RESULT_RETRY;
    }
    return JAI_ASYNC_RESULT_0;
}

bool JAUDynamicSeqDataBlocks::appendDynamicSeqDataBlock(JAUSeqDataBlock* seqDataBlock) {
#line 124
    JUT_ASSERT(( reinterpret_cast < u32 > ( seqDataBlock->region.addr ) & 0x1f ) == 0);
    rearrangeLoadingSeqs_();
    bool result = seqDataBlock->soundID_.isAnonymous() ? 
        freeBlocks_.append(&seqDataBlock->link_) : loadedBlocks_.append(&seqDataBlock->link_);

#line 133
    JUT_ASSERT(result);
    return true;
}

bool JAUDynamicSeqDataBlocks::removeDynamicSeqDataBlock(JAUSeqDataBlock *seqDataBlock, JAISeqDataUser *seqDataUser) {
    if (seqDataUser && seqDataBlock->soundID_ != 0xffffffff) {
        switch (seqDataUser->releaseSeqData(seqDataBlock->region)) {
        case 0:
#line 146
            JUT_WARNING_F2("%s", "cannot release sequence data block.\n");
        case 1:
            return false;
            break;
        }        
    }

    if (freeBlocks_.remove(&seqDataBlock->link_) || _c.remove(&seqDataBlock->link_) || loadedBlocks_.remove(&seqDataBlock->link_)) {
        return true;
    }
    return false;
}

static void JAUDynamicSeqDataBlocks_receiveLoaded_(u32 p1, u32 link) {
    JAUSeqDataBlock* seqDataBlock = ((JSULink<JAUSeqDataBlock>*)link)->getObject();
    if (p1 != NULL) {
        seqDataBlock->_1c = 2;
    } else {
        seqDataBlock->_1c = 0;
    }
}

bool JAUDynamicSeqDataBlocks::loadDynamicSeq(JAISoundID soundID, bool param_1,
                                             JAISeqDataUser* user) {
    if (seqDataArchive_ == NULL) {
#line 181
        JUT_WARNING_F2("%s", "cannot get sequence data archive.");
        return false;
    }

    JAUSoundInfo* soundInfo = JAUSoundInfo::getInstance();
    if (soundInfo == NULL) {
        JUT_WARNING_F2("%s", "cannot JAUSoundInfo::getInstance().");
        return false;
    }
    
    u16 resourceId = soundInfo->getBgmSeqResourceID(soundID);
    size_t resSize = JASResArcLoader::getResSize(seqDataArchive_, resourceId);
    JSULink<JAUSeqDataBlock>* link = freeBlocks_.seekFreeBlock(resSize);
    if (link == NULL) {
        if (param_1) {
            link = &releaseIdleDynamicSeqDataBlock_(user, resSize)->link_;
            if (link == NULL) {
                return false;
            }
        } else {
            return false;
        }
    }
    freeBlocks_.remove(link);
    JAUSeqDataBlock* seqDataBlock = link->getObject();
    seqDataBlock->soundID_ = soundID;
    link->getObject()->_1c = 1;
    _c.append(link);
    JASResArcLoader::loadResourceAsync(
        seqDataArchive_, resourceId,
        link->getObject()->region.addr, link->getObject()->region.size,
        JAUDynamicSeqDataBlocks_receiveLoaded_, (u32)link);
    return true;
}

u32 JAUDynamicSeqDataBlocks::releaseIdleDynamicSeqDataBlock(JAISeqDataUser* param_0) {
    rearrangeLoadingSeqs_();
    u32 size = 0;
    JSULink<JAUSeqDataBlock>* nextLink;
    for (JSULink<JAUSeqDataBlock>* link = loadedBlocks_.getFirst(); link != NULL; ) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (param_0 == NULL || !param_0->isUsingSeqData(seqDataBlock->region)) {
            loadedBlocks_.remove(link);
            JAUSeqDataBlock* seqDataBlock = link->getObject();
            link->getObject()->soundID_.setAnonymous();
            if (size < seqDataBlock->region.size) {
                size = seqDataBlock->region.size;
            }
            freeBlocks_.append(link);
        }
        link = nextLink;
    }
    return size;
}

/* 802A6E00-802A6EDC 2A1740 00DC+00 1/1 0/0 0/0 .text
 * releaseIdleDynamicSeqDataBlock___23JAUDynamicSeqDataBlocksFP14JAISeqDataUserUl */
JAUSeqDataBlock*
JAUDynamicSeqDataBlocks::releaseIdleDynamicSeqDataBlock_(JAISeqDataUser* param_0, u32 param_1) {
    rearrangeLoadingSeqs_();
    u32 minSize = 0xffffffff;
    JSULink<JAUSeqDataBlock>* foundBlock = NULL;
    JSULink<JAUSeqDataBlock>* link = loadedBlocks_.getFirst();
    JSULink<JAUSeqDataBlock>* nextLink;
    while (link != NULL) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (param_0 == NULL || !param_0->isUsingSeqData(seqDataBlock->region)) {
            u32 size = seqDataBlock->region.size;
            if (size >= param_1 && size < minSize) {
                foundBlock = link;
                minSize = size;
            }
        }
        link = nextLink;
    }
    if (foundBlock != NULL) {
        loadedBlocks_.remove(foundBlock);
        JAUSeqDataBlock* seqDataBlock = foundBlock->getObject();
        seqDataBlock->soundID_.setAnonymous();
        freeBlocks_.append(foundBlock);
        return foundBlock->getObject();
    }
    return NULL;
}

void JAUDynamicSeqDataBlocks::rearrangeLoadingSeqs_() {
    JSULink<JAUSeqDataBlock>* link = _c.getFirst();
    while (link != NULL) {
        JSULink<JAUSeqDataBlock>* nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (seqDataBlock->_1c == 0) {
            _c.remove(link);
            freeBlocks_.append(link);
        } else if (seqDataBlock->_1c == 2) {
            _c.remove(link);
            loadedBlocks_.append(link);
        }
        link = nextLink;
    }
}
