#ifndef _JSYSTEM_JKR_JKRDVDFILE_H
#define _JSYSTEM_JKR_JKRDVDFILE_H

#include "dolphin/OS.h"
#include "dolphin/dvd.h"
#include "JSystem/JKernel/JKRFile.h"

class JKRDvdFile : public JKRFile {
public:
    JKRDvdFile();
    JKRDvdFile(s32);
    void initiate();
    void sync();

    // Inline/Unused
    void readDataAsync(void *, s32, s32);
    void writeDataAsync(void *, s32, s32);
    void check();
    void load(const char *, JKRHeap *);

    virtual ~JKRDvdFile();                                           // 08
    virtual bool open(const char *fileName);                         // 0C
    virtual void close();                                            // 10
    virtual s32 writeData(const void *addr, s32 length, s32 offset); // 14
    virtual s32 readData(void *addr, s32 length, s32 offset);        // 18
    virtual s32 getFileSize() const;                                 // 1C
    virtual bool open(s32);                                          // 20

    static void doneProcess(s32, DVDFileInfo *);


    static JSUList<JKRDvdFile> sDvdList; // E4
private:
    OSMutex mMutex; //1C
    OSMutex mMutex_34; // probably a debug mutex, not used
    u8 _4c[4]; // is this u64? probably not since the only read(and reference) and write to this in in initiate()
    int _50; 
    u8 _54[4];
    int _58;
    DVDFileInfo mDvdFileInfo; // 5C
    // 98, not sure if there's padding or if DVDFileInfo has an extra field? gets recasted in doneProcess
    OSMessageQueue mMessageQueue; // 9C
    OSMessage mMessage; // BC
    OSMessageQueue mMessageQueue_C0; // Perhaps these were debug only, there don't seem to be any references to this
    OSMessage mMessage_E0;
    JSULink<JKRDvdFile> mLink; // E4
    OSThread *mThread; // F4
};

#endif