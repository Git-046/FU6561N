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
#include <MyProject.h>

/* Public variables --------------------------------------------------------- */

PIControl xdata PID;
#if (OverSpeedLimitEnable)
PIControl xdata PID2;
#endif

/**
 * @brief        PI
 * @date         2025-01-15
 */
void PI_Init(void)
{
    PID.Kp = LOOP_KP_Start;
    PID.Ki = LOOP_KI_Start;
    PID.ErrValue = 0x00;
    PID.PreErrValue = 0x00;
    PID.ValueMax = LOOP_PIMax - 50;
    PID.ValueMin = LOOP_PIMIN;	
}

/**
 * @brief        PI_Software
 * @brief        U[k] = U[k-1] + Kp*(E[k] - E[k-1]) + Ki*E[K];
 * @date         2025-01-15
 */
void PI_Software(void)
{
	int32 tmp;
	PID.ValuetempKp = (int32)(PID.ErrValue - PID.PreErrValue) * PID.Kp;
	PID.ValuetempKi = (int32)PID.ErrValue * PID.Ki;
	
	tmp = PID.ValueOuti32 + PID.ValuetempKp + PID.ValuetempKi;
	if(tmp < 0 || tmp > PI_I_MAX) /*溢出判断*/
	{
		PID.ValueOut = PID.ValueMax;
		PID.ValueOuti32 = PI_I_MAX;
	}
	else
	{
		PID.ValueOuti32 = tmp;
		PID.ValueOut = (int16)(PID.ValueOuti32 >> 15);
	}
//	PID.u32Valuetemp = PID.ValuetempKp + PID.ValuetempKi;
//    PID.u16Valuetemp = PID.u32Valuetemp>>15;
	
//	PID.ValueOut = (PID.u16Valuetemp) + PID.ValueOut;
	
	if (PID.ValueOut > PID.ValueMax)
	{
		PID.ValueOut = PID.ValueMax;
	}
	else if (PID.ValueOut < PID.ValueMin)
	{
		PID.ValueOut = PID.ValueMin;
	}
	PID.PreErrValue = PID.ErrValue;
}

#if (OverSpeedLimitEnable)
/*限速环*/
/**
 * @brief        PI
 * @date         2025-01-15
 */
void PI2_Init(void)
{
    PID2.Kp = S2KP;
    PID2.Ki = S2KI;
    PID2.ErrValue = 0x00;
    PID2.PreErrValue = 0x00;
    PID2.ValueMax = LOOP_PIMax - 50;
    PID2.ValueMin = LOOP_PIMIN;	
}

/**
 * @brief        PI_Software
 * @brief        U[k] = U[k-1] + Kp*(E[k] - E[k-1]) + Ki*E[K];
 * @date         2025-01-15
 */
void PI2_Software(void)
{
	int32 tmp;
	PID2.ValuetempKp = (int32)(PID2.ErrValue - PID2.PreErrValue) * PID2.Kp;
	PID2.ValuetempKi = (int32)PID2.ErrValue * PID2.Ki;
	
	tmp = PID2.ValueOuti32 + PID2.ValuetempKp + PID2.ValuetempKi;
	if(tmp < 0 || tmp > PI_I_MAX) /*溢出判断*/
	{
		PID2.ValueOut = PID2.ValueMax;
		PID2.ValueOuti32 = PI_I_MAX;
	}
	else
	{
		PID2.ValueOuti32 = tmp;
		PID2.ValueOut = (int16)(PID2.ValueOuti32 >> 15);
	}
//	PID2.u32Valuetemp = PID2.ValuetempKp + PID2.ValuetempKi;
//    PID2.u16Valuetemp = PID2.u32Valuetemp>>15;
	
//	PID2.ValueOut = (PID2.u16Valuetemp) + PID2.ValueOut;
	
	if (PID2.ValueOut > PID2.ValueMax)
	{
		PID2.ValueOut = PID2.ValueMax;
	}
	else if (PID2.ValueOut < PID2.ValueMin)
	{
		PID2.ValueOut = PID2.ValueMin;
	}
	PID2.PreErrValue = PID2.ErrValue;
}
#endif