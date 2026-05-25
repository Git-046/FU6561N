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
#ifndef __Parameter_H_
#define __Parameter_H_
#include <definition.h>
/* Define to prevent recursive inclusion -------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define SystemPowerUpTime               (50000)                                                       // 上电等待时间，cpu计数时钟

/*芯片参数值------------------------------------------------------------------*/
/*CPU and PWM Parameter*/

/*CPU Parameter*/
#define MCU_CLOCK                       (24.0)                                                        ///< (MHz) 主频
#define PWM_VALUE_LOAD                  (uint16)(MCU_CLOCK * 500 / PWM_FREQUENCY)                 ///< PWM 定时器重载值 20190514修改

/*deadtime Parameter*/
#define PWM_LOAD_DEADTIME               (PWM_DEADTIME * MCU_CLOCK)                                    ///< 死区设置值

#define PWM_VALUE_LOAD_Start            (uint16)(MCU_CLOCK * 500 / PWM_FREQUENCY_Start)                 ///< PWM 定时器重载值 20190514修改

/*deadtime Parameter*/
#define PWM_LOAD_DEADTIME_Start         (PWM_DEADTIME_Start * MCU_CLOCK)                                    ///< 死区设置值

/*硬件板子参数设置值------------------------------------------------------------------*/
/*hardware current sample Parameter*/


#if (HW_ADC_VREF==VREF5_0)
 #define HW_ADC_REF                     (5.0)                                    ///< (V)  ADC参考电压
#elif (HW_ADC_VREF==VREF4_5)
 #define HW_ADC_REF                     (4.5)                                    ///< (V)  ADC参考电压
#elif (HW_ADC_VREF==VREF4_0)
 #define HW_ADC_REF                     (4.0)                                    ///< (V)  ADC参考电压
#elif (HW_ADC_VREF==VREF3_0)
 #define HW_ADC_REF                     (3.0)                                    ///< (V)  ADC参考电压
#endif

#define RV                              ((RV1 + RV2) / RV2)                     ///< 分压比


/*电流基准的电路参数*/
#define HW_BOARD_CURR_MAX               (HW_ADC_REF / 2 / HW_AMPGAIN / HW_RSHUNT)                     ///< 最大采样电流
#define HW_BOARD_CURR_MIN               (-HW_BOARD_CURR_MAX)                                          ///< 最小采样电流
#define HW_BOARD_CURR_BASE              (HW_BOARD_CURR_MAX * 2)                                       ///< 电流基准

/*hardware voltage sample Parameter*/
/*母线电压采样分压电路参数*/
#define HW_BOARD_VOLT_MAX               (HW_ADC_REF * RV)                           ///< (V)  ADC可测得的最大母线电压
#define HW_BOARD_VOLTAGE_BASE           (HW_BOARD_VOLT_MAX / 1.732)                 ///< 电压基准
/* -----过温保护值设置----- */
#define Tempera_Value(NTC_Value) 		_Q15((5.0*NTC_Value/(10.0+NTC_Value))/HW_ADC_REF)	///< 10K上拉电阻时，NTC阻值对应Q15_AD值，单位：KΩ
/*硬件过流保护DAC值*/
// #define DAC_OverCurrentValue            _Q6(I_ValueX(HWOCValue)) + 0x1F             ///< 
#define DAC_OverCurrentValue            _Q8(I_ValueX(HWOCValue)) + 0x7F             ///< 

/*初始位置检测DAC值*/
#define DAC_IPDCurValue                 _Q8(I_ValueX(IPD_CurValue)) + 0x7F             ///< 

/* obsever parameter set value */
#define BASE_FREQ                       ((MOTOR_SPEED_BASE / 60.0) * Pole_Pairs)                        ///< 基准频率

#define POWER_BASE                      (HW_BOARD_VOLT_MAX*HW_BOARD_CURR_BASE/16)                        ///< 基准功率

/* -----Current Calib----- *
/* -----(Disable)---禁止-- */
/* -----(Enable)----使能-- */
#define CalibENDIS                    (Enable)

/*续流屏蔽+延迟换相计算公式*/
#define ANGLE_MASK(ANGLE_M)           {TIM1_CR1  =(TIM1_CR1 &(0x80)) | (uint8)(ANGLE_M*127/60);}
#define ANGLE_DELAY(ANGLE_D)          {TIM1_CR2  =(TIM1_CR2 &(0x80)) | (uint8)(ANGLE_D*127/60);}

#define ANGLE_MASK_R(ANGLE_M_R)       (uint8)(ANGLE_M_R*127/60)
#define ANGLE_DELAY_R(ANGLE_D_R)      (uint8)(ANGLE_D_R*127/60)

#define POWER_LOOP_CONTROL            0x0A
#define SPEED_LOOP_CONTROL            0x0B

/*
* TIM1的时钟频率对应电机的转速
                         最低转速   一步的时长        理论最高转速
                                                （最少100个计数值）
* 00 -- 24      MHz ---  3662eRPM ---   2.7ms  ---   2,400,000eRPM
* 00 -- 12      MHz ---  1831eRPM ---   5.4ms  ---   1,200,000eRPM
* 00 -- 6       MHz ---   915eRPM ---  10.9ms  ---     600,000eRPM
* 00 -- 3       MHz ---   457eRPM ---  21.8ms  ---     300,000eRPM
* 00 -- 1.5     MHz ---   228eRPM ---  43.7ms  ---     150,000eRPM
* 00 -- 750     KHz ---   114eRPM ---  87.3ms  ---      75,000eRPM
* 00 -- 375     KHz ---    57eRPM --- 174.7ms  ---      37,500eRPM
* 00 -- 187.5   KHz ---    28eRPM --- 349.5ms  ---      18,750eRPM
*/


/*预充电，DUTY控制占空比*/
#define UL_PWM				(ULE)
#define VL_PWM			    (VLE)
#define WL_PWM		        (WLE)

/*三相下桥全开，不受DUTY所控制*/
#define UL_VL_WL_ON			(ULP | VLP | WLP)

#define UVW_OFF             0x0000                                  //UVW全关



/*斩波方式-A ： 上PWM下ON，DUTY控制占空比*/
/*
	注：配置中高四位为120度换相，低四位为150度换相
*/
#define  WH_VL_A    (((    0   |    0   |    0   |    0   )<<4) | (    0   | T1ULP1 |    0   |    0   ))
#define  UH_VL_A    (((    0   |    0   | T1UHE1 |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  UH_WL_A    (((    0   |    0   | T1UHE1 |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  VH_WL_A    (((    0   |    0   |    0   | 	  0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  VH_UL_A    (((    0   | T1ULP1 |    0   | 	  0   )<<4) | (    0   | T1ULP1 |    0   | 	  0   ))
#define  WH_UL_A    (((    0   | T1ULP1 | 	 0   | 	  0   )<<4) | (    0   | T1ULP1 |    0   | 	  0   ))


/*斩波方式-B ： 上ON下PWM，DUTY控制占空比*/
#define  WH_VL_B    (((    0   |    0   |    0   |    0   )<<4) | (    0   |    0   |    0   | T1ULE1 ))
#define  UH_VL_B    ((( T1UHP1 |    0   |    0   |    0   )<<4) | ( T1UHP1 |    0   |    0   |    0   ))
#define  UH_WL_B    ((( T1UHP1 |    0   |    0   |    0   )<<4) | ( T1UHP1 |    0   |    0   |    0   ))
#define  VH_WL_B    (((    0   |    0   | 	 0   | 	  0   )<<4) | ( T1UHP1 |    0   |    0   |    0   ))
#define  VH_UL_B    (((    0   |    0   |    0   | T1ULE1 )<<4) | (    0   |    0   |    0   | T1ULE1 ))
#define  WH_UL_B    (((    0   |    0   | 	 0   | T1ULE1 )<<4) | (    0   |    0   |    0   | T1ULE1 ))


/*斩波方式-C ： 上PWM互补下ON，DUTY控制占空比*/
#define  WH_VL_C    (((    0   |    0   |    0   |    0   )<<4) | (    0   | T1ULP1 |    0   |    0   ))
#define  UH_VL_C    (((    0   | T1ULP1 | T1UHE1 | T1ULE1 )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULE1 ))
#define  UH_WL_C    (((    0   | T1ULP1 | T1UHE1 | T1ULE1 )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULE1 ))
#define  VH_WL_C    (((    0   |      0 |    0   | 	  0   )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULE1 ))
#define  VH_UL_C    (((    0   | T1ULP1 | 	 0   | 	  0   )<<4) | (    0   | T1ULP1 |    0   | 	  0   ))
#define  WH_UL_C    (((    0   | T1ULP1 | 	 0   | 	  0   )<<4) | (    0   | T1ULP1 |    0   | 	  0   ))


/*斩波方式-D ：上ON下PWM_COM，DUTY控制占空比*/
#define  WH_VL_D    (((    0   |    0   |    0   |    0   )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))
#define  UH_VL_D    ((( T1UHP1 |    0   |    0   |    0   )<<4) | ( T1UHP1 |    0   |    0   |    0   ))
#define  UH_WL_D    ((( T1UHP1 |    0   |    0   |    0   )<<4) | ( T1UHP1 |    0   |    0   |    0   ))
#define  VH_WL_D    (((    0   |    0   | 	 0   | 	 0    )<<4) | ( T1UHP1 |    0   |    0   |    0   ))
#define  VH_UL_D    ((( T1UHP1 |    0   | T1UHE1 | T1ULE1 )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))
#define  WH_UL_D    ((( T1UHP1 |    0   | T1UHE1 | T1ULE1 )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))


/*斩波方式-E ：上PWM_COM下PWM_COM，DUTY控制占空比*/
/*双极性互补斩波,最低占空比51%*/
#define  WH_VL_E    (((    0   |    0   |    0   |    0   )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))
#define  UH_VL_E    (((    0   | T1ULP1 | T1UHE1 | T1ULE1 )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULE1 ))
#define  UH_WL_E    (((    0   | T1ULP1 | T1UHE1 | T1ULE1 )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULE1 ))
#define  VH_WL_E    (((    0   |    0   | 	 0   |    0   )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULE1 ))
#define  VH_UL_E    ((( T1UHP1 |    0   | T1UHE1 | T1ULE1 )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))
#define  WH_UL_E    ((( T1UHP1 |    0   | T1UHE1 | T1ULE1 )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))
 
 
/*斩波方式-F ：上PWM下PWM_COM，DUTY控制占空比*/
#define  WH_VL_F    (((    0   |    0   |    0   |    0   )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))
#define  UH_VL_F    (((    0   |    0   | T1UHE1 |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  UH_WL_F    (((    0   |    0   | T1UHE1 |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  VH_WL_F    (((    0   |    0   | 	 0   |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  VH_UL_F    ((( T1UHP1 |    0   | T1UHE1 | T1ULE1 )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))
#define  WH_UL_F    ((( T1UHP1 |    0   | T1UHE1 | T1ULE1 )<<4) | ( T1UHP1 |    0   | T1UHE1 | T1ULE1 ))


/*斩波方式-G ：上PWM_COM下PWM，DUTY控制占空比*/
#define  WH_VL_G    (((    0   |    0   |    0   |    0   )<<4) | (    0   |    0   |    0   | T1ULE1 ))
#define  UH_VL_G    (((    0   | T1ULP1 | T1UHE1 | T1ULE1 )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULP1 ))
#define  UH_WL_G    (((    0   | T1ULP1 | T1UHE1 | T1ULE1 )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULP1 ))
#define  VH_WL_G    (((    0   |    0   | 	 0   |    0   )<<4) | (    0   | T1ULP1 | T1UHE1 | T1ULP1 ))
#define  VH_UL_G    (((    0   |    0   |    0   | T1ULE1 )<<4) | (    0   |    0   |    0   | T1ULE1 ))
#define  WH_UL_G    (((    0   |    0   |    0   | T1ULE1 )<<4) | (    0   |    0   |    0   | T1ULE1 ))


/*斩波方式-H ：上PWM下PWM，DUTY控制占空比*/
#define  WH_VL_H    (((    0   |    0   |    0   |    0   )<<4) | (    0   |    0   |    0   | T1ULE1 ))
#define  UH_VL_H    (((    0   |    0   | T1UHE1 |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  UH_WL_H    (((    0   |    0   | T1UHE1 |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  VH_WL_H    (((    0   |    0   |    0   |    0   )<<4) | (    0   |    0   | T1UHE1 |    0   ))
#define  VH_UL_H    (((    0   |    0   |    0   | T1ULE1 )<<4) | (    0   |    0   |    0   | T1ULE1 ))
#define  WH_UL_H    (((    0   |    0   |    0   | T1ULE1 )<<4) | (    0   |    0   |    0   | T1ULE1 ))


/*斩波方式选择*/
#if (Chopping_Mode_Use == Chopping_ModeA)
     #define WH_VL_Real             WH_VL_A
     #define UH_VL_Real             UH_VL_A 
     #define UH_WL_Real             UH_WL_A
     #define VH_WL_Real             VH_WL_A
     #define VH_UL_Real             VH_UL_A
     #define WH_UL_Real             WH_UL_A

#elif (Chopping_Mode_Use == Chopping_ModeB)
     #define WH_VL_Real             WH_VL_B
     #define UH_VL_Real             UH_VL_B 
     #define UH_WL_Real             UH_WL_B
     #define VH_WL_Real             VH_WL_B
     #define VH_UL_Real             VH_UL_B
     #define WH_UL_Real             WH_UL_B

#elif (Chopping_Mode_Use == Chopping_ModeC)
     #define WH_VL_Real             WH_VL_C
     #define UH_VL_Real             UH_VL_C 
     #define UH_WL_Real             UH_WL_C
     #define VH_WL_Real             VH_WL_C
     #define VH_UL_Real             VH_UL_C
     #define WH_UL_Real             WH_UL_C

#elif (Chopping_Mode_Use == Chopping_ModeD)
     #define WH_VL_Real             WH_VL_D
     #define UH_VL_Real             UH_VL_D 
     #define UH_WL_Real             UH_WL_D
     #define VH_WL_Real             VH_WL_D
     #define VH_UL_Real             VH_UL_D
     #define WH_UL_Real             WH_UL_D

#elif (Chopping_Mode_Use == Chopping_ModeE)
     #define WH_VL_Real             WH_VL_E
     #define UH_VL_Real             UH_VL_E 
     #define UH_WL_Real             UH_WL_E
     #define VH_WL_Real             VH_WL_E
     #define VH_UL_Real             VH_UL_E
     #define WH_UL_Real             WH_UL_E

#elif (Chopping_Mode_Use == Chopping_ModeF)
     #define WH_VL_Real             WH_VL_F
     #define UH_VL_Real             UH_VL_F 
     #define UH_WL_Real             UH_WL_F
     #define VH_WL_Real             VH_WL_F
     #define VH_UL_Real             VH_UL_F
     #define WH_UL_Real             WH_UL_F
  
#elif (Chopping_Mode_Use == Chopping_ModeG)
     #define WH_VL_Real             WH_VL_G
     #define UH_VL_Real             UH_VL_G 
     #define UH_WL_Real             UH_WL_G
     #define VH_WL_Real             VH_WL_G
     #define VH_UL_Real             VH_UL_G
     #define WH_UL_Real             WH_UL_G
  
#elif (Chopping_Mode_Use == Chopping_ModeH)
     #define WH_VL_Real             WH_VL_H 
     #define UH_VL_Real             UH_VL_H 
     #define UH_WL_Real             UH_WL_H
     #define VH_WL_Real             VH_WL_H
     #define VH_UL_Real             VH_UL_H
     #define WH_UL_Real             WH_UL_H
		 
#endif 


/*跳变沿检测*/
#define U_UP				(1<<12)
#define W_DOWN				(2<<12)
#define V_UP				(3<<12)
#define U_DOWN				(4<<12)
#define W_UP				(5<<12)
#define V_DOWN				(6<<12)
#define U_V_W_ALL		    (7<<12)


#endif
