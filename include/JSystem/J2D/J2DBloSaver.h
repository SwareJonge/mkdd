#ifndef _JSYSTEM_J2D_J2DBLOSAVER_H
#define _JSYSTEM_J2D_J2DBLOSAVER_H

#include "JSystem/J2D/J2DMaterialFactory.h"
#include "JSystem/ResTIMG.h"
#include "types.h"

struct J2DMaterial;
struct JUTNameTab;

// this is stripped in P2
struct J2DMaterial_SaveBlock {
    void countParts(J2DMaterial**, u16);
    void setOffset(J2DMaterial**, u16, JUTNameTab*);
};

// Stripped
template<typename T>
struct J2DSaverLink { 
    J2DSaverLink() { }
    ~J2DSaverLink() {}
};

template<typename T>
struct J2DSaverList {
    J2DSaverList() {
        mNext = new T();
        mPrev = new T();
        uniqueAppend(T());
    }
    ~J2DSaverList() {}
    void uniqueAppend(T data) { mNext = &data; }
    int getNthLink(int);
    void search(T &);

    T *mNext;
    T *mPrev;
};

struct J2DBloSaver {
    // NB: almost all of this is stripped
    struct ResFONTName {
    };
    struct ResTIMGName {
    };
    struct MaterialName {
    };

    struct CTextureNameConnect {
        struct TNC {
            TNC(const ResTIMG* img, const char* name);

            ~TNC() { delete[] mName; }

            const ResTIMG* mTexture; // _00
            const char* mName;       // _04
            TNC* mNext;              // _08
            J2DSaverList<J2DTevStage> mTevStageList;
        };

        CTextureNameConnect() { mElements = nullptr; }

        ~CTextureNameConnect();
        void set(const ResTIMG* img, const char* name);
        const char* get(const ResTIMG *);
        void clear();

        TNC* mElements; // _00
    };

    static CTextureNameConnect TextureNameConnect;

    // unused/stripped functions (to spawn weak functions):
    void writeMaterialBlock(const MaterialName*, J2DMaterial**, u16, const ResTIMGName*, u16);
};
#endif
