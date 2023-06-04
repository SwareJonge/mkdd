#ifndef JUTPALETTE_H
#define JUTPALETTE_H

#include <dolphin/gx.h>
#include <dolphin/types.h>
#include "JSystem/ResTLUT.h"

enum JUTTransparency
{
    UNK0,
    UNK1
};

class JUTPalette
{
public:
    JUTPalette(_GXTlut p1, _GXTlutFmt p2, JUTTransparency p3, u16 p4, void *p5) {
        storeTLUT(p1, p2, p3, p4, p5);
    }

    JUTPalette(GXTlut tlutNo, ResTLUT *p_tlutRes) {
        storeTLUT(tlutNo, p_tlutRes);
    }

    void storeTLUT(_GXTlut, ResTLUT *);
    void storeTLUT(_GXTlut, _GXTlutFmt, JUTTransparency, u16, void *);
    bool load();

    _GXTlut getTlutName() const { return (_GXTlut)mTlutName; }
    _GXTlutFmt getFormat() const { return (_GXTlutFmt)mFormat; }
    ResTLUT *getColorTable() const { return mColorTable; }
    u16 getNumColors() const { return mNumColors; }
    JUTTransparency getTransparency() const { return (JUTTransparency)mTransparency; }

private:
    _GXTlutObj mTlutObj;  // 0x0
    u8 mTlutName;         // 0x0C, _GXTlut
    u8 mFormat;           // 0x0D, _GXTlutFmt
    ResTLUT *mColorTable; // 0x10
    u16 mNumColors;       // 0x14
    u8 mTransparency;     // 0x16, JUTTransparency
};

#endif /* JUTPALETTE_H */