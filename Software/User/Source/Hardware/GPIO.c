/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      xxx.c
 * @author    Fortiortech  Appliction Team
 * @since     Create:2022-08-05
 * @date      Last modify:2022-08-05
 * @note      Last modify author is Leo.li
 * @brief
 */

#include <GPIO.h>
#include <Customer.h>
#include <definition.h>
/**
 * @brief      为提高芯片的抗干扰能力，降低芯片功耗，请在具体项目时，将不需要用的GPIO默认都配置为输入上拉。
 */
void GPIO_Default_Init(void)
{
    P0_OE = 0;
    P0_PU =  P01 | P02 | P03 | P04 | P06 | P07; // 需确认这些端口能接受上拉
    P1_OE = 0;
    P1_PU =  P11 | P12 | P16 | P17; // 需确认这些端口能接受上拉
}

/**
 * @brief        GPIO输出配置初始化
 * @date         2022-09-13
 */
void GPIO_Init(void)
{
	#if (FG_MODE == SOFT_TIMFG_OUTPUT)
	{
		SetBit(P0_OE, P00);
		SetBit(P0_PU, P00);
	}
	#elif (FG_MODE == HARD_TIMFG_OUTPUT)
	{
		SetBit(P1_OE, P13);
		SetBit(P1_PU, P13);
	}
	#endif
	SetBit(P0_OE, P00);
	SetBit(P0_OE, P03);
	SetBit(P0_AN, P04 | P05 | P06);
}
