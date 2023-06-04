#ifndef _JSYSTEM_J2D_J2DPRINT_H
#define _JSYSTEM_J2D_J2DPRINT_H

#include "types.h"
#include "JSystem/J2D/J2DTypes.h"
#include "JSystem/JUtility/TColor.h"
#include "JSystem/JUtility/JUTFont.h"

struct J2DPrint
{
    struct TSize
    {
        f32 _00; // _00
        f32 _04; // _04
    };

    J2DPrint(JUTFont *, float);
    J2DPrint(JUTFont *, JUtility::TColor, JUtility::TColor);
    J2DPrint(JUTFont *, float, float, JUtility::TColor, JUtility::TColor, JUtility::TColor, JUtility::TColor);

    virtual ~J2DPrint(); // _08
    // virtual void _0C() = 0; // _0C - possibly

    void initiate();
    void private_initiate(JUTFont *, float, float, JUtility::TColor, JUtility::TColor, JUtility::TColor, JUtility::TColor, bool);
    JUTFont *setFont(JUTFont *);
    void setBuffer(u32);
    void setFontSize();
    void locate(float, float);
    double print(float, float, char const *, ...);
    void print(float, float, u8, const char *, ...);
    void getWidth(const char *, ...);
    void printReturn(const char *, float, float, J2DTextBoxHBinding, J2DTextBoxVBinding, float, float, u8);
    void parse(const u8 *, int, int, u16 *, J2DPrint::TSize &, u8, bool);
    void doCtrlCode(int);
    void doEscapeCode(const u8 **, u8);
    void initchar();
    void getNumberS32(const u8 **, long, long, int);
    void getNumberF32(const u8 **, float, float, int);

    // _00 VTBL
    JUTFont *mFont;                  // _04
    JUtility::TColor _08;            // _08
    JUtility::TColor _0C;            // _0C
    f32 _10;                         // _10
    f32 _14;                         // _14
    f32 _18;                         // _18
    f32 _1C;                         // _1C
    short _20;                       // _20
    u8 _22;                          // _22 - could be padding
    f32 _24;                         // _24
    f32 _28;                         // _28
    f32 _2C;                         // _2C
    f32 mCursorV;                    // _30
    f32 _34;                         // _34
    JUtility::TColor _38;            // _38
    JUtility::TColor _3C;            // _3C
    JUtility::TColor mCharColor;     // _40
    JUtility::TColor mGradientColor; // _44
    f32 _48;                         // _48
    f32 _4C;                         // _4C
    f32 mGlyphWidth;                 // _50
    f32 mGlyphHeight;                // _54
    short _58;                       // _58
    u8 _5A;                          // _5A
};

#endif