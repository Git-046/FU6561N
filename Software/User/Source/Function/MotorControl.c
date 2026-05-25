/**
 * @file
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2025-2-15
 * @brief    该文件包含了...
 *
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
    Header Definition
********************************************************************************/
#include <MyProject.h>
/* Private variables ----------------------------------------------------------------------------*/

MotStateType  data  mcState;
MCRAMP        xdata MotorSpeed;
MOTORCTRL     xdata McCtrl;
SVPWMParaType xdata SvpwmPara;
PWMControl_TypeDef xdata mcPWMCtrl;
rotate_Signal_HandleType xdata SO_Var;
MotStaM McStaSet;

/**
 * @brief     电机控制状态机
 * @warning   电机的状态只能在电机状态控制中切换，禁止在其他地方切换电机状态
 * @date      2025-2-14
 */
void MC_Control(void)
{
    if (mcFaultSource != FaultNoSource)
    {
        mcState   = mcFault;
    }
    
    switch (mcState)
    {
        case mcReady:
            Motor_Ready();
            
            if ((mcCurOffset.OffsetFlag == 1) && (isCtrlPowOn == 1))
            {
                mcState = mcInit;
                MotorSpeed.FR      = FR_MODE;
                mcCurOffset.OffsetFlag = 0;
				mcCurOffset.OffsetCount = 0;
                /*****电机状态机时序变量***********/
                McStaSet.SetMode       = 0;
            }
            
            break;
            
        case mcInit:
            if (isCtrlPowOn == 0)
            {
                mcState = mcStop;
            }
            else
            {
                if (mcCurOffset.OffsetFlag == 1)
                {
                    Motor_Init(); 
                    McCtrl.State_Count = CHARGE_TIME;
                    mcState = mcCharge;
                }
            }
            
            break;
            
        case mcCharge:
            Motor_Charge();
            #if (IPMTEST ==Enable)
            {
                ;
            }
            #else
            {
                if (McCtrl.State_Count == 0)
                {
                    MOEH = 0; MOEL = 0;     // 关闭输出
                    DRV_CMR = 0x00;         // 不需要配置电平
                    McCtrl.State_Count = TAILWIND_TIME;
                    mcState            = mcTailWind;
                }
            }
            #endif
            break;
            
        case mcTailWind:
            if (isCtrlPowOn == 0)
            {
                mcState = mcStop;
            }
            else
            {
                if (McStaSet.SetFlag.TailWindSetFlag == 0)
                {
                    McStaSet.SetFlag.TailWindSetFlag = 1;
                    BEMFDetectInit();
                    McCtrl.Start_Mode = TAILWIND_DECT;
                }
                else
                {
                    if (McCtrl.State_Count > 0)
                    {
                        Motor_TailWind();
                    }
                    else
                    {
                        if(McCtrl.Start_Mode == TAILWIND_DECT) //没有检测到任何状态
						    McCtrl.Start_Mode   = STATIC_START;
                        /*静止启动先刹车一段时间*/
                        MC_Break();
                        if(mcBemf.BrakeTime <= 0)
                        {
                            CMP_CR0 = CMP_CR0 & 0xc0;			// 关CMP0/1/2中断使能
                            ClrBit(TIM2_CR1, T2EN);            // 停止计数
                            mcState   = mcAlign;                 /* 进入启动状态 */
                        }
                        
                    }
                }
            }
            
            break;
            
        case mcAlign:
            if (isCtrlPowOn == 0)
            {
                mcState = mcStop;
            }
            else
            {
            #if(ALIGN_MODE == ALIGN_NOMAL || ALIGN_MODE == ALIGN_TEST)
            {
                Motor_Align();                      /* 启动前先定位100ms */
				
				if( McCtrl.State_Count == 0)
				{
                    #if(ALIGN_MODE != ALIGN_TEST)
						McCtrl.State_Count = ALIGN_TIME;
						mcState            = mcStart;       /* 进入启动状态 */
						McStaSet.SetFlag.StartSetFlag = 0;
                    #endif
				}
				else if (McCtrl.State_Count > (ALIGN_TIME - CurrentRampTime))
				{
						SquStart.StartCurrentValue = ALIGN_CurrentStart + (CurrentRampSlope * (ALIGN_TIME - McCtrl.State_Count));
				}
				else
				{
						SquStart.StartCurrentValue = ALIGN_CurrentEnd;
				}
            }
            #elif (ALIGN_MODE == ALIGN_DSIABLE)
            {
                mcState = mcStart;       /* 进入启动状态 */
            }
            #endif
            }
            
            break;
            
        case mcStart: // 强拖启动
            if (isCtrlPowOn == 0)
            {
                mcState = mcStop;
            }
            else
            {
                if (McCtrl.Start_Mode == TAILWIND_START)
                {
                    McStaSet.SetFlag.RunFlag = 1;    //顺风切入

#if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
                    {
                        McCtrl.LoopRampValue = mcBemf.BEMFSpeed;
                    }
#endif
                }
                else
                {
                    Motor_Start();                 /* 电机静止启动初始化 */
                }
                
                /* 切入环路运行参数初始化*/
                if (McStaSet.SetFlag.RunFlag == 1)
                {
#if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
                    {
                        if(McCtrl.Start_Mode == STATIC_START)
                            McCtrl.LoopRampValue = S_Value(SwitchSVPWMSpeed);
                    }
#elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
                    {
                        McCtrl.LoopRampValue = McCtrl.mcPowerFlt + StartPowerRamp;
                    }
#endif
                    mcState = mcRun;
                    PI_Init();
                    #if(OverSpeedLimitEnable)
                        PI2_Init();
                        PID2.ValueOut = LOOP_PIMax;
                        PID2.ValueOuti32 = (int32)PID2.ValueOut * 32768;
                    #endif
                    PID.ValueOut = McCtrl.mcSvUq;
					PID.ValueOuti32 = PID.ValueOut*32768;
                    McCtrl.RUN_ECycleCnt = 0;
                    SvpwmPara.Sv_SpeedFltFlag = 0;
                }
            }
            
            break;
            
        case mcRun:
            if (isCtrlPowOn == 0)
            {
                mcState = mcStop;
            }
            else
            {
                Motor_Run();
            }
            break;
            
        case mcStop:
            MC_Stop();

            mcState = mcReady;
            break;
            
        case mcBrake:
            MC_Break();
            mcState = mcReady;
            break;
            
        case mcFault:
            MC_Stop();
			if (mcFaultSource == FaultNoSource)
			{
				mcState   = mcReady;
			}
            break;
            
        default:
            break;
    }
}