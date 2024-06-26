#ifndef SOUNDMGR_H
#define SOUNDMGR_H

#include "types.h"
#include "JSystem/JAudio/JAUSoundObject.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JMath/JMath.h"

#include "Inagaki/GameAudioCommon.h" // Temporary(so permanent) fix for that one file that actually uses this

namespace GameAudio
{
    template <typename T>
    class SoundMgr : public JAUDopplerSoundObject, public JKRDisposer
    {
    public:
        SoundMgr(Vec *, JKRHeap *, u8);
        virtual ~SoundMgr();
        virtual void init();
        virtual void setSe(u32 id);
        virtual void loop(u32);
    };

    template <typename T>
    class SoundWithEchoMgr : public SoundMgr<T> 
    {
    public:
        SoundWithEchoMgr(Vec *, JKRHeap *, u8); // 0x8012aaac
        virtual ~SoundWithEchoMgr(); // 0x8012ab18
        void checkEcho(); // 0x8012ab78
        void setInitialEcho(JAISoundHandle *); // 0x8012ad68
    };


class ObjectSoundMgr : public SoundWithEchoMgr<ObjectSoundMgr>
{
public:
    ObjectSoundMgr(Vec *, JKRHeap *); // 0x8012a588    
    void frameWork(); // 0x8012a6e4    
    void setSeWithVelocityVP(u32, f32, f32); // 0x8012a75c
    void setSeWithVelocityV(u32, f32, f32); // 0x8012a8c4
    void setSeWithVelocityP(u32, f32); // 0x8012a9c4

    // Vtable
    virtual ~ObjectSoundMgr() {} // 0x8012a5fc
    virtual void loop(); // 0x8012a65c
    virtual void setSe(u32); // 0x8012a6fc
}; // class ObjectSoundMgr

}

#endif