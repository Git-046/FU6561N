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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <FU6511.h>

//#define TIM3_Fre                       (750000.0)                                // TIM0计数频率750KHz
//#define TIM4_Fre                       (6000.0)                                  // TIM0计数频率750KHz

/*************************************************************************************///External Function

/** @defgroup TIM_ClockDivision TIM 時脈除頻
  * @{
  */
#define  TIM_CLOCKDIVISION_DIV1                (0x00)                                  /*!< 不除頻 (24MHz)          */
#define  TIM_CLOCKDIVISION_DIV2                (T4PSC0)                                /*!< 除以 2 (12MHz)          */ 
#define  TIM_CLOCKDIVISION_DIV4                (T4PSC1)                                /*!< 除以 4 (6MHz)           */
#define  TIM_CLOCKDIVISION_DIV8                (T4PSC1 | T4PSC0)                       /*!< 除以 8 (3MHz)           */ 
#define  TIM_CLOCKDIVISION_DIV16               (T4PSC2)                                /*!< 除以 16 (1.5MHz)        */
#define  TIM_CLOCKDIVISION_DIV32               (T4PSC2 | T4PSC0)                       /*!< 除以 32 (750KHz)        */ 
#define  TIM_CLOCKDIVISION_DIV64               (T4PSC2 | T4PSC1)                       /*!< 除以 64 (375KHz)        */ 
#define  TIM_CLOCKDIVISION_DIV128              (T4PSC2 | T4PSC1 | T4PSC0)              /*!< 除以 128 (187.5KHz)     */
/**
  * @}
  */
  
#define FG_CLOCKBASE_SELECT                    (TIM_CLOCKDIVISION_DIV8)              /*!< FG 除頻器設置 ( @ref SignalOutput_Type 為 FG_Signal 時有效) */

/*!< @brief FG TIM除頻器配置 */
#if (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV1)
  #define  FG_FREQ_BASE                         (24000000.0)            ///< 不除頻  (24MHz)
#elif (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV2)
  #define  FG_FREQ_BASE                         (12000000.0)            ///< 除2     (12MHz)
#elif (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV4)
  #define  FG_FREQ_BASE                         (6000000.0)             ///< 除4     (6MHz)
#elif (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV8)
  #define  FG_FREQ_BASE                         (3000000.0)             ///< 除8     (3MHz)
#elif (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV16)
  #define  FG_FREQ_BASE                         (1500000.0)             ///< 除16    (1.5MHz)
#elif (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV32)
  #define  FG_FREQ_BASE                         (750000.0)              ///< 除32    (750kHz)
#elif (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV64)
  #define  FG_FREQ_BASE                         (375000.0)              ///< 除64    (375kHz)
#elif (FG_CLOCKBASE_SELECT == TIM_CLOCKDIVISION_DIV128)
  #define  FG_FREQ_BASE                         (187500.0)              ///< 除128   (87.5kHz)
#else
  #error "FG_CLOCKBASE_SELECT setting error."
#endif

extern void Timer1_Init(void);
extern void Timer1_Sv_Init(void);
extern void Timer1_STH_Init(void);
extern void Timer2_Init(void);
extern void Timer3_Init(void);
extern void Timer4_Init(void);
extern void WatchDogRefresh(void);
extern void WatchDogConfig(uint16 Value, bool Status);
#endif
