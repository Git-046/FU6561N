/**
 * @file     
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2024-7-15
 * @brief    ??????...
 * 
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
    Header Definition
********************************************************************************/

#ifndef __ADATIVELEADANGLE_H_
#define __ADATIVELEADANGLE_H_
#include <FU6511.h>

#define Special_Detected            (1)         //第一个波不完整的时候提高超前角，不适合该情况时置0
#define AdaStuck_Gain				(int8)(2)	//堵孔增益
#define Ada_S_Gain					(int8)(10)	// 特殊情况增益
#define Ada_H_Gain				(int8)(2)	// 高占空比-2载波增益
#define Ada_L_Gain				(int8)(3)	// 低占空比-3载波增益
#define Ada_H_DeadZone			(int16)(500)	//高占空比-2载波判断死区
#define Ada_L_DeadZone			(int16)(300)	//低占空比-3载波判断死区
#define Ada_Acc_Comp            (int16)(10) //低功率切换到高功率时的超前角补偿
/* Define to prevent recursive inclusion -------------------------------------*/
extern void AdaLeadAngleSamp(void);
extern void AdaLeadAngleFast(void);
extern void AdaLeadAngleSlow(void);
extern void AdaLeadAngleApply(void);
extern void AdaLeadAngleLowPWM(void);
#endif