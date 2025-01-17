#include "JSystem/J2D/J2DBloSaver.h"
#include "JSystem/J2D/J2DMatBlock.h"
#include "JSystem/ResTIMG.h"
#include "types.h"

J2DBloSaver::CTextureNameConnect J2DBloSaver::TextureNameConnect;

void J2DBloSaver::writeMaterialBlock(const MaterialName*, J2DMaterial**, u16, const ResTIMGName*, u16)
{
    // obviously much much more than this, but need to spawn weak functions
    J2DTevBlock* block;
    block->setTexNo(0, 0);
    u16 texNo = block->getTexNo(0);
}

void J2DMaterial_SaveBlock::countParts(J2DMaterial**, u16)
{
    // obviously much much more than this, but need to spawn weak functions
    J2DTevBlock* block;
    block->getFontNo();
    block->getTevOrder(0);
    block->getTevSwapModeTable(0);
}

void J2DMaterial_SaveBlock::setOffset(J2DMaterial**, u16, JUTNameTab*)
{
    // obviously much much more than this, but need to spawn weak functions
    J2DIndBlock* block;
    block->getIndTexStageNum();
}

J2DBloSaver::CTextureNameConnect::~CTextureNameConnect() { clear(); }

void J2DBloSaver::CTextureNameConnect::clear()
{
    TNC* element;
    TNC* next;
    for (element = mElements; element != nullptr; element = next) {
        next = element->mNext;
        delete element;
    }
    mElements = nullptr;
}

const char* J2DBloSaver::CTextureNameConnect::get(const ResTIMG* tex) {
    // UNUSED
}

J2DBloSaver::CTextureNameConnect::TNC::TNC(const ResTIMG* tex, const char* name) {
    mTexture = tex;
    mName = name;
}
