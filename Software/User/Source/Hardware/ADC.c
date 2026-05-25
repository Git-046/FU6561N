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
#include <ADC.h>
#include <Customer.h>
#include <definition.h>
/**
 * @brief        ADC初始化
 * @date         2024-7-15
 */
void ADC_Init(void)
{
    /********************ADC 端口模拟功能设置************************/
	SetBit(P1_AN, P10);        // AD1模拟模式
    
	SetBit(P1_AN, P14);        // AD6模拟模式
	
	SetBit(P0_AN, P02);
    ClrBit(P0_OE, P02);
	
	SetBit(P0_AN, P05);
    ClrBit(P0_OE, P05);
	
	SetBit(P0_AN, P06);
    ClrBit(P0_OE, P06);
	
	SetBit(P0_AN, P07);
    ClrBit(P0_OE, P07);

#if(SPEED_MODE == SREFMODE)
    SetBit(P0_AN, P01);
    ClrBit(P0_OE, P01);
#endif

    /****************************************************************/
//		SetBit(ADC_MASK, CH1EN | CH2EN | CH3EN | CH4EN | CH5EN | CH6EN | CH7EN); 
#if(TEMP_SAMP_EN)
    SetBit(P0_AN, P03);
    ClrBit(P0_OE, P03);
    SetBit(ADC_MASK, CH1EN | CH2EN | CH3EN | CH4EN);
#else
	SetBit(ADC_MASK, CH1EN | CH2EN | CH4EN);
#endif
    ClrBit(ADC_CR, ADCIE);    // ADC中断使能
    SetBit(ADC_CR, ADCEN);    // Enable ADC0
	ADC_SCYC2 = 0x77;
}
