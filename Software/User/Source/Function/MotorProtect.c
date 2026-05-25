/**
 * @file
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2025-2-16
 * @brief    该文件包含了...
 *
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
    Header Definition
********************************************************************************/
#include <Myproject.h>

/* Private variables ---------------------------------------------------------*/
FaultStateType data mcFaultSource; ///< 故障类型
FaultVarible   xdata Fault;          ///< 故障检测相关结构体变量
/** 
 * @brief     软件过流检测
 * @date      2022-08-09
 */
void Fault_OverCurrent(void)
{
    if (mcFaultSource == FaultNoSource)
    {
        if (mcState == mcStart || mcState == mcAlign || mcState == mcRun|| mcState == mcStop)
        {
            
            Fault.Current.Ibus = McCtrl.mcIbusFlt;
            
            if (Fault.Current.Ibus >= SW_OC_CurrentVal)
            {
                if (Fault.Current.SWOC_DectTimeCnt < SW_OC_DectTime)
                {
                    Fault.Current.SWOC_DectTimeCnt++;
                }
                else
                {
                    Fault.Current.SWOC_DectTimeCnt  = 0;
                    mcFaultSource                   = FaultSoftOVCurrent;
                }
            }
            else
            {
                Fault.Current.SWOC_DectTimeCnt = 0;
            }
        }
    }
}


/** 
 * @brief     过温检测
 * @date      2022-07-14
 */
void Fault_Temperature(void)
{
    if (mcFaultSource == FaultNoSource)
    {
        if (McCtrl.NTCTempFlt <= OVER_Temperature) // 过温保护
        {
            if (Fault.Temperature.DetecCnt < TemperatureProtectTime)
            {
                Fault.Temperature.DetecCnt++;
            }
            else
            {
                Fault.Temperature.DetecCnt  = 0;
                mcFaultSource               = FaultOTErr;
            }
        }
        else
        {
            Fault.Temperature.DetecCnt = 0;
        }
    }
}

/**
 * @brief      电机超速检测
 */
void Fault_OverSpeed(void)
{
    if (mcState == mcRun)
    {
        if (MotorSpeed.mcSpeed > MOTOR_SPEED_OVER_RPM)
        {
             Fault.Voltage.OverSpeedCnt++;
            
            if ( Fault.Voltage.OverSpeedCnt> OVER_SpeedDetectTime)                                              
            {
                Fault.Voltage.OverSpeedCnt = 0;
                Fault.Voltage.OverSpeedCntRecover = 0;
                mcFaultSource = FaultOverSpeed;
            }
        }
        else
        {
            Fault.Voltage.OverSpeedCnt = 0;
        }
    }
}


/**
 * @brief     过欠压检测
 * @date      2025-2-16
 */
 void Fault_Voltage(void)
{
    /* 过压保护 */
    if ( McCtrl.mcDcbusFlt > OVER_PROTECT_VALUE)
    {
        Fault.Voltage.OverVoltDetecCnt++;
        
        if (Fault.Voltage.OverVoltDetecCnt > 500) // 过压保护时间500ms
        {
            Fault.Voltage.OverVoltDetecCnt = 0;
            Fault.Voltage.UnderVoltDetecCnt = 0;
            mcFaultSource = FaultOverVoltageDC;
            MOEH = 0; MOEL = 0;
        }
    }
    else
    {
        if (Fault.Voltage.OverVoltDetecCnt > 0)
        {
            Fault.Voltage.OverVoltDetecCnt--;
        }
    }
    
    /* 欠压保护 */
    if (McCtrl.mcDcbusFlt <  UNDER_PROTECT_VALUE)
    {
        Fault.Voltage.UnderVoltDetecCnt += 1;
        
        if (Fault.Voltage.UnderVoltDetecCnt >= 50)
        {
            Fault.Voltage.UnderVoltDetecCnt  = 0;
            mcFaultSource                    = FaultUnderVoltageDC;
        }
    }
    else
    {
        if (Fault.Voltage.UnderVoltDetecCnt > 0)
        {
            Fault.Voltage.UnderVoltDetecCnt--;
        }
    }
    
    /* 过欠压保护恢复 */
    if ((mcFaultSource == FaultUnderVoltageDC) || (mcFaultSource == FaultOverVoltageDC))
    {
        if ((McCtrl.mcDcbusFlt > UNDER_RECOVER_VALUE) && (McCtrl.mcDcbusFlt < OVER_RECOVER_VALUE))
        {
            Fault.Voltage.VoltRecoverCnt++;
            
            if (Fault.Voltage.VoltRecoverCnt > 100) // 保护恢复时间100ms
            {
                mcFaultSource = FaultNoSource;
                MOEH = 0; MOEL = 0;
                Fault.Voltage.VoltRecoverCnt = 0;
            }
        }
        else
        {
            if (Fault.Voltage.VoltRecoverCnt)
            {
                Fault.Voltage.VoltRecoverCnt--;
            }
        }
    }
}
/**
 * @brief     偏置电压恢复检测
 * @date      2025-2-16
 */
 void Fault_Curroffset(void)
{
    if(mcFaultSource == FaultCurroffset)
    {  
            Fault.Stall.Stall_RecoverCnt++;
            
            if (Fault.Stall.Stall_RecoverCnt > 6000) // 保护恢复时间6s
            {
                mcFaultSource = FaultNoSource;
                MOEH = 0; MOEL = 0;
                Fault.Stall.Stall_RecoverCnt = 0;
            }
       
    }
}


/**
 * @brief     启动检测
 * @date      2022-07-14
 */
void Fault_Start(void)
{
        //长时间在Start状态
        if (mcState == mcStart)        //
        {
            Fault.Start.StartTimes++;
            
            if (Fault.Start.StartTimes >= 600)
            {
                Fault.Start.StartTimes = 0;
                Fault.Start.SecondStartTimes++;
                MOEH = 0;MOEL = 0;
                mcFaultSource         = FaultStart;
            }
        }    
        //起动保护恢复
    #if ((SPEED_MODE != ONOFFTEST)&&(StartRecoverEn))    //起停测试时屏蔽起动保护恢复
    {
        if ((mcFaultSource == FaultStart) && (mcState == mcFault) && (Fault.Start.SecondStartTimes <= StartRecoverTimes))
        {
            Fault.Start.StartRecoverDelayCnt++;
            
            if (Fault.Start.StartRecoverDelayCnt > StartRecoverTime)  //启动保护延时恢复时间
            {
                Fault.Start.StartRecoverDelayCnt = 0;
                mcFaultSource = FaultNoSource;
            }
        }
    }
    #endif
}

/**
 * @brief     堵转检测
 * @date      2025-2-16
 */
void Fault_Stall(void)
{
    Fault.Stall.Stallvalue = StallMaxSpeed;
	Fault.Stall.SET_Cnt = StallProtectTime;
	if(mcState == mcRun)
    {
	if (MotorSpeed.mcSpeed >= Fault.Stall.Stallvalue || MotorSpeed.mcSpeed <= StallMinSpeed) /* 超速失步堵转判断 */
	{
		Fault.Stall.Stall_Cnt++; /* 堵转计数器 */
		/* 堵转判断防止转子位置与hold位置长时间重合或者转子不换相 */
		if (Fault.Stall.Stall_Cnt > (Fault.Stall.SET_Cnt+1))
		{
			mcFaultSource = FaultStall;
			MOEH = 0;MOEL = 0;
		}
	}
	else
	{
		if (Fault.Stall.Stall_Cnt)
			Fault.Stall.Stall_Cnt--;
	}
    }
	
	
}
/**
  * @brief        過電流恢復函式
  * @note         過電流相關執行流程
  * @retval       無
  */
void Fault_OC_Recover(void)
{
  if(mcFaultSource == FaultHardOVCurrent || mcFaultSource == FaultSoftOVCurrent)
  {
			Fault.OverCurrent.OverCurrentRecoverCnt++;
            
            if (Fault.OverCurrent.OverCurrentRecoverCnt > 6000) // 保护恢复时间6s
            {
                mcFaultSource = FaultNoSource;
                MOEH = 0; MOEL = 0;
                Fault.OverCurrent.OverCurrentRecoverCnt = 0;
            }
  }
}
/**
  * @brief        堵转恢复
  * @note         
  * @retval       
  */
void Stall_Recover(void)
{
  if(mcFaultSource == FaultStall)
  {
			Fault.Stall.Stall_RecoverCnt++;
            
            if (Fault.Stall.Stall_RecoverCnt > 1000) // 保护恢复时间1s
            {
                mcFaultSource = FaultNoSource;
                MOEH = 0; MOEL = 0;
                Fault.Stall.Stall_RecoverCnt = 0;
            }
  }
}
/**
 * @brief     保护恢复
 * @date      2025-2-16
 */
void Fault_Recovery(void)
{
	#if (CurroffsetProtectEn)///偏置电压保护恢复	
		Fault_Curroffset();
	#endif
	#if (Fault_OC_RecoverEn)///过流保护恢复	
		Fault_OC_Recover();
	#endif
	#if (Stall_RecoverEn)///过流保护恢复	
		Stall_Recover();
	#endif
    #if (StartProtectEn)
        Fault_Start();
    #endif
	
}
/**
 * @brief     保护判断
 * @date      2025-2-16
 */
void Fault_Detection(void)
{
    // 堵转检测
    #if (StallProtectEn == 1)
    {
       Fault_Stall(); // 堵转保护
    }
    #endif	
   
    // 软件过流
    #if (SWCurrent1_ProtectEn == 1)
    {
        Fault_OverCurrent();
    }
    #endif
    // 过欠压保护
    #if (VoltageProtectEn == 1)
    {
		Fault_Voltage();
    }
    #endif
    #if (OverSpeedProtectEn == 1)
    {
        Fault_OverSpeed();
    }
    #endif
	Fault_Recovery();
}


