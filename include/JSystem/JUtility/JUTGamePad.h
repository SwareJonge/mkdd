#ifndef JUTGAMEPAD_H
#define JUTGAMEPAD_H
// from Super Mario Eclipse
#include "types.h"
#include <dolphin/os.h>
#include <dolphin/PAD.h>
#include "JSystem/LGWheels.h"
#include "JSystem/JKernel/JKRDisposer.h"

typedef void (*JUTResetBtnCb)(int, void*);
// this currently is need to match testButton and testTrigger, if the entire file uses o4,p is unknown to me
#pragma push
#pragma optimize_for_size off
class JUTGamePad : public JKRDisposer
{
public:
    enum EPadPort
    {
        Port_unknown = -999,
        Port1 = 0,
        Port2,
        Port3,
        Port4,
        PortRecorder
    };

    enum EButtons
    {
        MAINSTICK_UP = 0x8000000,
        MAINSTICK_DOWN = 0x4000000,
        MAINSTICK_RIGHT = 0x2000000,
        MAINSTICK_LEFT = 0x1000000,
        CSTICK_UP = 0x80000,
        CSTICK_DOWN = 0x40000,
        CSTICK_RIGHT = 0x20000,
        CSTICK_LEFT = 0x10000,
        START = 0x1000,
        Y = 0x800,
        X = 0x400,
        B = 0x200,
        A = 0x100,
        L = 0x40,
        R = 0x20,
        Z = 0x10,
        DPAD_UP = 0x8,
        DPAD_DOWN = 0x4,
        DPAD_RIGHT = 0x2,
        DPAD_LEFT = 0x1
    };

    enum EStickMode
    {
        NonClamped,
        Clamped
    };

    enum EWhichStick
    {
        WhichStick_ControlStick,
        WhichStick_CStick
    };

    JUTGamePad(EPadPort port);
    virtual ~JUTGamePad();

    void assign();
    void checkResetSwitch();
    void clearForReset();
    static void init();
    void initList();
    void read();
    bool recalibrate(u32);
    void setButtonRepeat(u32, u32, u32);
    void update();

    static void setResetCallback(JUTResetBtnCb callback, void *param_0)
    {
        C3ButtonReset::sCallback = callback;
        C3ButtonReset::sCallbackArg = param_0;
    }


    bool testButton(u32 mask) const {
        return mButtons.mInput & mask;
    }

    bool testTrigger(u32 mask) const {
        return mButtons.mFrameInput & mask;
    }

    class CButton
    {
    public:
        CButton();

        void clear();
        void update(const PADStatus*, u32);
        void setRepeat(u32, u32, u32);

        u32 mInput; // _0
        u32 mFrameInput;
        u32 _8;
        u8 mAnalogA;  // _C
        u8 mAnalogB;  // _D
        u8 mTriggerL; // _E
        u8 mTriggerR; // _F
        f32 mAnalogL; // _10
        f32 mAnalogR; // _14
        u32 _18;
        u32 _1C;
        u32 _20;
        u32 _24;
        u32 _28;
        u32 _2C;
    };

    class CStick
    {
    public:
        CStick();

        void clear();
        u32 update(s8 x, s8 y, JUTGamePad::EStickMode, JUTGamePad::EWhichStick);
        u32 update(s8 x, s8 y, JUTGamePad::EStickMode, JUTGamePad::EWhichStick, u32);
        u32 getButton();

        f32 mStickX;
        f32 mStickY;
        f32 mLengthFromNeutral;
        u16 mAngle;
        u16 _E; // i assume unused
    };

    class CRumble
    {
    public:
        void clear(JUTGamePad*);
        static void stopMotor(s32);
        static void stopMotorHard(s32);
        void update(u16);
        void setEnable(u32);

        u32 _0;
        u32 _4;
        u32 _8;
        u32 _C;
    };

    class C3ButtonReset {
    public:
        C3ButtonReset() { mReset = false; }

        static u32 sResetPattern;
        static u32 sResetMaskPattern;
        static JUTResetBtnCb sCallback;
        static void *sCallbackArg;
        static OSTime sThreshold;
        static s32 sResetOccurredPort;
        static bool sResetOccurred;
        static bool sResetSwitchPushing;

    private:
        bool mReset;
    };

    static LGWheels* getLGWheels() { return mspLGWheels; }

    static LGWheels *mspLGWheels;

    static EStickMode sStickMode;

    CButton mButtons;     // _18
    CStick mControlStick; // _48
    CStick mCStick;       // _58
    CRumble mRumble;      // _68
    u16 mPort;            // _78
    u8 mPortConnected;    // _7A
    JSUPtrLink mPtrLink;  // _7C
    u32 _8C;
    u32 _90;
    u32 _94;
    u32 _98;
    u8 mResetFlag;     // _9C
    u8 _9D;            // padding?
    u8 _9E;            // ^^
    u8 _9F;            // ^^
    OSTime mResetTime; // _A0
};
#pragma pop
#endif