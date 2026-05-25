/**
 * @file     
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2024-7-15
 * @brief    该文件包含了...
 * 
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
    Header Definition
********************************************************************************/
#include <MyProject.h>

/**
 * @brief        运放初始化
 * @date         2024-7-15
 */
void AMP_Init(void)
{
		SetBit(P1_AN,P11);
	
		SetBit(P1_AN,P14);
		
		SetBit(P1_AN,P15);
		

		#if(HW_AMP_MODE == AMP_NOMAL)
		{
			 ClrBit(AMP_CR0, AMP0_GAIN2);
			 ClrBit(AMP_CR0, AMP0_GAIN1);
			 ClrBit(AMP_CR0, AMP0_GAIN0);
		}
		#elif(HW_AMP_MODE == AMP_PGA_DUAL)
		{
				#if (HW_AMPGAIN == AMP2x)
				{
					ClrBit(AMP_CR0, AMP0_GAIN2);
					ClrBit(AMP_CR0, AMP0_GAIN1);
					SetBit(AMP_CR0, AMP0_GAIN0);
				}
				#elif (HW_AMPGAIN == AMP4x)
				{
					ClrBit(AMP_CR0, AMP0_GAIN2);
					SetBit(AMP_CR0, AMP0_GAIN1);
					ClrBit(AMP_CR0, AMP0_GAIN0);
				}
				#elif (HW_AMPGAIN == AMP8x)
				{
					ClrBit(AMP_CR0, AMP0_GAIN2);
					SetBit(AMP_CR0, AMP0_GAIN1);
					SetBit(AMP_CR0, AMP0_GAIN0);
				}
				#elif (HW_AMPGAIN == AMP16x)
				{
					SetBit(AMP_CR0, AMP0_GAIN2);
					ClrBit(AMP_CR0, AMP0_GAIN1);
					ClrBit(AMP_CR0, AMP0_GAIN0);
				}
				#endif
				ClrBit(AMP_CR0, AMP0M_GND); // 禁止运放负输入端内部接地
		}
		#elif(HW_AMP_MODE == AMP_PGA_SINGEL)
		{
				#if (HW_AMPGAIN == AMP2x)
				{
					ClrBit(AMP_CR0, AMP0_GAIN2);
					ClrBit(AMP_CR0, AMP0_GAIN1);
					SetBit(AMP_CR0, AMP0_GAIN0);
				}
				#elif (HW_AMPGAIN == AMP4x)
				{
					ClrBit(AMP_CR0, AMP0_GAIN2);
					SetBit(AMP_CR0, AMP0_GAIN1);
					ClrBit(AMP_CR0, AMP0_GAIN0);
				}
				#elif (HW_AMPGAIN == AMP8x)
				{
					ClrBit(AMP_CR0, AMP0_GAIN2);
					SetBit(AMP_CR0, AMP0_GAIN1);
					SetBit(AMP_CR0, AMP0_GAIN0);
				}
				#elif (HW_AMPGAIN == AMP16x)
				{
					SetBit(AMP_CR0, AMP0_GAIN2);
					ClrBit(AMP_CR0, AMP0_GAIN1);
					ClrBit(AMP_CR0, AMP0_GAIN0);
				}
				#endif
				SetBit(AMP_CR0, AMP0M_GND); // 使能运放负输入端内部接地
		}
		#endif

		SetBit(AMP_CR0,AMP0EN);
//		SetBit(P1_AN,P16);
		SetBit(AMP_CR0, CP_EN);   //恒功率使能，用于采集平均电流，P1.0需外接1uf电容	
}
