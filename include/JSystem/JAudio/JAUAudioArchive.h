#ifndef JAUDIO_JAUAUDIOARCHIVE_H
#define JAUDIO_JAUAUDIOARCHIVE_H

#include "types.h"

struct JAUSection;

class JAUAudioArcInterpreter
{
public:
    JAUAudioArcInterpreter();
    virtual ~JAUAudioArcInterpreter();
    virtual void readWS(u32, const void *, u32) = 0;
    virtual void readBNK(u32, const void *) = 0;
    virtual void readBSC(const void *, u32) = 0;
    virtual void readBST(const void *, u32) = 0;
    virtual void readBSTN(const void *, u32) = 0;
    virtual void readBMS(u32, const void *, u32) = 0;
    virtual void readBMS_fromArchive(u32) = 0;
    virtual void newDynamicSeqBlock(u32) = 0;
    virtual void readBSFT(const void *) = 0;
    virtual void readMaxSeCategory(int, int, int) = 0;
    virtual void beginBNKList(u32, u32) = 0;
    virtual void endBNKList() = 0;    
    virtual bool readCommandMore(u32);
    bool readCommand_();
    bool parse(const void *);

    int readU8_() { return *mReadPtr++; } // does this really return int or u32? mkdd release needs it

    u32 readU32_()
    {
        u32 val = *(u32 *)mReadPtr;
        mReadPtr += 4;
        return val;
    }

    const void *getContent_(u32 offset) { return mBase + offset; }

    const u8 *mReadPtr; // 04
    const u8 *mBase;    // 08
};

class JAUAudioArcLoader : public JAUAudioArcInterpreter
{
public:
    JAUAudioArcLoader(JAUSection *);
    virtual ~JAUAudioArcLoader() {}
    
    virtual void readWS(u32, const void *, u32);
    virtual void readBNK(u32, const void *);
    virtual void readBSC(const void *, u32);
    virtual void readBST(const void *, u32);
    virtual void readBSTN(const void *, u32);
    virtual void readBMS(u32, const void *, u32);
    virtual void readBMS_fromArchive(u32);
    virtual void newDynamicSeqBlock(u32);
    virtual void readBSFT(const void *);
    virtual void readMaxSeCategory(int, int, int);
    virtual void beginBNKList(u32, u32);
    virtual void endBNKList();    

    bool load(const void *data);
    bool load(const char *path);
    JAUSection *mSection; // 0C
};

// Unused class
class JAUAudioArcLoader_withoutCopy : public JAUAudioArcInterpreter
{
public:
    JAUAudioArcLoader_withoutCopy(JAUSection *);
    virtual ~JAUAudioArcLoader_withoutCopy() {}

    virtual void readWS(u32, const void *, u32);
    virtual void readBNK(u32, const void *);
    virtual void readBSC(const void *, u32);
    virtual void readBST(const void *, u32);
    virtual void readBSTN(const void *, u32);
    virtual void readBMS(u32, const void *, u32);
    virtual void readBMS_fromArchive(u32);
    virtual void newDynamicSeqBlock(u32);
    virtual void readBSFT(const void *);
    virtual void readMaxSeCategory(int, int, int);
    virtual void beginBNKList(u32, u32);
    virtual void endBNKList();

    bool load(const void *data);
    bool load(const char *path);
    JAUSection *mSection; // 0C
};

#endif /* JAUDIO_JAUAUDIOARCHIVE_H */