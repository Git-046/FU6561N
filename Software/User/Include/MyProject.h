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
#ifndef __MYPROJECT_H_
#define __MYPROJECT_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <FU6511_MCU.h>
#include <definition.h>
#include <Customer.h>
#include <Parameter.h>
#include <Protect.h>
#include <Customer_Debug.h>
#include <AMP.h>
#include <GPIO.h>
#include <ADC.h>
#include <AMP.h>
#include <CMP.h>
#include <Driver.h>
#include <GPIO.h>
#include <TIMER.h>
#include <AddFunction.h>
#include <MotorControlFunction.h>
#include <MotorControl.h>
#include <OVCP.h>
#include <PIControl.h>
#include <TailWind.h>
#include <SquStart.h>
#include <Key_Driver.h>
#include <AdativeLeadAngle.h>
#include <UART.h>
#include <FU6511_Flash.h>
extern uint8 data g_1mTick;                   ///< 1ms滴答信号，每隔1ms在SYSTICK定时器被置1，需在大循环使用处清零
#endif
