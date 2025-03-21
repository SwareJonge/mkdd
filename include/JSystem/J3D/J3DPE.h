#ifndef _JSYSTEM_J3D_J3DPE_H
#define _JSYSTEM_J3D_J3DPE_H

#include "JSystem/J3D/J3DTypes.h"
#include "types.h"

struct J3DFog {
    /** @fabricated */
    /*inline J3DFog& operator=(const J3DFog& other)
    {
        mType = other.mType;
        _01 = other._01;
        _02 = other._02;
        _04 = other._04;
        _08 = other._08;
        _0C = other._0C;
        _10 = other._10;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        for (int i = 0; i < 10; i++) {
            _18[i] = other._18[i];
        }
        return *this;
    }*/

    u8 getType() const { return mType; }
    void setType(u8 type) { mType = type; }
    void setStartZ(f32 startZ) { mStartZ = startZ; }
    void setEndZ(f32 endZ) { mEndZ = endZ; }
    void setNearZ(f32 nearZ) { mNearZ = nearZ; }
    void setFarZ(f32 farZ) { mFarZ = farZ; }

    void setFogColor(GXColor &color) {
        mColorR = color.r;
        mColorG = color.g;
        mColorB = color.b;
        mColorA = color.a;
    }

    u8 mType;     // _00
    u8 _01;       // _01
    u16 _02;      // _02
    f32 mStartZ;  // _04
    f32 mEndZ;    // _08
    f32 mNearZ;   // _0C
    f32 mFarZ;    // _10
    u8 mColorR;   // _14
    u8 mColorG;   // _15
    u8 mColorB;   // _16
    u8 mColorA;   // _17
    u16 _18[0xA]; // _18
};

/**
 * @size{0x4}
 */
struct J3DPEBlock {
    virtual void reset(J3DPEBlock*);                // _08 (weak)
    virtual void load() = 0;                        // _0C
    virtual void patch();                           // _10 (weak)
    virtual void diff(unsigned long);               // _14 (weak)
    virtual void diffFog();                         // _18 (weak)
    virtual void diffBlend();                       // _1C (weak)
    virtual u32 countDLSize();                      // _20 (weak)
    virtual JBlockType getType() = 0;               // _24
    virtual void setFog(J3DFog);                    // _28 (weak)
    virtual void setFog(J3DFog*);                   // _2C (weak)
    virtual J3DFog* getFog();                       // _30 (weak)
    virtual void setAlphaComp(const J3DAlphaComp*); // _34 (weak)
    virtual void setAlphaComp(const J3DAlphaComp&); // _38 (weak)
    virtual J3DAlphaComp* getAlphaComp();           // _3C (weak)
    virtual void setBlend(const J3DBlend*);         // _40 (weak)
    virtual void setBlend(const J3DBlend&);         // _44 (weak)
    virtual J3DBlend* getBlend();                   // _48 (weak)
    virtual void setZMode(const J3DZMode*);         // _4C (weak)
    virtual void setZMode(J3DZMode);                // _50 (weak)
    virtual J3DZMode* getZMode();                   // _54 (weak)
    virtual void setZCompLoc(const unsigned char*); // _58 (weak)
    virtual void setZCompLoc(unsigned char);        // _5C (weak)
    virtual u8 getZCompLoc() const;                 // _60 (weak)
    virtual void setDither(const unsigned char*);   // _64 (weak)
    virtual void setDither(unsigned char);          // _68 (weak)
    virtual u8 getDither() const;                   // _6C (weak)
    virtual u32 getFogOffset() const;               // _70 (weak)
    virtual void setFogOffset(unsigned long);       // _74 (weak)
    virtual ~J3DPEBlock() { }                       // _78 (weak)
};

struct J3DPEBlockFogOff : public J3DPEBlock {
    virtual void reset(J3DPEBlock*);                // _08
    virtual void load();                            // _0C
    virtual void diff(unsigned long);               // _14 (weak)
    virtual void diffBlend();                       // _1C
    virtual u32 countDLSize();                      // _20
    virtual JBlockType getType();                   // _24 (weak)
    virtual void setAlphaComp(const J3DAlphaComp*); // _34 (weak)
    virtual void setAlphaComp(const J3DAlphaComp&); // _38 (weak)
    virtual J3DAlphaComp* getAlphaComp();           // _3C (weak)
    virtual void setBlend(const J3DBlend*);         // _40 (weak)
    virtual void setBlend(const J3DBlend&);         // _44 (weak)
    virtual J3DBlend* getBlend();                   // _48 (weak)
    virtual void setZMode(const J3DZMode*);         // _4C (weak)
    virtual void setZMode(J3DZMode);                // _50 (weak)
    virtual J3DZMode* getZMode();                   // _54 (weak)
    virtual void setZCompLoc(const unsigned char*); // _58 (weak)
    virtual void setZCompLoc(unsigned char);        // _5C (weak)
    virtual u8 getZCompLoc() const;                 // _60 (weak)
    virtual void setDither(const unsigned char*);   // _64 (weak)
    virtual void setDither(unsigned char);          // _68 (weak)
    virtual u8 getDither() const;                   // _6C (weak)
    virtual ~J3DPEBlockFogOff() { }                 // _78 (weak)

    void initialize();

    J3DAlphaComp mAlphaComp; // _04
    J3DBlend mBlend;         // _08
    J3DZMode mZMode;         // _0C
    u8 _0E;                  // _0E
    u8 mDither;              // _0F
};

struct J3DPEBlockFull : public J3DPEBlock {
    virtual void reset(J3DPEBlock*);  // _08
    virtual void load();              // _0C
    virtual void patch();             // _10
    virtual void diff(unsigned long); // _14
    virtual void diffFog();           // _18
    virtual void diffBlend();         // _1C
    virtual u32 countDLSize();        // _20
    /**
     * @reifiedAddress{800816D0}
     * @reifiedFile{JSystem/J3D/J3DMatBlock.cpp}
     */
    virtual JBlockType getType() // _24 (weak)
    {
        return JBT_PEFull;
    }
    virtual void setFog(J3DFog);                    // _28 (weak)
    virtual void setFog(J3DFog*);                   // _2C (weak)
    virtual J3DFog* getFog();                       // _30 (weak)
    virtual void setAlphaComp(const J3DAlphaComp*); // _34 (weak)
    virtual void setAlphaComp(const J3DAlphaComp&); // _38 (weak)
    virtual J3DAlphaComp* getAlphaComp();           // _3C (weak)
    virtual void setBlend(const J3DBlend*);         // _40 (weak)
    virtual void setBlend(const J3DBlend&);         // _44 (weak)
    virtual J3DBlend* getBlend();                   // _48 (weak)
    virtual void setZMode(const J3DZMode*);         // _4C (weak)
    virtual void setZMode(J3DZMode);                // _50 (weak)
    virtual J3DZMode* getZMode();                   // _54 (weak)
    virtual void setZCompLoc(const unsigned char*); // _58 (weak)
    virtual void setZCompLoc(unsigned char);        // _5C (weak)
    virtual u8 getZCompLoc() const;                 // _60 (weak)
    /**
     * @reifiedAddress{80081910}
     * @reifiedFile{JSystem/J3D/J3DMatBlock.cpp}
     */
    virtual void setDither(const unsigned char* dither) // _64 (weak)
    {
        mDither = *dither;
    }
    /**
     * @reifiedAddress{80081908}
     * @reifiedFile{JSystem/J3D/J3DMatBlock.cpp}
     */
    virtual void setDither(unsigned char dither) // _68 (weak)
    {
        mDither = dither;
    }
    /**
     * @reifiedAddress{8008191C}
     * @reifiedFile{JSystem/J3D/J3DMatBlock.cpp}
     */
    virtual u8 getDither() const // _6C (weak)
    {
        return mDither;
    }
    /**
     * @reifiedAddress{80081924}
     * @reifiedFile{JSystem/J3D/J3DMatBlock.cpp}
     */
    virtual u32 getFogOffset() const // _70 (weak)
    {
        return mFogOffset;
    }
    /**
     * @reifiedAddress{8008192C}
     * @reifiedFile{JSystem/J3D/J3DMatBlock.cpp}
     */
    virtual void setFogOffset(unsigned long fogOffset) // _74 (weak)
    {
        mFogOffset = fogOffset;
    }
    /**
     * @reifiedAddress{80081934}
     * @reifiedFile{JSystem/J3D/J3DMatBlock.cpp}
     */
    virtual ~J3DPEBlockFull() {}; // _78 (weak)

    void initialize();

    J3DFog mFog;             // _04
    J3DAlphaComp mAlphaComp; // _30
    J3DBlend mBlend;         // _34
    J3DZMode mZMode;         // _38
    u8 _3A;                  // _3A
    u8 mDither;              // _3B
    u32 mFogOffset;          // _3C
};

/**
 * @size{0x4}
 */
struct J3DPEBlockNull : public J3DPEBlock {
    virtual void load() { } // _0C (weak)
    /**
     * @reifiedAddress{8006F448}
     * @reifiedFile{JSystem/J3D/J3DMaterialFactory.cpp}
     */
    virtual JBlockType getType() // _24 (weak)
    {
        return JBT_PENull;
    }
    virtual ~J3DPEBlockNull() { } // _78 (weak)
};

/**
 * @size{0x4}
 */
struct J3DPEBlockOpa : public J3DPEBlock {
    virtual void load();          // _0C
    virtual u32 countDLSize();    // _20
    virtual JBlockType getType(); // _24 (weak)
    virtual ~J3DPEBlockOpa() { }  // _78 (weak)
};

/**
 * @size{0x4}
 */
struct J3DPEBlockTexEdge : public J3DPEBlock {
    virtual void load();             // _0C
    virtual u32 countDLSize();       // _20
    virtual JBlockType getType();    // _24 (weak)
    virtual ~J3DPEBlockTexEdge() { } // _78 (weak)
};

/**
 * @size{0x4}
 */
struct J3DPEBlockXlu : public J3DPEBlock {
    virtual void load();          // _0C
    virtual u32 countDLSize();    // _20
    virtual JBlockType getType(); // _24 (weak)
    virtual ~J3DPEBlockXlu() { }  // _78 (weak)
};

#endif
