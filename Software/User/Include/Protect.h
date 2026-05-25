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

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __PROTECT_H_
#define __PROTECT_H_

/*保护参数设置--------------------------------------------------------------------------------------------*/

/* 硬件过流保护比较值来源 */
#define COMPARE_MODE                        (COMPARE_DAC_MODE)                      ///< 硬件过流值的来源
#define HWOCValue                           (50.0) ///< (A) DAC模式下的硬件过流值
//#define CBC_OCValue                           (10.0) ///< (A) DAC模式下的逐波限流值

/* Faults processing Enable */
#define SWCurrent1_ProtectEn       (1) 			 ///< 1阶段软件过流保护使能： 0,不使能；1，使能
#define VoltageProtectEn           (1)       ///< 过欠压保护使能：        0,不使能；1，使能
#define StallProtectEn             (1)       ///< 堵转失速保护使能：      0,不使能；1，使能          0,不使能；1，使能
#define StartProtectEn             (1)       ///< 启动失败保护使能：      0,不使能；1，使能          0,不使能；1，使能
#define OverSpeedProtectEn         (0)       ///< 超速保护使能：      0,不使能；1，使能          0,不使能；1，使能

#define CurroffsetProtectEn        (1)       ///< 偏置电压保护使能：      0,不使能；1，使能          0,不使能；1，使能
#define Fault_OC_RecoverEn         (0)       ///< 过流恢复使能：  		    0,不使能；1，使能          0,不使能；1，使能
#define Stall_RecoverEn   		   (1)       ///< 堵转恢复使能：  		    0,不使能；1，使能          0,不使能；1，使能
#define StartRecoverEn             (1)       ///< 启动失败恢复使能：      0,不使能；1，使能          0,不使能；1，使能

/* 堵转保护参数 */
#define StallMaxSpeed              (MC_SPEED_MAX_RPM + S_Value(1000))
#define StallProtectTime           (200) 
#define StallMinSpeed              (MC_SPEED_MIN_RPM)

/* 母线电压保护参数 */
#define OVER_PROTECT_VALUE         UDC_Value(33.0)  ///< (V) 直流母线电压过压保护值
#define OVER_RECOVER_VALUE         UDC_Value(31.0)  ///< (V) 直流母线电压过压保护恢复值
#define UNDER_PROTECT_VALUE        UDC_Value(9.0)  ///< (V) 直流母线电压欠压保护值
#define UNDER_RECOVER_VALUE        UDC_Value(12.0) ///< (V) 直流母线电压欠压保护恢复值

/*启动保护参数*/
#define StartRecoverTimes          (5)  ///< 启动保护再次启动次数
#define StartRecoverTime           (100) ///< 启动保护恢复时间

/* -----NTC过温保护----- */
#define TemperatureProtectTime 			(1000)// (ms)温度保护检测时间									     								// (ms)温度保护检测时间
#define OVER_Temperature 		        Tempera_Value(5.97)	//34℃ 33.97kR		///< 过温保护阈值，根据NTC曲线设定，10K上拉电阻，80℃
#define UNDER_Temperature          		Tempera_Value(8.23)						///< 过温保护恢复阈值，根据NTC曲线设定，10K上拉电阻，70℃

/* 软件过流保护参数设置 */
#define SW_OC_CurrentVal                    I_Value(40.0)                            ///< (A)软件过流值
#define SW_OC_DectTime                      (30)                                    ///< (ms)软件过流检测时间

/* -----堵入风口超速保护----- */
#define	 MOTOR_SPEED_OVER_RPM		    S_Value(125000.0)                                ///< (RPM) 超速保护速度
#define	 OVER_SpeedDetectTime			(1400)                                  ///< (ms) 超速保护检测时间

/* -----堵入风口限速功能----- */
#define  OverSpeedLimitEnable			(Disable)	                                    ///< 限速功能，0，不使能，1使能
#define  MOTOR_SPEED_LIMIT_RPM          S_Value(90000.0)                               ///< (RPM) 限速保护最大速度

/* Exported types ------------------------------------------------------------ */
typedef enum
{
    FaultNoSource        = 0,        ///< 无故障
    FaultHardOVCurrent   = 1,        ///< 硬件过流
    FaultSoftOVCurrent   = 2,        ///< 软件过流
    FaultOverVoltageDC   = 3,        ///< 过压
    FaultUnderVoltageDC  = 4,        ///< 欠压
    FaultStall           = 5,        ///< 堵转
    FaultOTErr           = 6,        ///< MCU过温
	FaultCurroffset      = 7,		 ///< 偏置电压异常
    FaultStart           = 8,        ///< 启动保护
    FaultOverSpeed       = 9,        ///< 超速保护
} FaultStateType;


typedef struct
{
    uint8 SWOC_DectTimeCnt;                                                           
                                                  
	int16 Ibus;               											
	
    uint8 HWOC_Times;
    uint8 HWOC_DectTimeCnt;
    
}FaultCurrentVarible;

/*电流保护*/
typedef struct
{
    uint16 OverCurrentRecoverCnt;
    uint8 OverCurrentNum;      //
} FaultOverCurrentVarible;

typedef struct
{
    uint16 OverVoltDetecCnt;
    uint16 UnderVoltDetecCnt;
    uint16 VoltRecoverCnt;
    
    uint16 BusVoltDetecCnt;
    
    uint16 DectDealyCnt;
    
    uint16 OverVoltageVal;
    uint16 UnderVoltageVal;
    // 堵转超速
    uint16 OverSpeedCnt;
    uint16 OverSpeedCntRecover;
} FaultVoltageVarible;

typedef struct
{
    uint16 StartTimes;
    uint8 StartFlag;
    uint16 StartRecoverDelayCnt;
    uint16 SecondStartTimes;
} FaultStartTypedef;

/*堵转保护*/
typedef struct
{
    uint16 StallIbus;       /* 堵转功率判断值 */
    uint16 Stallvalue;      /* 堵转速度判断值 */
    uint16 DectDealyCnt;    /* 延时保护时间 */
    uint8 Stall_Cnt;      /* 堵转计数时间 */
    uint8 SET_Cnt;          /* 堵转判断时间 */
	uint16 Stall_Position_COUNT;      /* 堵转速度判断值 */
	uint16 Stall_RecoverCnt;
} FaultStallTypedef;

typedef struct
{
   uint16 DetecCnt;
   uint16 WarningDetecCnt;
   uint8  WarningFlag; 
}FaultTemperatureVarible;

typedef struct
{
	FaultCurrentVarible         Current;
	uint16 FaultCurroffset_cnt;
    FaultVoltageVarible Voltage;
    FaultOverCurrentVarible OverCurrent;
    FaultStallTypedef Stall;
    FaultStartTypedef Start;
    FaultTemperatureVarible     Temperature;
} FaultVarible;

///* Exported variables ---------------------------------------------------------------------------*/

extern FaultStateType data mcFaultSource;

extern FaultVarible xdata Fault;

/* Exported functions ---------------------------------------------------------------------------*/


extern void Fault_OverCurrent(void);
extern void Fault_Detection(void);
extern void Fault_Recovery(void);

#endif
