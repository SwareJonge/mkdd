#include "JSystem/JUtility/JUTAssert.h"

#include "JSystem/JAudio/Interface/JAISeqMgr.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/System/JASBankTable.h"

#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAIStreamMgr.h"

// TODO: move this mess to the header(s) they belong in

DECL_WEAK JAIStreamMgr *JASGlobalInstance<JAIStreamMgr>::sInstance(false);
DECL_WEAK JAISeqMgr *JASGlobalInstance<JAISeqMgr>::sInstance;
DECL_WEAK JAISeMgr *JASGlobalInstance<JAISeMgr>::sInstance;
DECL_WEAK JAISoundStarter *JASGlobalInstance<JAISoundStarter>::sInstance;
DECL_WEAK JASDefaultBankTable *JASGlobalInstance<JASDefaultBankTable>::sInstance;

#ifdef DEBUG
#pragma sym on
#endif

JAIAudience::~JAIAudience()
{
    #line 14
    JUT_ASSERT(! JAISeMgr::getInstance() || ( JAISeMgr::getInstance() ->getAudience() != this ))
    JUT_ASSERT(! JAISeqMgr::getInstance() || ( JAISeqMgr::getInstance() ->getAudience() != this ))
    JUT_ASSERT(! JAIStreamMgr::getInstance() || ( JAIStreamMgr::getInstance() ->getAudience() != this ))
}

#include "JSystem/JAudio/JASFakeMatch5.h"

#ifdef DEBUG
#pragma sym off
#endif