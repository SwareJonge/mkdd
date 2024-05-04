#ifndef SCENETITLELINE_H
#define SCENETITLELINE_H

#include <JSystem/J2D/J2DPane.h>
#include <JSystem/J2D/J2DGrafContext.h>
#include <JSystem/JKernel/JKRArchive.h>

#define NUM_TAGS 10

class SceneTitleLine
{
public:
    // Global
    SceneTitleLine(JKRArchive *arc, JKRHeap *heap, char *name); // 0x801f5788
    ~SceneTitleLine();                                          // 0x801f58f8
    void init_titleline(int);                                   // 0x801f595c
    void drawTitleLine(J2DOrthoGraph *ortho);                   // 0x801f59bc
    void calcAnmTitleLine(int);                                 // 0x801f5a10
    void calcMatAnmTitleLine();                                 // 0x801f5ad8
    // Inline/Unused
    SceneTitleLine(JKRHeap *);
private:
    J2DScreen *mScreen;              // 0
    J2DPane *mPane[NUM_TAGS];        // 4;
    J2DAnmTransform *mTransform;     // 2c
    J2DAnmTextureSRTKey *mTexSRTKey; // 30
    int mAnmFrame;                   // 34
    int mMatAnmFrame;                // 38

    static const u64 mTag_Titleline[NUM_TAGS]; // 0x8037b370

}; // Size: unk

#endif // SCENETITLELINE_H