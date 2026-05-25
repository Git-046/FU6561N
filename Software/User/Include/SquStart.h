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

#ifndef __SQUSTART_H_
#define __SQUSTART_H_
#include <FU6511.h>

/* Define to prevent recursive inclusion -------------------------------------*/

typedef struct 
{
	uint8 StartState;
	uint8 Sector;
//	int16 TargetTheta;
	int16 Ibus_Offset;
	int16 Isample;
	int16 IsampleSum1;
	int16 IsampleSum2;
	int16 IsampleRecord;
	int16 IsampleSumMin;
	int16 IsampleSumMax;
	uint16 ForcedStartCount;
	uint16 IgnoreCount;
	uint16 FocredPeriod;
	uint8 IinterCount1;
	uint8 IinterCount2;
	uint8 PhaseCommutatCount;
	uint8 PhaseCommutatCount1;
	int16	PhaseCommutCurrentThre;
	uint8 ThetaRamp;
	uint8 ThetaRampEnable;
	int16 PWMFrequency;
	uint16 MotorSpeedCount1;
	uint16 MotorSpeedCount2;
	uint16 MotorSpeedCount3;
	uint16 SwitchSVPWMThreshold;
	uint16 MotorSpeedCountSum;
	int16 StartCurrentValue;
	int16 StartCurrentHoldValue;
	uint8 UQRampEnable;
	int16 StartUq;
//	uint8 SquStartCount;
	uint8 FR;
}SquStartVar;

typedef struct 
{
//	uint8 FRFlag;
//	uint8 CMPOUT;
	uint8 BEMFCWCount;
	uint8 BEMFCCWCount;
}BMEFUpdateVar;


extern SquStartVar data SquStart;
extern BMEFUpdateVar data BMEFUpdate;

extern void BEMFDetectProcess(void);

extern void PhaseCommutationInit(void);	
extern void SquStartProcess(void);
extern void PhaseCommutationProcess(void);
#endif