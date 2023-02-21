#ifndef J3DEFFECTMGR
#define J3DEFFECTMGR

class J3DEffectMgr {
public:
    J3DEffectMgr();
    static void createMgr(){sJ3DMgr = new J3DEffectMgr(); }
    void createModel(JKRSolidHeap *, u32);
    void reset();
    static J3DEffectMgr *getJ3DEffectMgr() { return sJ3DMgr; }
    static J3DEffectMgr *sJ3DMgr;
private:
    u8 _00[0x78];
};

class J3DEfctKartAnmMgr {
public:
    J3DEfctKartAnmMgr();
    static void createMgr(){sJ3DKartAnmMgr = new J3DEfctKartAnmMgr(); };
    static J3DEfctKartAnmMgr *sJ3DKartAnmMgr;
private:
    u8 _00[0x18];
};

inline void CreateJ3DEfctKarAnmMgr(){J3DEfctKartAnmMgr::createMgr(); };
inline void CreateJ3DEfctMgr(){J3DEffectMgr::createMgr();};
inline J3DEffectMgr *GetJ3DEfctMgr() {return J3DEffectMgr::getJ3DEffectMgr(); }

#endif