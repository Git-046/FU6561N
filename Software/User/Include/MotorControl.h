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
#ifndef __MOTORCONTROL_H_
#define __MOTORCONTROL_H_


/* Exported types -------------------------------------------------------------------------------*/
typedef enum
{
    mcReady     = 0,    ///< 准备状态,该状态电机空闲，等待控制命令 
    mcInit      = 1,    ///< 初始化,该状态进行启动前的变量初始化
    mcCharge    = 2,    ///< 预充电,电机启动前给自举电容充电，一般用于高压驱动，低压驱动一般不需要
    mcTailWind  = 3,    ///< 顺逆风检测,该状态下电机进行顺逆风检测 
    mcAlign     = 4,    ///< 预定位
    mcStart     = 5,    ///< 启动，用于配置启动代码
    mcRun       = 6,    ///< 运行，
    mcStop      = 7,    ///< 停止
    mcFault     = 8,    ///< 故障状态
    mcBrake     = 9,   ///< 刹车
} MotStateType;



typedef union
{
    uint8 SetMode;                                                              ///< 整个配置模式使能位
    struct
    {
        uint8 CalibFlag        :1;                                              ///< 电流校准的标志位
        uint8 ChargeSetFlag    :1;                                              ///< 预充电配置标志位
        uint8 AlignSetFlag     :1;                                              ///< 预定位配置标志位
        uint8 TailWindSetFlag  :1;                                              ///< 顺逆风配置标志位
        uint8 StartSetFlag     :1;                                              ///< 启动配置标志位
        uint8 PosiCheckSetFlag :1;                                              ///< 位置检测配置标志位
        uint8 Break            :1;                                              ////</ 刹车配置标志位
				uint8 RunFlag          :1;                                              ////</ 刹车配置标志位
    } SetFlag;
}MotStaM;


typedef struct{
	uint32 Base_BCOR;
	uint16 Bcor;
	uint16 Sv_RThetaDelay;
	uint8  Sv_SpeedFltFlag;

	// uint16 last_BCCR;      // 上一个电周期捕获值
    // int16 angle_comp;      // 角度补偿值(Q15格式)
    // int16 accel_factor;      // 加速度因子
    // uint8 comp_enable;     // 补偿使能标志
	
	
}SVPWMParaType;
	

/* Exported variables ---------------------------------------------------------------------------*/
extern MotStateType data mcState;

extern MotStaM    McStaSet;

extern SVPWMParaType xdata SvpwmPara;

/* Exported functions ---------------------------------------------------------------------------*/
extern void MC_Control(void);
extern void MotorcontrolInit(void);
extern void McTailWindDealwith(void);
extern void TailWindDealwith(void);
extern void Sample_Swicth(void);
extern void SVPWM_Init(void);
extern void SVPWM_Swtich_Wind(void);

#endif
/*** (C) Copyright 2024	Fortior	Technology Co.,	Ltd. ***/
