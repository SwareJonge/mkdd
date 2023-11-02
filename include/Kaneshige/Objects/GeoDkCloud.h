#ifndef GEODKCLOUD_H
#define GEODKCLOUD_H

#include "Sato/GeographyObj.h"

class GeoDkCloud : public GeographyObj
{ // Autogenerated
public:
    GeoDkCloud(const CrsData::SObject &);                                // 0x801ca7cc
    virtual void reset();                                                // 0x801ca83c
    virtual void loadAnimation();                                        // 0x801ca950
    virtual void createModel(JKRSolidHeap *, u32);                       // 0x801ca9a0
    virtual void simpleDraw(u32);                                        // 0x801caa08
    virtual void calc();                                                 // 0x801caa54
    virtual void update();                                               // 0x801caaa8
    virtual ~GeoDkCloud() {}                                             // 0x801caac8
    virtual char *getBmdFileName() { return "/Objects/DonkyCloud.bmd"; } // 0x801cab30
private:
    static J3DAnmTextureSRTKey *sBtkAnm; // 0x80416498
    // Unused
    // static u16 sCreateNum;
    // TODO
};     // class GeoDkCloud
#endif // GEODKCLOUD_H