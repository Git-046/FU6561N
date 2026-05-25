/**
    @file
    @version  V1.0.0
    @author   Leo.Li
    @date     2025-2-15
    @brief    该文件包含了...

    @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

    /********************************************************************************
    Header Definition
********************************************************************************/

#include <MyProject.h>

///< SPI debug 输出通道缓存，SPI调试器会将该变量值进行输出
uint16 xdata Spidebug[4];

/**
    @brief        低于预警中断与过温中断
    @brief        开启低压检测 中断后,MCU会对输入电压进行监测,当输入电压低于设定值，则会触发中断
    @brief        开启过温保护中断后,MCU会对内部结温进行监测,当内部结温高于设定值，则会触发中断
    @date         2025-2-14
*/

void LVW_TSD_INT(void) interrupt 0 // LVW & TSD interrupt
{
    if (ReadBit(INT_SR3, LVWIF))
    {
        ClrBit(INT_SR3, LVWIF);
    }

    if (ReadBit(INT_SR3, TSDIF))
    {
        ClrBit(INT_SR3, TSDIF);
    }
}

/**
    @brief        External interrupt 0
    @brief        Generally used to wake up from sleep
    @date         2025-2-14
*/
void EXTERN0_INT(void) interrupt 1
{
    if (ReadBit(INT_SR4, IF0))
    {
				// MOEH = 0;MOEL = 0;  
				// MC_Stop();	
				// mcFaultSource = FaultHardOVCurrent;     // 硬件过流保护

        ClrBit(INT_SR4, IF0);
    }
}

/**
    @brief        外部中断1
    @date         2025-2-14
*/
void EXTERN1_INT(void) interrupt 2 //  外部中断1
{
    if (EXT1_IF)
    {
        EXT1_IF = 0;
    }
}

/**
    @brief        Drv中断,每个载波周期执行一次，用于处理响应较高的程序，中断优先级第二
    @date         2025-2-14
*/
void DRV_ISR(void) interrupt 3
{

    if (ReadBit(INT_SR2, DCIF)) // 比较中断
    {
        switch (mcState)
        {
        case mcAlign:
#if (ALIGN_MODE != ALIGN_DSIABLE)
            SetBit(ADC_CR, ADCBSY);
            while (ReadBit(ADC_CR, ADCBSY))
                ;
            MotorAlignProcess();
#endif
            break;
        case mcStart:
        {
            if (McCtrl.Start_Mode != TAILWIND_START)
            {
                SetBit(ADC_CR, ADCBSY);
                while (ReadBit(ADC_CR, ADCBSY))
                    ;

                SquStartProcess();

                if (SquStart.StartState == 4)
                {
                    SquStart.StartUq = SquStart.StartUq;
                    if (SquStart.StartUq > SwitchMinUq)
                    {
                        McCtrl.mcSvUq = SquStart.StartUq;
                    }
                    else
                    {
                        McCtrl.mcSvUq = SwitchMinUq;
                    }
                    SquStart.StartState = 0;
                    McCtrl.mcSv_RTheStep = RampSwitchSVPWMSpeed * (float)(SwitchCount) / (float)(SquStart.MotorSpeedCountSum);
                    SPEED_STEP = McCtrl.mcSv_RTheStep;

                    /******切入开窗正弦*****/
                    SvpwmPara.Bcor = (SvpwmPara.Base_BCOR) / McCtrl.mcSv_RTheStep;
                    MotorSpeed.mcSpeed = MotorSpeed.mcSpeedBase / SvpwmPara.Bcor;
                    SVPWM_Swtich_Wind();
                    memset(&SquStart, 0, sizeof(SquStartVar));
                    /*设置中断位置*/
                    DRV_COMR = DrvCOMR_L;
                    ClrBit(DRV_CR1, DCIM0);
                    SetBit(DRV_CR1, DCIM1);
                }
            }
        }
        break;
        case mcRun:
#if (!MANUAL_LEAD_ANGLE)
            if(McCtrl.RunMode == 0)
            {
                AdaLeadAngleSamp();
            }
#endif
            if (McCtrl.SampFlag) /*允许进行电流电压采样*/
            {
                switch (SquStart.UQRampEnable)
                {
                case 0:
                    // ADC_MASK = 0;
                    // SetBit(ADC_MASK, CH1EN | CH2EN);
                    // GP01 = 1;
                    SetBit(ADC_CR, ADCBSY);
                    break;
                case 1:
                {
                    // GP01 = 0;
                    McCtrl.mcDcbus = ADC2_DR; // 读取母线电压
                    break;
                }
                case 2:
                {
                    McCtrl.mcIbusADC = ADC1_DR; // 读取母线电压
                    McCtrl.mcIbusADC = McCtrl.mcIbusADC - mcCurOffset.I_busOffset;
                }
                }
                SquStart.UQRampEnable++;
                if (SquStart.UQRampEnable >= 3)
                    SquStart.UQRampEnable = 0;
            }

            break;
        default:
            break;
        }
        McCtrl.mcSv_Theta = THETA;
        McCtrl.SPIVar1 = SV_UQ;

        /*尽量不要在中断进行移位操作，容易导致时间超长*/
        Spidebug[0] = THETA; // TIM1_CR4 << 11; //		THETA C McCtrl.mcPowerFlt
        Spidebug[1] = McCtrl.SPIVar1; //TIM1_CR4 << 11;   //TIM1__UFLP;		SV_UQ			McCtrl.mcDcbusFlt  McCtrl.mcIbusFlt
        // Spidebug[2] = McCtrl.mcPowerFlt << 2;

        INT_SR2 = (INT_SR2 | SYSTIF) & (~DCIF);
    }

}

/**
    @brief        Timer2中断服务函数
    @note
    @date         2025-02-14
*/
void TIM2_INT(void) interrupt 4
{
    if (ReadBit(INT_SR2, T2IP))
    {
        INT_SR2 = ~T2IP;
    }

    if (ReadBit(INT_SR2, T2IF)) // 溢出中断,用于判断静止,时间为349ms。
    {
        if (mcState == mcTailWind)
        {
            mcBemf.BEMFSpeed = 0; // 定时器溢出
        }

        INT_SR2 = ~T2IF;
    }

    if (ReadBit(INT_SR2, T2IR))
    {
        INT_SR2 = ~T2IR;
    }
}

/**
    @brief        定时器1中断服务函数
    @note
    @date         2025-2-14
*/
void TIM1_INT(void) interrupt 5
{
    if (ReadBit(INT_SR0, T1BDIF))
    {
        ClrBit(INT_SR0, T1BDIF);
    }

    if (ReadBit(INT_SR0, T1WOIF)) /*开窗中断*/
    {
        McCtrl.WindowFlag = 1;
        /*低载波比下考虑打开这句，要求开窗点小于150°*/
        // #if(!MANUAL_LEAD_ANGLE)
        // 		ADC_MASK = 0;
        // 		SetBit(ADC_MASK, CH5EN);
        // 		McCtrl.SampFlag = 0;
        // #endif
        ClrBit(INT_SR0, T1WOIF);
    }

    if (ReadBit(INT_SR0, T1PDIF))
    {
#if (FG_MODE == SOFT_TIMFG_OUTPUT)
        FPinONOFF;
#endif
        McCtrl.WindowFlag = 0;
        ADC_Channel_Recover(); //恢复ADC采样通道
        if (mcState == mcRun)
        {
            Fault.Stall.Stall_Position_COUNT = 0;
#if (!MANUAL_LEAD_ANGLE)
            if (McCtrl.RunMode == 0)
            {
                McCtrl.SampFinishFlag = 1;
                if ((mcRun == mcState) && (SquStart.Sector != 4)) /*防止卡死，有必要留着*/
                {
                    SquStart.Sector = 0;
                }

                if (MotorSpeed.mcSpeed_Acc == 1) /*切换高挡位时超前角不够，需要手动增加*/
                {
                    McCtrl.mcLeadAngleIntComp += Ada_Acc_Comp;
                }
            }
#endif
            SvpwmPara.Bcor = TIM1__BCOR;

            MotorSpeed.Speed_Updatflag = 1; // 速度更新标志

            if (McCtrl.RUN_ECycleCnt < 10000)
            {
                McCtrl.RUN_ECycleCnt++;
            }
            // MotorSpeed.mcSpeed = MotorSpeed.mcSpeedBase / SvpwmPara.Bcor; // 计算最新速度
            Sample_Swicth(); // 比较器 ON/OFF 采样切换
        }
        McCtrl.SampFlag = 1;
        ClrBit(INT_SR0, T1PDIF);
    }

    if (ReadBit(INT_SR0, T1BOIF))
    {
        ClrBit(INT_SR0, T1BOIF);
    }

    if (ReadBit(INT_SR0, T1ROIF))
    {
        ClrBit(INT_SR0, T1ROIF);
    }

    if (ReadBit(INT_SR0, T1WTIF))
    {
        ClrBit(INT_SR0, T1WTIF);
    }

    if (ReadBit(INT_SR0, T1ADIF))
    {
        ClrBit(INT_SR0, T1ADIF);
    }

    if (ReadBit(INT_SR0, T1ABDIF))
    {
        ClrBit(INT_SR0, T1ABDIF);
    }
}

void CMP012_INT(void) interrupt 7
{
    if ((ReadBit(CMP_SR, CMP2OUT)) || (ReadBit(CMP_SR, CMP1OUT)) || ReadBit(CMP_SR, CMP0OUT))
    {
        if (mcBemf.HighSpdStart == 0)
        {
            BemfProcess();
        }
        else if (mcBemf.HighSpdStart == 1) // 顺风参数计算完毕
        {
            if (((CMP_SR & 0x07) == 0x05 && MotorSpeed.FR == CW) || ((CMP_SR & 0x07) == 0x03 && MotorSpeed.FR == CCW)) // 03状态 对应90°启动
            {
                ClrBit(CMP_CR0, (CMP2IM1 | CMP2IM0 | CMP1IM1 | CMP1IM0 | CMP0IM1 | CMP0IM0));
                ClrBit(CMP_CR2, CMP0EN);
                BEMFCloseLoopStart();
            }
        }

        ClrBit(CMP_SR, CMP2OUT);
        ClrBit(CMP_SR, CMP1OUT);
        ClrBit(CMP_SR, CMP0OUT);
        INT_SR3 = INT_SR3 & 0xF1;
    }
}

/**
    @brief        定时器3中断服务函数
    @note         本例程中用于PWM调速信号捕获
    @date         2025-2-14
*/
void TIM3_INT(void) interrupt 9
{
    if (ReadBit(INT_SR1, T3IR))
    {
        ClrBit(INT_SR1, T3IR);
    }

    if (ReadBit(INT_SR1, T3IP))
    {
        if (mcPWMCtrl.isBusy == 0)
        {
            mcPWMCtrl.Compare = TIM3__DR;
            mcPWMCtrl.Cycle = TIM3__ARR;
            mcPWMCtrl.isBusy = 1;
        }

        ClrBit(INT_SR1, T3IP);
    }

    if (ReadBit(INT_SR1, T3IF))
    {
        if (mcPWMCtrl.isBusy == 0)
        {
            if (PWM_IO)
            {
                mcPWMCtrl.Compare = 8000;
                mcPWMCtrl.Cycle = 8000;
            }
            else
            {
                mcPWMCtrl.Compare = 1;
                mcPWMCtrl.Cycle = 8000;
            }

            mcPWMCtrl.isBusy = 1;
        }

        ClrBit(INT_SR1, T3IF);
    }
}

/**
    @brief        滴答定时器，默认用于产生1ms定时间隔
    @date         2025-2-14
 **/
void SYStick_INT(void) interrupt 10
{
    if (ReadBit(INT_SR2, SYSTIF)) // SYS TICK中断
    {
        WatchDogRefresh();
        g_1mTick = 1;
        /* 电机状态机的时序处理 */
        if (McCtrl.State_Count > 0)
        {
            McCtrl.State_Count--;
        }
        INT_SR2 = (INT_SR2 | DCIF) & (~SYSTIF);
    }
}

/**
    @brief        比较器硬件过流保护，该中断仅提供 故障码 赋值,用于状态机的切换。
                 需要开启比较器CMP3  发生过流 自动清除MOE功能
    @date         2025-2-14
 **/
void CMP3_INT(void) interrupt 12
{
    if (ReadBit(INT_SR3, CMP3IF))
    {
       mcFaultSource = FaultHardOVCurrent;
       MOEH = 0;
       MOEL = 0; // 关闭输出,防止误配置异常输出

        ClrBit(INT_SR3, CMP3IF);
    }
}


/* -------------------------------------------------------------------------------------------------
    Function Name  : UART_INT
    Description    : 串口中断
    Date           : 2020-09-12
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void UART_INT(void)  interrupt 13
{
    #if (SPEED_MODE == UARTMODE)
    {
        if(1 == TI)
        {
            Uart.RTXDataCnt++;
            
            if (Uart.RTXDataCnt < Uart.UsaTxlen)
            {
                UART_SendData(Uart.T_DATA[Uart.RTXDataCnt]);
            }
            else
            {
                UART_TX_IO_H;                           //发送引脚置高
                Uart.RTXDataCnt = 0;
                RI = 0;
                REN = 1;
            }
            TI = 0;
        }
        
        if(1 == RI)
        {
            Uart.RecevingFlag = 1;
            Uart.RXTimeOutCnt = 0;
            
            if (!Uart.ResponceFlag)
            {
                Uart.R_DATA[Uart.RTXDataCnt] = UT_DR;
                
                if (Uart.RTXDataCnt < UART_RX_BUF_SIZE)
                { 
                    Uart.RTXDataCnt++; 
                }
                
                if (Uart.R_DATA[0] == 0xAA)             //帧头
                {
                    if(Uart.RTXDataCnt >= 3)            //byte2为帧长
                    {
                        Uart.UsaRxlen = Uart.R_DATA[2]; //帧长 0~UART_RX_BUF_SIZE
                        
                        if (Uart.UsaRxlen <= UART_RX_BUF_SIZE)
                        {
                            if (Uart.RTXDataCnt >= Uart.UsaRxlen)
                            {
                                Uart.RecevingFlag = 0;
                                Uart.ResponceFlag = 1;  //接收到一帧数据
                                Uart.RTXDataCnt = 0;
                                Uart.RXTimeOutCnt = 0;
                                
    //                            REN = 0;
                                ES0 = 0;
                            }
                            else
                            {
                                REN = 1;
                                UART_TX_IO_H;
                            }
                        }
                        else
                        {
                            UART_Init();                //帧长溢出时重新初始化UART
                        }
                    }
                    else
                    {
                        REN = 1;
                        UART_TX_IO_H;
                    }
                }
                else
                {
                    UART_TX_IO_H;
                    Uart.RTXDataCnt = 0;
//                    Uart.ResponceFlag = 0;
                }
            }
            RI = 0;
        }
    }
    #endif    
}