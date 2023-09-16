#include <dolphin/dvd.h>
#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRArchive.h"

JKRArcFinder::JKRArcFinder(JKRArchive * archive, long startindex, long entries) : JKRFileFinder() {
    mArchive = archive;

    mIsAvailable = entries > 0;
    mStartIndex = startindex;
    mEndIndex = startindex + entries - 1;
    mNextIndex = mStartIndex;

    findNextFile();
}

bool JKRArcFinder::findNextFile() {
    if(mIsAvailable) {
        mIsAvailable = (mNextIndex <= mEndIndex);
        if(mIsAvailable) {
            JKRArchive::SDirEntry dirEntry;
            mIsAvailable = mArchive->getDirEntry(&dirEntry, mNextIndex);
            mFileName = dirEntry.mName;
            mFileIndex = mNextIndex;
            mFileID = dirEntry.mID;
            mFileTypeFlags = dirEntry.mFlags;
            mIsFileOrDir = (mFileTypeFlags & 2) != 0;
            mNextIndex++;
        }
    }
    return mIsAvailable;
}

// UNUSED
JKRDvdFinder::JKRDvdFinder(const char *path) : JKRFileFinder() {
    mIsDvdOpen = DVDOpenDir(const_cast<char *>(path), &mDir);
    mIsAvailable = mIsDvdOpen;
    findNextFile();
}

// UNUSED, not sure if it matches
bool JKRDvdFinder::findNextFile() {
    if (mIsAvailable)
    {
        DVDDirEntry entry;
        mIsAvailable = DVDReadDir(&mDir, &entry);
        if (mIsAvailable)
        {
            mIsFileOrDir = (bool)entry.isDir;
            mFileName = entry.name;
            mFileIndex = entry.entryNum;
            mFileID = 0;

            mFileTypeFlags = mIsFileOrDir ? 2 : 1;
        }
    }
    return mIsAvailable;
}
