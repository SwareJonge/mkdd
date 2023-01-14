#ifndef JAUSOUNDOBJECT_H
#define JAUSOUNDOBJECT_H

#include "types.h"
#include "JSystem/JGeometry.h"

class JAISoundHandle;

class JAUSoundObject
{
public:
    JAISoundHandle *mHandles; // _0
    u32 _4;

    JAUSoundObject();
    ~JAUSoundObject();

    virtual void process();
    virtual void dispose();
    virtual void stopOK(JAISoundHandle &);
    virtual void *startSound(int);
    //virtual void startSoundIndex(int, unsigned char);
    //virtual void stopSound(int soundID, u32); // unused, not sure
    virtual void *startLevelSound(int);
    //virtual void startLevelSoundIndex(int, unsigned char);

    bool _C;
    u8 _D[3];
    u32 _10;
    JGeometry::TVec3<f32> *_14;
};

class JAUDopplerSoundObject : JAUSoundObject
{ 
    virtual void process();
    virtual void *startSound(int soundID);
    virtual void *startLevelSound(int soundID);
};

#endif JAUSOUNDOBJECT_H