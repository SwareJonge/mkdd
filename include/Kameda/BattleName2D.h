#ifndef BATTLENAME2D_H
#define BATTLENAME2D_H

#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/ResTIMG.h>

class BattleName2D
{
public:
    enum BattleName
    {
        BALLOON_BATTLE,
        BOMB_BATTLE,
        SHINE_THIEF,
        BATTLE_MAX
    };

    BattleName2D(JKRArchive *archive);                          // 0x801947ac
    ~BattleName2D();                                            // 0x8019483c
    const ResTIMG *getBattleNameTexture(BattleName battleName); // 0x80194890
private:
    static BattleName2D *mBattleName2D; // 0x80416380

    const ResTIMG *mTextures[3];
};
#endif // BATTLENAME2D_H
