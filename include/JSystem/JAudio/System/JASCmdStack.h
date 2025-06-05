#ifndef JAUDIO_JASCMDSTACK_H
#define JAUDIO_JASCMDSTACK_H

#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JSupport/JSUList.h"

struct JASPortArgs {
    JASTrack* track;
    u32 _04;
    u32 _08;
    f32 _0C;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    u32 _24;
    f32 _28;
};

class JASPortCmd : public JSULink<JASPortCmd> {
public:
    JASPortCmd() : JSULink<JASPortCmd>(this) {}
    ~JASPortCmd() {}

    typedef void (*JASportCmdFunc)(JASPortArgs*);

    class TPortHead : public JSUList<JASPortCmd> {
    public:
        void execCommandOnce();
        void execCommandStay();
    };
    static void addPortCmdOnce();
    void addPortCmdStay();
    void cancelPortCmdStay();
    void setPortCmd(JASportCmdFunc cb, JASPortArgs *args);
    static void execAllCommand();

    JASportCmdFunc getFunc() { return mFunc; }
    JASPortArgs *getArgs() { return mArgs; }

private:
    JASportCmdFunc mFunc;
    JASPortArgs* mArgs;

    static TPortHead sCommandListOnce;
    static TPortHead sCommandListStay;
}; // class JASCmdStack

#endif // JAUDIO_JASCMDSTACK_H
