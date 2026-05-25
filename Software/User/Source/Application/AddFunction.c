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

/* Public variables --------------------------------------------------------- */
/* 私有變數      Private variables -------------------------------------------*/
#define  Motor_rawSpeedCurve(_S1_, _S0_, _D1_, _D0_)  ((float)(((_S1_)) - ((_S0_))) / (float)((_D1_) - (_D0_)))
ONVarible     ONOFFTest;
bool data isCtrlPowOn; // 电机运行启动标志
//    const uint16 ControlNode_Array[MOTOR_SPEEDMODE_MAX_NODE]  = MOTOR_VSPMODE_VOLT_NODE;
const uint16 ControlNode_Array[MOTOR_SPEEDMODE_MAX_NODE]  = MOTOR_PWMMODE_DUTY_NODE;

const uint16 SpeedNode_Array[MOTOR_SPEEDMODE_MAX_NODE] = MOTOR_SPEED_NODE;

const uint16 PowerNode_Array[MOTOR_SPEEDMODE_MAX_NODE] = MOTOR_POWER_NODE;

/**
    @brief        对变量取16位的绝对值
    @param[in]    value
    @return       绝对值
    @date         2025-2-14
*/
uint16 Abs_F16(int16 value)
{
    if (value < 0)
    {
        return (-value);
    }
    else
    {
        return (value);
    }
}

/**
    @brief        对变量取32位的绝对值
    @param[in]    value
    @return       绝对值
    @date         2025-2-14
*/
uint32 Abs_F32(int32 value)
{
    if (value < 0)
    {
        return (-value);
    }
    else
    {
        return (value);
    }
}


/** 
 * @brief        VSP调速信号处理
 * @date         2022-07-14 
 */
void VSPSample(void)
{
    int16 VSP;
    /*****VREF的采样获取值并滤波******/
    VSP = ADC9_DR;
    McCtrl.mcVSPFltu32.s32 = (VSP * (65536 - 62000)) + (McCtrl.mcVSPFlt * 62000); // 母线电压滤波
    McCtrl.mcVSPFlt = McCtrl.mcVSPFltu32.s16[0];
    
    if ((McCtrl.mcVSPFlt > PWM_TURNON_DUTY))                       //在ONPWMDuty-OFFPWMDutyHigh之间，电机有转速运行
    {
        isCtrlPowOn = true;  // 开机
    }
    else if ((McCtrl.mcVSPFlt < PWM_TURNOFF_DUTY))
    {
        isCtrlPowOn = false;  // 关机
    }
    
    //转速曲线计算
    if (isCtrlPowOn == true) //
    {
        #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
        {
            if (McCtrl.mcVSPFlt <= PWM_MIN_DUTY)   //最小转速运行
            {
                McCtrl.TargetValue = MC_SPEED_MIN_RPM;
            }
            else if (McCtrl.mcVSPFlt < PWM_MAX_DUTY) //调速
            {
                McCtrl.TargetValue = MC_SPEED_MIN_RPM + SPEED_K * (McCtrl.mcVSPFlt - PWM_MIN_DUTY);
            }
            else                    //最大转速运行
            {
                McCtrl.TargetValue = MC_SPEED_MAX_RPM;
            }
        }
		#elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
		{
			
			if (McCtrl.mcVSPFlt <= PWM_MIN_DUTY)   //最小功率运行
            {
                McCtrl.TargetValue = MC_POWER_MIN;
            }
            else if (McCtrl.mcVSPFlt < PWM_MAX_DUTY) //调速
            {
                McCtrl.TargetValue = MC_POWER_MIN + POWER_K * (McCtrl.mcVSPFlt - PWM_MIN_DUTY);
            }
            else                    //最大功率运行
            {
                McCtrl.TargetValue = MC_POWER_MAX;
            }
		}
        #endif
    }
    else
    {
        McCtrl.TargetValue = 0;
    }
}

/** 
 * @brief        调速信号处理包含：开关机控制、将调速信号处理成控制目标给定信号
 * @date         2022-07-14  
 */
void TargetRef_Process(void)
{

#if (SPEED_MODE == PWMMODE)
    {
        PWM_speedControl();
    }
#elif (SPEED_MODE == SREFMODE)
{
    VSPSample();
}
#elif (SPEED_MODE == NONEMODE)
    {
        isCtrlPowOn              = true;  // 开机
    #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
            {
                McCtrl.TargetValue = Motor_Target_Speed;
            }
    #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
            {
                McCtrl.TargetValue = Motor_Target_Power;
            }
    #endif
    }
#elif (SPEED_MODE == KEYMODE)
{
    isCtrlPowOn              = true;  // 开机
    KeyScan();
}
#elif (SPEED_MODE == ONOFFTEST)
{
    ONOFF_Starttest();
}
#elif (SPEED_MODE == UARTMODE)
{
    UartDealComm();
		McCtrl.TargetValue = Uart.UartTarget;
}
#endif
}

/**
    @brief        默认1ms周期服务函数，运行信号采样，调速信号处理，闭环控制，故障检测,ATO爬坡函数
                 该函数运行于大循环中，由SYSTICK定时器间隔1ms触发运行。
    @date         2025-2-14
*/
void TickCycle_1ms(void)
{
    int16 Speed_Dif = 0;
    int16 LeadAngle_TempVar = 0;
    static int16 u16TempPower = 0;

    if (mcState == mcRun)
    {
        McCtrl.mcDcbusFltu32.s32 = (McCtrl.mcDcbus * (65536 - 62000)) + (McCtrl.mcDcbusFlt * 62000); // 母线电压滤波
        McCtrl.mcDcbusFlt = McCtrl.mcDcbusFltu32.s16[0];
        McCtrl.mcIbusFltu32.s32 = (McCtrl.mcIbusADC * (65536 - 62000)) + (McCtrl.mcIbusFlt * 62000); // 电流滤波
        McCtrl.mcIbusFlt =  McCtrl.mcIbusFltu32.s16[0];
        McCtrl.BEMF_THR_H = (McCtrl.mcDcbusFlt >> 2) * 3; //比较阈值为母线的0.75倍

        /*掉电时电压降低极快，当高速下断开电源时有过冲可能。有断开电池包工况时打开以下程序*/
        if ((McCtrl.LastmcDcbus - McCtrl.mcDcbus > 3000) && (McCtrl.mcSvUq > 30000))
        {
            McCtrl.PowerDownCnt = 0;
            McCtrl.RunMode = 2;
            MOEH = 0;  MOEL = 0; // 关闭输出

        }
        McCtrl.LastmcDcbus = McCtrl.mcDcbus;
    }
    else
    {
        McCtrl.mcDcbus = ADC2_DR;                                                                // 读取母线电压
        McCtrl.mcDcbusFltu32.s32 = (McCtrl.mcDcbus * (65536 - 55000)) + (McCtrl.mcDcbusFlt * 55000); // 电流滤波
        McCtrl.mcDcbusFlt = McCtrl.mcDcbusFltu32.s16[0];

        McCtrl.mcIbusADC = ADC1_DR; // 读取母线电流
        McCtrl.mcIbusADC = McCtrl.mcIbusADC - mcCurOffset.I_busOffset;
        McCtrl.mcIbusFltu32.s32 = (McCtrl.mcIbusADC * (65536 - 55000)) + (McCtrl.mcIbusFlt * 55000); // 电流滤波
        McCtrl.mcIbusFlt = McCtrl.mcIbusFltu32.s16[0];
    }

    TargetRef_Process();

/*****FG输出*****/
#if (FG_MODE == HARD_TIMFG_OUTPUT)
    FGOutput_Update();
#endif

#if (TEMP_SAMP_EN)
    McCtrl.NTCTempFlt = ADC3_DR;
#endif
    /*启动时屏蔽超前角慢调节一段时间*/
    if (McCtrl.mcLeadAngleCount < (STARTUP_ANGLE_DIS_TIME + 10))
    {
        McCtrl.mcLeadAngleCount++;
        McCtrl.mcAdapLeadAngleEnable = 0;
    }
    else
    {
        McCtrl.mcLeadAngleCount = (STARTUP_ANGLE_DIS_TIME + 10);
        McCtrl.mcAdapLeadAngleEnable = 1;
    }

    if (mcState == mcTailWind)
    {
        if (McCtrl.Start_Mode == STATIC_START || McCtrl.Start_Mode == HEADWIND_START) // 静止启动先刹车100ms
        {
            mcBemf.BrakeTime--;
            if (mcBemf.BrakeTime <= 0)
            {
                mcBemf.BrakeTime = 0;
            }
        }
    }
    if ((mcState == mcStart) || (mcState == mcRun))
    {
        MotorSpeed.mcSpeed = MotorSpeed.mcSpeedBase / SvpwmPara.Bcor; // 计算最新速度(若转速不重要，可以多个周期计算一次)
        McCtrl.u32mcPower = (int32)(McCtrl.mcIbusFlt) * McCtrl.mcDcbusFlt;
        McCtrl.mcPower = (int16)(McCtrl.u32mcPower >> 11);
        u16TempPower = McCtrl.mcPower - McCtrl.mcPowerFlt;
        McCtrl.mcPowerFlt = McCtrl.mcPowerFlt + ((u16TempPower * 3) >> 6); //功率滤波
        if (mcState == mcRun)
        {
            // 环路控制
            Loop_response();
            // SPWM角度控制
            SVPWM_AngleAdjust();
            if(McCtrl.RunMode != 2)
            {
            #if(WeakenMode == SquMode)
            {
                if(McCtrl.mcSvUq > EnterUq && McCtrl.Mode_SwitchFlag == 0)
                {
                    McCtrl.RunMode = 1; //方波模式
                    McCtrl.Mode_SwitchFlag = 1;
                    SVPWM_to_Squ();
                }
                else if(McCtrl.mcSvUq < ExitUq && McCtrl.Mode_SwitchFlag == 1)
                {
                    McCtrl.RunMode = 0;
                    McCtrl.Mode_SwitchFlag = 0;
                    Squ_to_SVPWM();
                }                
            }
            #elif(WeakenMode == AngleMode)
            {
                if(McCtrl.mcSvUq > EnterUq || MotorSpeed.mcSpeed > WeakenSpeed)
                {
                    McCtrl.WeakenCnt++;
                    if(McCtrl.WeakenCnt > WeakenEnterWait)
                    {
                        McCtrl.WeakenCnt = WeakenEnterWait;
                        McCtrl.RunMode = 1;
                    }                      
                }
                else if(McCtrl.mcSvUq < ExitUq)
                {
                    McCtrl.WeakenCnt--;
                    if(McCtrl.WeakenCnt < 0)
                    {
                        McCtrl.WeakenCnt = 0;
                        McCtrl.RunMode = 0;
                    }
                }          
            }
            #endif
            }
            else
            {
                McCtrl.PowerDownCnt++;
                if((McCtrl.PowerDownCnt > 500)) /*等待500ms重启*/
                {
                    McCtrl.RunMode = 0;
                    isCtrlPowOn = false;
                }
            }
        }
    }
    /*不在Run状态时的采样*/
    else
    {
        ADC_Channel_Recover();
        SetBit(ADC_CR, ADCBSY);
    }
    /* 保护处理 */
    Fault_Detection();
		
		#if (SPEED_MODE == UARTMODE)
    {
        if (Uart.RXTimeOutCnt < 100)                                        //UART接收一帧数据过程中持续30ms未进接收中断计时
        {
            Uart.RXTimeOutCnt++;
        }
        
        if (Uart.UATxDelayCnt > 0)
        {
            Uart.UATxDelayCnt--;                                            // 串口接收到指令后延时回复信息。
        }
    }
    #endif    

    if(McCtrl.TargetValue == 0)
    {
        isCtrlPowOn = false;
    }

}

/**
    @brief        环路响应
    @date         2025-2-14
*/
void Loop_response(void)
{
    int16 speed_err = 0;
    McCtrl.mcLoopTime ++;
    
    if (McCtrl.mcLoopTime >= LOOP_TIME)
    {
        McCtrl.mcLoopTime = 0;
			
        #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
        {
            if (McCtrl.LoopRampValue < (McCtrl.TargetValue - SPEEDLOOP_RAMP_INC))
            {
                McCtrl.LoopRampValue += SPEEDLOOP_RAMP_INC;
                if (MotorSpeed.mcSpeed > Accele_ParaSpeed)
                {
                    PID.Kp = LOOP_KP_Accele;
                    PID.Ki = LOOP_KI_Accele;
                }
            }
            else if (McCtrl.LoopRampValue > (McCtrl.TargetValue + SPEEDLOOP_RAMP_DEC))
            {
                McCtrl.LoopRampValue -= SPEEDLOOP_RAMP_DEC;
            }
            else
            {
                McCtrl.LoopRampValue = McCtrl.TargetValue;
                if (MotorSpeed.mcSpeed > (McCtrl.TargetValue - S_Value(2000)))
                {
                    PID.Kp = LOOP_KP_Steady;
                    PID.Ki = LOOP_KI_Steady;
                }
            }
        }
        #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
        {
            if (McCtrl.LoopRampValue < (McCtrl.TargetValue - POWERLOOP_RAMP_INC))
            {
                McCtrl.LoopRampValue += POWERLOOP_RAMP_INC;
                /*静止启动，大于一定转速切换PI*/
                /*顺风启动，大于一定电周期数切换PI*/
                if (((MotorSpeed.mcSpeed > Accele_ParaSpeed) && (McCtrl.Start_Mode == STATIC_START))
                    || ((McCtrl.Start_Mode == TAILWIND_START) && (McCtrl.RUN_ECycleCnt > 20)))
                {
                    PID.Kp = LOOP_KP_Accele;
                    PID.Ki = LOOP_KI_Accele;
                }
            }
            else if (McCtrl.LoopRampValue > (McCtrl.TargetValue + POWERLOOP_RAMP_DEC))
            {
                McCtrl.LoopRampValue -= POWERLOOP_RAMP_DEC;
            }
            else
            {
                McCtrl.LoopRampValue = McCtrl.TargetValue;
                if ((McCtrl.mcPowerFlt > (McCtrl.TargetValue - P_Value(3))) 
                && (McCtrl.mcPowerFlt < (McCtrl.TargetValue + P_Value(3))))
                {
                    PID.Kp = LOOP_KP_Steady;
                    PID.Ki = LOOP_KI_Steady;
                }
                else
                {
                    PID.Kp = LOOP_KP_Accele;
                    PID.Ki = LOOP_KI_Accele;                    
                }
            }
        }
#endif
        
        #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
        {
            PID.ErrValue = McCtrl.LoopRampValue - MotorSpeed.mcSpeed;
        }
        #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
        {
            #if(OverSpeedLimitEnable)
                McCtrl.OverSpeedLimit = MOTOR_SPEED_LIMIT_RPM;
                PID2.ErrValue = McCtrl.OverSpeedLimit - MotorSpeed.mcSpeed;
                PI2_Software();
                PID.ValueMax = PID2.ValueOut;
            #endif
            PID.ErrValue = McCtrl.LoopRampValue - McCtrl.mcPowerFlt;
        }
        #endif						

		PI_Software();	
		McCtrl.mcSvUq = PID.ValueOut;

        SV_UQ         = McCtrl.mcSvUq;
#if(WeakenMode == SquMode)
        if(McCtrl.RunMode == 1)
        {
            McCtrl.mcDuty = (uint16)(((int32)McCtrl.mcSvUq * (int16)PWM_VALUE_LOAD) >> 15);
            DRV_DR = McCtrl.mcDuty;
        }
#endif
    }
}

/**========================================
    @brief    PWM 调速控制函數。
    @retval   None
============================================*/
void PWM_speedControl(void)
{
    uint16 NodeNUM = 0, t_inputValue = 0;
    static float speed_ratio = ((MC_SPEED_MAX_RPM - MC_SPEED_MIN_RPM) / (PWM_FULLSPEED_DUTY - PWM_TURNON_DUTY));
    
    //1.计算占空比
    if (mcPWMCtrl.isBusy)
    {
        if ((Abs_F32(mcPWMCtrl.Compare - mcPWMCtrl.PWMCompareOld) < 50) // 两次比较值相近，减少读错率
            && (Abs_F32(mcPWMCtrl.Cycle - mcPWMCtrl.PWMARROld) < 50)) // 两次周期值相近，减少读错率
        {
            if (mcPWMCtrl.Cycle == 0)
            {
                mcPWMCtrl.Cycle = 1;
            }
            
            t_inputValue = _Q15((float)(mcPWMCtrl.Compare) / (float)mcPWMCtrl.Cycle);
            
            /* PWM反比例控制 */
            #if (PWM_INVERT_ENABLED == 0)
            {
                mcPWMCtrl.Duty = t_inputValue;
            }
            #elif (PWM_INVERT_ENABLED == 1)
            {
                mcPWMCtrl.Duty = (32767 - t_inputValue);
            }
            #endif
        }
        
        mcPWMCtrl.PWMCompareOld = mcPWMCtrl.Compare; //将此次比较值赋值给上次比较值
        mcPWMCtrl.PWMARROld = mcPWMCtrl.Cycle; //将此次周期值赋值给上次周期值
        mcPWMCtrl.isBusy = 0;
    }
    
    /* 启动占空比、停止占空比 控制启停 */
    #if (MOTOR_SHUTDOWN_ENABLED == 1)
    {
        if (isCtrlPowOn == 1 && (mcPWMCtrl.Duty < PWM_TURNOFF_DUTY))    //小于停止占空比，停转
        {
            if (mcPWMCtrl.TurnOff_Filter > MOTOR_SHUTDOWN_FILTER_PERIOD)
            { isCtrlPowOn = 0; }
            else
            { mcPWMCtrl.TurnOff_Filter += 1; }
        }
        else
        { mcPWMCtrl.TurnOff_Filter = 0; }
        
        if (isCtrlPowOn == 0 && (mcPWMCtrl.Duty > PWM_TURNON_DUTY))     //大于启动占空比，启动
        {
            if (mcPWMCtrl.TurnOn_Filter > MOTOR_SHUTDOWN_FILTER_PERIOD)
            { isCtrlPowOn = 1; }
            else
            { mcPWMCtrl.TurnOn_Filter += 1; }
        }
        else
        { mcPWMCtrl.TurnOn_Filter = 0; }
    }
    #endif
    
    //2.1 速度环
    #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
    {
        if(SPEED_CONTROL_MODE == SPEED_NODE_MODE)   //速度节点模式
        {
            if(mcPWMCtrl.Duty <= ControlNode_Array[0])
            {
                McCtrl.TargetValue = (SpeedNode_Array[0]);
            }
            else if (mcPWMCtrl.Duty <= ControlNode_Array[MOTOR_SPEEDMODE_MAX_NODE - 1])
            {
                for (NodeNUM = 1; NodeNUM < MOTOR_SPEEDMODE_MAX_NODE; NodeNUM++)
                {
                    if (mcPWMCtrl.Duty > ControlNode_Array[NodeNUM - 1] && mcPWMCtrl.Duty <= ControlNode_Array[NodeNUM])
                    { McCtrl.TargetValue = (SpeedNode_Array[NodeNUM - 1]) + Motor_rawSpeedCurve(SpeedNode_Array[NodeNUM], SpeedNode_Array[NodeNUM - 1], ControlNode_Array[NodeNUM], ControlNode_Array[NodeNUM - 1]) * (mcPWMCtrl.Duty - ControlNode_Array[NodeNUM - 1]); }
                }
            }
            else
            { 
                McCtrl.TargetValue = (SpeedNode_Array[MOTOR_SPEEDMODE_MAX_NODE - 1]); 
            }
        }
        else if(SPEED_CONTROL_MODE == SPEED_PWM_MODE)   //线性调速模式
        {
            /* 控制启停 */
            if(isCtrlPowOn == 1 && mcPWMCtrl.Duty < PWM_TURNOFF_DUTY)
            {
                isCtrlPowOn = 0;
            }
            else if(isCtrlPowOn == 0 && mcPWMCtrl.Duty > PWM_TURNON_DUTY)
            {
                isCtrlPowOn = 1;
            }
            /* 速度控制 */
            if(mcPWMCtrl.Duty >= PWM_TURNON_DUTY)
            {
                McCtrl.TargetValue = (uint16)(mcPWMCtrl.Duty * speed_ratio + MC_SPEED_MIN_RPM);
                if(McCtrl.TargetValue > MC_SPEED_MAX_RPM)
                    McCtrl.TargetValue = MC_SPEED_MAX_RPM;
            }
        }
    }
    //2.2 功率环
    #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
    {
        if (mcPWMCtrl.Duty <= ControlNode_Array[0])
        {
            McCtrl.TargetValue = (PowerNode_Array[0]);
        }
        else if (mcPWMCtrl.Duty <= ControlNode_Array[MOTOR_SPEEDMODE_MAX_NODE - 1])
        {
            for (NodeNUM = 1; NodeNUM < MOTOR_SPEEDMODE_MAX_NODE; NodeNUM++)
            {
                if (mcPWMCtrl.Duty > ControlNode_Array[NodeNUM - 1] && mcPWMCtrl.Duty <= ControlNode_Array[NodeNUM])
                { 
                    McCtrl.TargetValue = (PowerNode_Array[NodeNUM - 1]) + Motor_rawSpeedCurve(PowerNode_Array[NodeNUM], PowerNode_Array[NodeNUM - 1], ControlNode_Array[NodeNUM], ControlNode_Array[NodeNUM - 1]) * (mcPWMCtrl.Duty - ControlNode_Array[NodeNUM - 1]);
                }
            }
        }
        else
        { 
            McCtrl.TargetValue = (PowerNode_Array[MOTOR_SPEEDMODE_MAX_NODE - 1]); 
        }
    }
    #endif
}


#if (FG_MODE == HARD_TIMFG_OUTPUT)
/**
    @brief    转速信号更新函数
    @note     
    @retval   无
*/
void FGOutput_Update(void)
{
    if ((mcFaultSource != FaultNoSource) || isCtrlPowOn == 0)
    {
        if (ReadBit(PH_SEL, T4SEL))
        { ClrBit(PH_SEL, T4SEL); }
        
        GP13 = ((mcFaultSource != FaultNoSource) ? (FG_ALARM_LOGIC) : (1));
        SO_Var.refreshTimes = FG_INITIAL_DELAY_TIMES;
    }
    else
    {
        if (SO_Var.refreshTimes > 0 && mcState == mcRun)
        { SO_Var.refreshTimes -= 1; }
        
        if (SO_Var.refreshTimes == 0)
        {
            if (!ReadBit(PH_SEL, T4SEL))
            {
                TIM4__CNTR = 0;
                SetBit(PH_SEL, T4SEL);
            }
            
            SO_Var.FG_ARR_Val = M_speedFlt2FG(MotorSpeed.mcSpeed);
        }
    }
    
    if (ReadBit(INT_SR1, T4IF))
    {
        if (SO_Var.FG_ARR_Val > TIM4__CNTR )
        {
            TIM4__ARR = SO_Var.FG_ARR_Val;
            TIM4__DR = (TIM4__ARR >> 1);
            ClrBit(INT_SR1, T4IF);
        }
    }
}
#endif

/*  -------------------------------------------------------------------------------------------------
    Function Name  : ONOFF_Starttest
    Description    : 启动测试
    Date           : 2020-04-10
    Parameter      : h_test: [输入/出]
    ------------------------------------------------------------------------------------------------- */
void ONOFF_Starttest(void)
{
    if (ONOFFTest.ONOFF_Flag == 1)
    {
        ONOFFTest.ON_Count++;
        #if(MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
            McCtrl.TargetValue = MOTOR_ONOFF_POW;
        #else
            McCtrl.TargetValue = MOTOR_ONOFF_SPEED;
        #endif
        isCtrlPowOn   = true;
        if (ONOFFTest.ON_Count > StartON_Time)
        {
            ONOFFTest.ON_Count        = 0;
            ONOFFTest.ONOFF_Times++;
            ONOFFTest.ONOFF_Flag      = 0;
        }
    }
    else
    {
        if (mcState != mcFault)
        {
            ONOFFTest.OFF_Count++;
            isCtrlPowOn   = false;
            McCtrl.TargetValue = 0;            
            if (ONOFFTest.OFF_Count > StartOFF_Time)
            {
                ONOFFTest.OFF_Count       = 0;
                ONOFFTest.ONOFF_Flag      = 1;
            }
        }
    }
}
