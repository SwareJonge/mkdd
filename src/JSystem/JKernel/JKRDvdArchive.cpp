#include <dolphin/math.h>
#include <dolphin/stl.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/JKernel/JKRDvdRipper.h>
#include <JSystem/JUtility/JUTDbg.h>

// WIP

JKRDvdArchive::JKRDvdArchive(s32 entryNum, EMountDirection mountDirection) : JKRArchive(entryNum, MOUNT_DVD)
{
    mIsMounted = false;
    mMountDirection = mountDirection;
    if (!open(entryNum))
    {
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

JKRDvdArchive::~JKRDvdArchive() {
    if(mIsMounted == true) {
        if (mArcInfoBlock)
        {
            SDIFileEntry *fileEntries = mFileEntries;
            for (int i = 0; i < mArcInfoBlock->num_file_entries; i++, fileEntries++)
            {
                if (fileEntries->mData != nullptr)
                {
                    JKRFreeToHeap(mHeap, fileEntries->mData);
                }
            }
            JKRFreeToHeap(mHeap, mArcInfoBlock);
            mArcInfoBlock = nullptr;
        }

        if (mExpandSizes)
        {
            JKRFree(mExpandSizes);
            mExpandSizes = nullptr;
        }
        if (mDvdFile)
        {
            delete mDvdFile;
        }

        sVolumeList.remove(&mFileLoaderLink);
        mIsMounted = false;
    }
}

bool JKRCompArchive::open(long entryNum)
{
    mArcInfoBlock = nullptr;
    _64 = 0;
    mDirectories = nullptr;
    mFileEntries = nullptr;
    mStrTable = nullptr;

    mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
    if (mDvdFile = nullptr)
    {
        mMountMode = 0;
        return 0;
    }
    SDIFileEntry *mem = (SDIFileEntry *)JKRAllocFromSysHeap(32, -32); // NOTE: unconfirmed if this struct was used here
    if (mem == nullptr)
    {
        mMountMode = 0;
    }
    else
    {
        JKRDvdToMainRam(entryNum, (u8 *)mem, Switch_1, 32, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0, &mCompression, nullptr);
        DCInvalidateRange(mem, 32);
        int alignment = -32;
        if (mMountMode == MOUNT_DVD)
            alignment = 32;

            mArcInfoBlock = (SArcDataInfo *)JKRAllocFromHeap(mHeap, mem->mSize, alignment);
        if (mArcInfoBlock == nullptr) {
            mMountMode = 0;
        }
        else {
            JKRDvdToMainRam(entryNum, (u8 *)mArcInfoBlock, Switch_1, mem->mSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 32, nullptr, nullptr);
            DCInvalidateRange(mArcInfoBlock, mem->mSize);

            mDirectories = (SDIDirEntry *)((u8 *)mArcInfoBlock + mArcInfoBlock->node_offset);
            mFileEntries = (SDIFileEntry *)((u8 *)mArcInfoBlock + mArcInfoBlock->file_entry_offset);
            mStrTable = (const char *)((u8 *)mArcInfoBlock + mArcInfoBlock->string_table_offset);
            mExpandSizes = nullptr;

            u32 flags = 0;

            SDIFileEntry *fileEntry = mFileEntries;
            for (int i = 0; i < mArcInfoBlock->num_file_entries; i++, fileEntry++)
            {
                if (((u8)(fileEntry->mFlag >> 24) & 1)) {
                    flags |= ((u8)(fileEntry->mFlag >> 24) & JKRARCHIVE_ATTR_COMPRESSION);
                }
            }

            if(flags) {
                mExpandSizes = (u32 *)JKRAllocFromHeap(mHeap, mArcInfoBlock->num_file_entries << 2, abs(alignment));
                if (mExpandSizes == nullptr)
                {
                    JKRFreeToSysHeap(mArcInfoBlock);
                    mMountMode = 0;
                    goto cleanup;
                }
                memset(mExpandSizes, 0, mArcInfoBlock->num_file_entries << 2);
            }
            _64 = mem->mDataOffset + mem->mSize; // End of data offset?
        }
    }
    cleanup:
    if (mem != nullptr)
    {
        JKRFreeToSysHeap(mem);
    }
    if (mMountMode = 0)
    {
        JUT_REPORT_MSG(":::Cannot alloc memory [%s][%d]\n", __FILE__, 397); // TODO: macro
        if (mDvdFile != nullptr)
        {
            delete mDvdFile;
        }
        return false;
    }
    return true;
}

void *JKRDvdArchive::fetchResource(SDIFileEntry *entry, u32 *outSize) {
    JUT_ASSERT(428, isMounted());

    u32 size;
    u8 *data;

    if(outSize == nullptr) {
        outSize = &size;
    }

    CompressionMethod compression = JKRConvertAttrToCompressionType((u8)(entry->mFlag >> 24));

    if(entry->mData == nullptr) {
        size = fetchResource_subroutine(mEntryNum, _64 + entry->mDataOffset, entry->mSize, mHeap, (int)compression, mCompression, &data);
        *outSize = size;
        if(size == 0) {
            return nullptr;
        }
        entry->mData = data;
        if (compression == TYPE_YAZ0) {
            setExpandSize(entry, *outSize);
        }
    }
    else if (compression == TYPE_YAZ0) {
        *outSize = getExpandSize(entry);
    }
    else {
        *outSize = entry->mSize;
    }

    return entry->mData;
}
