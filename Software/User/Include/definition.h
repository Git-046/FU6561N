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
#ifndef __DEFINITION_H_
#define __DEFINITION_H_


/* Q format define ---------------------------------------------------------------------------------*/
#define _Q5(A)                          (int8) ((A) * 0x1f)         ///< Q7 format
#define _Q6(A)                          (int8) ((A) * 0x3f)         ///< Q7 format
#define _Q7(A)                          (int8) ((A) * 0x7f)         ///< Q7 format
#define _Q8(A)                          (int16)((A) * 0xff)         ///< Q8 format
#define _Q9(A)                          (int16)((A) * 0x1ff)        ///< Q9 format
#define _Q10(A)                         (int16)((A) * 0x3ff)        ///< Q10 format
#define _Q11(A)                         (int16)((A) * 0x07ff)       ///< Q11 format
#define _Q12(A)                         (int16)((A) * 0x0fff)       ///< Q12 format
#define _Q13(A)                         (int16)((A) * 0x1fff)       ///< Q13 format
#define _Q14(A)                         (int16)((A) * 0x3fff)       ///< Q14 format
#define _Q15(A)                         (int16)((A) * 0x7fff)       ///< Q15 format
#define _Q30(A)                         (int32)(A * 32767 * 32767)
#define _Q16                            (65535.0)                   ///< Q16 format value
#define _2PI                            (3.1415926 * 2)             ///< 2 PI value

#define I_ValueX(Curr_Value)            (Curr_Value * HW_RSHUNT * HW_AMPGAIN / (HW_ADC_REF))

#define I_Value(Curr_Value)             _Q15(I_ValueX(Curr_Value))

#define S_Value(SpeedValue)             _Q15((float)(SpeedValue) / MOTOR_SPEED_BASE)

#define P_Value(PowerValue)             _Q15((float)(PowerValue) / POWER_BASE)

#define UDC_Value(val)                  _Q15((val)/ HW_BOARD_VOLT_MAX)

#define TIM2_Fre                        (187500.0)      ///< 定时器频率


/* 使能定义 */
#define Disable                         (0)
#define Enable                          (1)


/* 转向定义 */
#define CW                              (0xA0)             ///<正转
#define CCW                             (0xB0)             ///<反转


/* ADC参考电压配置 */
#define VREF3_0                         (0xA0)          ///< 0XA0 远离实际值  减少直接填写数值导致错误，其他同理
#define VREF4_0                         (0xB0)
#define VREF4_5                         (0xC0)
#define VREF5_0                         (0xD0)


/* VHALF系数 */
#define VHALF1_8                        (0xA0)          ///< 0XA0 远离实际值  减少直接填写数值导致错误，其他同理
#define VHALF1_4                        (0xB0)
#define VHALF25_64                      (0xC0)
#define VHALF1_2                        (0xD0)


/* 运放模式选择 */
#define AMP_NOMAL                       (0xA0)          ///< 外部放大
#define AMP_PGA_DUAL                    (0xB0)          ///< 内部PGA 双端差分输入
#define AMP_PGA_SINGEL                  (0xC0)          ///< 内部PGA 单端输入

/* 内部PGA放大倍数选择 */
#define AMP2x                           (2)
#define AMP4x                           (4)
#define AMP8x                           (8)
#define AMP16x                          (16)


/* 内部PGA放大倍数选择 */
#define INTERNAL                        (0xA0)
#define EXTERNAL                        (0xB0)


#define Ratio_11                        (0xA0)
#define Ratio_6_5                       (0xB0)


/* --驱动电平设置-- */
#define HIGH_LEVEL                      (0xA0)          ///< 驱动高电平有效
#define LOW_LEVEL                       (0xB0)          ///< 驱动低电平有效
#define UP_H_DOWN_L                     (0xC0)          ///< 上桥臂高电平有效，下桥臂低电平有效
#define UP_L_DOWN_H                     (0xD0)          ///< 上桥臂低电平有效，下桥臂高电平有效


#define ALIGN_DSIABLE                   (0xA0)          ///< 禁止
#define ALIGN_NOMAL                     (0xB0)          ///< 正常预定位
#define ALIGN_TEST                      (0xC0)          ///< 测试模式，可用于手动测试电机极对数


/* 调速模式 */
#define NONEMODE                        (0xA0)          ///< 直接给定值，不调速
#define SREFMODE                        (0xB0)          ///< 模拟调速
#define PWMMODE                         (0xC0)          ///< PWM调速
#define ONOFFTEST                       (0xD0)          ///< 启停测试工具
#define KEYMODE                         (0xE0)          ///< 按键调速
#define UARTMODE												(0xF0)					///< UART调速

#define SPEED_K                         ((float)(MC_SPEED_MAX_RPM-MC_SPEED_MIN_RPM)/(float)(PWM_MAX_DUTY-PWM_MIN_DUTY))
#define POWER_K                         ((float)(MC_POWER_MAX-MC_POWER_MIN)/(float)(PWM_MAX_DUTY-PWM_MIN_DUTY))
/* 顺逆风判断设置 */
#define NoTailWind                      (0xA0)          ///< 无逆风顺风判断
#define BEMFMethod                      (0xB0)          ///< BEMF比较器方法


/* 启动模式 */
#define STATIC_START                    (0xA0)          ///< 静止启动
#define TAILWIND_START                  (0xB0)          ///< 顺风启动
#define HEADWIND_START                  (0xC0)          ///< 逆风启动
#define TAILWIND_DECT                   (0xD0)          ///< 顺逆风检测中

/*FG输出方式设置值----------------------------------------------------------------*/
#define DISABLE_FG_OUTPUT               (0xA0)                                     ///< 不反馈FG
#define SOFT_TIMFG_OUTPUT               (0xB0)                                     ///< 通过软件定时器输出FG信号
#define HARD_TIMFG_OUTPUT               (0xC0)                                     ///< 通过硬件定时器输出FG信号
#define THETA_FG_OUTPUT					(0xD0)									   ///< 通过角度输出FG信号 
    
#define Chopping_ModeA                  (1)                                  
#define Chopping_ModeB                  (2)                             
#define Chopping_ModeC                  (3)                        
#define Chopping_ModeD                  (4)                       
#define Chopping_ModeE                  (5)                           
#define Chopping_ModeF                  (6)                               
#define Chopping_ModeG                  (7)
#define Chopping_ModeH                  (8)

#define SquMode                             (0xA0)
#define AngleMode                           (0xB0)
#define NoWeaken                            (0xC0)
#endif
