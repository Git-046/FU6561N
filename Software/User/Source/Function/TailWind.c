/**
 * @file     
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2025-2-15
 * @brief     The file contains...
 * 
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
    Header Definition
********************************************************************************/
#include <MyProject.h>

uint8  code table_Bemf_CWNext[6]   = {3, 6, 2, 5, 1, 4};
uint8  code table_Bemf_CWPre[6]    = {5, 3, 1, 6, 4, 2};


BEMFDetect_TypeDef xdata mcBemf;

static uint8 FR_Detect(uint8 bemfStatus, uint8 setFr);

/**
* @brief        Bemf的顺逆风检测，比较器硬件初始化，注意需要核对IO是否对应
* @date         2025-02-21
*/
void BEMF_CMP_Init(void)
{	
    /*  ---------------------------------------------------------------------------------------------
    CMP0_MOD：
	CMPG的模式设置
	0: BEMF模式，内置中心点
	1: 双比较器模式，U相与V相比较，U相与W相比较
    -------------------------------------------------------------------------------------------------*/
   	ClrBit(CMP_CR2, CMP0MOD);
	
    /*  -------------------------------------------------------------------------------------------------
        比较器输出选择配置，与CMP0_MOD配合使用
        CMP0_SEL[1:0]=00，比较器0工作在3比较器轮询模式，正端在CMP0P、CMP1P、CMP2P之间自动轮流选择，
                      负端固定接内置BEMF电阻的中心点，其输出结果分别送至CMP0_OUT、CMP1_OUT、CMP2_OUT
        CMP0_SEL[1:0]=01，比较器0选择CMP0对应的端口组合，正端接CMP0P，负端接内置BEMF电阻的中心点，输出接CMP0_OUT
        CMP0_SEL[1:0]=10，比较器0选择CMP1对应的端口组合，正端接CMP1P，负端接内置BEMF电阻的中心点，输出接CMP1_OUT
        CMP0_SEL[1:0]=11，比较器0选择CMP2对应的端口组合，正端接CMP2P，负端接内置BEMF电阻的中心点，输出接CMP2_OUT
        -----------------------------------------------------------------------------*/
    SetReg(CMP_CR2, CMP0SEL0 | CMP0SEL1, 0x00);
    /*  -------------------------------------------------------------------------------------------------
        比较器迟滞电压选择
        00: 无迟滞  01: ±3mV
        10: +6mV    11: +-12mV
        -------------------------------------------------------------------------------------------------*/
    SetReg(CMP_CR1, CMP0HYS0 | CMP0HYS1 , CMP0HYS0 | CMP0HYS1);
    /*  -------------------------------------------------------------------------------------------------
        CMP0的轮询时间设置
        -------------------------------------------------------------------------------------------------*/
    SetReg(CMP_CR2, CMP0SEL1 | CMP0SEL0, 0x00);
    /*  -------------------------------------------------------------------------------------------------
        比较器中断模式配置 CMP_CR0[CMP2IM1], CMP_CR0[CMP2IM0]
        00: 不产生中断  01: 上升沿产生中断  10: 下降沿产生中断  11: 上升/下降沿产生中断
        -------------------------------------------------------------------------------------------------*/
    SetReg(CMP_CR0, CMP2IM0 | CMP2IM1, CMP2IM0 | CMP2IM1);
    SetReg(CMP_CR0, CMP1IM0 | CMP1IM1, CMP1IM0 | CMP1IM1);
    SetReg(CMP_CR0, CMP0IM0 | CMP0IM1, CMP0IM0 | CMP0IM1);
   
    SetBit(CMP_CR2, CMP0EN);  //开三个比较器	
}

/**
* @brief         Timer2 初始化函数
* @date         2025-02-21
*/
static void Time2_Bemf_Init(void)
{
	SetBit(CK_CR , T2CKEN);
    /*  -------------------------------------------------------------------------------------------------
        先停止计数，配置完寄存器后，最后启动计数
        -------------------------------------------------------------------------------------------------*/
    ClrBit(TIM2_CR1, T2EN); // 0，停止计数；1,使能计数
    /*  -------------------------------------------------------------------------------------------------
        时钟分频设置(T2PSC)
        000:cpuclk(24MHz)         001:cpuclk/2^1(12MHz)   010:cpuclk/2^2(6MHz)    011:cpuclk/2^3(3MHz)
        100:cpuclk/2^4(1.5MHz)    101:cpuclk/2^5(750KHz)  110:cpuclk/2^6(375KHz)  111:cpuclk/2^7(187.5KHz)
        -------------------------------------------------------------------------------------------------*/
    SetReg(TIM2_CR0, T2PSC0 | T2PSC1 | T2PSC2, T2PSC0 | T2PSC1 | T2PSC2);
    /*  -------------------------------------------------------------------------------------------------
        /模式选择
		工作模式选择
		0: 输入捕获模式
		1: 输出模式

        -------------------------------------------------------------------------------------------------*/
    SetBit(TIM2_CR0, T2MOD) ;
    /*  -------------------------------------------------------------------------------------------------
        清除中断标志位
        禁止PWM周期检测中断使能
        -------------------------------------------------------------------------------------------------*/
    
    
    SetBit(TIM2_CR1, T2IFE);                                // 溢出中断使能
    ClrBit(TIM2_CR1, T2IR | T2IF | T2IP);                   // 清零中断标志位
    /*  -------------------------------------------------------------------------------------------------
        配置周期值、比较值、计数值
        禁止PWM周期检测中断使能
        使能计数器上溢中断使能
        -------------------------------------------------------------------------------------------------*/
    TIM2__ARR  = 60000;                                    // TIM2 Period = 0.32s
    TIM2__DR   = TIM2__ARR;
    TIM2__CNTR = 0;
    /*-----------启动计数------------------------------------------------*/
    SetBit(TIM2_CR1, T2EN);                                 //启动计数
}

/**
* @brief        基于Bemf的顺逆风检测初始化函数，电机状态切入 顺逆风检测状态时 运行一次
* @date         2022-07-09
*/
void BEMFDetectInit(void)
{
    //BEMF检测前关闭mos输出
    memset(&mcBemf, 0, sizeof(BEMFDetect_TypeDef));
    mcBemf.FR                   = BEMF_FR_ERR;
    mcBemf.FRPre                = BEMF_FR_ERR;
    mcBemf.SpeedUpdate          = 0;
    mcBemf.BEMFSpeedBase        = BEMFSpeedCalBase;
	
    mcBemf.FR_SET               = MotorSpeed.FR;	
    McCtrl.State_Count = TAILWIND_TIME;
    /* -----使能比较器----- */
	BEMF_CMP_Init();
	/* -----使能定时器1用于检测时间----- */
    Time2_Bemf_Init();
}



/* -------------------------------------------------------------------------------------------------
    Function Name  : GetBEMFStatus
    Description    : 
    Date           : 2024-09-28
    Parameter      : None
------------------------------------------------------------------------------------------------- */
uint8  GetBEMFStatus(void)
{
    uint8 BEMFStatus = 0;
    
    if (ReadBit(CMP_SR, CMP2OUT))
    {
        BEMFStatus += 4;
    }
    
    if (ReadBit(CMP_SR, CMP1OUT))
    {
        BEMFStatus += 2;
    }
    
    if (ReadBit(CMP_SR, CMP0OUT))
    {
        BEMFStatus += 1;
    }    
    return BEMFStatus;
}


/**
* @brief        bemf顺序为 5,1,3,2,6,4
* @param        bemfStatus 当前bemf状态
* @return       BEMF_FR_CCW 反转
* @return       BEMF_FR_CW  正转
* @return       BEMF_FR_ERR 错误
* @date         2022-07-09
*/
static uint8 FR_Detect(uint8 bemfStatus, uint8 setFr)
{
    static uint8 temp_FR = 0;
    static uint8 temp_bemfStatusPre = 0;
    
    if (temp_bemfStatusPre == 0)
    {
        temp_bemfStatusPre = bemfStatus;
        temp_FR = BEMF_FR_ERR;
    }
    
    if (temp_bemfStatusPre != bemfStatus)
    {
        if (temp_bemfStatusPre == table_Bemf_CWPre[bemfStatus - 1])
        {
            
            if (setFr  == CW)                 ///< 转向设置                            
            {
               temp_FR = BEMF_FR_CW; 
            }
            else
            {
                temp_FR = BEMF_FR_CCW; 
            }
            
            
        }
        else if (temp_bemfStatusPre == table_Bemf_CWNext[bemfStatus - 1])
        {
            if (setFr  == CW)                 ///< 转向设置                            
            {
               temp_FR = BEMF_FR_CCW; 
            }
            else
            {
                temp_FR = BEMF_FR_CW; 
            }
        }
        else
        {
            temp_FR = BEMF_FR_ERR;
        }
        
        temp_bemfStatusPre = bemfStatus;
    }   
    return temp_FR;
}


/**
* @brief        void BemfProcess(void)基于Bemf的顺逆风检测，运行于Bemf检测比较器中断
* @date         2022-07-09
*/
void BemfProcess(void)
{
	static uint8 temp_Cnt = 0;
    static uint8 temp_SumCnt = 0;
    uint16 temp_Sum = 0;
    int16 temp_speedCal =0;	
	
	if (temp_Cnt > 5)  // 防溢出越界
	{
		temp_Cnt = 0;
	}
	
	mcBemf.SectorTime[temp_Cnt++] = TIM2__CNTR;
	TIM2__CNTR = 0;
	
	mcBemf.BEMFStatus = GetBEMFStatus();
	/* -----根据BEMF状态判断FR状态----- */
	mcBemf.FRPre = mcBemf.FR;               // 获取上一次的转向
	mcBemf.FR = FR_Detect(mcBemf.BEMFStatus,mcBemf.FR_SET);   // 获取当前转向
	
	if (mcBemf.FR == BEMF_FR_CW)            // 当前为正转
	{
		if (mcBemf.FRPre == mcBemf.FR)
		{			
			if (mcBemf.FRCount < 100)
			{
				mcBemf.FRCount++;           //  连续两次均为正转则 正转计数+1
			}
			
			if (mcBemf.FRCount > 12)
			{
				temp_Sum = 0;
				
				for (temp_SumCnt = 0; temp_SumCnt < 6; temp_SumCnt++)
				{
					temp_Sum += (mcBemf.SectorTime[temp_SumCnt] >> 3); // 防止溢出
				}
				
				mcBemf.PeriodTime = temp_Sum;
				mcBemf.Status = BEMF_FORWARD; // 连续12次均为正向转动,则bemf状态切换为 BEMF_FORWARD
			}
		}
		else
		{
			mcBemf.FRCount = 0;
			mcBemf.Status = BEMF_DETECTING;
		}
	}
	else if (mcBemf.FR == BEMF_FR_CCW)
	{
		if (mcBemf.FRPre == mcBemf.FR)
		{			
			if (mcBemf.FRCount > - 100)
			{
				mcBemf.FRCount--;//  连续两次均为反转则 反转计数+1
			}
			
			if (mcBemf.FRCount < -12)
			{
				temp_Sum = 0;
				
				for (temp_SumCnt = 0; temp_SumCnt < 6; temp_SumCnt++)
				{
					temp_Sum += (mcBemf.SectorTime[temp_SumCnt] >> 3); // 防止溢出
				}
				
				mcBemf.PeriodTime = temp_Sum;
				mcBemf.Status =  BEMF_REVERSE;
			}
		}
		else
		{
			mcBemf.FRCount = 0;
			mcBemf.Status = BEMF_DETECTING;
		}
	}
	else
	{
		mcBemf.FRCount = 0; // 转向检测错误则清零转向计数
		mcBemf.Status = BEMF_DETECTING;
	}	
	
    if (mcBemf.Status == BEMF_FORWARD || mcBemf.Status == BEMF_REVERSE)  // 有效转向
    {   
        temp_speedCal = mcBemf.BEMFSpeedBase /mcBemf.PeriodTime ;
        
        if (mcBemf.Status == BEMF_REVERSE)
        {
            mcBemf.BEMFSpeed = -temp_speedCal;
        }
        else
        {
            mcBemf.BEMFSpeed  = temp_speedCal;
        }
        
        mcBemf.SpeedUpdate  = 1;
    }
}


/**
 * @brief     切入环路控制
 * @date      2025-02-13
 */
void BEMFCloseLoopStart(void)
{
/*根据转速调节切环后UQ给定，不宜差太多*/
        if (mcBemf.BEMFSpeed > S_Value(50000))
            McCtrl.mcSvUq = _Q15(0.40);
        else if (mcBemf.BEMFSpeed > S_Value(40000))
            McCtrl.mcSvUq = _Q15(0.35);
        else if (mcBemf.BEMFSpeed > S_Value(30000))
            McCtrl.mcSvUq = _Q15(0.30);
        else if (mcBemf.BEMFSpeed > S_Value(20000))
            McCtrl.mcSvUq = _Q15(0.20);
        else if (mcBemf.BEMFSpeed > S_Value(10000))
            McCtrl.mcSvUq = _Q15(0.10);
        else
            McCtrl.mcSvUq = _Q15(0.05);
    // SV_UQ      = McCtrl.mcSvUq;

    SVPWM_Init();
    // 计算当前切入的 SPEED_STEP
    McCtrl.mcSv_RTheStep = SPEED_STEP_K * mcBemf.BEMFSpeed; 
    SetBit(DRV_CR0, SVEN);             // SVPWM模块使能
    SPEED_STEP = McCtrl.mcSv_RTheStep; 
    DRV_CMR = 0x0ABF;                 // 输出
    SetBit(DRV_CR0, MESEL);            // ME工作模式选择 0: 方波驱动模式 1: SVPWM驱动模式
    SV_UQ = McCtrl.mcSvUq;             // 电压预定位占空比0.07%
    SvpwmPara.Bcor = SvpwmPara.Base_BCOR / McCtrl.mcSv_RTheStep; /*此处容易超出uint16需要注意*/

    SetBit(SV_CR0, ATADD);
    SVPWM_Swtich_Wind();
    TIM1_CR1 = (TIM1_CR1 & (0x80)) | (SV_BSEL_ANGLE_RUN);
    WIN_DLY = WIN_DLY_TAILWIND;
    
    if (MotorSpeed.FR == CW)
    {
        THETA = _Q15(30.0 / 180.0);
    }
    else
    {
        THETA = _Q15(180.0 / 180.0);
    }

    MOEH = 1;
    MOEL = 1;                 // 使能驱动输出 */
    CMP_CR0 = CMP_CR0 & 0xc0; // 关CMP0/1/2中断使能
    if(McCtrl.mcSvUq < SAM_THR_H_SV)
    {
            DRV_COMR = DrvCOMR_L;
            ClrBit(DRV_CR1, DCIM0);
            SetBit(DRV_CR1, DCIM1);
            ClrBit(CMP_CR3, SAMSEL1); /*OFF时比较器采样*/
            SetBit(CMP_CR3, SAMSEL0);
            LEAD_ANGLE = LEADANGLE_TAILWIND;
    }
    else
    {
            DRV_COMR = DrvCOMR_H;
            SetBit(DRV_CR1, DCIM0);
            ClrBit(DRV_CR1, DCIM1);      
            SetBit(CMP_CR3, SAMSEL1); /*ON时比较器采样*/
            SetBit(CMP_CR3, SAMSEL0);  
            LEAD_ANGLE = LEAD_ANGLE_RUN_1;
    }

    mcBemf.HighSpdStart = 2;
}
