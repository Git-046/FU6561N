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

#ifndef __MOTORCONTROLFUNCTION_H_
#define __MOTORCONTROLFUNCTION_H_
#include <FU6511.h>

/* Define to prevent recursive inclusion -------------------------------------*/

typedef union
{
    int32 s32;
    int16 s16[2];
}s32tos16;

typedef struct
{
  int16   Iw_busOffset;   //Iw或Ibus的偏置电压
  int32   Iw_busOffsetSum;//Iw或Ibus的偏置电压总和
	
  int16   I_busOffset;   //Iw或Ibus的偏置电压
  int32   I_busOffsetSum;//Iw或Ibus的偏置电压总和
	
  int16   OffsetCount;    //偏置电压采集计数
  int8    OffsetFlag;     //偏置电压结束标志位
}CurrentOffset;

typedef struct
{
    int8  FlagONOFF;  
	uint8 FR;             // 方向标志位 

	uint8  Speed_Updatflag;          //
	uint32 mcSpeedBase;              //电机速度基准
	uint16 mcSpeed;                  //电机速度	
	uint8  mcSpeed_Acc;			//加减速度情况
	int16  mcSpeed_Cnt;
	uint16 mcSpeed_RPM;
} MCRAMP;

typedef struct
{
	uint8 ChargeStep; // 预充电的步骤
	uint8 Start_Mode;
	uint16 State_Count; // 电机各个状态的时间计数

	s32tos16 mcVSPFltu32;
	int16 mcVSPFlt;

	int16 mcSvUq;			 // 输出占空比
	uint16 mcSv_RTheStep;	 // 输出爬坡输出
	int16 mcSv_Theta;		 // 当前输出角度
	int16 mcLeadAngle;		 // 超前角配置
	int16 RealLeadAngle;	// 真实超前角

	int16 mcPosCheckAngle; // 初始位置检测角度

	uint16 PowerDownCnt;
	int16 LastmcDcbus;
	int16 mcDcbus;	  // 母线电压滤波后的低16位
	int16 mcDcbusFlt; // 母线电压滤波后的高16位
	s32tos16 mcDcbusFltu32;
	int16 mcIbusADC; // 读取的AD值，还没有减去偏置电压
	int16 mcIbusFlt; // 母线电流
	s32tos16 mcIbusFltu32;
	int32 u32mcPower; // 电机功率
	int16 mcPower;	  // 电机功率
	int16 mcPowerFlt; // 电机功率

	uint16 TargetValue; //环路最终目标值
	uint16 LoopRampValue; //环路爬坡目标值
	uint16 mcLoopTime; // 环路时间控制

	uint16 TargetDlyAngle;		  //
	uint16 TargetBSEL_ANGLE;	  //
	uint16 RUN_ECycleCnt;		  // 运行电周期计数
	uint8 OnOffSampleSwitch_Flag; // 采样过零时序切换
	uint8 OnOffSampleStatus;	  // 采样状态标志位
	int16 NTCTempFlt;			//NTC温度值

	uint8 SampFlag;	//采样电流电压标志位
	uint8 WindowFlag; //开窗标志位
	uint8 SampFinishFlag; //反电动势采样结束标志
	int16 mcLeadAngleActComp;
	int16 mcLeadAngleIntComp;
	uint16 mcLeadAngleAct;
	uint16 mcLeadAngleCount;
	uint8 mcAdapLeadAngleEnable;
	s32tos16 sq_speedU32;
	uint16 K_Speed_BEMF;
	uint8 AdaMode;		//自适应模式
	uint8 AdaHighFlag; //检测到载波标志
	uint8 AdaLowFlag; //检测到续流标志
	int16 BEMF_THR_H; //反电动势高比较阈值
	int16 BEMF_THR_L; //反电动势低比较阈值

	int8 LowPWMCnt; //低载波比计数
	uint8 LowPWMMode; //低载波比模式

	uint16 OverSpeedLimit; //转速限制值
	uint8 RunMode;	//0:SVPWM普通模式 1:方波或者SVPWM弱磁模式 2:掉电模式
	uint8 Mode_SwitchFlag; //弱磁切换模式标志位
	uint16 mcDuty;
	uint8 BLDC_SpeedCnt;
	int16 WeakenCnt;
	int16 WeakenAngle;

	int16 SPIVar1;
	int16 SPIVar2;
	int16 SPIVar3;
	int16 SPIVar4;

} MOTORCTRL;

/* PWMControl_TypeDef 結構 */
typedef struct
{
  volatile uint8  isBusy;     ///< PWM 信號處理忙碌旗標
  volatile uint16 Compare;    ///< PWM 信號比較值
  volatile uint16 Cycle;      ///< PWM 信號週期值
  
  uint16  Duty;               ///< PWM 信號佔空比
  uint8   TurnOff_Filter;     ///< 關機濾波計數值
  uint8   TurnOn_Filter;      ///< 開機濾波計數值
  uint16  PWMCompareOld;// 上一次的比较值 
  uint16  PWMARROld;    // 上一次的周期值
}PWMControl_TypeDef;

typedef struct 
{
  uint8  Update;              ///< FG 更新旗標
  uint16 FG_ARR_Val;          ///< FG ARR 暫存

  uint16 refreshTimes;        ///< 信號更新計數
}rotate_Signal_HandleType;


extern CurrentOffset xdata mcCurOffset;
extern MCRAMP        xdata MotorSpeed;
extern MOTORCTRL     xdata McCtrl;
extern PWMControl_TypeDef xdata mcPWMCtrl;
extern rotate_Signal_HandleType xdata SO_Var;
extern void VariablesPreInit(void);

extern void GetCurrentOffset(void);
extern void SquStartInit(void);
extern void Motor_Ready (void);
extern void Motor_Init (void);
extern void Motor_Charge(void);
extern void Motor_TailWind(void);
extern void Motor_Align(void);
extern void Motor_Start(void);
extern void Motor_Run(void);
extern void MC_Stop(void);
extern void MC_Break(void);
extern void SVPWM_LeadAngle(void);
extern void SVPWM_FlowAngle(void);
extern void SVPWM_WindowAngle(void);
extern void SVPWM_AngleAdjust(void);
extern void MotorAlignProcess(void);
extern void ADC_Channel_Recover(void);

extern void SVPWM_to_Squ(void);
extern void Squ_to_SVPWM(void);
#endif