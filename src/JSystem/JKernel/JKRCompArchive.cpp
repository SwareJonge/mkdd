#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/JKernel/JKRAram.h>
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

JKRCompArchive::~JKRCompArchive() {
    if (mArcInfoBlock) {
        SDIFileEntry *fileEntries = mFileEntries;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++, fileEntries++) {
            if (((u8)(fileEntries->mFlag >> 24) & 16) == 0 && fileEntries->mData != nullptr) {
                JKRFreeToHeap(mHeap, fileEntries->mData);
            }
        }
        JKRFreeToHeap(mHeap, mArcInfoBlock);
        mArcInfoBlock = nullptr;
    }
    if (_68) {
        delete _68;
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
    _68 = nullptr;
    _6C = 0;
    mSizeOfMemPart = 0;
    mSizeOfAramPart = 0;
    _7C = 0;
    mDirectories = nullptr;
    mFileEntries = nullptr;
    mStrTable = nullptr;

    mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
    if(mDvdFile = nullptr) {
        mMountMode = 0;
        return 0;
    }
    u8 *mem = (u8 *)JKRAllocFromSysHeap(32, -32);
    if(mem == nullptr) {
        mMountMode = 0;
    }
    else {
        JKRDvdToMainRam(entryNum, mem, Switch_1, 32, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0, &mCompression, nullptr);
        DCInvalidateRange(mem, 32);
        JUT_ASSERT(352, ( mSizeOfMemPart & 0x1f ) == 0);
        JUT_ASSERT(353, ( mSizeOfAramPart & 0x1f ) == 0);

        if (mCompression == JKRCOMPRESSION_YAY0) {
            s32 alignedSize = ALIGN_NEXT(mDvdFile->getFileSize(), 32);
            int alignment = -32; // probably a ternary
            if (mMountDirection == MOUNT_DIRECTION_HEAD)
                alignment = 32;            
        }
    }
    if(mem != nullptr) {
        JKRFreeToSysHeap(mem);
    }
    if(mMountMode = 0) {
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
            if (fileEntry->mData != nullptr)
            {
                if (((u8)(fileEntry->mFlag >> 0x18) & 0x10) == 0)
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