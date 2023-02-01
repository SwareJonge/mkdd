#ifndef J3DEFFECTMGR
#define J3DEFFECTMGR

class J3DEffectMgr {
public:
    
    void createMgr();
    void createModel(JKRSolidHeap *, u32);
    void reset();
    static J3DEffectMgr *getJ3DEffectMgr() { return sJ3DMgr; };
    static J3DEffectMgr *sJ3DMgr;
};

class CreateJ3DEfctKarAnmMgr {
    void createMgr();
};

inline void CreateJ3DEfctKarAnmMgr();
inline void CreateJ3DEfctMgr();
inline J3DEffectMgr *GetJ3DEfctMgr() {return J3DEffectMgr::getJ3DEffectMgr(); };

#endif