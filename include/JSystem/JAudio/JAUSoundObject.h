#ifndef JAUDIO_JAUSOUNDOBJECT_H
#define JAUDIO_JAUSOUNDOBJECT_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JGeometry/Vec.h"
#include "types.h"

class JAUSoundObject : public JAISoundHandles
{
public:
    JAUSoundObject(JGeometry::TVec3f *, u8, JKRHeap *);
    ~JAUSoundObject();

    virtual void process();
    virtual void dispose();
    virtual bool stopOK(JAISoundHandle &);
    virtual JAISoundHandle *startSound(JAISoundID);
    virtual JAISoundHandle *startLevelSound(JAISoundID);
    
    JAISoundHandle *getFreeHandleNotReserved();
    JAISoundHandle *getLowPrioSound(JAISoundID);
    //void startLevelSoundIndex(int, unsigned char);
    //void startSoundIndex(int, unsigned char);
    //void stopSound(int soundID, u32); // unused, not sure

    u32 _C; // size of object?
    u8 _10;
    JGeometry::TVec3<f32> *mSoundPos; // 14
};

class JAUDopplerSoundObject : public JAUSoundObject
{ 
public:
    JAUDopplerSoundObject(JGeometry::TVec3f *, u8, JKRHeap *);
    ~JAUDopplerSoundObject();

    virtual void process();
    virtual JAISoundHandle *startSound(JAISoundID soundID);
    virtual JAISoundHandle *startLevelSound(JAISoundID soundID);

    JGeometry::TVec3f _18;
    JGeometry::TVec3f _24;
};

#endif
