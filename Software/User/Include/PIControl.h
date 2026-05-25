/**
 * @file     
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2025-1-15
 * @brief    ...
 * 
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
    Header Definition
********************************************************************************/
#ifndef __PICONTROL_H__
#define __PICONTROL_H__

#include <FU6511.h>
/*************************************************************************************///External Function
typedef struct
{
	int16 ActualVaule;          //ʵ��ֵ
    int16 ErrValue;             //��ֵE(k)
    int16 PreErrValue;          //��ֵE(k-1)
    int16 Kp;                   //����Kp
    int16 Ki;                   //����Ki
    int16 ValueMin;             //
    int16 ValueMax;             //
//	int32 u32Valuetemp;
//    int16 u16Valuetemp;
    int16 ValueOut;
		int32 ValueOuti32;
    int32 ValuetempKp;
    int32 ValuetempKi;
}PIControl;

extern PIControl xdata PID;
extern PIControl xdata PID2;
extern void PI_Init(void);
extern void PI_Software(void);
extern void PI2_Init(void);
extern void PI2_Software(void);
#endif