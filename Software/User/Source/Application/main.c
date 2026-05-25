/**
    @file
    @version  V1.0.0
    @author   Leo.Li
    @date     2024-7-15
    @brief    该文件包含了...

    @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

    /********************************************************************************
    Header Definition
********************************************************************************/
#include <MyProject.h>
#include <SanityCheck.h>
uint8 data g_1mTick = 0; // 1ms滴答信号，每隔1ms在SYSTICK定时器被置1，需在大循环使用处清零
uint16 ut;
/**
    @brief        Function Definition
*/
static void HardwareInit(void);
static void SoftwareInit(void);
void VREFConfigInit(void);

/**
    @brief     硬件初始化，初始化需要使用的硬件设备配置，配置：运放电压、运放初始化、ADC初始化、Driver初始化
              其他的可根据实际需求加。
    @date      2023-07-14
*/
static void HardwareInit(void)
{
    // 为提高芯片的抗干扰能力，降低芯片功耗，请在具体项目时，将不需要用的GPIO默认都配置为输入上拉。
    // 具体配置可在GPIO_Default_Init设置。
    GPIO_Default_Init();
    /************************硬件外设初始化**************************/
    CMP3_Init();
    CMP3_Interrupt_Init();
//    CMP4_OverCurrent_Init();
    /*********输出仿真信号**************/
    #if (DISABLE_FG_OUTPUT)
//    SetReg(CMP_CR3, DBGSEL1 | DBGSEL0, GP13_DBG_Conf0);                 // 输出Debug信号选择，输出到P1.3端口
    #endif
//		SetBit(PH_SEL1, CMPSELCT0);
//		
//    SetReg(CMP_CR3, CMPSEL2 | CMPSEL1 | CMPSEL0, GP03_DBG_Conf1);       // CMP输出信号选择，输出到P0.7端口

    GPIO_Init();            /* IO口初始话化 */
    ADC_Init();             /* 初始化需要在DR之前 */
    SetBit(ADC_CR, ADCBSY); // 使能ADC
    Driver_Init();    /* Driver初始化*/
    AMP_Init();       /* 运放初始化  */
    /*  SYSTICK定时器配置  */
    ClrBit(IP2, PSYSTICK1);
    SetBit(IP2, PSYSTICK0);     /* 1ms定时中断优先级别为1 */
    SetBit(SYST_CR, SYST_SEL0); /* @01: 0.25ms @10: 0.5ms @11: 1ms */
    SetBit(SYST_CR, SYST_SEL1); /* SYST_CR[ SYST_SEL]不为 0 自动使能 Systick 中断 */
    EA = 1;
}

/**
    @brief        部分变量初始化，上电运行一次
    @date         2022-07-14
*/
static void SoftwareInit(void)
{
    MotorcontrolInit();
    mcState = mcBrake;
    mcFaultSource = FaultNoSource;
}

/**
    @brief        参考电压，偏置电压配置
    @date         2022-07-14
*/
void VREFConfigInit(void)
{
    /************************VREF&VHALF Config************************/
    #if (HW_ADC_VREF == VREF3_0)
    {
        SetBit(VREF_VHALF_CR, VRVSEL1); // 00-->4.5V   01-->VDD5
        ClrBit(VREF_VHALF_CR, VRVSEL0); // 10-->3.0V   11-->4.0V
    }
    #elif (HW_ADC_VREF == VREF4_0)
    {
        SetBit(VREF_VHALF_CR, VRVSEL1); // 00-->4.5V   01-->VDD5
        SetBit(VREF_VHALF_CR, VRVSEL0); // 10-->3.0V   11-->4.0V
    }
    #elif (HW_ADC_VREF == VREF4_5)
    {
        ClrBit(VREF_VHALF_CR, VRVSEL1); // 00-->4.5V   01-->VDD5
        ClrBit(VREF_VHALF_CR, VRVSEL0); // 10-->3.0V   11-->4.0V
    }
    #elif (HW_ADC_VREF == VREF5_0)
    {
        ClrBit(VREF_VHALF_CR, VRVSEL1); // 00-->4.5V   01-->VDD5
        SetBit(VREF_VHALF_CR, VRVSEL0); // 10-->3.0V   11-->4.0V
    }
    #endif
    #if (VREF_OUT_EN)
    SetBit(P0_AN, P07); // VREF 输出  外接电容
    #endif
    SetBit(VREF_VHALF_CR, VHALFEN); // 使能VHLFEN
    SetBit(VREF_VHALF_CR, VREFEN);  // 使能VREF
    /* ---------------------------------------------------------------------- */
}



/**
    @brief        主函数，大循环运行偏置电流采集函数，电机状态机控制函数，以及环路响应函数
    @date         2023-07-14
*/
void main(void)
{
    uint16 PowerUpCnt = 0;
    
    /* ----- 上电空指令延时 等待系统稳定 ----- */
    for (PowerUpCnt = 0; PowerUpCnt < SystemPowerUpTime; PowerUpCnt++);

    VREFConfigInit(); /* ADC参考电压电压配置 */
    AMP_Init();       /* 运放初始化 */
    /* ----- 部分变量初始化 ----- */
    SoftwareInit();
    /* ----- 硬件初始化，配置MCU外设 ----- */
    HardwareInit();

    WatchDogConfig(200, Enable);

    #if ((FG_MODE == DISABLE_FG_OUTPUT) && (SPEED_MODE != PWMMODE)) /*DEBUG小工具，量产程序可以删除*/
    //  SPI使能
    ClrBit(OL_CR, OLCT1);
    SetBit(OL_CR, OLCT0);
    ClrBit(OL_CR, OLSEL);
    #endif
    EA = 1;
    mcState = mcReady;
    #if (SPEED_MODE == PWMMODE)
    {
        Timer3_Init();
    }
		#elif (SPEED_MODE == UARTMODE)
		{
			UART_Init();
		}
    #endif
    #if (FG_MODE == HARD_TIMFG_OUTPUT)
    Timer4_Init();
    SO_Var.refreshTimes = FG_INITIAL_DELAY_TIMES;
    #endif

    while (1)
    {
        GetCurrentOffset(); // 获取电流采样偏置电压
        MC_Control();           // 电机控制状态机
        
        /* -----1ms处理函数----- */
        if (g_1mTick)
        {
            g_1mTick = 0;       // 控制逻辑处理
            TickCycle_1ms();
        }
    }
}


