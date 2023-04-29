#include <dolphin/math.h>

#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/JKernel/JKRAram.h>
#include <JSystem/JKernel/JKRDvdAramRipper.h>
#include <JSystem/JKernel/JKRDvdRipper.h>
#include <JSystem/JUtility/JUTDbg.h>

// WIP

JKRCompArchive::JKRCompArchive(long entryNum, EMountDirection mountDirection) : JKRArchive(entryNum, MOUNT_COMP)
{
    mMountDirection = mountDirection;
    if (!open(entryNum))  {
        return;
    }
    else
    {
        mVolumeType = 'RARC';
        mVolumeName = &mStrTable[mDirectories->mOffset];
        sVolumeList.prepend(&mFileLoaderLink);
        mIsMounted = true;
    }
}

void stringGen() {
    JUT_PANIC(111, "isMounted()");
    JUT_PANIC(111, "mMountCount == 1");
}

JKRCompArchive::~JKRCompArchive() {
    if (mArcInfoBlock) {
        SDIFileEntry *fileEntries = mFileEntries;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
            if (((u8)(fileEntries->mFlag >> 24) & 16) == 0 && fileEntries->mData != nullptr) {
                JKRFreeToHeap(mHeap, fileEntries->mData);
            }
            fileEntries++;
        }
        JKRFreeToHeap(mHeap, mArcInfoBlock);
        mArcInfoBlock = nullptr;
    }
    if (mAramPart) {
        delete mAramPart;
    }
    if (mExpandSizes) {
        JKRFree(mExpandSizes);
        mExpandSizes = nullptr;
    }
    if (mDvdFile) {
        delete mDvdFile;
    }

    sVolumeList.remove(&mFileLoaderLink);
    mIsMounted = false;
}

bool JKRCompArchive::open(long entryNum) {
    mArcInfoBlock = nullptr;
    _64 = 0;
    mAramPart = nullptr;
    _6C = 0;
    mSizeOfMemPart = 0;
    mSizeOfAramPart = 0;
    _7C = 0;
    mDirectories = nullptr;
    mFileEntries = nullptr;
    mStrTable = nullptr;

    mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
    if(mDvdFile == nullptr) {
        mMountMode = 0;
        return 0;
    }
    SArcHeader *arcHeader = (SArcHeader *)JKRAllocFromSysHeap(sizeof(SArcHeader), -32); // NOTE: unconfirmed if this struct is used
    if(arcHeader == nullptr) {
        mMountMode = 0;
    }
    else {
        int alignment;

        JKRDvdToMainRam(entryNum, (u8 *)arcHeader, Switch_1, 32, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0, &mCompression, nullptr);
        DCInvalidateRange(arcHeader, 32);

        mSizeOfMemPart = arcHeader->_14;
        mSizeOfAramPart = arcHeader->_18;
        JUT_ASSERT(352, ( mSizeOfMemPart & 0x1f ) == 0);
        JUT_ASSERT(353, ( mSizeOfAramPart & 0x1f ) == 0);

        switch (mCompression)
        {
        case JKRCOMPRESSION_NONE:
        case JKRCOMPRESSION_YAZ0:
            alignment = mMountDirection == 1 ? 32 : -32;
            mArcInfoBlock = (SArcDataInfo *)JKRAllocFromHeap(mHeap, arcHeader->file_data_offset + mSizeOfMemPart, alignment);
            if (mArcInfoBlock == nullptr) {
                mMountMode = 0;
            }
            else
            {
                JKRDvdToMainRam(entryNum, (u8 *)mArcInfoBlock, Switch_1, (u32)arcHeader->file_data_offset + mSizeOfMemPart,
                                nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0x20, nullptr, nullptr);
                DCInvalidateRange(mArcInfoBlock, (u32)arcHeader->file_data_offset + mSizeOfMemPart);
                _64 = (u32)mArcInfoBlock + arcHeader->file_data_offset;

                if (mSizeOfAramPart != 0) {
                    mAramPart = JKRAllocFromAram(mSizeOfAramPart, JKRAramHeap::AM_Head);
                    if(mAramPart == nullptr) {
                        mMountMode = 0;
                        break;
                    }

                    JKRDvdToAram(entryNum, mAramPart->getAddress(), Switch_1, (u32)((SDIFileEntry *)arcHeader)->mDataOffset + ((SDIFileEntry *)arcHeader)->mSize + mSizeOfMemPart, 0, nullptr);
                }

                mDirectories = (SDIDirEntry*)((u32)mArcInfoBlock + mArcInfoBlock->node_offset);
                mFileEntries = (SDIFileEntry *)((u32)mArcInfoBlock + mArcInfoBlock->file_entry_offset);
                mStrTable = (const char*)((u32)mArcInfoBlock + mArcInfoBlock->string_table_offset);
                _6C = arcHeader->header_length + arcHeader->file_data_offset;
            }
            break;

        case JKRCOMPRESSION_YAY0:
            u32 alignedSize = ALIGN_NEXT(mDvdFile->getFileSize(), 32);
            alignment = ((mMountDirection == 1) ? 32 : -32); // for whatever reason there's an extra mr here
            void *buf = JKRAllocFromSysHeap(alignedSize, -alignment);

            if(buf == nullptr) {
                mMountMode = 0;
            }
            else {
                JKRDvdToMainRam(entryNum, (u8 *)buf, Switch_2, alignedSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0, nullptr, nullptr);
                DCInvalidateRange((u8*)buf, alignedSize);
                u32 expandSize = ALIGN_NEXT(JKRDecompExpandSize((u8 *)buf), 32);
                u8 *mem = (u8 *)JKRAllocFromHeap(mHeap, expandSize, -alignment);

                if(mem == nullptr) {
                    mMountMode = 0;
                }
                else {
                    arcHeader = (SArcHeader *)mem;
                    JKRDecompress((u8 *)buf, (u8 *)mem, expandSize, 0);
                    JKRFreeToSysHeap(buf);

                    mArcInfoBlock = (SArcDataInfo *)JKRAllocFromHeap(mHeap, arcHeader->file_data_offset + mSizeOfMemPart, alignment);
                    if(mArcInfoBlock == nullptr) {
                        mMountMode = 0;
                    }
                    else {
                        // arcHeader + 1 should lead to 0x20, which is the data after the header
                        JKRHeap::copyMemory((u8 *)mArcInfoBlock, arcHeader + 1, (arcHeader->file_data_offset + mSizeOfMemPart));
                        _64 = (u32)mArcInfoBlock + arcHeader->file_data_offset;
                        if (mSizeOfAramPart != 0) {
                            mAramPart = JKRAllocFromAram(mSizeOfAramPart, JKRAramHeap::AM_Head);
                            if(mAramPart == nullptr) {
                                mMountMode = 0;                                
                            }
                            else {
                                JKRMainRamToAram((u8 *)mem + arcHeader->header_length + arcHeader->file_data_offset + mSizeOfMemPart,
                                                 mAramPart->getAddress(), mSizeOfAramPart, Switch_0, 0, nullptr, -1, nullptr);
                            }
                        }
                    }                    
                }
            }
            mDirectories = (SDIDirEntry *)((u32)mArcInfoBlock + mArcInfoBlock->node_offset);
            mFileEntries = (SDIFileEntry *)((u32)mArcInfoBlock + mArcInfoBlock->file_entry_offset);
            mStrTable = (const char *)((u32)mArcInfoBlock + mArcInfoBlock->string_table_offset);
            _6C = arcHeader->header_length + arcHeader->file_data_offset;
            break;
        }
        mExpandSizes = nullptr;
        u8 compressedFiles = 0;
        SDIFileEntry *fileEntry = mFileEntries;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++)
        {
            u8 flag = fileEntry->mFlag >> 0x18;
            if (((flag & 0x1) != 0) && (((flag)&0x10) == 0))
            {
                compressedFiles = compressedFiles | (flag & 4);
            }
            fileEntry++;
        }

        if (compressedFiles != 0)
        {
            mExpandSizes = (u32 *)JKRAllocFromHeap(mHeap, mArcInfoBlock->num_file_entries * 4, abs(alignment));
            if (mExpandSizes == nullptr)
            {
                JKRFreeToSysHeap(mArcInfoBlock);
                mMountMode = 0;
            }
            else
            {
                memset(mExpandSizes, 0, mArcInfoBlock->num_file_entries * 4);
            }
        }
    }

    if (arcHeader != nullptr)
    {
        JKRFreeToSysHeap(arcHeader);
    }
    if(mMountMode == 0) {
        JUT_REPORT_MSG(":::[%s: %d] Cannot alloc memory in mounting CompArchive\n", __FILE__, 567); // TODO: macro
        if(mDvdFile != nullptr) {
            delete mDvdFile;
        }
        return false;
    }
    return true;
}

void JKRCompArchive::removeResourceAll()
{
    if (mArcInfoBlock != nullptr && mMountMode != MOUNT_MEM)
    {
        SDIFileEntry *fileEntry = mFileEntries;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++)
        {
            u8 flag = fileEntry->mFlag >> 0x18;
            if (fileEntry->mData != nullptr)
            {
                if ((flag & 0x10) == 0)
                {
                    JKRFreeToHeap(mHeap, fileEntry->mData);
                }

                fileEntry->mData = nullptr;
            }
        }
    }
}

bool JKRCompArchive::removeResource(void *resource)
{
    SDIFileEntry *fileEntry = findPtrResource(resource);
    if (!fileEntry)
        return false;

    if (((u8)(fileEntry->mFlag >> 0x18) & 0x10) == 0)
    {
        JKRFreeToHeap(mHeap, resource);
    }

    fileEntry->mData = nullptr;
    return true;
}

// might be long/s32 return
u32 JKRCompArchive::getExpandedResSize(const void *resource) const
{
    if (mExpandSizes == 0) {
        return getResSize(resource);
    }

    SDIFileEntry *fileEntry = findPtrResource(resource);
    if(!fileEntry) {
        return 0xffffffff;
    }

    u32 flags = (fileEntry->mFlag >> 0x18);
    if((flags & 4) == 0) { // not compressed
        return getResSize(resource);
    }

    if ((flags & 0x10) != 0) {
        return JKRDecompExpandSize((u8 *)resource);
    }

    u8 buf[64];
    u8 *bufPtr = (u8 *)ALIGN_NEXT((u32)buf, 32);
    if ((flags & 0x20) != 0)
    {
        JKRAramToMainRam(fileEntry->mDataOffset + mAramPart->mAddress, bufPtr, sizeof(buf) / 2, Switch_0, 0, nullptr, -1, nullptr);
        DCInvalidateRange(bufPtr, sizeof(buf) / 2);
    }
    else if ((flags & 0x40) != 0)
    {
        JKRDvdToMainRam(mEntryNum, bufPtr, Switch_2, sizeof(buf) / 2, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, _6C + fileEntry->mDataOffset, nullptr, nullptr);
        DCInvalidateRange(bufPtr, sizeof(buf) / 2);
    }
    else {
        JUT_PANIC(943, "illegal resource.");
    }
    u32 expandSize = JKRDecompExpandSize(bufPtr);
    const_cast<JKRCompArchive *>(this)->setExpandSize(fileEntry, expandSize);
    return expandSize;
}