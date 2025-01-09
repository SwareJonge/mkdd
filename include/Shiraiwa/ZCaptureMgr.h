#ifndef ZCAPTUREMGR_H
#define ZCAPTUREMGR_H

#include "types.h"

class ZCaptureScreen {
public:
    ZCaptureScreen();
    virtual ~ZCaptureScreen();
    void reset();
    void captureAll();
    void capture(u32);
    void setPosition(u32, s16, s16);
    void getZValue(u32) const;

    class ZCaptureNode {
    public:    
        ZCaptureNode();
        virtual ~ZCaptureNode();
        void reset();
        void setPosition(s16, s16);
        void capture();
        void getZValue();
    private:
        s16 mPosX;
        s16 mPosY;
        u8 *_8;
        bool mPosSet;
    };

private:
    ZCaptureNode *mNodes[11];
};

class ZCaptureMgr { // Autogenerated
public:
    ZCaptureMgr(u32); // 0x802d7940
    virtual ~ZCaptureMgr(); // 0x802d79cc
    void reset(); // 0x802d7a68
    void capture(u32); // 0x802d7abc
    void captureDone(); // 0x802d7b58
    void setPosition(u32, u32, s16, s16); // 0x802d7b78
    void getZValue(u32, u32); // 0x802d7c1c
    
    class ZCaptureScreen {
    public:
        // Inline/Unused
        void setActive(u32);
        void setInactive(u32);
        void isActive(u32);

        //void scCaptureSizeX;
        //void scCaptureSizeY;
        //void scBufferSize;
    };
    // Inline/Unused
    void captureAll();
    void setActive(u32);
    void setInactive(u32);
    bool isActive(u32);

private:
    ZCaptureScreen *mScreens;
    u32 mNumViews;
}; // class ZCaptureMgr


#endif // ZCAPTUREMGR_H
