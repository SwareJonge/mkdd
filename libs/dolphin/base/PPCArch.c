#include <dolphin/base.h>

union FpscrUnion
{
    f64 f;
    struct
    {
        u32 fpscr_pad;
        u32 fpscr;
    }   u;
};

void PPCMthid0 ( u32 newHID0 );

asm u32 PPCMfmsr (void)
{
    nofralloc
    mfmsr r3
    blr
}

asm void PPCMtmsr (register u32 newMSR)
{
    nofralloc
    mtmsr newMSR
    blr
}

void PPCOrMsr(void)
{
	// UNUSED FUNCTION
}

void PPCAndMsr(void)
{
	// UNUSED FUNCTION
}

void PPCAndCMsr(void)
{
	// UNUSED FUNCTION
}

asm u32 PPCMfhid0 (void)
{
    nofralloc
    mfspr r3, HID0
    blr
}

asm void PPCMthid0 (register u32 newHID0)
{
    nofralloc
    mtspr HID0, newHID0
    blr
}

void PPCMfhid1(void)
{
	// UNUSED FUNCTION
}

asm u32 PPCMfl2cr (void)
{
    nofralloc
    mfspr r3, L2CR
    blr
}

asm void PPCMtl2cr (register u32 newL2cr)
{
    nofralloc
    mtspr L2CR, newL2cr
    blr
}

__declspec ( weak ) asm void PPCMtdec ( register u32 newDec )
{
    nofralloc
    mtdec newDec
    blr
}

void PPCMfdec(void)
{
	// UNUSED FUNCTION
}

asm void PPCSync (void)
{
    nofralloc
    sc
    blr
}

asm void PPCEieio(void) {
    nofralloc
    mfmsr       r5
    rlwinm      r6, r5, 0, 0x11, 0xf
    mtmsr       r6
    mfspr       r3, hid0
    ori         r4, r3, 8
    mtspr       hid0, r4
    isync
    eieio
    isync

    mtspr       hid0, r3
    mtmsr       r5
    isync

    blr
}

__declspec ( weak ) asm void PPCHalt (void) //spins infinitely
{
    nofralloc

    sync

_spin:
    nop
    li r3, 0
    nop
    b _spin

    // NEVER REACHED
}

asm void PPCMfmmcr0(void)
{
	nofralloc
	mfspr r3, MMCR0
	blr
}

asm void PPCMtmmcr0 (register u32 newMmcr0)
{
    nofralloc
    mtspr       MMCR0, newMmcr0
    blr
}

asm void PPCMfmmcr1(void)
{
    nofralloc
    mfspr       r3, MMCR1
    blr
}


asm void PPCMtmmcr1 (register u32 newMmcr1)
{
    nofralloc
    mtspr       MMCR1, newMmcr1
    blr
}

asm void PPCMfpmc1(void)
{
    nofralloc
    mfspr       r3, PMC1
    blr
}


asm void PPCMtpmc1 (register u32 newPmc1)
{
    nofralloc
    mtspr       PMC1, newPmc1
    blr
}

asm void PPCMfpmc2(void)
{
    nofralloc
    mfspr       r3, PMC2
    blr
}

asm void PPCMtpmc2 (register u32 newPmc2)
{
    nofralloc
    mtspr       PMC2, newPmc2
    blr
}

asm void PPCMfpmc3(void)
{
    nofralloc
    mfspr       r3, PMC2
    blr
}

asm void PPCMtpmc3 (register u32 newPmc3)
{
    nofralloc
    mtspr       PMC3, newPmc3
    blr
}

asm void PPCMfpmc4(void)
{
    nofralloc
    mfspr       r3, PMC4
    blr
}

asm void PPCMtpmc4 (register u32 newPmc4)
{
    nofralloc
    mtspr       PMC4, newPmc4
    blr
}

asm void PPCMfsia(void)
{
    nofralloc
    mfspr       r3, SIA
    blr
}

asm void PPCMtsia(register u32 newSia)
{
    nofralloc
    mtspr       SIA, newSia
    blr
}

u32 PPCMffpscr(void)
{
    union FpscrUnion m;


    asm
    {
        mffs f31
        stfd f31, m.f;
    }

    return m.u.fpscr;
}

void PPCMtfpscr(register u32 newFPSCR)
{
    union FpscrUnion m;

    asm
    {
        li    r4, 0
        stw   r4, m.u.fpscr_pad;
        stw   newFPSCR, m.u.fpscr
        lfd   f31, m.f
        mtfsf 0xff, f31
    }
}

asm u32 PPCMfhid2 ( void )
{
    nofralloc
    mfspr r3, 0x398
    blr
}

asm void PPCMthid2 ( register u32 newhid2 )
{
    nofralloc
    mtspr 0x398, newhid2
    blr
}

asm u32 PPCMfwpar(void)
{
    nofralloc
    sync
    mfspr r3, WPAR
    blr
}

asm void PPCMtwpar ( register u32 newwpar )
{
    nofralloc
    mtspr WPAR, newwpar
    blr
}

asm void PPCMfdmaU(void)
{
    nofralloc
    mfspr       r3, DMA_U
    blr
}

asm void PPCMfdmaL(void)
{
    nofralloc
    mfspr       r3, DMA_L
    blr
}

void PPCMtdmaU(void)
{
	// UNUSED FUNCTION
}

void PPCMtdmaL(void)
{
	// UNUSED FUNCTION
}

void PPCMfpvr(void)
{
	// UNUSED FUNCTION
}

void PPCEnableSpeculation(void)
{
	// UNUSED FUNCTION
}

void PPCDisableSpeculation (void)
{
    PPCMthid0(PPCMfhid0() | HID0_SPD);
}

asm void PPCSetFpIEEEMode(void)
{
    nofralloc
    mtfsb0      4*7+1
    blr
}

asm void PPCSetFpNonIEEEMode (void)
{
    nofralloc
    mtfsb1 29
    blr
}
// clang-format on