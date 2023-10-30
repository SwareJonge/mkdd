#ifndef SHAPEPKTCONTROL_H
#define SHAPEPKTCONTROL_H

#include <JSystem/J3D/J3DPacket.h>

#include "types.h"

#define SHAPEPKT_MAGIC 0x53485043 // SHPC

class ShapePktControl
{
public:
    ShapePktControl();    // 0x801ba000
    void ctrlPacket(u32); // 0x801ba020

    // Inline Functions
    void hide() { mFlags = 0xffff; }
    void hide(u32 idx) { mFlags |= 1 << idx; }
    void show() { mFlags = 0; }
    void show(u32 idx) { mFlags &= ~(1 << idx); }

    bool isHidding(u32 idx) const { return mFlags & 1 << idx; }
    bool isValid() const { return mMagic == SHAPEPKT_MAGIC; }    
    void setShapePacket(J3DShapePacket *shapPkt) { mShapePkt = shapPkt; }

private:
    u32 mMagic;                // 0x0
    J3DShapePacket *mShapePkt; // 0x4
    u16 mFlags;                // 0x8
    u16 _10; // unused?
};
#endif // SHAPEPKTCONTROL_H