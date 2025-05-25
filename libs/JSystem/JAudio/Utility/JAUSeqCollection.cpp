#include "JSystem/JAudio/JAUSeqCollection.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JUtility/JUTAssert.h"

JAUSeqCollection::JAUSeqCollection() {
    collection_ = NULL;
}

void JAUSeqCollection::init(const void *collection) {
    collection_ = (const JAUSeqCollectionData*)collection;
    if (collection_->groupId != 0x53 || collection_->_1 != 0x43) {
        collection_ = NULL;
        return;
    }

    groupId_ = collection_->_2;
    size_ = collection_->size;
    _4 = &collection_->_8;
}

bool JAUSeqCollection::getSeqData(int groupID, int shortID, JAISeqData *data) {
    if (groupID >= groupId_) {
        return false;
    }
    u32* puVar2 = (u32*)((u8*)collection_ + _4[groupID]);
    if (shortID >= puVar2[0]) {
        return false;
    }
    data->set((void*)collection_, puVar2[shortID + 1]);
    return true;
}

bool JAUSeqCollection::getSeqDataRegion(JAISeqDataRegion *region) {
    if (isValid()) {
        region->addr = (u8*)collection_;
        region->size = size_;
        return true;
    }
    return false;
}

JAUSeqDataMgr_SeqCollection::JAUSeqDataMgr_SeqCollection() {
    user_ = NULL;
}

bool JAUSeqDataMgr_SeqCollection::setSeqDataUser(JAISeqDataUser *user) {
#line 60
    JUT_ASSERT(user_ == 0);
    user_ = user;
    return true;
}

JAISeqDataResult JAUSeqDataMgr_SeqCollection::releaseSeqData() {
    if (user_) {
        JAISeqDataRegion region;
        getSeqDataRegion(&region);
        return user_->releaseSeqData(region);
    }
    return JAI_ASYNC_RESULT_OK;
}

JAISeqDataResult JAUSeqDataMgr_SeqCollection::getSeqData(JAISoundID soundID, JAISeqData *data) {
    if (!isValid()) {
        return JAI_ASYNC_RESULT_0;
    }
    if (JAUSeqCollection::getSeqData(soundID.mId.mBytes.mGroupId, soundID.mId.mAdvancedId.mShortId, data)) {
        return JAI_ASYNC_RESULT_OK;
    }
    return JAI_ASYNC_RESULT_0;
}
