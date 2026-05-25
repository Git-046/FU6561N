;/**
; * @file	 startup_bt.asm
; * @version	 V1.0.0
; * @author	 FortiorTech Software Team
; * @date	 2023-06-09	10:45:18
; * @brief	 This file contains	interrupt vector map for user btldr
; *          with FTM8ForgeIDE
; * @copyright Copyright(C) 2022, Fortior Technology  Co., Ltd. All rights reserved.
; */

	.module startup_bt
	.optftcc -mmcs51 --model-large
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	;.globl _main
	;.globl _Write_One_Cmd_Flash
	;.globl _Read_Flash
	;.globl _GP01
	;.globl _GP00
	
	;it is significant, don't modify it in general.
	.equ BOOT_APP_STA, 0x7E
	;it may be modified according to the app start address.
	.equ APP_STA, 0x400

;--------------------------------------------------------
; interrupt vector mapping table for 16k flash
;--------------------------------------------------------
	.area BOOT (ABS,CODE)
	.org 0x0000
		ljmp BOOT_APP_STA
	;	ljmp 0x0083
	.org 0x0003
		ljmp APP_STA + 0x03
	.ds 5
	.org 0x000b
		ljmp APP_STA + 0x0b
	.ds 5
	.org 0x0013
		ljmp APP_STA + 0x13
	.ds 5
	.org 0x001b
		ljmp APP_STA + 0x1b
	.ds 5
	.org 0x0023
		ljmp APP_STA + 0x23
	.ds 5
	.org 0x002b
		ljmp APP_STA + 0x2b
	.ds 5
	.org 0x0033
		ljmp APP_STA + 0x33
	.ds 5
	.org 0x003b
		ljmp APP_STA + 0x3b
	.ds 5
	.org 0x0043
		ljmp APP_STA + 0x43
	.ds 5
	.org 0x004b
		ljmp APP_STA + 0x4b
	.ds 5
	.org 0x0053
		ljmp APP_STA + 0x53
	.ds 5
	.org 0x005b
		ljmp APP_STA + 0x5b
	.ds 5
	.org 0x0063
		ljmp APP_STA + 0x63
	.ds 5
	.org 0x006b
		ljmp APP_STA + 0x6b
	.ds 5
	.org 0x0073
		ljmp APP_STA + 0x73
	.ds 5
	.org 0x007b
		ljmp APP_STA + 0x7b
