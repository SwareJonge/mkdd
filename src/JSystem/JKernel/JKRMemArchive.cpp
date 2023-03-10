#include <dolphin/stl.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/JKernel/JKRDecomp.h>
#include <JSystem/JKernel/JKRDvdRipper.h>
#include <JSystem/JUtility/JUTDbg.h>

JKRMemArchive::JKRMemArchive(s32 entryNum, EMountDirection mountDirection) : JKRArchive(entryNum, MOUNT_MEM)
{
    mIsMounted = false;
    mMountDirection = mountDirection;
    if (!open(entryNum, mMountDirection)) {
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

JKRMemArchive::JKRMemArchive(void *mem, u32 size, JKRMemBreakFlag breakFlag) : JKRArchive((s32)mem, MOUNT_MEM)
{
    mIsMounted = false;
    if (!open(mem, size, breakFlag)) {
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

JKRMemArchive::~JKRMemArchive()
{
    if (mIsMounted == true)
    {
        if (mIsOpen && mArcHeader)
            JKRFreeToHeap(mHeap, mArcHeader);

        sVolumeList.remove(&mFileLoaderLink);
        mIsMounted = false;
    }
}

bool JKRMemArchive::open(s32 entryNum, JKRArchive::EMountDirection mountDirection)
{
    mArcHeader = nullptr;
    mArcInfoBlock = nullptr;
    mArchiveData = nullptr;
    mDirectories = nullptr;
    mFileEntries = nullptr;
    mStrTable = nullptr;
    mIsOpen = false;
    mMountDirection = mountDirection;

    if (mMountDirection == JKRArchive::MOUNT_DIRECTION_HEAD)
    {
        u32 loadedSize;
        mArcHeader = (SArcHeader *)JKRDvdRipper::loadToMainRAM(
            entryNum, nullptr, Switch_1, 0, mHeap, JKRDvdRipper::ALLOC_DIR_TOP,
            0, (int *)&mCompression, &loadedSize);
        if (mArcHeader)
        {
            DCInvalidateRange(mArcHeader, loadedSize);
        }
    }
    else
    {
        u32 loadedSize;
        mArcHeader = (SArcHeader *)JKRDvdRipper::loadToMainRAM(
            entryNum, nullptr, Switch_1, 0, mHeap,
            JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, (int *)&mCompression, &loadedSize);
        if (mArcHeader)
        {
            DCInvalidateRange(mArcHeader, loadedSize);
        }
    }

    if (!mArcHeader)
    {
        mMountMode = UNKNOWN_MOUNT_MODE;
    }
    else
    {
        JUT_ASSERT(438, mArcHeader->signature == 'RARC');
        mArcInfoBlock = (SArcDataInfo *)((u8 *)mArcHeader + mArcHeader->header_length);
        mDirectories = (SDIDirEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->node_offset);
        mFileEntries = (SDIFileEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->file_entry_offset);
        mStrTable = (char *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->string_table_offset);

        mArchiveData =
            (u8 *)((u32)mArcHeader + mArcHeader->header_length + mArcHeader->file_data_offset);
        mIsOpen = true;
    }
#if DEBUG
    // OS Assert?
    if (mMountMode == 0) {
        OSReport(":::Cannot alloc memory [%s][%d]\n", __FILE__, 460);
    }
#endif

    if (mMountMode == UNKNOWN_MOUNT_MODE)
        return false;
    return true;
}

bool JKRMemArchive::open(void *buffer, u32 bufferSize, JKRMemBreakFlag flag)
{
    mArcHeader = (SArcHeader *)buffer;
    JUT_ASSERT(491, mArcHeader->signature == 'RARC');
    mArcInfoBlock = (SArcDataInfo *)((u8 *)mArcHeader + mArcHeader->header_length);
    mDirectories = (SDIDirEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->node_offset);
    mFileEntries = (SDIFileEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->file_entry_offset);
    mStrTable = (char *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->string_table_offset);
    mArchiveData = (u8 *)(((u32)mArcHeader + mArcHeader->header_length) + mArcHeader->file_data_offset);
    mIsOpen = (flag == MBF_1) ? true : false; // mIsOpen might be u8
    mHeap = JKRHeap::findFromRoot(buffer);
    mCompression = TYPE_NONE;
    return true;
}

void *JKRMemArchive::fetchResource(SDIFileEntry *fileEntry, u32 *resourceSize)
{
    if (!fileEntry->mData)
        fileEntry->mData = mArchiveData + fileEntry->mDataOffset;

    if (resourceSize)
        *resourceSize = fileEntry->mSize;

    return fileEntry->mData;
}

void *JKRMemArchive::fetchResource(void *buffer, u32 bufferSize, SDIFileEntry *fileEntry,
                                   u32 *resourceSize)
{
    u32 srcLength = fileEntry->mSize;
    if (srcLength > bufferSize)
    {
        srcLength = bufferSize;
    }

    if (fileEntry->mData != nullptr)
    {
        memcpy(buffer, fileEntry->mData, srcLength);
    }
    else
    {
        CompressionMethod compression = JKRConvertAttrToCompressionType(fileEntry->getAttr());
        void *data = mArchiveData + fileEntry->mDataOffset;
        srcLength =
            fetchResource_subroutine((u8 *)data, srcLength, (u8 *)buffer, bufferSize, compression);
    }

    if (resourceSize)
    {
        *resourceSize = srcLength;
    }

    return buffer;
}

void JKRMemArchive::removeResourceAll(void)
{
    JUT_ASSERT(642 isMounted());

    if (mArcInfoBlock == nullptr)
        return;
    if (mMountMode == MOUNT_MEM)
        return;

    // !@bug: looping over file entries without incrementing the fileEntry pointer. Thus, only the
    // first fileEntry will clear/remove the resource data.
    SDIFileEntry *fileEntry = mFileEntries;
    for (int i = 0; i < mArcInfoBlock->num_file_entries; i++)
    {
        if (fileEntry->mData)
        {
            fileEntry->mData = nullptr;
        }
    }
}

bool JKRMemArchive::removeResource(void *resource)
{
    JUT_ASSERT(673 isMounted());

    SDIFileEntry *fileEntry = findPtrResource(resource);
    if (!fileEntry)
        return false;

    fileEntry->mData = nullptr;
    return true;
}

u32 JKRMemArchive::fetchResource_subroutine(u8 *src, u32 srcLength, u8 *dst, u32 dstLength, int compression)
{
    switch ((CompressionMethod)compression) // Maybe change CompressionMethod to defines?
    {
    case TYPE_NONE:
        if (srcLength > dstLength)
            srcLength = dstLength;

        memcpy(dst, src, srcLength);
        return srcLength;

    case TYPE_YAY0:
    case TYPE_YAZ0:
        u32 expendedSize = JKRDecompExpandSize(src);
        srcLength = expendedSize;
        if (expendedSize > dstLength)
            srcLength = dstLength;

        JKRDecompress(src, dst, srcLength, 0);
        return srcLength;

    default:
    {
        JUT_PANIC(723, "??? bad sequence\n");
        return 0;
    }
    }

    return srcLength;
}

u32 JKRMemArchive::getExpandedResSize(const void *resource) const
{
    SDIFileEntry *fileEntry = findPtrResource(resource);
    if (fileEntry == nullptr)
        return -1;

    if (((u8)(fileEntry->mFlag >> 24) & 4) == 0)
        return getResSize(resource);
    else
        return JKRDecompExpandSize((u8 *)resource);
}