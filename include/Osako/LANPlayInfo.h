#ifndef LANPLAYINFO_H
#define LANPLAYINFO_H

#include "Osako/LANSelectMode.h"
#include "Osako/ResMgr.h"
#include "kartEnums.h"

class LANPlayInfo {
public:
    void init();                       // 0x80218580
    ECourseID getCrsID();              // 0x8021865c
    ResMgr::CourseOrder getCrsOrder(); // 0x80218904
    void resumeInfo(LANSelectMode *);  // 0x8021890c
    void saveInfo(LANSelectMode *);    // 0x802189a4
    void getRandomMap(int, u32);       // 0x80218a48
    void shuffleKart(int);             // 0x80218ab4
    void getMaskedRandom(int, u32);    // 0x80218be4
    void getRideKart(u8);              // 0x80218d10
    void getFront(int);                // 0x80218ec4
    void getBack(int);                 // 0x80218ee4
    void getDisplayNum();              // 0x80218f04
    // Inline/Unused
    LANPlayInfo();

private:
    u8 _0[0x44];
};
extern LANPlayInfo gLANPlayInfo;

#endif // LANPLAYINFO_H
