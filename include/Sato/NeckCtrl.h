#ifndef NECKCTRL_H
#define NECKCTRL_H

class CharIKParamMgr {
public:
    static void createMgr(){mMgr = new CharIKParamMgr(); };
    static CharIKParamMgr *mMgr;
};

inline CharIKParamMgr *CreateCharIKParamMgr() { CharIKParamMgr::createMgr(); }

#endif