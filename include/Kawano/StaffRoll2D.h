#ifndef STAFFROLL2D_H
#define STAFFROLL2D_H

#include <JSystem/J2D/J2DPane.h>
#include <JSystem/JKernel/JKRHeap.h>

#include "JSystem/JGeometry/Box.h"
#include "kartLocale.h"

#include "types.h"

class Font1letterPrinter {
public:
    Font1letterPrinter();
    ~Font1letterPrinter();
    void get(J2DTextBox *, f32, f32, char *, f32);
    void print();
};

class ExitBoxInfo {
public:
    ExitBoxInfo() {}
    void setBox(J2DPicture *);
    JGeometry::TBox2f &getBox() { return mBox; }
    int getDir() { return mDir; }
private:
    JGeometry::TBox2f mBox;
    int mDir;
}; // Size: 0x14 

class StaffRoll2D {
public:
    StaffRoll2D(JKRHeap *, Language, VideoMode, int); // 0x801f5b40
    void init(); // 0x801f6640
    void reset(); // 0x801f6694
    void draw(); // 0x801f66ec
    void calc(); // 0x801f6948
    void start(int); // 0x801f6c28
    ExitBoxInfo *getExitBoxInfoPtr(int, int); // 0x801f6d2c
    int getExitBoxNum(int); // 0x801f6d64
    void getVersion(Language, VideoMode); // 0x801f6d94
    void getSceneDataID(int, int); // 0x801f6db0
    
    static u64 mTag_textbox[375]; // 0x8037b9d8
    static u64 mTag_exitbox[200]; // 0x8037c590
    static int maConvertChartSceneNo2DataID[14]; // 0x8037cbd0, unsure
    static u64 mTag_textboxTrue[2]; // 0x8037cc00
    const char *mBloName[25]; // 0x8039bcc8
    const char *mBckName[25]; // 0x8039bd2c
    const char *mBrkName[25]; // 0x8039bd90
    const char *mName_jpa[375]; // 0x8039bdf4
    const char *mBloNameTrue[2]; // 0x80414840
    const char *mBckNameTrue[2]; // 0x80414848
    const char *mBpkNameTrue[2]; // 0x80414850
    // Inline/Unused    
    //void Constructer_JPA(JKRHeap *);
    ~StaffRoll2D();
    //void initExitBoxInfo();
    //void getWidth(char *);

    u8 _0[0x4598]; // beeg
};

#endif
