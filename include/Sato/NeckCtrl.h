#ifndef NECKCTRL_H
#define NECKCTRL_H

class CharIKParamMgr {
public:
    CharIKParamMgr();
    static void createMgr(){mMgr = new CharIKParamMgr(); };
    static CharIKParamMgr *mMgr;
    u8 _0[4];
};

inline CharIKParamMgr *CreateCharIKParamMgr() { CharIKParamMgr::createMgr(); }

#endif