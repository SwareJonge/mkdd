# This file is needed to fix __sinit functions unless i start decomping this. problem: Dolphin.a reguires GC MW 1.2.5 instead of 2.5 which the game uses

.include "macros.inc"

.global __init_cpp
__init_cpp:
/* 800F31D8 7C0802A6 */ mflr        r0
/* 800F31DC 90010004 */ stw         r0, 4(r1)
/* 800F31E0 9421FFF0 */ stwu        r1, -0x10(r1)
/* 800F31E4 93E1000C */ stw         r31, 0xc(r1)
/* 800F31E8 3C608031 */ lis         r3, _ctors@ha
/* 800F31EC 380360A0 */ addi        r0, r3, _ctors@l
/* 800F31F0 7C1F0378 */ mr          r31, r0
/* 800F31F4 48000004 */ b           lbl_800f31f8
lbl_800f31f8:
/* 800F31F8 48000004 */ b           lbl_800f31fc
lbl_800f31fc:
/* 800F31FC 48000010 */ b           lbl_800f320c
lbl_800f3200:
/* 800F3200 7D8803A6 */ mtlr        r12
/* 800F3204 4E800021 */ blrl        
/* 800F3208 3BFF0004 */ addi        r31, r31, 0x4
lbl_800f320c:
/* 800F320C 819F0000 */ lwz         r12, 0(r31)
/* 800F3210 280C0000 */ cmplwi      r12, 0
/* 800F3214 4082FFEC */ bne+        lbl_800f3200
/* 800F3218 80010014 */ lwz         r0, 0x14(r1)
/* 800F321C 83E1000C */ lwz         r31, 0xc(r1)
/* 800F3220 38210010 */ addi        r1, r1, 0x10
/* 800F3224 7C0803A6 */ mtlr        r0
/* 800F3228 4E800020 */ blr         
