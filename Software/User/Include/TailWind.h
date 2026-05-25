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
#ifndef __TAILWIND_H__
#define __TAILWIND_H__
#include <FU6511.h>
/*************************************************************************************///External Function

#define BEMF_FR_CCW                           (0xA0) // 
#define BEMF_FR_CW                            (0xB0) // 
#define BEMF_FR_ERR                           (0xC0) // 


#define  BEMF_FORWARD                           (1)
#define  BEMF_REVERSE                           (2)
#define  BEMF_STATIC                            (3)
#define  BEMF_DETECTING                         (0)

#define BEMFSpeedCalBase                        (int32)(32767.0 / 8.0 * (TIM2_Fre * 60 / Pole_Pairs / MOTOR_SPEED_BASE))

#define BEMFSpeedCalMinPeriod                   (int32)(1.0/8.0 * (TIM2_Fre * 60 / Pole_Pairs / MOTOR_SPEED_BASE)) + 1

#define SPEED_STEP_K                            (float)(Pole_Pairs *256 * 2 * MOTOR_SPEED_BASE /(1.5 * 1000 * 1000 * 60))
/*与角度累加的周期有关，当前设置1.5MHz触发（寄存器SV_CR0)*/

#define FORCE_BRAKE_THR                         (2000)
typedef struct
{
	uint8  BEMFStatus;                  // bemf检测状态
    int16  BEMFSpeed;                   //反电动势检测的速度
    uint32 BEMFSpeedBase;               //反电动势检测的速度基准
    uint8  Status;                      // bemf检测状态
    
    
    uint8  FR_SET;
    uint8  FR ;                         // 当前转向
    uint8  FRPre;                       // 上一次转向
    int8   FRCount;                       // 正确转向计数，正数为CW，负数为CCW
    
    uint8  SpeedUpdate;              ///< 速度更新标志
    uint8  HighSpdStart;
      
    uint16 PeriodTime;                  //转一圈的周期计数值/8,因除数只能是16位的
    uint16 SectorTime[6];               // 60度扇区时间

    uint16 BrakeTime;
    uint16 ForceBrakeCnt;

}BEMFDetect_TypeDef;

extern BEMFDetect_TypeDef xdata mcBemf;
extern void BEMFDetectInit(void);
extern MotStateType Bemf_Start_Process(void);
extern void BemfProcess(void);
extern void BEMF_CMP_Init(void);
extern uint8 GetBEMFStatus(void);
extern void BEMFCloseLoopStart(void);


#endif