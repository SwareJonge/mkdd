#ifndef MENUTITLELINE_H
#define MENUTITLELINE_H

#include "JSystem/J2D/J2DPane.h"
#include "JSystem/JKernel/JKRArchive.h"

class MenuTitleLine
{
public:
    enum Title
    {

    };

    // Global
    MenuTitleLine(JKRArchive *); // 0x80219158
    void drop(Title);            // 0x802192e8
    void lift();                 // 0x802193c8
    void calc();                 // 0x802193d4

    ~MenuTitleLine() { mspMenuTitleLine = nullptr; }

    static MenuTitleLine *create(JKRArchive *archive, JKRHeap *heap)
    {
        if (mspMenuTitleLine == nullptr)
            mspMenuTitleLine = new (heap, 0) MenuTitleLine(archive);
        return mspMenuTitleLine;
    }

    static MenuTitleLine *ptr() { return mspMenuTitleLine; }

private:
    static MenuTitleLine *mspMenuTitleLine; // 0x80416a90

    u8 _0[8 - 0];
    J2DScreen mScreen; // 08 - 120 J2DScreen
    u8 _120[0x160 - 0x120];
};     // Size: 0x160
#endif // MENUTITLELINE_H
