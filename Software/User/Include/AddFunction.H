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
#ifndef __AddFunction_H_
#define __AddFunction_H_

/******************************************************************************/

#include <FU6511.h>
/******************************************************************************/

/* Exported types ------------------------------------------------------------*/

typedef struct
{
    uint32 ONOFF_Times;                                                         // 启停次数
    uint16 ON_Count;                                                            // 运行时间计数
    uint16 OFF_Count;                                                           // 停止时间计数
    uint8  ONOFF_Flag;                                                          // 启停测试中启动标志位
} ONVarible;
/* Exported variables ---------------------------------------------------------------------------*/
extern bool data isCtrlPowOn; // 电机运行启动标志
extern ONVarible ONOFFTest;
/* Exported functions ---------------------------------------------------------------------------*/


extern void VariablesPreInit(void);
extern void PWM_speedControl(void);
extern void FGOutput_Update(void);
extern void TickCycle_1ms(void);
extern void FGOutput(void);
extern void Loop_response(void);
extern void ONOFF_Starttest(void);

extern uint32 Abs_F32(int32 value);

/******* Sleep控制**************/
extern void SleepMode_Init(void);
extern void SleepMode(void);

/******* **********************/
#endif
