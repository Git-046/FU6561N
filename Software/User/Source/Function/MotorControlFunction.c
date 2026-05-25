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

CurrentOffset xdata mcCurOffset; ///< 电流采样偏置电压采集缓存

/**
 * @brief     上电时，先对硬件电路的电流进行采集，写入对应的校准寄存器中。
              调试时，需观察mcCurOffset结构体中对应变量是否在范围内。采集结束后，OffsetFlag置1。
 * @date      2025-2-14
 */
void GetCurrentOffset(void)
{
	if(!mcCurOffset.OffsetFlag)
	{
		SetBit(ADC_CR, ADCBSY); // 使能ADC
		
		while (ReadBit(ADC_CR, ADCBSY));
		
		mcCurOffset.Iw_busOffsetSum += ((ADC4_DR & 0x7ff8));
		mcCurOffset.Iw_busOffset = mcCurOffset.Iw_busOffsetSum >> 4;
		mcCurOffset.Iw_busOffsetSum -= mcCurOffset.Iw_busOffset;
		mcCurOffset.I_busOffsetSum += ((ADC1_DR & 0x7ff8));
		mcCurOffset.I_busOffset = mcCurOffset.I_busOffsetSum >> 4;
		mcCurOffset.I_busOffsetSum -= mcCurOffset.I_busOffset;
		mcCurOffset.OffsetCount++;
		
		if (mcCurOffset.OffsetCount > Calib_Time)
		{
			#if (CurroffsetProtectEn==1)
			if (abs(mcCurOffset.Iw_busOffset - 16383) > _Q14(0.3f/(HW_ADC_REF/2.0f)))
			{
				mcFaultSource = FaultCurroffset;
				MOEH = 0; MOEL = 0;
			}
			#endif
//			DAC0_DR = _Q8(I_ValueX(HWOCValue)) + (mcCurOffset.Iw_busOffset >> 7);
			mcCurOffset.OffsetFlag = 1;
		}
	}
}

/**
 * @brief     控制变量上电初始化,包括保护参数的初始化、电机状态初始化
 * @brief     上电只运行一次
 * @date      2025-2-14
 */
void MotorcontrolInit(void)
{
    VariablesPreInit(); // 电机相关变量初始化
    /*****电机状态机时序变量***********/
    McStaSet.SetMode = 0;
    mcCurOffset.Iw_busOffsetSum = 16383;
    #if (SPEED_MODE == ONOFFTEST)
		memset(&ONOFFTest, 0, sizeof(ONOFFTest));
	#endif
}

/**
 * @brief     初始化电机参数，每次电机重启均会被调用
 * @warning   需要注意填写的变量是否可以在此时刻被初始化
 * @date      2025-2-14
 */
void VariablesPreInit(void)
{
    memset(&McCtrl, 0, sizeof(MOTORCTRL));
    memset(&SvpwmPara, 0, sizeof(SVPWMParaType));
}

/**
 * @brief     关闭输出，关闭FOC，电机切换到mcReady状态被调用一次
 * @date      2025-2-14
 */
void Motor_Ready(void)
{
    if (McStaSet.SetFlag.CalibFlag == 0)
    {
		ADC_Init();
        McStaSet.SetFlag.CalibFlag = 1;
        ClrBit(TIM1_CR0, T1BCEN | T1RWEN | T1RCEN); // 关闭TIM1的定时器
        MOEH = 0; MOEL = 0;
    }
}

/**
 * @brief     电机初始化，对电机相关变量、PI进行初始化设置,关闭FOC所需要使用到的ADC
 * @note      关闭FOC所需要使用到的ADC，FOC模块会自动调用相应ADC 无需外部使能
 * @date      2025-2-14
 */
void Motor_Init(void)
{
	int16 Target;
	Target = McCtrl.TargetValue; //Init后会把McCtrl结构体清空，会把给定置0
	VariablesPreInit();
	McCtrl.TargetValue = Target;
	SetBit(DRV_CR1, DCIM0);
	ClrBit(DRV_CR1, DCIM1);
	DRV_COMR = DrvCOMRValue;
	TIM1_CR0 = 0;
    TIM1_CR1 = 0;
    TIM1_CR2 = 0;
    TIM1_CR3 = 0;
    TIM1_IER = 0;
}

/**
 * @brief     预充电分三步，第一步是对U相进行预充电，第二步是对V两相进行预充电;第三步是对W三相进行预充电。
 * @brief     低压应用一般不需要预充电，在customer.h禁止预充电功能寄即可
 * @exception 在低功率高压应用中，过流值较小，充电电流过大可能导致触发硬件过流, 需要注意检查
 * @date      2025-2-14
 */
void Motor_Charge(void)
{
    if (McStaSet.SetFlag.ChargeSetFlag == 0)
    {
        McStaSet.SetFlag.ChargeSetFlag = 1;
        DRV_DR = CHARGE_DUTY * DRV_ARR; // 下桥臂10% duty
        DRV_CMR = 0x00;                 //预定位不需要互补输出
        McCtrl.ChargeStep = 0;
    }
    
    if ((McCtrl.State_Count < CHARGE_TIME) && (McCtrl.ChargeStep == 0))
    {
        McCtrl.ChargeStep = 1;
        DRV_CMR |= 0x01;                          // U相下桥臂通
        MOEH = 1; MOEL = 1;
    }
    
    if (( McCtrl.State_Count <= (CHARGE_TIME << 1) / 3) && (McCtrl.ChargeStep == 1))
    {
        McCtrl.ChargeStep = 2;
        DRV_CMR |= 0x04;                         // V相下桥臂通
    }
    
    if ((McCtrl.State_Count <= CHARGE_TIME / 3) && (McCtrl.ChargeStep == 2))
    {
        McCtrl.ChargeStep = 3;
        DRV_CMR |= 0x10;                          // W相下桥臂通
    }
    
    #if (IPMTEST ==Enable)
    {
        if (McCtrl.ChargeStep == 3)
        {
            McCtrl.ChargeStep = 4;
            DRV_CMR = 0x0ABF;
        }
    }
    #endif
}

/**
 * @brief     开启次功能启动时候会将电机强拉到 设定角度，之后再启动
 * @date      2025-2-14
 */
void Motor_Align(void)
{
	if (McStaSet.SetFlag.StartSetFlag == 0)
	{
		SetBit(DRV_CR0, SVEN); // SVPWM模块使能

		ADC_MASK = 0;
		SetBit(ADC_MASK, CH2EN | CH4EN);

		LEAD_ANGLE = LEAD_ANGLE_START;

		DRV_CMR = 0x0ABF; // 输出
		SquStart.Sector = ALIGN_Sector;
		SquStart.StartCurrentValue = ALIGN_CurrentStart;

			switch (SquStart.Sector)
			{
			case 1:
				THETA = _Q15(0.0 / 180.0);
				DRV_CMR = 0x0A3C;
				break;
			case 2:
				THETA = _Q15(60.0 / 180.0);
				DRV_CMR = 0x028F;
				break;
			case 3:
				THETA = _Q15(120.0 / 180.0);
				DRV_CMR = 0x08B3;
				break;
			case 4:
				THETA = _Q15(180.0 / 180.0);
				DRV_CMR = 0x0A3C;
				break;
			case 5:
				THETA = _Q15(240.0 / 180.0);
				DRV_CMR = 0x028F;
				break;
			case 6:
				THETA = _Q15(300.0 / 180.0);
				DRV_CMR = 0x08B3;
				break;
			default:
				break;
			}

		SPEED_STEP = 0x00;		// 静止，角度不增加
		SetBit(DRV_CR0, MESEL); // ME工作模式选择 0: 方波驱动模式 1: SVPWM驱动模式
		MOEH = 1;
		MOEL = 1; // 使能驱动输出 */
		McCtrl.mcSvUq = DeadTimeComp;
		SV_UQ = McCtrl.mcSvUq; // 电压预定位占空比0.07%
		McStaSet.SetFlag.StartSetFlag = 1;
	}
}

/**
 * @brief     顺逆风处理函数
 * @date      2025-2-14
 */
void Motor_TailWind(void)
{
    if (mcBemf.SpeedUpdate == 1) //检测到转速
    {
        if (mcBemf.BEMFSpeed < (-TAILWIND_SPEED))  // 逆风
        {
            McCtrl.State_Count               = TAILWIND_TIME;
            mcBemf.SpeedUpdate               = 0;
            McCtrl.Start_Mode                = HEADWIND_START;
            ClrBit(CMP_CR2, CMP0EN);    // 关闭比较器
            ClrBit(TIM2_CR1, T2EN);     // 关闭定时器
			mcBemf.BrakeTime = 200;
        }
        else if (mcBemf.BEMFSpeed < (-TAILWIND_SPEED) * 2) //高速逆风
        {
            mcBemf.SpeedUpdate = 0;
			mcBemf.ForceBrakeCnt++;
			McCtrl.State_Count = TAILWIND_TIME; //延长顺逆风检测时间
			if(mcBemf.ForceBrakeCnt > FORCE_BRAKE_THR) /*长时间检测不到正常的信号，强制刹车启动*/
			{
				mcBemf.ForceBrakeCnt = 0;
				McCtrl.State_Count = 0;
				McCtrl.Start_Mode                = HEADWIND_START;
				ClrBit(CMP_CR2, CMP0EN);    // 关闭比较器
				ClrBit(TIM2_CR1, T2EN);     // 关闭定时器			
				mcBemf.BrakeTime = 200;	
			}
        }
		else if (mcBemf.BEMFSpeed > (TAILWIND_SPEED) * 10) //正转
        {
            mcBemf.SpeedUpdate = 0;
			mcBemf.ForceBrakeCnt++;
			McCtrl.State_Count = TAILWIND_TIME; //延长顺逆风检测时间
			if(mcBemf.ForceBrakeCnt > FORCE_BRAKE_THR)
			{
				mcBemf.ForceBrakeCnt = 0;
				McCtrl.State_Count = 0;
				McCtrl.Start_Mode                = HEADWIND_START;
				ClrBit(CMP_CR2, CMP0EN);    // 关闭比较器
				ClrBit(TIM2_CR1, T2EN);     // 关闭定时器			
				mcBemf.BrakeTime = 200;	
			}
        }
        else if (mcBemf.BEMFSpeed > TAILWIND_SPEED)  //正转
        {
            if (mcBemf.HighSpdStart == 2) //启动完成，切状态机
            {
                McCtrl.Start_Mode       = TAILWIND_START;
                mcState                 = mcStart;
				mcBemf.BrakeTime = 0;
            }
            
            if (mcBemf.HighSpdStart == 0)
            {
                mcBemf.HighSpdStart = 1; // 设置启动
            }
        }
        else  //其他，如静止
        {
            McCtrl.Start_Mode   = STATIC_START;
            // mcState       = mcPosiCheck;
            ClrBit(CMP_CR2, CMP0EN);     // 关闭比较器
            ClrBit(TIM2_CR1, T2EN);      // 关闭定时器
			mcBemf.BrakeTime = 100;
        }
    }
	else
	{
		if(mcBemf.BrakeTime == 0)
			mcBemf.BrakeTime = 100;
	}
}

/**
 * @brief     电机启动处理函数
 * @date      2025-2-14
 */
void Motor_Start(void)
{
    if (McStaSet.SetFlag.StartSetFlag == 0)
    {
		McStaSet.SetFlag.StartSetFlag = 1;

		/***SVPWM 初始化***/
		SetBit(DRV_CR0, SVEN); // SVPWM模块使能
		SetBit(DRV_CR0, MESEL); // ME工作模式选择 0: 方波驱动模式 1: SVPWM驱动模式
		LEAD_ANGLE = LEAD_ANGLE_START;
		SVPWM_Init();
		SquStartInit();
		SPEED_STEP = McCtrl.mcSv_RTheStep;
		McCtrl.mcSvUq = SquStart.StartUq;
		SV_UQ = SquStart.StartUq;
		MOEH = 1;
		MOEL = 1; // 使能驱动输出 */
	}
}

/**
 * @brief     电机运行处理函数
 * @date      2025-2-14
 */
void Motor_Run(void)
{
	/*调节提前角*/
	if(McCtrl.SampFinishFlag)
	{
		AdaLeadAngleFast();
		AdaLeadAngleSlow();
		McCtrl.SampFinishFlag = 0;
	}
}

/**
 * @brief     电机停止输出
 * @date      2025-2-14
 */
void MC_Stop(void)
{
    MOEH = 0; MOEL = 0;
    ClrBit(DRV_CR0, SVEN);
    TIM1_CR0   = 0;
    TIM1_CR1   = 0;
    TIM1_CR2   = 0;
    TIM1_CR3   = 0;
    TIM1_CR4   = 7;
    TIM1_IER   = 0;
    TIM1__BCCR = 0;
    TIM1__BCOR = 0;
}

/**
 * @brief     三下桥刹车
 * @date      2025-2-14
 */
void MC_Break(void)
{
    DRV_DR      = DRV_ARR + 1;
    DRV_CMR    &= 0xFFC0;
    DRV_CMR    |= 0x0015;
    MOEH = 1; MOEL = 1;             /* 使能输出*/
}


/**
 * @brief     比较器 ON/OFF 采样切换
 * @date      2025-02-13
 */
void Sample_Swicth(void)
{
    if ((McCtrl.mcSvUq > SAM_THR_H_SV) && (McCtrl.OnOffSampleSwitch_Flag == 0))
    {
        McCtrl.OnOffSampleSwitch_Flag = 1;
        SetReg(CMP_CR3, SAMSEL1 | SAMSEL0, SAMSEL1 | SAMSEL0);      // 切换到ON采样
    }
    else if ((McCtrl.mcSvUq < SAM_THR_L_SV) && (McCtrl.OnOffSampleSwitch_Flag == 1))
    {
        McCtrl.OnOffSampleSwitch_Flag = 0;
        SetReg(CMP_CR3, SAMSEL1 | SAMSEL0, SAMSEL0);      // 切换到OFF采样
    }
    else if((McCtrl.OnOffSampleSwitch_Flag !=1)&&(McCtrl.OnOffSampleSwitch_Flag !=0))
    {
        McCtrl.OnOffSampleSwitch_Flag = 0;
    }

	if (McCtrl.mcSvUq > AdaAngle_THR_H)
	{
		McCtrl.AdaMode = 1;
		DRV_COMR = DrvCOMR_H;
		SetBit(DRV_CR1, DCIM0);
		ClrBit(DRV_CR1, DCIM1);
	}
	else if (McCtrl.mcSvUq < AdaAngle_THR_L)
	{
		McCtrl.AdaMode = 0;
		DRV_COMR = DrvCOMR_L;
		ClrBit(DRV_CR1, DCIM0);
		SetBit(DRV_CR1, DCIM1);
	}

	/******* 角度补偿初始化 *******/
    // SvpwmPara.last_BCCR = TIM1__BCCR;   // 记录初始周期值
    // SvpwmPara.angle_comp = 0;           // 初始补偿为0
    // SvpwmPara.accel_factor = 2.0;      // 根据电机特性调整(1.0-5.0)
    // SvpwmPara.comp_enable = 1;          // 使能补偿
	
}

/**
 * @brief     SVPWM初始化
 * @date      2025-02-13
 */
void SVPWM_Init(void)
{
	CMP012_Init();                                              // 比较器初始化
	SetReg(CMP_CR3, SAMSEL1 | SAMSEL0, 0);						// 一直采样
	SetBit(CMP_CR2, CMP0SEL0);
	ClrBit(CMP_CR2, CMP0SEL1);
	Timer1_Sv_Init();                                           // 定时器初始化
	
	ADC_MASK = 0;

	SetBit(ADC_MASK, CH2EN | CH4EN); 

    MotorSpeed.mcSpeedBase = MOTORSPEEDBASE_TIMER;
    SvpwmPara.Base_BCOR    = DIV_A_SPD;
//    McCtrl.mcSv_RTheStep = 0;
    if (MotorSpeed.FR == CCW) /* 电机运行方向 */
    {
        SetBit(DRV_CR0, DDIR);
    }
    else
    {
        ClrBit(DRV_CR0, DDIR);
    }
    
    #if (OverModulation)
    {
        SetBit(SV_CR1, OVMDL);                                  // 过调制使能
    }
    #endif
		
    // SVPWM模块使能
    SetReg(SV_CR0, TCLKSEL1 | TCLKSEL0, TCLKSEL0);             // 角度累加触发信号选择
    LEAD_ANGLE     = LEAD_ANGLE_START;
    if (mcState == mcStart)
    {
        SetBit(SV_CR0, ATADD);    // 角度自动累加使能
    }
		
		DRV_CMR    = 0x0A3C;
    SetBit(SV_CR0, WINDIS);				//开窗		
	McCtrl.SampFlag = 1;

}


/**
 * @brief     切入开窗正弦
 * @date      2025-02-13
 */
void SVPWM_Swtich_Wind(void)
{
	ADC_Channel_Recover();
//		SetBit(SV_CR0, SEG7EN);
	DRV_ARR = PWM_VALUE_LOAD;    // 载波频率的周期值
	DRV_DTR = PWM_LOAD_DEADTIME; // 死区时间
	
	SetBit(CMP_CR2, CMP0EN);
	DRV_CMR    = 0x0ABF;

	SetBit(CMP_CR4, FAEN);
	SetReg(TIM1_CR6, WINMD1 | WINMD0, WINMD1 | WINMD0);                // 窗口模式选择 00:6窗口 01:3窗口 10:2窗口 11:1窗口
   
	LEAD_ANGLE     = LEAD_ANGLE_START;
    TIM1__BCOR     = SvpwmPara.Bcor;                                     // 当前转速
    TIM1__BCNTR    = 7*(SvpwmPara.Bcor/12);
    TIM1__BCCR     = SvpwmPara.Bcor;
    TIM1__RARR     = SvpwmPara.Bcor;                                     // 若下一拍开窗，设为延迟开窗角度；若下一拍不开窗，设为60°
    TIM1__RCNTR = (SvpwmPara.Bcor >> 1);
    
	SetBit(SV_CR1, SPDUPD);																							// 位置检测事件后自动更新转速
    SetBit(SV_CR0, THELIM);                                             // 角度累加限制使能
    SetBit(SV_CR0, ATADD);
    ClrBit(SV_CR0, WINDIS);
	
	SetReg(CMP_CR3, SAMSEL1 | SAMSEL0, SAMSEL0);
    // 60度强制换相使能
    WIN_DLY = WIN_DLY_START;                                       // 延迟开窗角度选择 窗口宽度 = 60°-（WIN_DLY/128*60°）  0x50：20°
    
    /***********设置初始状态***********/
    if (MotorSpeed.FR == CCW)
    {
        TIM1_CR4 = 6;                                               // 设置起始CR4
        ClrBit(TIM1_CR6, ESEL);                                       // 检测边沿选择 0:上升沿 1:下降沿
		THETA = _Q15(150.0/180.0);
    }
    else if (MotorSpeed.FR == CW)
    {
        TIM1_CR4 = 4;
        SetBit(TIM1_CR6, ESEL);                                       // 检测边沿选择 0:上升沿 1:下降沿
		THETA = _Q15(30.0/180.0);
    }    
    /***********使能计数器***********/
    SetBit(TIM1_CR0, T1BCEN | T1RWEN | T1RCEN);                       // 基本定时器的计数器使能
    INT_SR0  = 0x00;
    TIM1_IER = T1PDIE;
    SetReg(IP1, PTIM11 | PTIM10, PTIM11 | PTIM10);					//TIM1中断优先级3
    SetBit(SV_CR0, CMUPD);                                            // 换相角度更新使能
    SetBit(TIM1_CR7, T1WOIE);                                         // SVPWM开窗中断使能 0: Disable 1: Enable
    McStaSet.SetFlag.RunFlag = 1;	
    SV_UQ        = McCtrl.mcSvUq;
	
	TIM1__UCOP = 0x1800;								// 设置UCOP初始值，防止UQ太小时未采样UCOP无法正常工作
	// TIM1_KRMAX = 0xD8;                                  // 向上续流电压比较系数 0xff：VCC
	// TIM1_KFMIN = 0x30;                                  // 向下续流电压比较系数 0x00：GND
	TIM1_KRMAX = 0x00;                                  // 禁止自适应续流
	TIM1_KFMIN = 0xFF;                                  // 禁止自适应续流
	
	ClrBit(INT_SR0, T1ABDIF);                           // 续流屏蔽结束中断标志位清零
	ClrBit(TIM1_CR5, T1ABDIE);  
	ClrBit(TIM1_CR5, T1BDIE);  
	
	MOEH = 1; 
	MOEL = 1;

	McCtrl.K_Speed_BEMF = K_Delta_BEMF1;
	McCtrl.mcLeadAngle = LEAD_ANGLE_START;
	McCtrl.mcLeadAngleCount = 0;
	// ClrBit(TIM1_CR5, ITRIP_DIS);
	// ClrBit(TIM1_CR6, UDCSEL);
	// ClrBit(PH_SEL1, DBGCT);
	// SetReg(CMP_CR3, DBGSEL1 | DBGSEL0, DBGSEL0); /*Debug输出比较器过零点信号*/
}
 
/**
 * @brief     超前角度调整
 * @date      2025-02-13
 */
void SVPWM_LeadAngle(void)
{
	// int16 Speederr = 0;

    if (MotorSpeed.mcSpeed < MC_SPEED_MIN_RPM)
    {
        McCtrl.mcLeadAngle = LEAD_ANGLE_START;
    }
    else if (MotorSpeed.mcSpeed > MC_SPEED_MAX_RPM)
    {
        McCtrl.mcLeadAngle = LEAD_ANGLE_RUN_5;
    }
    else
    {				
		if (MotorSpeed.mcSpeed < LEAD_ANGLE_Speed1)
        {
            McCtrl.mcLeadAngle = ((uint32)(LEAD_ANGLE_Scope1*(MotorSpeed.mcSpeed - MC_SPEED_MIN_RPM)) >> 16) + LEAD_ANGLE_START;
        }
        else if (MotorSpeed.mcSpeed < LEAD_ANGLE_Speed2)
        {
            McCtrl.mcLeadAngle = ((uint32)(LEAD_ANGLE_Scope2*(MotorSpeed.mcSpeed - LEAD_ANGLE_Speed1)) >> 16) + LEAD_ANGLE_RUN_1;
        }
        else if (MotorSpeed.mcSpeed < LEAD_ANGLE_Speed3)
        {
            McCtrl.mcLeadAngle = ((uint32)(LEAD_ANGLE_Scope3*(MotorSpeed.mcSpeed - LEAD_ANGLE_Speed2)) >> 16) + LEAD_ANGLE_RUN_2;
        }
        else if (MotorSpeed.mcSpeed < LEAD_ANGLE_Speed4)
        {
            McCtrl.mcLeadAngle = ((uint32)(LEAD_ANGLE_Scope4*(MotorSpeed.mcSpeed - LEAD_ANGLE_Speed3)) >> 16) + LEAD_ANGLE_RUN_3;
        }
        else
        {
            McCtrl.mcLeadAngle = ((uint32)(LEAD_ANGLE_Scope5*(MotorSpeed.mcSpeed - LEAD_ANGLE_Speed4)) >> 16) + LEAD_ANGLE_RUN_4;
        }
    }
	if(McCtrl.RunMode == 1)/*弱磁模式*/
	{
		if((McCtrl.mcSvUq > 32000) && (PID.ErrValue > 300)) /*保证Uq处于满输出状态，弱磁效率更高*/
		{
			McCtrl.WeakenAngle += 5;
		}
		else if((McCtrl.mcSvUq < 31000))
		{
			McCtrl.WeakenAngle -= 5;
		}
		if(McCtrl.WeakenAngle < WeakenAngleMin)
		{
			McCtrl.WeakenAngle = WeakenAngleMin;
		}
		else if(McCtrl.WeakenAngle > WeakenAngleMax)
		{
			McCtrl.WeakenAngle = WeakenAngleMax;
		}
		McCtrl.mcLeadAngle += (uint16)McCtrl.WeakenAngle; //添加弱磁角度
	}
	// else if(McCtrl.RunMode == 2)
	// {
	// 	// McCtrl.mcLeadAngle = LEAD_ANGLE_RUN_1;
	// 	McCtrl.mcLeadAngleIntComp -= 100;
	// }
	else
	{
		McCtrl.WeakenAngle = 0;
	}
#if(MANUAL_LEAD_ANGLE)
    LEAD_ANGLE = McCtrl.mcLeadAngle;
#else
		AdaLeadAngleApply();
#endif
}
/**
 * @brief     续流角度调整
 * @date      2025-02-13
 */
void SVPWM_FlowAngle(void)
{
    uint8  TempTIM1_CR1      = 0;
    TempTIM1_CR1 = (TIM1_CR1 & 0x7F);
    
    if (TempTIM1_CR1 < McCtrl.TargetBSEL_ANGLE)
    {
        TempTIM1_CR1 += 1;
    }
    else if (TempTIM1_CR1 > McCtrl.TargetBSEL_ANGLE)
    {
        TempTIM1_CR1 -= 1;
    }
    
    TIM1_CR1 = (TIM1_CR1 & (0x80)) | (TempTIM1_CR1);
}

/**
 * @brief     开窗角度调整
 * @date      2025-02-13
 */
void SVPWM_WindowAngle(void)
{
    uint8  TempWIN_DLY   = 0;
    TempWIN_DLY = WIN_DLY;
    
    if (TempWIN_DLY < McCtrl.TargetDlyAngle)
    {
        TempWIN_DLY += 1;
    }
    else if (TempWIN_DLY > McCtrl.TargetDlyAngle)
    {
        TempWIN_DLY -= 1;
    }
    
    WIN_DLY = TempWIN_DLY;
}


/**
 * @brief     角度调整
 * @date      2025-02-13
 */
void SVPWM_AngleAdjust(void)
{
    //加速过程，开窗--WIN_DLY_Start
    //降速或稳定运行过程，开窗--WIN_DLY_RUN
		#if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
		{
				if ((MotorSpeed.mcSpeed < (McCtrl.TargetValue - 200)) && (McCtrl.LoopRampValue < (McCtrl.TargetValue - SPEEDLOOP_RAMP_INC)))  //加速过程
				{
						McCtrl.TargetDlyAngle = WIN_DLY_START;
						McCtrl.TargetBSEL_ANGLE = SV_BSEL_ANGLE;
				}
				else if ((MotorSpeed.mcSpeed > (McCtrl.TargetValue - 200)) && (McCtrl.LoopRampValue >= (McCtrl.TargetValue + SPEEDLOOP_RAMP_DEC))) //降速过程
				{
						McCtrl.TargetDlyAngle = WIN_DLY_RUN;
						McCtrl.TargetBSEL_ANGLE = SV_BSEL_ANGLE_RUN;
				}
				else if ((McCtrl.LoopRampValue >= (McCtrl.TargetValue - SPEEDLOOP_RAMP_INC)) || (McCtrl.LoopRampValue <= (McCtrl.TargetValue + SPEEDLOOP_RAMP_DEC))) //&&(MotorSpeed.mcSpeed > LEAD_ANGLE_Speed4))
				{
						McCtrl.TargetDlyAngle = WIN_DLY_RUN;
						McCtrl.TargetBSEL_ANGLE = SV_BSEL_ANGLE_RUN;
						
						if (SvpwmPara.Sv_SpeedFltFlag == 0)
						{
								SvpwmPara.Sv_SpeedFltFlag = 1;
								SvpwmPara.Bcor = TIM1__BCOR;             // UQ稳定后，对转速加滤波，需要主动更新一次BCOR
								SetBit(TIM1_CR0, T1CFLT1);
								ClrBit(TIM1_CR0, T1CFLT0);
								TIM1__BCOR = SvpwmPara.Bcor;			
						}
				}
		}
		#elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
		{

				if(McCtrl.mcLeadAngleCount > BSEL_ANGLE_CNT) /*加速过程中缓慢提高续流屏蔽角度和开窗角度*/
				{
						McCtrl.TargetBSEL_ANGLE = SV_BSEL_ANGLE_RUN;					

					if ((McCtrl.LoopRampValue < (McCtrl.TargetValue - POWERLOOP_RAMP_INC)))  //加速过程
					{
							McCtrl.TargetBSEL_ANGLE = WIN_DLY_RUN;
					}

					else if ((McCtrl.LoopRampValue >= (McCtrl.TargetValue - POWERLOOP_RAMP_INC)) && (McCtrl.LoopRampValue <= (McCtrl.TargetValue + POWERLOOP_RAMP_DEC))) //&&(MotorSpeed.mcSpeed > LEAD_ANGLE_Speed4))
					{
							McCtrl.TargetDlyAngle = WIN_DLY_RUN;
							
							if (SvpwmPara.Sv_SpeedFltFlag == 0)
							{								
									SvpwmPara.Sv_SpeedFltFlag = 1;
									SvpwmPara.Bcor = TIM1__BCOR;             // UQ稳定后，对转速加滤波，需要主动更新一次BCOR
									SetBit(TIM1_CR0, T1CFLT1);
									ClrBit(TIM1_CR0, T1CFLT0);
									TIM1__BCOR = SvpwmPara.Bcor;			
							}
					}
				}
				else /*启动时，开窗角度和续流屏蔽可以给低一些，能够很好地优化启动电流波形*/
				{
						McCtrl.TargetBSEL_ANGLE = WIN_DLY_START;
						McCtrl.TargetBSEL_ANGLE = SV_BSEL_ANGLE;					
				}
		}
		#endif
				
    
    
    if ((MotorSpeed.Speed_Updatflag))
    {
		MotorSpeed.Speed_Updatflag = 0;

		// 调整开窗

		if (SvpwmPara.Sv_SpeedFltFlag)
		{
			// 延迟角度
			SVPWM_WindowAngle();
			// 调整续流角
			SVPWM_FlowAngle();
		}
		// 调整超前角
		SVPWM_LeadAngle();
	}
		
		
}

void MotorAlignProcess(void)
{
	if (McStaSet.SetFlag.StartSetFlag == 1)
	{
			SquStart.Isample = ADC4_DR;
			SquStart.Isample = SquStart.Isample - mcCurOffset.Iw_busOffset;

			if (SquStart.Isample < SquStart.StartCurrentValue)
			{
					SquStart.StartUq = SquStart.StartUq + 5;
			}
			else
			{
					if (SquStart.StartUq > MinUq)
					{
						SquStart.StartUq = SquStart.StartUq - 5;
					}						
			}
			SV_UQ = SquStart.StartUq;
	}		
}

void SquStartInit(void)
{
		DRV_ARR = PWM_VALUE_LOAD_Start;    // ????????
		DRV_DTR = PWM_LOAD_DEADTIME_Start; // ????
		DRV_COMR = DrvCOMRValue;
		memset(&SquStart, 0, sizeof(SquStartVar));
		SquStart.FR = MotorSpeed.FR;
		SquStart.Ibus_Offset = mcCurOffset.Iw_busOffset;
		BMEFUpdate.BEMFCWCount = 0;
		BMEFUpdate.BEMFCCWCount = 0;
		SquStart.Sector = 1;
//		SquStart.TargetTheta = _Q15(0.0/180.0);
		THETA = _Q15(0.0/180.0);		
		SquStart.IsampleSum1 = 0;
		SquStart.IsampleSum2 = 0;
		SquStart.IinterCount1 = 0;
		SquStart.IinterCount2 = 0;
		SquStart.IsampleSumMin = 30000;
		SquStart.IsampleSumMax = -30000;
		SquStart.PWMFrequency = PWM_FREQUENCY_Start;
		SquStart.PhaseCommutatCount1 = 0;
		SquStart.PhaseCommutatCount = 0;
		SquStart.MotorSpeedCount1 = 30000;
		SquStart.MotorSpeedCount2 = 30000;
		SquStart.MotorSpeedCount3 = 30000;
		SquStart.IgnoreCount = (uint16)(3 * SquStart.PWMFrequency);
		SquStart.FocredPeriod = (uint16)(ForcedSwitchTime * SquStart.PWMFrequency);
		SquStart.PhaseCommutCurrentThre = START_Curernt_Threshold;
		SquStart.SwitchSVPWMThreshold = SwitchCount;
		SquStart.StartCurrentValue = START_Current;
		SquStart.StartCurrentHoldValue = START_Current_Hold;
		SquStart.UQRampEnable = 1;
		SquStart.StartUq = MinUq;
		SquStart.StartState = 2;		
}

/**
 * 恢复到正常运行的ADC采样通道，包括电压、电流以及可能的其余通道
 */
void ADC_Channel_Recover(void)
{
	uint16 ADC_SAMP_CHANNAL = 0;
	ADC_SAMP_CHANNAL |= (CH1EN | CH2EN);
#if(TEMP_SAMP_EN)
	ADC_SAMP_CHANNAL |= TEMP_ADC_CHANNEL;
#endif
#if(SPEED_MODE == SREFMODE)
	ADC_SAMP_CHANNAL |= CH9EN;
#endif
	ADC_MASK = 0;
	SetBit(ADC_MASK, ADC_SAMP_CHANNAL);
}

/*高速下切方波*/
void SVPWM_to_Squ(void)
{
	int32 uq;
	int16 arr;
	// CMP_SAMON = 0x0F;
	// CMP_SAMOFF = 0x0F;
#if(Motor_BLDC_CONTROL_Mode == BLDC_120_CONTROL)
	ANGLE_MASK(BLDC_120_MASK_ANGLE);
	TIM1_CR2  &= 0x80; //位置检测复位
	ANGLE_DELAY(BLDC_150_MASK_ANGLE); //延迟换相角
#else
	ANGLE_MASK(BLDC_150_MASK_ANGLE);
	TIM1_CR2  &= 0x80; //位置检测复位
	ANGLE_DELAY(BLDC_150_DELAY_ANGLE); // 延迟换相角
	SetBit(TIM1_CR3, BLDC150EN); //150°方波
#endif

	/*注：DBR的顺序不可改变*/
	TIM1_DBR1 = WH_VL_Real;            // 4  WH_VL
    TIM1_DBR2 = UH_VL_Real;            // 5  UH_VL
    TIM1_DBR3 = UH_WL_Real;            // 1  UH_WL
    TIM1_DBR4 = VH_WL_Real;            // 3  VH_WL
    TIM1_DBR5 = VH_UL_Real;            // 2  VH_UL
    TIM1_DBR6 = WH_UL_Real;            // 6  WH_UL

	SetBit(TIM1_CR6, ATSW); //启动模式自动切换
	ClrBit(DRV_CR0, MESEL); //设置为方波模式

	/*方波占空比是DRV_DR/DRV_ARR，SVPWM是SV_UQ/0x7fff*/
	uq = SV_UQ;
	arr = DRV_ARR;
	DRV_DR = (uint16)((uq * arr) >> 15);
	// while(ReadBit(TIM1_SR, SWBUSY)); //等待切换完成

	// SV_CR0 = 0;
	// SV_CR1 = 0;
}

void Squ_to_SVPWM(void)
{
	
	// CMP_SAMON = 0x02;
	// CMP_SAMOFF = 0x02;
	SetBit(TIM1_CR6, ATSW); //启动模式自动切换
	SetBit(DRV_CR0, MESEL); //设置为方波模式
	/*方波占空比是DRV_DR/DRV_ARR，SVPWM是SV_UQ/0x7fff*/

}