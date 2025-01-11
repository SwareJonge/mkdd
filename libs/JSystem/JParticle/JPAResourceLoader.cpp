#include "JSystem/JParticle/JPABlock.h"
#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JParticle/JPAShape.h"
#include "JSystem/JParticle/JPATexture.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "types.h"

/*

*/

#define GetTypeFromByteStream(data, type, offset) (*(type*)((data) + (offset)))

JPAResourceLoader::JPAResourceLoader(const u8 *p1, JPAResourceManager* p_res_mgr, u16)
{
    // Fabricated for assert, doesn't match

	if (*(int*)(p1 + 4) == '2-10') {
		load_jpc(p1, p_res_mgr);
	} else {
#line 48
        JUT_WARNING_F2("JPA : wrong version file %s\n", *(u32*)(p1 + 4));
    }
    JUT_WARNING_F2("JPA : no jpa file\n");
}

JPAResourceLoader::JPAResourceLoader(const u8 *p1, JPAResourceManager *p_res_mgr)
{
	if (*(int*)(p1 + 4) == '2-10') {
		load_jpc(p1, p_res_mgr);
	} else {
#line 48
        JUT_WARNING_F2("JPA : wrong version file\n");
    }
}

void JPAResourceLoader::load_jpa(const u8 *p1, JPAResourceManager *p_res_mgr , u16 resourceSlotCount)
{
    // Fabricated for assert, doesn't match

    JUT_WARNING_F2("tex_num &&\"a resource have to use one or more texture\"");
    JKRHeap *heap         = p_res_mgr->pHeap;
    int resourceOffset          = 0x10;
    for (int i = 0; i < resourceSlotCount; i++) {
        const u8 *resourceData = (p1 + resourceOffset);
        JPAResource *p_res     = new (heap, 0) JPAResource;
        JUT_ASSERT(p_res != 0)

        p_res->fldNum = resourceData[4];
        p_res->ppFld  = (p_res->fldNum != 0 ? new (heap, 0) JPAFieldBlock*[p_res->fldNum] : nullptr);
        JUT_ASSERT((p_res->ppFld != 0) || (p_res->fldNum == 0));
        
        p_res->keyNum   = resourceData[5];
        p_res->ppKey     = (p_res->keyNum != 0 ? new (heap, 0) JPAKeyBlock*[p_res->keyNum] : nullptr);
        JUT_ASSERT((p_res->ppKey != 0) || (p_res->keyNum == 0));
        
        int fld_no       = 0;
        int key_no       = 0;
        p_res->texNum    = resourceData[6];
        p_res->texIdxTbl = nullptr;
        resourceOffset += 8;
        p_res->mUsrIdx = *(u16*)resourceData;

        JUT_ASSERT((p_res->texIdxTbl != 0) || (p_res->texNum == 0)); // not sure what members should be renamed
        for (int j = 0; j < GetTypeFromByteStream(resourceData, u16, 2); j++) {
            const u8 *blockData = (p1 + resourceOffset);
            int blockLength     = GetTypeFromByteStream(blockData, int, 4);
            switch (GetTypeFromByteStream(blockData, int, 0)) {
            case 'FLD1':
                p_res->ppFld[fld_no] = new (heap, 0) JPAFieldBlock(blockData, heap);
                JUT_ASSERT(p_res->ppFld[fld_no] != 0);
                fld_no++;
                break;
            case 'KFA1':
                p_res->ppKey[key_no] = new (heap, 0) JPAKeyBlock(blockData);
                JUT_ASSERT(p_res->ppKey[key_no] != 0);
                key_no++;
                break;
            case 'BEM1':
                p_res->pDyn = new (heap, 0) JPADynamicsBlock(blockData);
                JUT_ASSERT(p_res->pDyn != 0);
                break;
            case 'BSP1':
                p_res->pBsp = new (heap, 0) JPABaseShape(blockData, heap);
                JUT_ASSERT(p_res->pBsp != 0);
                break;
            case 'ESP1':
                p_res->pEsp = new (heap, 0) JPAExtraShape(blockData);
                JUT_ASSERT(p_res->pEsp != 0);
                break;
            case 'SSP1':
                p_res->pCsp = new (heap, 0) JPAChildShape(blockData);
                JUT_ASSERT(p_res->pCsp != 0);
                break;
            case 'ETX1':
                p_res->pEts = new (heap, 0) JPAExTexShape(blockData);
                JUT_ASSERT(p_res->pEts != 0);
                break;
            case 'TDB1':
                p_res->texIdxTbl = (u16*)(blockData + 8);
                break;
            default:
                JUT_WARNING_F2("JPA : wrong type block in jpc file %x\n", resourceData);
                break;
            }
            resourceOffset += blockLength;
        }
    }
}

void JPAResourceLoader::load_jpc(const u8 *p1, JPAResourceManager *p_res_mgr)
{
	JKRHeap *heap         = p_res_mgr->pHeap;
	p_res_mgr->resMaxNum  = GetTypeFromByteStream(p1, u16, 0x08);
	p_res_mgr->texMaxNum  = GetTypeFromByteStream(p1, u16, 0x0A);
	p_res_mgr->pResAry    = new (heap, 0) JPAResource*[p_res_mgr->resMaxNum];
	p_res_mgr->pTexAry    = new (heap, 0) JPATexture*[p_res_mgr->texMaxNum];
#line 199
	JUT_ASSERT((p_res_mgr->pResAry != 0) && (p_res_mgr->pTexAry != 0))

    int resourceOffset = 0x10;
    
	for (int i = 0; i < GetTypeFromByteStream(p1, u16, 0x08); i++) {
        const u8 *resourceData = (p1 + resourceOffset);

#line 210
		JPAResource *p_res     = new (heap, 0) JPAResource();
        JUT_ASSERT(p_res != 0)

#line 214
		p_res->fldNum = resourceData[4];
		p_res->ppFld  = (p_res->fldNum != 0 ? new (heap, 0) JPAFieldBlock*[p_res->fldNum] : nullptr);
		JUT_ASSERT((p_res->ppFld != 0) || (p_res->fldNum == 0));

#line 219
        p_res->keyNum   = resourceData[5];
		p_res->ppKey     = (p_res->keyNum != 0 ? new (heap, 0) JPAKeyBlock*[p_res->keyNum] : nullptr);
		JUT_ASSERT((p_res->ppKey != 0) || (p_res->keyNum == 0));
        
		p_res->texNum    = resourceData[6];
		p_res->texIdxTbl = nullptr;
        p_res->mUsrIdx   = *(u16*)resourceData;
		resourceOffset  += 8;		
        int fld_no       = 0;
		int key_no       = 0;
		for (int j = 0; j < GetTypeFromByteStream(resourceData, u16, 2); j++) {
			u32 type = GetTypeFromByteStream((p1 + resourceOffset), u32, 0);
            int blockLength     = GetTypeFromByteStream(p1 + resourceOffset, int, 4);
			
			switch (type) {
			case 'FLD1':
#line 243
				p_res->ppFld[fld_no] = new (heap, 0) JPAFieldBlock((p1 + resourceOffset), heap);
				JUT_ASSERT(p_res->ppFld[fld_no] != 0);
                fld_no++;
                break;
			case 'KFA1':
				p_res->ppKey[key_no] = new (heap, 0) JPAKeyBlock((p1 + resourceOffset));
				JUT_ASSERT(p_res->ppKey[key_no] != 0);
                key_no++;
                break;
			case 'BEM1':
				p_res->pDyn = new (heap, 0) JPADynamicsBlock((p1 + resourceOffset));
				JUT_ASSERT(p_res->pDyn != 0);
                break;
			case 'BSP1':
				p_res->pBsp = new (heap, 0) JPABaseShape((p1 + resourceOffset), heap);
				JUT_ASSERT(p_res->pBsp != 0);
                break;
			case 'ESP1':
				p_res->pEsp = new (heap, 0) JPAExtraShape((p1 + resourceOffset));
				JUT_ASSERT(p_res->pEsp != 0);
                break;
			case 'SSP1':
				p_res->pCsp = new (heap, 0) JPAChildShape((p1 + resourceOffset));
				JUT_ASSERT(p_res->pCsp != 0);
                break;
			case 'ETX1':
				p_res->pEts = new (heap, 0) JPAExTexShape((p1 + resourceOffset));
				JUT_ASSERT(p_res->pEts != 0);
                break;
			case 'TDB1':
				p_res->texIdxTbl = (u16*)((p1 + resourceOffset) + 8);
				break;
            default: JUT_WARNING_F2("JPA : wrong type block in jpc file %d %x\n", GetTypeFromByteStream(resourceData, u16, 2), resourceOffset); break;
            }
			resourceOffset += blockLength;
		}
		p_res->init(heap);
		p_res_mgr->registRes(p_res);
	}

	resourceOffset = GetTypeFromByteStream(p1, int, 0xC);
	for (int i = 0; i < GetTypeFromByteStream(p1, u16, 0xA); i++) {
#line 296
		int blockLength     = GetTypeFromByteStream(p1 + resourceOffset, int, 4);
        JPATexture *p_tex = new (heap, 0) JPATexture(p1 + resourceOffset);
        JUT_ASSERT(p_tex != 0);
        p_res_mgr->registTex(p_tex);
        resourceOffset += blockLength;
	}
}
