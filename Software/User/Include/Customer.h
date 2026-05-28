/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      Customer.h
 * @author    Fortiortech  Appliction Team
 * @note      Last modify author is Marcel He
 * @since     2019-05-17
 * @date      2022-07-14
 * @brief     This file contains customer parameter used for Motor Control.  
 */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __CUSTOMER_H_
#define __CUSTOMER_H_
#include <FU6511_MCU.h>
 /**********************基础参数*****************************
 1. 芯片参数
 2. 电流采样参数
 3. 母线电压采样参数
  **************************************************************/

/* ----------------------------------------------------------------------------------------------------------------------------
                                             1.芯片参数                                                 
---------------------------------------------------------------------------------------------------------------------------- */
/* PWM Parameter */
#define PWM_FREQUENCY                     (36.0)                         ///< (kHz) 载波频率
/*deadtime Parameter*/
#define PWM_DEADTIME                      (1.0)                          ///< (us) 死区时间
/* ------------------------------------------------------------------------------ */
/*电机参数值*/
#define Pole_Pairs                        (5.0)                          ///< 极对数
/*------------------------------------------------------------------------------*/
#define TIM1_FREQ					                (3.0)			            	       //MHz，这个值修改后，定时器需要同步修改
#define THE_CLK_FREQ				              (1500.0)		                   //kHz  不要需改
#define DIV_A_SPD						              (uint32) (256*65536*TIM1_FREQ*166.67/THE_CLK_FREQ)
#define MOTOR_SPEED_BASE                  (5000)                         //速度基准
#define MOTORSPEEDBASE_TIMER			        (uint32)(32767.0*(TIM1_FREQ * 1000 * 1000*60/Pole_Pairs/6.0/MOTOR_SPEED_BASE ))

/**
 * 硬件PCBA参数设置根据驱动芯片的类型选择
 * @param (HIGH_LEVEL)   驱动高电平有效
 * @param (LOW_LEVEL)    驱动高电平有效
 * @param (UP_H_DOWN_L)  驱动高电平有效
 * @param (UP_L_DOWN_H)  驱动高电平有效
 */
#define PWM_LEVEL_MODE                    (HIGH_LEVEL)                   ///< 驱动电平设置

/* ----------------------------------------------------------------------------------------------------------------------------
                                             2.电流采样参数                                                   
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * @breaf  运放模式选择
 * @param (AMP_NOMAL)       外部放大
 * @param (AMP_PGA_DUAL)    内部PGA双端差分输入
 * @param (AMP_PGA_SINGEL)  内部PGA单端输入
 */
#define HW_AMP_MODE                       (AMP_PGA_DUAL)                 ///< 运放模式选择
#define HW_RSHUNT                         (0.001)                         ///< (Ω)  采样电阻

/**
 * 放大倍数设置
 * @param (AMP2x)       内部PGA放大2倍
 * @param (AMP4x)       内部PGA放大4倍
 * @param (AMP8x)       内部PGA放大8倍
 * @param (AMP16x)      内部PGA放大16倍
 * @param (xxxxxx)      外部放大模式填写相应倍数
 */
#define HW_AMPGAIN                        (AMP4x)                         ///< 放大倍数设置     其他倍数要手动配置
 
/**
 * 参考电压设置
 * @param (VREF3_0)       参考电压设置为3.0V
 * @param (VREF4_0)       参考电压设置为4.0V
 * @param (VREF4_5)       参考电压设置为4.5V
 * @param (VREF5_0)       参考电压设置为5.0V
 */
#define HW_ADC_VREF                       (VREF5_0)                       ///< (V)  ADC参考电压

/**
 * 基准电压VREF对外输出使能
 * @param (Disable)       禁止
 * @param (Enable)        使能
 */
#define VREF_OUT_EN                       (Enable)                        ///< 基准电压VREF对外输出使能

/* ----------------------------------------------------------------------------------------------------------------------------
                                             3.母线电压采样参数                                                  
---------------------------------------------------------------------------------------------------------------------------- */
/** @brief 母线电压分压配置 */
#define  RV1                              (30.0)                    /*!< (单位: kΩ) 母线分压电阻1 (当 @ref HW_RV_MODE 为 @ref RVEXT 时有效) */ 
#define  RV2                              (5.1)                     /*!< (单位: kΩ) 母线分压电阻2 ( @ref HW_RV_MODE 为 @ref RVEXT 时有效) */

#define Calib_Time                        (1000)                    ///< 校正次数，固定1000次，单位:次

 /**********************电机运行参数*****************************
 1.  预充电参数
 2.  预定位参数
 3.  启动参数
 4.  环路参数 
 5.  调速开关模式 
 6.  顺逆风参数 
 7.  正弦开窗参数   
 8.  过调制
 9.  正反转模式  
 10. PWM调速开关参数
 11. 启停测试参数  
 12. FG输出 
  **************************************************************/

/* ----------------------------------------------------------------------------------------------------------------------------
                                            1.预充电参数                                                  
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 预充电使能
 * @param (Disable)       禁止
 * @param (Enable)        使能
 */
#define CHARGE_EN                      (Enable)          ///< 预充电使能
/**
 * @param (CHARGE_DUTY)        预充电占空比
 * @param (CHARGE_TIME)        预充电时间
 */
#define CHARGE_DUTY                       (0.5)                           ///< (%)  UVW每相单独输出
#define CHARGE_TIME                       (100)                           ///< (ms) 预充电时间
/**
 * IPM测试模式，用以检测MCU——MOS间电路是否正常,
 * @param (Disable)       禁止
 * @param (Enable)        使能
 */
#define IPMTEST                           (Disable)

/* ----------------------------------------------------------------------------------------------------------------------------
                                          2.预定位参数                                                    
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 预定位模式选择
 * @param (ALIGN_DSIABLE)       禁止
 * @param (ALIGN_NOMAL)         正常预定位
 * @param (ALIGN_TEST)          测试模式，可用于手动测试电机极对数
 */
#define ALIGN_MODE                       (ALIGN_DSIABLE)                    ///< 预定位模式选择
#define ALIGN_Sector                     (5) 							                  ///< 预定位扇区 1对应0度，2对应60度
#define ALIGN_CurrentStart               _Q15(2.0/HW_BOARD_CURR_BASE) 
#define ALIGN_CurrentEnd                 _Q15(2.0/HW_BOARD_CURR_BASE) 
#define CurrentRampTime	                 (10.0) 							              ///< 预定位电流爬坡时间
#define CurrentRampSlope                 ((float)(ALIGN_CurrentEnd - ALIGN_CurrentStart))/CurrentRampTime
#define ALIGN_TIME                       (uint16)(2)                          

#define ALIGN_ANGLE                      _Q15((ALIGN_Sector - 1) * 60.0 / 180.0) 		///< 预定位角度
 
/* ----------------------------------------------------------------------------------------------------------------------------
                                            3.启动参数                                                   
---------------------------------------------------------------------------------------------------------------------------- */
/*默认为六步启动*/
#define PWM_FREQUENCY_Start              (16.0)                         ///< (kHz) 载波频率
#define PWM_DEADTIME_Start               (1.0)                          ///< (us) 死区时间
#define SwitchSVPWMSpeed                 (500.0)						            ///切环转速
#define SwitchCount                      (PWM_FREQUENCY_Start*1000.0*(0.5/(SwitchSVPWMSpeed*Pole_Pairs/60.0)))
#define RampSwitchSVPWMSpeed             (float)(SwitchSVPWMSpeed * Pole_Pairs /(THE_CLK_FREQ * 1000 * 60 /(256*65536)))
#define START_Current                    _Q15(1.0/HW_BOARD_CURR_BASE)  //启动电流(第一拍)
#define START_Current_Hold               _Q15(1.5/HW_BOARD_CURR_BASE)  //启动电流(第一拍以后)
#define START_Curernt_Threshold          _Q15(2.5/HW_BOARD_CURR_BASE)  	//启动换相阈值(值越大越难换相，越小越好换相)

#define DeadTimeComp			               _Q15(1.5*PWM_DEADTIME_Start/(1000.0/PWM_FREQUENCY_Start))
#define DeadTimeFreComp			             _Q15((PWM_DEADTIME/(1000.0/PWM_FREQUENCY))-(PWM_DEADTIME_Start/(1000.0/PWM_FREQUENCY_Start)))		//不同载波频率切环间的电压指令死区补偿
#define ForcedSwitchTime                 (20.0)						              //ms 强拖换相时间
#define CCW_Period_Threshold             (7.0)                          //ms 反转判断时间
#define DrvCOMRValue              	   	 (DRV_ARR - 85)					        //采样点设置，通过软件设置drver中断比较值采电流
#define MinUq				           	         (DeadTimeComp + _Q15(0.01))	  //最小输出电压占空比，保证采样准确度
#define UqRamp			               	     (uint8)(160)					          //Uq步进值，每一拍若电流小于START_Current_Hold，则Uq步进该值
#define SwitchMinUq	              	   	 _Q15(0.03)						          //切环时的最小占空比

/* ----------------------------------------------------------------------------------------------------------------------------
                                            4. 环路参数                                                
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 闭环方式选择
 * @param (POWER_LOOP_CONTROL)         功率环
 * @param (SPEED_LOOP_CONTROL)         速度环
 */
#define MOTOR_CTRL_MODE                  (SPEED_LOOP_CONTROL) ///< 闭环方式选择

#define LOOP_TIME                         (2)                                 // (ms) 速度环调节周期

#define Motor_Target_Speed                S_Value(2000.0)                	    // 目标转速
#define Motor_Target_Power                P_Value(50.0)                	      // 目标功率

#define LOOP_KP_Start                     _Q12(1.0)                           // 环路 KP
#define LOOP_KI_Start                     _Q15(0.01)                          // 环路 KI
#define StartPowerRamp                    P_Value(10.0)                       //切环功率目标值阶跃
#define Accele_ParaSpeed                  S_Value(1200)                       //启动->加速PI切环转速

#define LOOP_KP_Accele                    _Q12(3.0)                           // 环路 KP
#define LOOP_KI_Accele                    _Q15(0.03)                          // 环路 KI

#define LOOP_KP_Steady                    _Q12(0.1)                           // 环路 KP  功率环0.3
#define LOOP_KI_Steady                    _Q15(0.006)                         // 环路 KI  功率环0.006

#define LOOP_PIMax                        _Q15(0.99)                          //环路最大输出(大电感开启过调制后，不能写满)
#define LOOP_PIMIN                        _Q15(0.03)                          //环路最小输出

#define SPEEDLOOP_RAMP_INC                S_Value(20)                         //环路爬坡增量 
#define SPEEDLOOP_RAMP_DEC                S_Value(10)                         //环路爬坡减量

#define POWERLOOP_RAMP_INC                P_Value(1.0)                         //环路爬坡增量
#define POWERLOOP_RAMP_DEC                P_Value(1.0)                         //环路爬坡减量

/* -----限速环KPKI设置----- */
#define S2KP                              _Q12(2.0)                      ///< 限流环KP
#define S2KI                              _Q15(0.027)                    ///< 限流环KI

/* ----------------------------------------------------------------------------------------------------------------------------
                                        5. 调速开关模式                                                   
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 闭环方式选择
 * @param (SREFMODE)      模拟调速
 * @param (PWMMODE)       PWM
 * @param (NONEMODE)      直接给定值，不调速
 * @param (ONOFFTEST)     启停测试工具 
 * @param (KEYMODE)       按键换挡 
 * @param (UARTMODE)      串口调速
 */
#define SPEED_MODE                        (NONEMODE)                        ///< 闭环方式选择

/* ----------------------------------------------------------------------------------------------------------------------------
                                          6. 顺逆风参数                                                   
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 顺逆风检测方式
 * @param (NoTailWind)      无逆风顺风判断
 * @param (BEMFMethod)      BEMF比较器方法
 */
#define TAILWIND_MODE                      (BEMFMethod)     ///< 顺逆风检测方式

#define TAILWIND_TIME                      (100)                          ///< (ms) 顺逆风检测时间 
#define TAILWIND_SPEED                     S_Value(2000)                  // 顺风最小转速，小于该设置值，顺风走静止启动
#define TAILWIND_STARTDUTY                 _Q15(0.2)                      // 顺逆风切环占空比
#define WIN_DLY_TAILWIND				          (WIN_DLY_RUN * 0.5)             // 顺风切环延迟角
#define LEADANGLE_TAILWIND                 _Q15(10.0 / 180.0)

/* ----------------------------------------------------------------------------------------------------------------------------
                                          7. 正弦开窗设置                                                 
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 手动超前角
 * @param (Disable)      禁止
 * @param (Enable)       使能
 */
#define MANUAL_LEAD_ANGLE         (Disable) //手动超前角使能后，超前角依照如下表，根据速度线性插补

#define LEAD_ANGLE_START				  _Q15(16.0 / 180.0)
#define LEAD_ANGLE_RUN_1				  _Q15(20.0 / 180.0)
#define LEAD_ANGLE_RUN_2				  _Q15(35.0 / 180.0)
#define LEAD_ANGLE_RUN_3				  _Q15(45.0 / 180.0)
#define LEAD_ANGLE_RUN_4				  _Q15(50.0 / 180.0) 
#define LEAD_ANGLE_RUN_5				  _Q15(55.0 / 180.0) //超前角设置根据转速线性调整角度（调小可以慢慢恢复，调太大容易失控）

/**
  采样配置切换，高占空比时用ON采样
*/
#define SAM_THR_H						  (0.35)   //切换到ON采样的占空比阈值
#define SAM_THR_L						  (0.30)   //切换到OFF采样的占空比阈值

#define SAM_THR_H_SV					  (uint16)(SAM_THR_H * 32767)
#define SAM_THR_L_SV					  (uint16)(SAM_THR_L * 32767)
/**
  自动超前角配置切换，高占空比时用ON采样
*/
#define AdaAngle_THR_H					_Q15(0.50)   //切换到ON采样的占空比阈值
#define AdaAngle_THR_L					_Q15(0.40)   //切换到OFF采样的占空比阈值

#define DrvCOMR_H   					(DRV_ARR - 100) //ON采样位置
#define DrvCOMR_L  						(130)            //OFF采样位置

#define AdaAngle_THR_Speed_UH           S_Value(95000) //堵孔下的最小转速

/**
 * 自动超前角参数
 * @param (AdaAngle_K_H)      高占空比时判断增益值
 * @param (AdaAngle_K_L)      低占空比时判断增益值
 */
#define AdaAngle_K_H              (1.0) //高占空比-2载波下判断增益 （收敛后如果电流波形上大下小则改小，上小下大则改大）
#define AdaAngle_K_L              _Q15(0.75) //低占空比-3载波下判断增益

#define BEMF_Samp_THR_L         (3000)  //检测到续流的低阈值
#define BEMF_Samp_THR_LL        (500)   //低占空比检测到续流的低阈值

#define LEAD_ANGLE_Speed1				  (MC_SPEED_MIN_RPM  + (MC_SPEED_MAX_RPM - MC_SPEED_MIN_RPM)*0.2)
#define LEAD_ANGLE_Speed2				  (MC_SPEED_MIN_RPM  + (MC_SPEED_MAX_RPM - MC_SPEED_MIN_RPM)*0.4)
#define LEAD_ANGLE_Speed3				  (MC_SPEED_MIN_RPM  + (MC_SPEED_MAX_RPM - MC_SPEED_MIN_RPM)*0.6)
#define LEAD_ANGLE_Speed4				  (MC_SPEED_MIN_RPM  + (MC_SPEED_MAX_RPM - MC_SPEED_MIN_RPM)*0.8)

#define LEAD_ANGLE_Scope1				  (65536.0*(LEAD_ANGLE_RUN_1 - LEAD_ANGLE_START))/(LEAD_ANGLE_Speed1 - MC_SPEED_MIN_RPM)
#define LEAD_ANGLE_Scope2				  (65536.0*(LEAD_ANGLE_RUN_2 - LEAD_ANGLE_RUN_1))/(LEAD_ANGLE_Speed2 - LEAD_ANGLE_Speed1)
#define LEAD_ANGLE_Scope3				  (65536.0*(LEAD_ANGLE_RUN_3 - LEAD_ANGLE_RUN_2))/(LEAD_ANGLE_Speed3 - LEAD_ANGLE_Speed2)
#define LEAD_ANGLE_Scope4				  (65536.0*(LEAD_ANGLE_RUN_4 - LEAD_ANGLE_RUN_3))/(LEAD_ANGLE_Speed4 - LEAD_ANGLE_Speed3)
#define LEAD_ANGLE_Scope5				  (65536.0*(LEAD_ANGLE_RUN_5 - LEAD_ANGLE_RUN_4))/(MC_SPEED_MAX_RPM - LEAD_ANGLE_Speed4)

#define STARTUP_ANGLE_DIS_TIME    (1000)          //启动屏蔽                               自动超前角时间
#define LEAD_ANGLE_MAX            _Q15(20.0 / 180.0) /*超前角补偿限幅*/ 
#define LEAD_ANGLE_MIN            -(_Q15(25.0 / 180.0))
#define LEAD_ANGLE_MAX_H          _Q15(20.0 / 180.0) /*超前角补偿最大值*/ 
#define LEAD_ANGLE_MIN_H          -_Q15(25.0 / 180.0) /*超前角补偿最小值*/ 

#define BEMF_RV1                       (18.0)
#define BEMF_RV2                       (3.0)
#define HW_BEMF_VOLT_MAX               (((RV1 + RV2) / RV2) * HW_ADC_REF)
#define KeVpp                          (11)                                     ///< (V)      反电动势测量的峰峰值
#define KeT                            (19.8)                                   ///< (ms)     反电动势测量的周期
#define Ke                             (Pole_Pairs * KeVpp * KeT / 207.846)     ///< (V/KRPM) 反电动势常数
#define Psi                            (float)(Ke * 207.84 / 1000.0 / 2.0 / 1.732 / Pole_Pairs / _2PI) ////磁链8.18e-4
#define K_Delta_BEMF                   (MOTOR_SPEED_BASE * PWM_FREQUENCY * 1000 * Psi * HW_BOARD_VOLT_MAX / _2PI / 32767)
#define K_Delta_BEMF1                  _Q12(MOTOR_SPEED_BASE * _2PI * _2PI * Psi * AdaAngle_K_H / 60 / HW_BEMF_VOLT_MAX)
/*当电机温度上升，磁链可能因此改变，导致不准*/

/*续流屏蔽角度参数配置*/
#define BSEL_ANGLE                        (25.0)
#define BSEL_ANGLE_RUN                    (25.0)
#define SV_BSEL_ANGLE                     (uint8)(BSEL_ANGLE * 127 / 60)      //续流角度
#define SV_BSEL_ANGLE_RUN                 (uint8)(BSEL_ANGLE_RUN * 127 / 60)  //续流角度
#define BSEL_ANGLE_CNT                    (2000)                              //启动后续流屏蔽角度开始变化时间
/*开窗角度参数配置*/
#define WIN_DLY_START0          (10.0)
#define WIN_DLY_RUN0            (27.0)
#define WIN_DLY_START					  (uint8)(WIN_DLY_START0/60*127)						    //开窗角度等于60-WIN_DLY_START
#define WIN_DLY_RUN						  (uint8)(WIN_DLY_RUN0/60*127)
#define WIN_THETA_CW            _Q15(145.0 / 180.0)                           //正转开窗开始角度
#define WIN_THETA_CCW           _Q15(35.0 / 180.0)                            //反转开窗开始角度


/* ----------------------------------------------------------------------------------------------------------------------------
                                          8. 过调制                                                
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 过调制
 * @param (Disable)      禁止
 * @param (Enable)       使能
 */
#define OverModulation                 (Disable)             ///< 开启过调制UD,UQ会被放大1.15倍，但极限状态可能导致电流畸变   

/* ----------------------------------------------------------------------------------------------------------------------------
                                          9. 正反转模式                                                
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 转向设置
 * @param (CW)        顺时针
 * @param (CCW)       逆时针
 */
#define FR_MODE                          (CW)                           ///< 转向设置

/* ----------------------------------------------------------------------------------------------------------------------------
                                          10. PWM调速开关参数                                                
---------------------------------------------------------------------------------------------------------------------------- */
/* 开关机Duty设置 */
#define  MOTOR_SHUTDOWN_ENABLED             (0)                        /*!< 启用 PWM 开关机控制  @param (0) 禁用 @param (1) 启用       ( @ref SPEED_MODE 为 PWMMODE 时有效) */
#define  PWM_TURNOFF_DUTY                   _Q15(0.03)                 /*!< (单位: %) PWM 关占空比 范围: ( @ref 0 ~ 0.999]             ( @ref SPEED_MODE 为 PWMMODE 时有效) */
#define  PWM_TURNON_DUTY                    _Q15(0.05)                 /*!< (单位: %) PWM 开占空比 范围: ( @ref 0 ~ 0.999]             ( @ref SPEED_MODE 为 PWMMODE 时有效) */
#define  PWM_FULLSPEED_DUTY                 _Q15(0.98)                 /*!< (单位：%) PWM 全速占空比 范围：( @ref 0 ~ 0.999]           ( @ref SPEED_MODE 为 PWMMODE 时有效)*/
#define  MOTOR_SHUTDOWN_FILTER_PERIOD       (25)                       /*!< 开关机滤波计数值                                           ( @ref SPEED_MODE 不為 NONEMODE 时有效) */
#define  PWM_INVERT_ENABLED                 (0)                        /*!< 启用 PWM 信号倒相  @param (0) 禁用 @param (1) 启用         ( @ref SPEED_MODE 为 PWMMODE 时有效 */
#define  MC_SPEED_MIN_RPM                   S_Value(200)               ///< (RPM) 运行最小转速
#define  MC_SPEED_MAX_RPM                   S_Value(2000)              ///< (RPM) 运行最大转速
#define  MC_POWER_MIN                       P_Value(50.0)              ///< (W) 运行最小功率
#define  MC_POWER_MAX                       P_Value(250.0)             ///< (W) 运行最大功率
#define  PWM_MIN_DUTY                       _Q15(0.100)                /*!< (单位: %) PWM 最低占空比 范围: ( @ref 0 ~ 0.999]           ( @ref SPEED_MODE 为 PWMMODE 时有效) */
#define  PWM_MAX_DUTY                       _Q15(0.990)                /*!< (单位: %) PWM 最高占空比 范围: ( @ref 0 ~ 0.999]           ( @ref SPEED_MODE 为 PWMMODE 时有效) */
#define  PWM_IO                             (GP13)                     //PWM输入捕获口
/** 
  * @brief   调速节点数量
  * @details 节点数量范围: 2 ~ 24点。 ( @ref SPEED_MODE 为 PWMMODE 时有效)
  * @note    过多的节点会造成容量紧迫，需注意使用场景是否需要如此多的节点。
  */
#define  MOTOR_SPEEDMODE_MAX_NODE           (2)    /*!< 调速节点数量  ( @ref SPEED_MODE 为 PWMMODE 时有效) */  
/** 
  * @brief   PWM 调速占空比节点
  * @details (单位: %) PWM 调速节点占空比数量必须与 @ref MOTOR_SPEEDMODE_MAX_NODE 点数相同  ( @ref SPEED_MODE 为 PWMMODE 时有效)
  */
#define  MOTOR_PWMMODE_DUTY_NODE            {PWM_MIN_DUTY, PWM_MAX_DUTY}

/** 
* @brief   电机转速节点
* @details (单位: RPM) 电机转速数量必须与 @ref MOTOR_SPEEDMODE_MAX_NODE 点数相同  ( @ref SPEED_MODE 为 PWMMODE、VSPMODE 时有效)
  */
#define  MOTOR_SPEED_NODE                   {(MC_SPEED_MIN_RPM), (MC_SPEED_MAX_RPM)}

#define  MOTOR_POWER_NODE                   {(MC_POWER_MIN), (MC_POWER_MAX)}

#define SPEED_NODE_MODE         (0)
#define SPEED_PWM_MODE          (1)
#define SPEED_CONTROL_MODE      (SPEED_PWM_MODE)
/* ----------------------------------------------------------------------------------------------------------------------------
                                          11. 启停测试参数                                                
---------------------------------------------------------------------------------------------------------------------------- */
#define StartON_Time                   (2000)                                  // (ms) 启动运行时间
#define StartOFF_Time                  (500)                                   // (ms) 停止时间
#define MOTOR_ONOFF_POW                 P_Value(150.0)                         // 启停测试目标功率
#define MOTOR_ONOFF_SPEED               S_Value(60000.0)                       // 启停测试目标速度

#define StopBrake_Enble                (0)
#define StopWaitTime                   (4000)                                  // (ms) 刹车等待时间
#define Stop_MOTOR_SPEED_RPM            S_Value(300)                           ///< (RPM) 小于该转速刹车
/* ----------------------------------------------------------------------------------------------------------------------------
                                          12. FG输出                                                  
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * @brief FG输出模式选择
 * @param (DISABLE_FG_OUTPUT)    不反馈FG 
 * @param (HARD_TIMFG_OUTPUT)    通过硬件定时器输出FG信号 
 * @param (SOFT_TIMFG_OUTPUT)    通过软件定时器输出FG信号 
 */
#define	 FG_MODE														(HARD_TIMFG_OUTPUT)
#define  FG_INITIAL_DELAY_TIMES             (100)                      /*!< (单位: mS) FG输出时机  (范围: 1000 ~ 6000) */
#define  FG_PULSE_PERIOD                    (2)                        /*!< FG 每转脉波个数  ( @ref ROTATE_SIGNAL_TYPE 為 RD_TYPE 时有效) */
#define  FG_ALARM_LOGIC                     (1)                        /*!< FG 信号报警状态  @see LOGIC_HIGH 高电平 @see LOGIC_LOW 低电平 */
#define  FG_FAKE_ENABLED                    (0)                        /*!< 启用假转速输出功能  (注意: PWM_FULLCONTROL_ENABLED 与本功能冲突，不得同时启用)  @param (0) 禁用 @param (1) 启用 */
#define  M_speedFlt2RPM(_Val_)           		(uint16)((float)(_Val_) / 2 / 32767.0f * (float)MOTOR_SPEED_BASE) //注意防止溢出
#define  M_speedFlt2FG(_Val_)            		(uint16)(FG_FREQ_BASE * 2 / ((float)M_speedFlt2RPM(_Val_) / (60.0 / (float)FG_PULSE_PERIOD)))

/* ----------------------------------------------------------------------------------------------------------------------------
                                          13. 温度采样                                                  
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 温度采样
 * @param (Disable)      禁止
 * @param (Enable)       使能
 */
#define TEMP_SAMP_EN                        (Disable)
#define TEMP_ADC_CHANNEL                    (CH3EN) //温度采样ADC通道

/* ----------------------------------------------------------------------------------------------------------------------------
                                          14. 弱磁                                                  
---------------------------------------------------------------------------------------------------------------------------- */
/**
 * 弱磁方式
 * @param (SquMode)      方波模式
 * @param (AngleMode)    超前角模式
 * @param (NoWeaken)     禁用弱磁
 */
#define WeakenMode                          (AngleMode)

/*方波弱磁*/
#define DelayAngle                          (5.0) /*延迟换相角度*/
#define Duty2Uq_K                           (32767.0 / PWM_VALUE_LOAD)
#define EnterUq                             _Q15(0.9)
#define ExitUq                              _Q15(0.8)
#define BLDC_120_CONTROL             (0)                                     
#define BLDC_150_CONTROL             (1)  
#define Motor_BLDC_CONTROL_Mode      (BLDC_150_CONTROL)

#define BLDC_120_MASK_ANGLE           (27.0)
#define BLDC_120_DELAY_ANGLE          (5.0)

#define BLDC_150_MASK_ANGLE           (27.0)
#define BLDC_150_DELAY_ANGLE          (0.0)

/**
 * @brief 斩波方式选择.
 * @param Chopping_ModeA -  上桥PWM下桥ON  (常用模式)
 * @param Chopping_ModeB -  上桥ON下桥PWM
 * @param Chopping_ModeC -  上桥PWM互补下桥ON  (互补时注意给死区时间，若21V母线电压直供6561 VCC则注意降速冲电压情况，此时不推荐C，使用A更好 )
 * @param Chopping_ModeD -  上桥ON下桥PWM互补
 * @param Chopping_ModeE -  上桥PWM_互补下桥PWM互补
 * @param Chopping_ModeF -  上桥PWM下桥PWM_COM
 * @param Chopping_ModeG -  上桥PWM_互补下桥PWM
 * @param Chopping_ModeH -  上桥PWM下桥PWM
 */
#define Chopping_Mode_Use                 (Chopping_ModeC)

/*超前角弱磁*/
#define WeakenSpeed                   S_Value(120000)
#define WeakenAngleMin                _Q15(2.0 / 180.0)
#define WeakenAngleMax                _Q15(10.0 / 180.0)
#define WeakenEnterWait               (50) //进入弱磁等待时间(ms)


/*功率环挡位设置*/
#define POW_GEAR1                           P_Value(150.0)  //挡位1
#define POW_GEAR2                           P_Value(100.0)
#define POW_GEAR3                           P_Value(100.0)
#define POW_GEAR4                           P_Value(100.0)
#define POW_GEAR5                           P_Value(100.0)

#define SPD_GEAR1                           S_Value(30000.0)  //挡位1
#define SPD_GEAR2                           S_Value(40000.0)  //挡位2
#define SPD_GEAR3                           S_Value(50000.0)  //挡位3
#define SPD_GEAR4                           S_Value(60000.0)  //挡位4
#define SPD_GEAR5                           S_Value(70000.0)  //挡位5

#define PI_I_MAX                _Q30(0.985)  //积分限幅最大值
#define PI_I_MIN                -_Q30(0.99)

#endif
