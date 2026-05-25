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


#include <definition.h>
#include <Customer.h>
#include <Parameter.h>
#include <Driver.h>

/**
 * @brief        Derier输出配置初始化
 * @date         2024-7-15
 */
void Driver_Init(void)
{
    DRV_ARR = PWM_VALUE_LOAD;    // 载波频率的周期值
    DRV_DTR = PWM_LOAD_DEADTIME; // 死区时间
    DRV_DR = 0;
	DRV_OUT = 0x00; // 空闲电平，默认高电平有效
    /*驱动有效电平和空闲电平*/
    #if (PWM_LEVEL_MODE == HIGH_LEVEL)
    {
        DRV_CMR = 0x00; // UH/VH/WH UL/VL/WL 互补       
    }
    #elif (PWM_LEVEL_MODE == LOW_LEVEL)
    {
        DRV_CMR = 0x0fc0; // UH/VH/WH UL/VL/WL 互补
		SetBit(PI_CR, HINV);
		SetBit(PI_CR, LINV);
    }
    #elif (PWM_LEVEL_MODE == UP_H_DOWN_L)
    {
        DRV_CMR = 0x0A80; // UH/VH/WH UL/VL/WL 互补
		
		ClrBit(PI_CR, HINV);
		SetBit(PI_CR, LINV);
    }
    #elif (PWM_LEVEL_MODE == UP_L_DOWN_H)
    {
        DRV_CMR = 0x0a80; // UH/VH/WH UL/VL/WL 互补
		
		SetBit(PI_CR, HINV);
		ClrBit(PI_CR, LINV);
    }
    #endif //end PWM_Level_Mode
    /*  -------------------------------------------------------------------------------------------------
    [7]    TIM2步进电机模式的输入模式选择
    [6:3]  RSV
    [2]    计数模式         0--三角波模式     1--锯齿波模式(FOC不能选择)
    [1]    上桥反向使能     0--不使能         1--使能
    [0]    下桥反向使能     0--不使能         1--使能
    -------------------------------------------------------------------------------------------------*/
    /*驱动有效电平和空闲电平*/
    /*清中断标志位，使能中断，配置比较值，设置中断优先级*/
    ClrBit(INT_SR2, DCIF); // 清除DRV中断标志位
    //    SetBit(DRV_CR1 , DRV_MD);  // 0-->三角波       1-->锯齿波
    /**************************************************
    DRV比较匹配中断模式
    当计数值等于DRV_COMR时，根据DCIM的设置判断是否产生中断标记
    
    Driver 计数器从0 开始计数到DRV_ARR/2 - 1，产生上溢事件，然后向下计数到0
    
    00：不产生中断        01：上升方向
    10：下降方向          11：上升/下降方向
    *************************************************/
    SetBit(DRV_CR1, DCIM0);
    ClrBit(DRV_CR1, DCIM1);
    /*设置DRV计数器的比较匹配值，当DRV计数值与COMR相等时，根据DRV_CR1寄存器的DCIM是否产生比较匹配事件*/
    //    DRV_COMR = (DRV_ARR >> 2) - 1;
		
//    DRV_COMR = 10;
		
		DRV_COMR = DrvCOMRValue;
		
    SetBit(IP0, PDRV1); // 中断优先级设置为2，优先级低于FO硬件过流
    ClrBit(IP0, PDRV0);
    ClrBit(DRV_CR1, DCIP);  // 0-->1个计数周期产生中断  1-->2个计数周期产生中断
    ClrBit(DRV_CR0, SVEN); // FOC模块不使能
    ClrBit(DRV_CR0, MESEL);//BLDC模式
    SetBit(DRV_CR0, DRPE);  // 计数器比较值预装载使能      0-->Disable               1-->Enable
    SetBit(DRV_CR0, DRVOE); // Driver输出使能              0-->Disable              1-->Enable
    SetBit(DRV_CR0, DRVEN);
    MOEH = 0; MOEL = 0;
}
