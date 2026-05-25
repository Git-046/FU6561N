/**
 * @file      FU6511_MCU.h
 * @version  V1.0.0
 * @author   FortiorTech Hardware Team
 * @date      2025-03-06 16:35:41
 * @brief    Application Department Version V1.0.1, update date 2025-07-14
 *
 * @copyright Copyright(C) 2022, Fortior Technology Co., Ltd. All rights reserved.
 */

#ifndef __FU6511_MCU_H__
#define __FU6511_MCU_H__

// Include external header file.
#include "FU6511_Type.h"

#ifdef __cplusplus
extern "C"{
#endif


    /*******************************************************************************///Define Macro
#define FREC                            (24000000)                              // MCU工作频率

    /*******************************************************************************///Function Subject
#define SetReg(reg, val1, val2)         ((reg) = (reg) & (~(val1)) | (val2))    // 将reg中val1对应的位写val2
#define SetBit(reg, val)                ((reg) |=  (val))                       // 将reg中val对应的位写1
#define ClrBit(reg, val)                ((reg) &= ~(val))                       // 将reg中val对应的位写0
#define XorBit(reg, val)                ((reg) ^=  (val))                       // 将reg中val对应的位取反
#define ReadBit(reg, val)               (((reg) & (val)) != 0)                  // 判断reg中val对应的位是否为1
    /*******************************************************************************///Register Map
    // 搜索快照寄存器请在Keil搜索"[0-9A-Z]__[A-Z]"，勾选"Match Case"和"Regular Expressior"
    // 快速搜索外设方法:在搜索对话框中输入"//"+外设名称.例:搜索TIMER1,则输入"//TIM1"
    /**
     * @addtogroup FU6511_StdPeriphDriver_C51
     * @{
     */
    /******************************************************************************///SFR
 SFR16(DPTR, 0x82);                                                             // RV:0000H  8051指针
 SFR(PSW, 0xd0);                                                                // RV:  00H  8051状态寄存器
    SBIT(CY, 0xd0, 7);                                                          // rw-- 进/借位标志
    SBIT(AC, 0xd0, 6);                                                          // rw-- BCD进/借位标志
    SBIT(F0, 0xd0, 5);                                                          // rw-- 用户自定义标志0
    SBIT(RS1,0xd0, 4);                                                          // rw-- R寄存器换页位1
    SBIT(RS0,0xd0, 3);                                                          // rw-- R寄存器换页位0
    SBIT(OV, 0xd0, 2);                                                          // rw-- 算术运算溢出标志
    SBIT(F1, 0xd0, 1);                                                          // rw-- 用户自定义标志1
    SBIT(P,  0xd0, 0);                                                          // rw-- 奇偶标志

 SFR(ACC, 0xe0);                                                                // RV:  00H  8051 A寄存器
    /******************************************************************************///RST
 SFR(RST_SR, 0xc9);                                                             // RV:  xxH  复位状态寄存器
#define RSTPOW                          0x80                                    // r--- 上电复位标志
#define RSTCLR                          0x80                                    // w1-- 复位标志清零
#define RSTEXT                          0x40                                    // r--- 外部复位标志
#define RSTLVD                          0x20                                    // r--- 低压复位标志
#define RSTWDT                          0x08                                    // r--- 看门狗复位标志
#define RSTFED                          0x04                                    // r--- 代码保护复位
#define RSTDBG                          0x02                                    // r--- 调试接口复位
#define SOFTR                           0x01                                    // rw1- 清除复位标志
    /******************************************************************************///RTC
#define RTC_TM                          *(_IO uint16 xdata *)0x402c             // RV:  FFFFH  RTC计数寄存器
#define RTC_STA                         *(_IO  uint8 xdata *)0x402e             // RV:  00H  RTC控制寄存器
#define RTC_EN                          0x80                                    // rw-- RTC使能
#define ISOSCEN                         0x10                                    // rw-- 内部慢时钟使能

    /******************************************************************************///CLOCK_CAL
#define CAL_CR0                     *(_IO  uint8 xdata *)0x4044                 // RV:  80H  时钟校准寄存器
#define CAL_STA                               0x80                              //               w-- CLOCK_CAL使能,   r-- 0-->Done 1-->ING
#define CAL_CR1                     *(_IO  uint8 xdata *)0x4045                 // RV:  00H  时钟校准寄存器
    /******************************************************************************///IRQ
    //  IRQ Channel
    //  LVW/TSD         interrupt 0
    //  INT0            interrupt 1
    //  INT1            interrupt 2
    //  DRV             interrupt 3
    //  TIM2            interrupt 4
    //  TIM1            interrupt 5
    //  ADC             interrupt 6
    //  CMP/HALL        interrupt 7
    //  RTC             interrupt 8
    //  TIM3            interrupt 9
    //  SysTick         interrupt 10
    //  TIM4            interrupt 11
    //  CMP3            interrupt 12
    //  UART            interrupt 13

SFR(TCON, 0x88);                                                                // RV:  00H  8051中断控制寄存器
SBIT(IT11, 0x88, 4);                                                            //               rw-- 外部中断1触发配置1
SBIT(IT10, 0x88, 3);                                                            //               rw-- 外部中断1触发配置0
SBIT(IT01, 0x88, 1);                                                            //               rw-- 外部中断0触发配置1
SBIT(IT00, 0x88, 0);                                                            //               rw-- 外部中断0触发配置0

SFR(IE, 0xa8);                                                                  // RV:  00H  8051 中断开关
SBIT(EA,   0xa8, 7);                                                            //               rw-- MCU总中断使能
SBIT(RTCIE,0xa8, 6);                                                            //               rw-- RTC中断使能
SBIT(ES0,  0xa8, 4);                                                            //               rw-- UART中断使能
SBIT(EX1,  0xa8, 2);                                                            //               rw-- 外部中断1使能
SBIT(TSDIE,0xa8, 1);                                                            //               rw-- TSD中断使能
SBIT(EX0,  0xa8, 0);                                                            //               rw-- 外部中断0使能

SFR(IP0, 0x8a);                                                                 // RV:  00H  外设中断优先级设置G0
#define PDRV1                                    0x80                           //               rw-- DRV优先级配置1
#define PDRV0                                    0x40                           //               rw-- DRV优先级配置0
#define PX11                                     0x20                           //               rw-- 外部中断1优先级配置1
#define PX10                                     0x10                           //               rw-- 外部中断1优先级配置0
#define PX01                                     0x08                           //               rw-- 外部中断0优先级配置1
#define PX00                                     0x04                           //               rw-- 外部中断0优先级配置0
#define PLVW_TSD1                                0x02                           //               rw-- LVW/TSD优先级配置1
#define PLVW_TSD0                                0x01                           //               rw-- LVW/TSD优先级配置0

 SFR(IP1, 0x8b);                                                                // RV:  00H  外设中断优先级设置G1
#define PCMP_HALL1                               0x80                           //               rw-- CMP/HALL优先级配置1
#define PCMP_HALL0                               0x40                           //               rw-- CMP/HALL优先级配置0
#define PADC1                                    0x20                           //               rw-- ADC优先级配置1
#define PADC0                                    0x10                           //               rw-- ADC优先级配置0
#define PTIM11                                   0x08                           //               rw-- TIM1优先级配置1
#define PTIM10                                   0x04                           //               rw-- TIM1优先级配置0
#define PTIM21                                   0x02                           //               rw-- TIM2优先级配置1
#define PTIM20                                   0x01                           //               rw-- TIM2优先级配置0

SFR(IP2, 0x8c);                                                                 // RV:  00H  外设中断优先级设置G2
#define PTIM41                                    0x80                          //               rw-- TIM4优先级配置1
#define PTIM40                                    0x40                          //               rw-- TIM4优先级配置0
#define PSYSTICK1                                 0x20                          //               rw-- SysTick优先级配置1
#define PSYSTICK0                                 0x10                          //               rw-- SysTick优先级配置0
#define PTIM31                                    0x08                          //               rw-- TIM3优先级配置1
#define PTIM30                                    0x04                          //               rw-- TIM3优先级配置0
#define PRTC1                                     0x02                          //               rw-- RTC优先级配置1
#define PRTC0                                     0x01                          //               rw-- RTC优先级配置0

SFR(IP3, 0x8d);                                                                 // RV:  00H  外设中断优先级设置G3
#define PUART11                              	 0x08                           //               rw-- UART1优先级配置1
#define PUART10                                  0x04                           //               rw-- UART1优先级配置0
#define PCMP31                           		 0x02                           //               rw-- CMP3优先级配置1
#define PCMP30                           		 0x01                           //               rw-- CMP3优先级配置0


SFR(SPC_SR0, 0xf2);
#define SPCNCIF                                  0x40                           //               rw0- SPC角度未校正中断标志
#define SPC90IF                                  0x20                           //               rw0- SPC90度中断标志
#define SPCBOIF                                  0x10                           //               rw0- SPC基本定时器上溢中断标志
#define SPCWTIF                                  0x04                           //               rw0- SPC写入时序中断标志
#define SPCPDIF                                  0x02                           //               rw0- SPC位置检测中断标志

    /******************************************************************************///INT_SR
SFR(INT_SR0, 0xf2);
#define T1WOIF                                   0x80                           //               rw0- SVPWM开窗中断标志
#define T1ABDIF                                  0x40                           //               rw0- 自适应续流屏蔽中断标志
#define T1ADIF                                   0x20                           //               rw0- ADC检测中断标志
#define T1BOIF                                   0x10                           //               rw0- 基本定时器上溢中断标志
#define T1ROIF                                   0x08                           //               rw0- 重载定时器上溢中断标志
#define T1WTIF                                   0x04                           //               rw0- 写入时序中断标志
#define T1PDIF                                   0x02                           //               rw0- 位置检测中断标志
#define T1BDIF                                   0x01                           //               rw0- 屏蔽续流结束中断标志



SFR(INT_SR1, 0xf3);
#define T4IR                                     0x20                           //               rw0- 比较匹配/捕获脉宽标志
#define T4IP                                     0x10                           //               rw0- 周期检测标志
#define T4IF                                     0x08                           //               rw0- 计数上溢标志
#define T3IR                                     0x04                           //               rw0- 比较匹配/捕获脉宽标志
#define T3IP                                     0x02                           //               rw0- 周期检测标志
#define T3IF                                     0x01                           //               rw0- 计数上溢标志

SFR(INT_SR2, 0xf4);
#define RTC_IF                                   0x40                           //               rw0- RTC中断标志
#define SYSTIF                                   0x20                           //               rw0- SysTick中断标志
#define DCIF                                     0x08                           //               rw0- 比较匹配中断标志
#define T2IR                                     0x04                           //               rw0- 比较匹配/捕获脉宽标志
#define T2IP                                     0x02                           //               rw0- 周期检测标志
#define T2IF                                     0x01                           //               rw0- 计数上溢标志

SFR(INT_SR3, 0xf5);
#define TSDIF                                    0x40                           //               rw0- TSD温度感应中断标志
#define HALL_IF                                  0x20                           //               rw0- HALL中断触发标志位
#define CMP3IF                                   0x10                           //               rw0- CMP3中断标志
#define CMP2IF                                   0x08                           //               rw0- CMP2中断标志
#define CMP1IF                                   0x04                           //               rw0- CMP1中断标志
#define CMP0IF                                   0x02                           //               rw0- CMP0中断标志
#define LVWIF                                    0x01                           //               rw0- VCC低电压预警中断标志

SFR(INT_SR4, 0xf6);
#define IF0                                      0x02                           //               rw0- 外部中断0标志
#define ADCIF                                    0x01                           //               rw0- ADC中断标志

SFR(PI_CR, 0xf9);
#define HINV                                     0x02                           //               rw-- 上桥反向使能
#define LINV                                     0x01                           //               rw-- 下桥反向使能

    /******************************************************************************///EXT1
SFR(EXT1_IE, 0xd1);                                                             // RV:  00H  P1外部中断使能寄存器
#define P06_IE                                   0x20                           //               rw-- P0.6端口外部中断1使能
#define P05_IE                                   0x10                           //               rw-- P0.5端口外部中断1使能
#define P04_IE                                   0x08                           //               rw-- P0.4端口外部中断1使能
#define P17_IE                                   0x04                           //               rw-- P1.7端口外部中断1使能
#define P13_IE                                   0x02                           //               rw-- P1.3端口外部中断1使能
#define P12_IE                                   0x01                           //               rw-- P1.2端口外部中断1使能

SFR(EXT1_IF, 0xd2);                                                             // RV:  00H  P1外部中断状态寄存器
#define P06_IF                                   0x20                           //               rw0-- P0.6端口外部中断1中断标志位
#define P05_IF                                   0x10                           //               rw0-- P0.5端口外部中断1中断标志位
#define P04_IF                                   0x08                           //               rw0-- P0.4端口外部中断1中断标志位
#define P17_IF                                   0x04                           //               rw0-- P1.7端口外部中断1中断标志位
#define P13_IF                                   0x02                           //               rw0-- P1.3端口外部中断1中断标志位
#define P12_IF                                   0x01                           //               rw0-- P1.2端口外部中断1中断标志位

    /******************************************************************************///CK_CR
SFR(CK_CR, 0x91);
#define UARTCKEN                                 0x40                            //              rw-- UART模块CLK使能
#define T4CKEN                                   0x04                            //              rw-- Timer4模块CLK使能
#define T3CKEN                                   0x02                            //              rw-- Timer3模块CLK使能
#define T2CKEN                                   0x01                            //              rw-- Timer2模块CLK使能

    /******************************************************************************///UART
SFR(UT_CR, 0x98);                                                                // RV:  00H  UART控制寄存器
SBIT(UT_MOD1,0x98, 7);                                                           //               rw-- UART模式配置1
SBIT(UT_MOD0,0x98, 6);                                                           //               rw-- UART模式配置0
SBIT(SM2,    0x98, 5);                                                           //               rw-- 多机通信使能
SBIT(REN,    0x98, 4);                                                           //               rw-- 接收使能
SBIT(TB8,    0x98, 3);                                                           //               rw-- 9位模式下发送的第9位
SBIT(RB8,    0x98, 2);                                                           //               rw-- 9位模式下接收的第9位
SBIT(TI,     0x98, 1);                                                           //               rw-- 发送完成标志
SBIT(RI,     0x98, 0);                                                           //               rw-- 接收完成标志

 SFR(UT_DR, 0x99);                                                               // RV:  00H  UART数据寄存器

 SFR16(UT_BAUD, 0x9a);                                                           // RV:009bH  UART波特率控制寄存器
#define BAUD_SEL                              0x8000                           //               rw--倍频使能
#define UART_RX_INV                           0x4000                           //               rw-- RxD反相使能
#define UART_TX_INV                           0x2000                           //               rw-- TxD反相使能

 /*****************************************************************************///OLDBG 单线Debug
#define OL_CR                          *(_IO  uint8 xdata *)0x4030             // RV:  0bH  OLDBG控制寄存器
#define OLSEL                                    0x04                          //               rw-- 模式选择：0：软件模式    1：硬件模式，开启后四个通道分别输出UCOP、UFLP、URES、ITRIP
#define OLCT1                                    0x02                          //               rw-- OLDBG使能及端口复用配置1
#define OLCT0                                    0x01                          //               rw-- OLDBG使能及端口复用配置0

/******************************************************************************///TIM1
#define TIM1_CR0                       *(_IO  uint8 xdata *)0x4068              // RV:  04H  TIMER1控制寄存器0
#define T1RWEN                                   0x80                           //               w1-- T1RCEN操作允许
#define T1CFLT1                                  0x40                           //               rw-- 换相滤波配置1
#define T1CFLT0                                  0x20                           //               rw-- 换相滤波配置0
#define T1FORC                                   0x10                           //               rw-- 60°强制换相
#define T1OPS1                                   0x08                           //               rw-- 数据传输方式配置1
#define T1OPS0                                   0x04                           //               rw-- 数据传输方式配置0
#define T1BCEN                                   0x02                           //               rw-- 基本定时器计数使能
#define T1RCEN                                   0x01                           //               rw-- 重载定时器计数使能

#define TIM1_CR1                       *(_IO  uint8 xdata *)0x4069              // RV:  00H  TIMER1控制寄存器1
#define T1BAPE                                   0x80                           //               rw-- TIM1__BARR自动装载使能

#define TIM1_CR2                       *(_IO  uint8 xdata *)0x406a              // RV:  00H  TIMER1控制寄存器2
#define T1BRS                                    0x80                           //               rw-- 基本定时器复位源选择

#define TIM1_CR3                       *(_IO  uint8 xdata *)0x406b              // RV:  00H  TIMER1控制寄存器3
#define BLDC150EN                                0x80                           //               rw-- 0:120°方波 1：150°方波
#define T1PSC2                                   0x40                           //               rw-- 定时器分频配置2
#define T1PSC1                                   0x20                           //               rw-- 定时器分频配置1
#define T1PSC0                                   0x10                           //               rw-- 定时器分频配置0
#define T1TIS1                                   0x08                           //               rw-- 输入源选择配置1
#define T1TIS0                                   0x04                           //               rw-- 输入源选择配置0
#define T1INM1                                   0x02                           //               rw-- 输入源噪声滤波配置1
#define T1INM0                                   0x01                           //               rw-- 输入源噪声滤波配置0

#define TIM1_CR4                       *(_IO  uint8 xdata *)0x406c              // RV:  00H  TIMER1控制寄存器4
#define T1CST3                                   0x08                           //               rw-- 换相状态机配置3
#define T1CST2                                   0x04                           //               rw-- 换相状态机配置2
#define T1CST1                                   0x02                           //               rw-- 换相状态机配置1
#define T1CST0                                   0x01                           //               rw-- 换相状态机配置0

#define TIM1_CR5                       *(_IO  uint8 xdata *)0x4072              // RV:  00H  TIMER1控制寄存器5
#define T1ABDIE                                  0x80                           //               rw-- 自适应续流屏蔽中断使能
#define T1WTS                                    0x40                           //               rw-- PWM OFF时候换相，消除窄脉冲
#define ITRIP_DIS                                0x08                           //               rw-- 方波忽略ITRIP电流采样
#define UCOP_DIS                                 0x04                           //               rw-- 方波忽略导通相电压
#define T1AFL1                                   0x02                           //               rw-- ADC采样点压计算结果滤波
#define T1AFL0                                   0x01                           //               rw-- ADC采样点压计算结果滤波

#define TIM1_CR6                       *(_IO  uint8 xdata *)0x4073              // RV:  00H  TIMER1控制寄存器6
#define UDCSEL                                   0x80                           //               rw-- 方波ITRIP/UDC自动采样选择
#define ATSW	                                 0x40                           //               rw-- 自动切换使能
#define ADCMD 	                                 0x20                           //               rw-- ADC采样UFLP-UCOM位置检测模式使能
#define PSEL1	                                 0x10                           //               rw-- 采样相选择1
#define PSEL0	                                 0x08                           //               rw-- 采样相选择0
#define ESEL	                                 0x04                           //               rw-- 采样沿选择
#define WINMD1                                   0x02                           //               rw-- 窗口模式选择1
#define WINMD0                                   0x01                           //               rw-- 窗口模式选择0

#define TIM1_CR7                       *(_IO  uint8 xdata *)0x407c              // RV:  00H  TIMER1 位置检测输入控制寄存器
#define ABDMD                                    0x10                           //               rw-- 快速自适应续流屏蔽使能
#define T1WOIE                                   0x08                           //               rw-- SVPWM开窗中断使能
#define CPEFD                                    0x04                           //               rw-- 置位后强制不检测任何边沿
#define CPEFE                                    0x02                           //               rw-- 置位后强制检测三相双沿

#define TIM1_SR                        *(_IO  uint8 xdata *)0x407d              // RV:  00H  TIMER1 状态寄存器
#define FD1                              		 0x04                           //               r--- 续流方向标志位，仅ABD_MD = 1时有效
#define FD0                              		 0x02                           //               r--- 00:续流时间小于4us 01:相位滞后（采上升沿时开窗后走上管续流） 10:相位超前（采上升沿时开窗后走下管续流）
#define SWBUSY                                   0x01                           //               r--- 模式切换BUSY

#define TIM1_IER                       *(_IO  uint8 xdata *)0x406d              // RV:  00H  TIMER1中断控制
#define T1UPD                                    0x80                           //               w1-- OPS=00时，触发数据传输
#define T1MAME                                   0x40                           //               rw-- TIM1手动模式使能
#define T1ADIE                                   0x20                           //               rw-- ADC检测中断使能
#define T1BOIE                                   0x10                           //               rw-- 基本定时器上溢中断使能
#define T1ROIE                                   0x08                           //               rw-- 重载定时器上溢中断使能
#define T1WTIE                                   0x04                           //               rw-- 写入时序中断使能
#define T1PDIE                                   0x02                           //               rw-- 位置检测中断使能
#define T1BDIE                                   0x01                           //               rw-- 屏蔽续流结束中断使能

#define TIM1_DBR1                      *(_IO  uint8 xdata *)0x4074              // RV:  00H  TIMER1 U相在CST=2,1的数据
#define T1UHP2                                   0x80                           //               rw-- CST=2时U相上桥输出极性
#define T1ULP2                                   0x40                           //               rw-- CST=2时U相下桥输出极性
#define T1UHE2                                   0x20                           //               rw-- CST=2时U相上桥输出使能
#define T1ULE2                                   0x10                           //               rw-- CST=2时U相下桥输出使能
#define T1UHP1                                   0x08                           //               rw-- CST=1时U相上桥输出极性
#define T1ULP1                                   0x04                           //               rw-- CST=1时U相下桥输出极性
#define T1UHE1                                   0x02                           //               rw-- CST=1时U相上桥输出使能
#define T1ULE1                                   0x01                           //               rw-- CST=1时U相下桥输出使能

#define TIM1_DBR2                      *(_IO  uint8 xdata *)0x4075              // RV:  00H  TIMER1 U相在CST=4,3的数据
#define TIM1_DBR3                      *(_IO  uint8 xdata *)0x4076              // RV:  00H  TIMER1 U相在CST=6,5的数据
#define TIM1_DBR4                      *(_IO  uint8 xdata *)0x4077              // RV:  00H  TIMER1 U相在CST=8,7的数据
#define TIM1_DBR5                      *(_IO  uint8 xdata *)0x4078              // RV:  00H  TIMER1 U相在CST=10,9的数据
#define TIM1_DBR6                      *(_IO  uint8 xdata *)0x4079              // RV:  00H  TIMER1 U相在CST=12,11的数据

#define TIM1__BCOR                     *(_IO uint16 xdata *)0x4070              // RV:0000H  捕获基本定时器计数值滤波值
#define TIM1__BCNTR                    *(_IO uint16 xdata *)0x4082              // RV:0000H  基本定时器计数值
#define TIM1__BCCR                     *(_IO uint16 xdata *)0x4084              // RV:0000H  捕获基本定时器计数值
#define TIM1__BARR                     *(_IO uint16 xdata *)0x4086              // RV:0000H  基本定时器自动重载值
#define TIM1__RARR                     *(_IO uint16 xdata *)0x4088              // RV:0000H  重载定时器自动重载值
#define TIM1__RCNTR                    *(_IO uint16 xdata *)0x408a              // RV:ffffH  重载定时器计数值
#define TIM1__ITRIP                    *(_I  uint16 xdata *)0x40e6              // RV:0000H  滤波后的母线电流
#define TIM1__UCOP                     *(_IO uint16 xdata *)0x408c              // RV:0000H  导通相电压
#define TIM1__UFLP                     *(_IO uint16 xdata *)0x408e              // RV:0000H  悬空相电压
#define TIM1__URES                     *(_IO  int16 xdata *)0x4090              // RV:0000H  计算结果，公式发生符号变化即产生换相信号
#define TIM1_KRMAX                     *(_IO  uint8 xdata *)0x4092              // RV:  00H  上升沿系数最大值
#define TIM1_KFMIN                     *(_IO  uint8 xdata *)0x4093              // RV:  00H  下降沿系数最小值
#define TIM1_KF                        *(_IO uint16 xdata *)0x4094              // RV:0000H  下降沿系数
#define TIM1_KR                        *(_IO uint16 xdata *)0x4096              // RV:0000H  上升沿系数

/******************************************************************************///SVPWM
#define SV_UQ                          *(_IO  int16 xdata *)0x40a6              // RV:0000H  SVPWM输出电压（占空比）

#define SV_CR0                         *(_IO  uint8 xdata *)0x40a8              // RV:  00H  SVPWM控制寄存器0
#define TCLKSEL1                             	 0x80                           //               rw-- 角度累加触发源选择1
#define TCLKSEL0                                 0x40                           //               rw-- 角度累加触发源选择0
#define CENSEL                                   0x20                           //               rw-- SVPWM对齐中点选择
#define SEG7EN                                   0x10                           //               rw-- 七段式使能
#define WINDIS                                   0x08                           //               rw-- 开窗禁止
#define THELIM                                   0x04                           //               rw-- 角度累加限制使能
#define ATADD	                                 0x02                           //               rw-- 角度自动累加使能
#define CMUPD                                    0x01                           //               rw-- 换相角度更新使能

#define SV_CR1                         *(_IO  uint8 xdata *)0x40a9              // RV:  00H  SVPWM控制寄存器1
#define SPDUPD                                   0x08                           //               rw-- 速度自动更新使能
#define OVMDL                                    0x04                           //               rw-- 过调制使能
#define HALLUPD                                  0x02                           //               rw-- HALL角度更新使能（三相）
#define UDSEL                                    0x01                           //               rw-- D-Q轴电压输入使能

#define THETA                          *(_IO uint16 xdata *)0x40aa              // RV:0000H  SVPWM的16位角度

#define SPEED_STEP                     *(_IO uint16 xdata *)0x40f8              // RV:0000H  SVPWM的16位角度增量

#define LEAD_ANGLE                     *(_IO uint16 xdata *)0x40ae              // RV:0000H  SVPWM的14位提前角

#define SV_UD                          *(_IO  int16 xdata *)0x40ae              // RV:0000H  SVPWM的D轴电压幅值(仅SV_CR1[UDSEL]=1时有效)

#define WIN_DLY                        *(_IO  uint8 xdata *)0x40b0              // RV:  00H  延迟开窗选择

#define THETA_HALL                     *(_IO uint16 xdata *)0x40e4              // RV:0000H  HALL上升沿角度

/******************************************************************************///SPC
#define SPC_CR0                        *(_IO  uint8 xdata *)0x4068              // RV:  00H  SPC控制寄存器0
#define SPCCFLT1                                 0x40                           //               rw-- 换相滤波配置1
#define SPCCFLT0                                 0x20                           //               rw-- 换相滤波配置0
#define SPCOPS                                   0x04                           //               rw-- 数据传输方式配置
#define SPCBCEN                                  0x02                           //               rw-- 基本定时器计数使能

#define SPC_CR1                        *(_IO  uint8 xdata *)0x4069              // RV:  00H  SPC控制寄存器1
#define SPCBAPE                                  0x80                           //              rw-- SPC__BARR自动装载使能

#define SPC_CR2                        *(_IO  uint8 xdata *)0x406a              // RV:  00H  SPC控制寄存器2
#define SPCBRS                                   0x80                           //               rw-- 基本定时器复位源选择

#define SPC_CR3                        *(_IO  uint8 xdata *)0x406b              // RV:  00H  SPC控制寄存器3
#define SPCPSC2                                  0x40                           //               rw-- 定时器分频配置2
#define SPCPSC1                                  0x20                           //               rw-- 定时器分频配置1
#define SPCPSC0                                  0x10                           //               rw-- 定时器分频配置0
#define SPCINM1                                  0x02                           //               rw-- 输入源噪声滤波配置1
#define SPCINM0                                  0x01                           //               rw-- 输入源噪声滤波配置0

#define SPC_CR4                        *(_IO  uint8 xdata *)0x406c              // RV:  00H  SPC控制寄存器4
#define SPCCST1                                  0x02                           //               rw-- 换相状态机配置1
#define SPCCST0                                  0x01                           //               rw-- 换相状态机配置0

#define SPC_CR5                        *(_IO  uint8 xdata *)0x4072              // RV:  00H  SPC控制寄存器5
#define SPCNCIE                                  0x80                           //               rw-- 角度未校正中断使能
#define SPCEN                                    0x10                           //               rw-- SPC使能

#define SPC_CR6                        *(_IO  uint8 xdata *)0x4073              // RV:  00H  SPC控制寄存器6
#define SPCSSEL1                                 0x80                           //               rw-- SPC__BCCR累加次数1
#define SPCSSEL0                                 0x40                           //               rw-- SPC__BCCR累加次数0
#define SPCSCNT1                                 0x20                           //               rw-- SPC__BCCR当前累加次数1
#define SPCSCNT0                                 0x10                           //               rw-- SPC__BCCR当前累加次数0
#define SPCPWMR                                  0x04                           //               rw-- 上坡状态标志
#define SPCPWMB                                  0x02                           //               rw-- 平台状态标志
#define SPCPWMF                                  0x01                           //               rw-- 下坡状态标志

#define SPC_CR7                        *(_IO  uint8 xdata *)0x407c              // RV:  00H  SPC控制寄存器7
#define SPCAOF                                   0x40                           //               rw-- SPC__BCNTR防溢出使能

#define SPC_CR8                        *(_IO  uint8 xdata *)0x40a8              // RV:  00H  SPC控制寄存器8
#define SPCTCLK1                             	 0x80                           //               rw-- 角度累加触发源选择1
#define SPCTCLK0                             	 0x40                           //               rw-- 角度累加触发源选择0
#define SPCTADD	                                 0x02                           //               rw-- 角度自动累加使能

#define SPC_CR9                        *(_IO  uint8 xdata *)0x40a9              // RV:  00H  SPC控制寄存器9
#define SPCSUPD                                  0x08                           //               rw-- 速度自动更新使能
#define SPCDIVB                                  0x04                           //               rw-- 单相除数选择

#define SPC_IER                        *(_IO  uint8 xdata *)0x406d              // RV:  00H  SPC中断控制器
#define SPCUPD                                   0x80                           //               w--- SPCOPS=0时，触发数据传输
#define SPC90IE                                  0x20                           //               rw-- 单相90度中断使能
#define SPCBOIE                                  0x10                           //               rw-- 基本定时器上溢中断使能
#define SPCWTIE                                  0x04                           //               rw-- 写入时序中断使能
#define SPCPDIE                                  0x02                           //               rw-- 位置检测中断使能

#define SPC_DBR1                       *(_IO  uint8 xdata *)0x4074              // RV:0000H  SPC U相、V相在SPCCST=1或平台的数据
#define SPC_DBR2                       *(_IO  uint8 xdata *)0x4075              // RV:0000H  SPC U相、V相在SPCCST=2的数据
#define SPC_DBR3                       *(_IO  uint8 xdata *)0x4076              // RV:0000H  SPC U相、V相在SPCCST=3的数据
#define SPCVHP                                   0x80                           //               rw-- V相上管输出极性
#define SPCVLP                                   0x40                           //               rw-- V相下管输出极性
#define SPCUHP                                   0x20                           //               rw-- U相上管输出极性
#define SPCULP                                   0x10                           //               rw-- U相下管输出极性
#define SPCVHE                                   0x08                           //               rw-- V相上管输出使能
#define SPCVLE                                   0x04                           //               rw-- V相下管输出使能
#define SPCUHE                                   0x02                           //               rw-- U相上管输出使能
#define SPCULE                                   0x01

#define SPC__BCNTR                     *(_IO uint16 xdata *)0x4082              // RV:0000H  SPC基本定时器计数值
#define SPC__BARR                      *(_IO uint16 xdata *)0x4086              // RV:0000H  SPC基本定时器自动重载值
#define SPC__BCCR                      *(_IO uint16 xdata *)0x4084              // RV:0000H  SPC捕获基本定时器计数值
#define SPC__BCOR                      *(_IO uint16 xdata *)0x4070              // RV:0000H  SPC捕获基本定时器计数值滤波值
#define SPC__BSUM                      *(_IO uint16 xdata *)0x40E4              // RV:0000H  SPC基本捕获累加器
#define SPC__THETA                     *(_IO uint16 xdata *)0x40AA              // RV:0000H  SPC的16位角度
#define SPC__SPEED                     *(_IO uint16 xdata *)0x40F8              // RV:0000H  SPC的16位角度增量
#define SPC_ERRMAX                     *(_IO  uint8 xdata *)0x4077              // RV:  00H  SPC硬件自动校正角度范围
#define SPC_DTHE                       *(_IO  int16 xdata *)0x4078              // RV:0000H  SPC角度误差
#define SPC__LANG                      *(_IO uint16 xdata *)0x40AE              // RV:0000H  SPC的16位提前角
#define SPC__FANG                      *(_IO uint16 xdata *)0x408C              // RV:0000H  SPC下坡角度设置
#define SPC__BANG                      *(_IO uint16 xdata *)0x408E              // RV:0000H  SPC平台角度设置
#define SPC__RANG                      *(_IO uint16 xdata *)0x4090              // RV:0000H  SPC上坡角度设置
#define SPC_RSTA                       *(_IO   int8 xdata *)0x4092              // RV:  00H  SPC上坡起点占空比
#define SPC_FPWMDDR                    *(_IO uint16 xdata *)0x4094              // RV:0000H  SPC控制SPC__PWMDR的占空比减量
#define SPC_RPWMDDR                    *(_IO uint16 xdata *)0x4096              // RV:0000H  SPC控制SPC__PWMDR的占空比增量
#define SPC__PWMDR                     *(_IO uint16 xdata *)0x40A6              // RV:0000H  SPC软切换占空比
/******************************************************************************///TIM2
SFR(TIM2_CR0, 0xa1);                                                            // RV:  00H  TIMER2控制寄存器0
#define T2PSC2                                   0x80                           //               rw-- 定时器分频配置2
#define T2PSC1                                   0x40                           //               rw-- 定时器分频配置1
#define T2PSC0                                   0x20                           //               rw-- 定时器分频配置0
#define T2OCM                                    0x10                           //               rw-- 定时器比较匹配/捕获脉宽模式配置
#define T2IRE                                    0x08                           //               rw-- 比较匹配/捕获脉宽中断
#define T2H0M                                    0x04                           //               rw-- TIMER2霍尔检测模式
#define T2OPM                                    0x02                           //               rw-- 单次模式使能
#define T2MOD                                    0x01                           //               rw-- 定时器模式配置0

SFR(TIM2_CR1, 0xa9);                                                            // RV:  00H  TIMER2控制寄存器1
#define T2IPE                                    0x10                           //               rw-- 周期检测中断使能
#define T2IFE                                    0x08                           //               rw-- 上溢中断使能
#define T2INM1                                   0x04                           //               rw-- 噪音滤波配置1
#define T2INM0                                   0x02                           //               rw-- 噪音滤波配置0
#define T2EN                                     0x01                           //               rw-- 定时器使能

SFR16(TIM2__CNTR, 0xaa);                                                        // RV:0000H  TIMER2计数值
SFR16(TIM2__DR, 0xac);                                                          // RV:0000H  TIMER2匹配值
SFR16(TIM2__ARR, 0xae);                                                         // RV:0000H  TIMER2重载值

/******************************************************************************///TIM3
SFR(TIM3_CR0, 0x9c);                                                            // RV:  00H  TIMER3控制寄存器0
#define T3PSC2                                   0x80                           //               rw-- 定时器分频配置2
#define T3PSC1                                   0x40                           //               rw-- 定时器分频配置1
#define T3PSC0                                   0x20                           //               rw-- 定时器分频配置0
#define T3OCM                                    0x10                           //               rw-- 定时器比较匹配/捕获脉宽模式配置
#define T3IRE                                    0x08                           //               rw-- 比较匹配/捕获脉宽中断
#define T3OPM                                    0x02                           //               rw-- 单次模式使能
#define T3MOD                                    0x01                           //               rw-- 定时器模式配置0

SFR(TIM3_CR1, 0x9d);                                                            // RV:  00H  TIMER3控制寄存器1
#define T3IPE                                    0x10                           //               rw-- 周期检测中断使能
#define T3IFE                                    0x08                           //               rw-- 上溢中断使能
#define T3INM1                                   0x04                           //               rw-- 噪音滤波配置1
#define T3INM0                                   0x02                           //               rw-- 噪音滤波配置0
#define T3EN                                     0x01                           //               rw-- 定时器使能

SFR16(TIM3__CNTR, 0xa2);                                                        // RV:0000H  TIMER3计数值
SFR16(TIM3__DR, 0xa4);                                                          // RV:0000H  TIMER3匹配值
SFR16(TIM3__ARR, 0xa6);                                                         // RV:0000H  TIMER3重载值

    /******************************************************************************///TIM4
SFR(TIM4_CR0, 0x9e);                                                            // RV:  00H  TIMER4控制寄存器0
#define T4PSC2                                   0x80                           //               rw-- 定时器分频配置2
#define T4PSC1                                   0x40                           //               rw-- 定时器分频配置1
#define T4PSC0                                   0x20                           //               rw-- 定时器分频配置0
#define T4OCM                                    0x10                           //               rw-- 定时器比较匹配/捕获脉宽模式配置
#define T4IRE                                    0x08                           //               rw-- 比较匹配/捕获脉宽中断
#define T4FGM                                    0x04                           //               rw-- FG模式使能
#define T4OPM                                    0x02                           //               rw-- 单次模式使能
#define T4MOD                                    0x01                           //               rw-- 定时器模式配置0

SFR(TIM4_CR1, 0x9f);                                                            // RV:  00H  TIMER4控制寄存器1
#define T4IPE                                    0x10                           //               rw-- 周期检测中断使能
#define T4IFE                                    0x08                           //               rw-- 上溢中断使能
#define T4INM1                                   0x04                           //               rw-- 噪音滤波配置1
#define T4INM0                                   0x02                           //               rw-- 噪音滤波配置0
#define T4EN                                     0x01                           //               rw-- 定时器使能

SFR16(TIM4__CNTR, 0x92);                                                        // RV:0000H  TIMER4计数值
SFR16(TIM4__DR, 0x94);                                                          // RV:0000H  TIMER4匹配值
SFR16(TIM4__ARR, 0x96);                                                         // RV:0000H  TIMER4重载值

/******************************************************************************///SYSTICK
#define SYST_CR                        *(_IO uint8 xdata *)0x4064                         // RV:       SysTick模式选择
#define SYST_SEL1                                0x02
#define SYST_SEL0                                0x01

/******************************************************************************///DRV
SFR(DRV_OUT, 0xf8);                                                             // RV:  00H  驱动输出控制寄存器
    SBIT(MOEH,  0xf8, 7);                                                       //               rw-- 主输出高位使能
    SBIT(MOEL,  0xf8, 6);                                                       //               rw-- 主输出低位使能
    SBIT(OISWL, 0xf8, 5);                                                       //               rw-- WL & XL输出空闲电平
    SBIT(OISWH, 0xf8, 4);                                                       //               rw-- WH & XH输出空闲电平
    SBIT(OISVL, 0xf8, 3);                                                       //               rw-- VL输出空闲电平
    SBIT(OISVH, 0xf8, 2);                                                       //               rw-- VH输出空闲电平
    SBIT(OISUL, 0xf8, 1);                                                       //               rw-- UL输出空闲电平
    SBIT(OISUH, 0xf8, 0);                                                       //               rw-- UH输出空闲电平

#define DRV_DR                         *(_IO uint16 xdata *)0x4058              // RV:0000H  手动控制输出时设定的比较值
#define DRV_COMR                       *(_IO uint16 xdata *)0x405a              // RV:0000H  计数器的比较匹配值

#define DRV_CMR                        *(_IO uint16 xdata *)0x405c              // RV:0000H  输出配置寄存器
#define WHP                                      0x0800                         //               rw-- W相管反相使能
#define WLP                                      0x0400                         //               rw-- W相下管反相使能
#define VHP                                      0x0200                         //               rw-- V相上管反相使能
#define VLP                                      0x0100                         //               rw-- V相下管反相使能
#define UHP                                      0x0080                         //               rw-- U相上管反相使能
#define ULP                                      0x0040                         //               rw-- U相下管反相使能
#define WHE                                      0x0020                         //               rw-- W相上管输出使能
#define WLE                                      0x0010                         //               rw-- W相下管输出使能
#define VHE                                      0x0008                         //               rw-- V相上管输出使能
#define VLE                                      0x0004                         //               rw-- V相下管输出使能
#define UHE                                      0x0002                         //               rw-- U相上管输出使能
#define ULE                                      0x0001                         //               rw-- U相下管输出使能

#define DRV_ARR                        *(_IO uint16 xdata *)0x405e              // RV:0000H  计数器重载值
#define DRV_DTR                        *(_IO  uint8 xdata *)0x4060              // RV:  00H  上下管死区时间

#define DRV_CR1                        *(_IO  uint8 xdata *)0x4061              // RV:  00H  Driver状态寄存器
#define DTRMD                                    0x80                           //               r--- 死区范围选择
#define DRV_MD                                   0x40                           //               rw-- 0:中央对齐 1:锯齿波
#define DCIP                                     0x04                           //               rw-- 产生比较匹配中断的间隔
#define DCIM1                                    0x02                           //               rw-- 比较匹配中断模式配置1
#define DCIM0                                    0x01                           //               rw-- 比较匹配中断模式配置0

#define DRV__CNTR                      *(_IO uint16 xdata *)0x4066              // RV:0000H  计数器值

#define DRV_CR0                        *(_IO  uint8 xdata *)0x4062              // RV:  00H  Driver控制寄存器
#define DRVEN                                    0x80                           //               rw-- 载波计数器使能
#define DDIR                                     0x40                           //               rw-- 输出方向
#define SVEN                                     0x20                           //               rw-- FOC/SVPWM/SPWM模块使能
#define DRPE                                     0x10                           //               rw-- DRV_DR预装载使能
#define CMRDIS                                   0x08                           //               rw-- DRMCMR禁止模式
#define MESEL                                    0x04                           //               rw-- ME核工作模式选择
#define DRVOE                                    0x01                           //               rw-- Driver输出使能

    /******************************************************************************///WDT
#define WDT_CR                         *(_IO  uint8 xdata *)0x4026              // RV:  00H  看门狗控制寄存器
#define WDTRF                                    0x01                           //               rw-- 看门狗初始化标志

#define WDT_ARR                        *(_IO  uint8 xdata *)0x4027              // RV:  00H  看门狗重载计数器

    /******************************************************************************///GPIO
SFR(P0, 0x80);                                                                  // RV:  00H  Port 0
    SBIT(GP00, 0x80, 0);
    SBIT(GP01, 0x80, 1);
    SBIT(GP02, 0x80, 2);
    SBIT(GP03, 0x80, 3);
    SBIT(GP04, 0x80, 4);
    SBIT(GP05, 0x80, 5);
    SBIT(GP06, 0x80, 6);
    SBIT(GP07, 0x80, 7);

SFR(P1, 0x90);                                                                  // RV:  00H  Port 1
    SBIT(GP10, 0x90, 0);
    SBIT(GP11, 0x90, 1);
    SBIT(GP12, 0x90, 2);
    SBIT(GP13, 0x90, 3);
    SBIT(GP14, 0x90, 4);
    SBIT(GP15, 0x90, 5);
    SBIT(GP16, 0x90, 6);
    SBIT(GP17, 0x90, 7);

SFR(P0_OE,  0xfc);                                                              // RV:  00H  P0输出使能寄存器
#define P0_PU                          *(_IO  uint8 xdata *)0x4053              // RV:  00H  P0上拉使能寄存器
#define P0_AN                          *(_IO  uint8 xdata *)0x4051              // RV:  00H  P0模拟使能寄存器
#define P07                                      0x80                           //               rw-- 使能端口7
#define P06                                      0x40                           //               rw-- 使能端口6
#define P05                                      0x20                           //               rw-- 使能端口5
#define P04                                      0x10                           //               rw-- 使能端口4
#define P03                                      0x08                           //               rw-- 使能端口3
#define P02                                      0x04                           //               rw-- 使能端口2
#define P01                                      0x02                           //               rw-- 使能端口1
#define P00                                      0x01                           //               rw-- 使能端口0

SFR(P1_OE,  0xfd);                                                              // RV:  00H  P1输出使能寄存器
#define P1_PU                          *(_IO  uint8 xdata *)0x4054              // RV:  00H  P1上拉使能寄存器
#define P17                                      0x80                           //               rw-- 使能端口7
#define P16                                      0x40                           //               rw-- 使能端口6
#define P15                                      0x20                           //               rw-- 使能端口5
#define P14                                      0x10                           //               rw-- 使能端口4
#define P13                                      0x08                           //               rw-- 使能端口3
#define P12                                      0x04                           //               rw-- 使能端口2
#define P11                                      0x02                           //               rw-- 使能端口1
#define P10                                      0x01                           //               rw-- 使能端口0

#define P1_AN                          *(_IO  uint8 xdata *)0x4050              // RV:  00H  P1模拟使能寄存器
#define P16                                      0x40                           //               rw-- 使能端口6
#define P15                                      0x20                           //               rw-- 使能端口5
#define P14                                      0x10                           //               rw-- 使能端口4
#define HBMOD                                    0x08                           //               rw-- 端口P13模式配置
#define HDIO                                     0x04                           //               rw-- 端口P13模式配置
#define P11                                      0x02                           //               rw-- 使能端口1
#define P10                                      0x01                           //               rw-- 使能端口0

#define PX_PL                          *(_IO  uint8 xdata *)0x4052              // RV:  00H  P3模拟使能寄存器
#define P17_PL                                   0x80                           //               rw-- P17下拉使能
#define P13_PL									 0x08							//               rw-- P13下拉使能
#define ODE1                                     0x02                           //               rw-- P03漏极开路使能
#define ODE0                                     0x01                           //               rw-- P02漏极开路使能

#define PIN7                                     0x80                           //               rw-- PIN7, 用于替代上面的端口编号
#define PIN6                                     0x40                           //               rw-- PIN6, 用于替代上面的端口编号
#define PIN5                                     0x20                           //               rw-- PIN5, 用于替代上面的端口编号
#define PIN4                                     0x10                           //               rw-- PIN4, 用于替代上面的端口编号
#define PIN3                                     0x08                           //               rw-- PIN3, 用于替代上面的端口编号
#define PIN2                                     0x04                           //               rw-- PIN2, 用于替代上面的端口编号
#define PIN1                                     0x02                           //               rw-- PIN1, 用于替代上面的端口编号
#define PIN0                                     0x01                           //               rw-- PIN0, 用于替代上面的端口编号

#define PH_SEL                         *(_IO  uint8 xdata *)0x404c              // RV:  00H  端口复用
#define UARTEN                                   0x40                           //               rw-- 端口复用为UART1
#define T4SEL                                    0x10                           //               rw-- 端口复用为TIM4
#define T3SEL                                    0x08                           //               rw-- 端口复用为TIM3
#define T2SEL                                    0x04                           //               rw-- 端口复用为TIM2

#define PH_SEL1                        *(_IO  uint8 xdata *)0x404d              // RV:  00H  端口复用
#define CMPSELCT1                                0x80                           //               rw-- 比较器输出功能转移：00:P02  01:P03  10:P04
#define CMPSELCT0                                0x40                           //               rw-- 比较器输出功能转移
#define UARTCH                                   0x20                           //               rw-- UART功能转移
#define DBGCT                                    0x10                           //               rw-- Debug输出功能转移
#define T2CT                                     0x08                           //               rw-- TIM2功能转移
#define T4CT                                     0x04                           //               rw-- TIM4功能转移
#define T3CT1                                    0x02                           //               rw-- TIM3功能转移0
#define T3CT0                                    0x01                           //               rw-- TIM3功能转移1

/******************************************************************************///ADC
#define ADC_MASK                       *(_IO uint16 xdata *)0x4036              // RV:0000H  ADC通道使能
#define CH9EN                                    0x0200                         //               rw-- ADC通道9使能
#define CH8EN                                    0x0100                         //               rw-- ADC通道8使能
#define CH7EN                                    0x0080                         //               rw-- ADC通道7使能
#define CH6EN                                    0x0040                         //               rw-- ADC通道6使能
#define CH5EN                                    0x0020                         //               rw-- ADC通道5使能
#define CH4EN                                    0x0010                         //               rw-- ADC通道4使能
#define CH3EN                                    0x0008                         //               rw-- ADC通道3使能
#define CH2EN                                    0x0004                         //               rw-- ADC通道2使能
#define CH1EN                                    0x0002                         //               rw-- ADC通道1使能
#define CH0EN                                    0x0001                         //               rw-- ADC通道0使能

#define ADC_SCYC1                      *(_IO  uint8 xdata *)0x4035              // RV:  08H  DAC控制寄存器配置
#define DAC0_1EN                                 0x80                           //               rw-- DAC0,1使能
#define DACMOD                                   0x40                           //               rw-- DAC模式设置

#define ADC_SCYC2                      *(_IO  uint8 xdata *)0x4038              // RV:  33H  ADC通道采样时间配置

#define ADC_CR                         *(_IO  uint8 xdata *)0x4039              // RV:  00H  ADC控制寄存器
#define ADCEN                                    0x80                           //               rw-- 使能ADC
#define ADCBSY                                   0x40                           //               rw1- ADC启动 & ADC忙标志
#define ADCIE                                    0x02                           //               rw-- ADC中断使能

#define ADC0_DR                        *(_I  uint16 xdata *)0x0220              // RV:0000H  ADC通道0转换结果
#define ADC1_DR                        *(_I  uint16 xdata *)0x0222              // RV:0000H  ADC通道1转换结果
#define ADC2_DR                        *(_I  uint16 xdata *)0x0224              // RV:0000H  ADC通道2转换结果
#define ADC3_DR                        *(_I  uint16 xdata *)0x0226              // RV:0000H  ADC通道3转换结果
#define ADC4_DR                        *(_I  uint16 xdata *)0x0228              // RV:0000H  ADC通道4转换结果
#define ADC5_DR                        *(_I  uint16 xdata *)0x022a              // RV:0000H  ADC通道5转换结果
#define ADC6_DR                        *(_I  uint16 xdata *)0x022c              // RV:0000H  ADC通道6转换结果
#define ADC7_DR                        *(_I  uint16 xdata *)0x022e              // RV:0000H  ADC通道7转换结果
#define ADC8_DR                        *(_I  uint16 xdata *)0x0230              // RV:0000H  ADC通道8转换结果
#define ADC9_DR                        *(_I  uint16 xdata *)0x0232              // RV:0000H  ADC通道9转换结果
    /*****************************************************************************///DAC
#define DAC0_DR                        *(_IO  uint8 xdata *)0x404b              // RV:  00H  DAC0输出值

#define DAC1_DR                        *(_IO  uint8 xdata *)0x404a              // RV:  00H  DAC0,1输出值
    /******************************************************************************///VREF & VHALF
#define VREF_VHALF_CR                  *(_IO  uint8 xdata *)0x404f              // RV:  06H  VREF & VHALF 控制寄存器
#define VRVSEL1                                  0x80                           //               rw-- VREF电压配置1
#define VRVSEL0                                  0x40                           //               rw-- VREF电压配置0
#define VREFEN                                   0x10                           //               rw-- VREF使能
#define VHALFSEL1                                0x04                           //               rw-- VHalf电压配置1
#define VHALFSEL0                                0x02                           //               rw-- VHalf电压配置0
#define VHALFEN                                  0x01                           //               rw-- VHalf使能
    /******************************************************************************///AMP
#define AMP_CR0                        *(_IO  uint8 xdata *)0x404e              // RV:  00H  运放控制寄存器0
#define AMP0M_GND                                0x20                           //               rw-- 运放初始化
#define CP_EN                                    0x10                           //               rw-- 运放初始化
#define AMP0_GAIN2                               0x08                           //               rw-- 运放0配置0
#define AMP0_GAIN1                               0x04                           //               rw-- 运放0配置0
#define AMP0_GAIN0                               0x02                           //               rw-- 运放0配置0
#define AMP0EN                                   0x01                           //               rw-- 运放0使能
    /******************************************************************************///CMP
SFR(CMP_CR0, 0xd5);                                                             // RV:  00H  比较器控制寄存器0
#define CMP3IM1                                  0x80                           //               rw-- CMP3中断模式配置1
#define CMP3IM0                                  0x40                           //               rw-- CMP3中断模式配置0
#define CMP2IM1                                  0x20                           //               rw-- CMP2中断模式配置1
#define CMP2IM0                                  0x10                           //               rw-- CMP2中断模式配置0
#define CMP1IM1                                  0x08                           //               rw-- CMP1中断模式配置1
#define CMP1IM0                                  0x04                           //               rw-- CMP1中断模式配置0
#define CMP0IM1                                  0x02                           //               rw-- CMP0中断模式配置1
#define CMP0IM0                                  0x01                           //               rw-- CMP0中断模式配置0

SFR(CMP_CR1, 0xd6);                                                             // RV:  00H  比较器控制寄存器1
#define HALLHYS1                                 0x80							//			     rw-- HALL比较器迟滞电压配置1
#define HALLHYS0                                 0x40                           //               rw-- HALL比较器迟滞电压配置0
#define CMP3MOD                                  0x20                           //               rw-- CMP3模式配置
#define CMP3EN                                   0x10                           //               rw-- CMP3使能
#define CMP3HYS                                  0x08                           //               rw-- CMP3迟滞使能
#define HALL0REN								 0x04							//				 rw-- HALL0反相使能
#define CMP0HYS1                                 0x02                           //               rw-- CMP0迟滞电压配置1
#define CMP0HYS0                                 0x01                           //               rw-- CMP0迟滞电压配置0


SFR(CMP_CR2, 0xda);                                                             // RV:  00H  比较器控制寄存器2
#define CMP4EN                                   0x80                           //               rw-- CMP0模式配置1
#define CMP0MOD                                  0x20                           //               rw-- CMP0模式配置0
#define CMP0SEL1                                 0x10                           //               rw-- CMP0端口组合配置1
#define CMP0SEL0                                 0x08                           //               rw-- CMP0端口组合配置0
#define CMP0EN                                   0x01                           //               rw-- CMP0使能

SFR(CMP_CR3, 0xdc);                                                             // RV:  00H  比较器控制寄存器3
#define DBGSEL1                                  0x40                           //               rw-- DBG信号配置1
#define DBGSEL0                                  0x20                           //               rw-- DBG信号配置0
#define SAMSEL1                                  0x10                           //               rw-- CMP0/1/2 & ADC 采样时机配置1
#define SAMSEL0                                  0x08                           //               rw-- CMP0/1/2 & ADC 采样时机配置0
#define CMPSEL2                                  0x04                           //               rw-- 比较器输出选择配置2
#define CMPSEL1                                  0x02                           //               rw-- 比较器输出选择配置1
#define CMPSEL0                                  0x01                           //               rw-- 比较器输出选择配置0

SFR(CMP_CR4, 0xe1);                                                             // RV:  00H  比较器控制寄存器4
#define CMP4OUT                                  0x80                           //               r -- 比较器4输出
#define C0HDIG                               	 0x08                           //               rw-- 比较器0迟滞
#define FAEN                                     0x04                           //               rw-- 滤波采样系数扩大使能
#define C0HMOD1                               	 0x02                           //               rw-- 比较器0迟滞模式选择
#define C0HMOD0                               	 0x01                           //               rw-- 比较器0迟滞模式选择

SFR(HALL_CR, 0xe2);                                                             // RV:  00H  HALL控制寄存器
#define HALL_IE                                  0x80                           //               rw-- HALL中断使能
#define H0PDIS                                   0x40                           //               rw-- HALL0上升沿禁止
#define H0NDIS                                   0x20                           //               rw-- HALL0下降沿禁止
#define HALLSEL1                                 0x10                           //               rw-- 霍尔信号输入选择1
#define HALLSEL0                                 0x08                           //               rw-- 霍尔信号输入选择0
#define HALL2                                    0x04                           //               rw-- HALL2滤波后输出电平
#define HALL1                                    0x02                           //               rw-- HALL1滤波后输出电平
#define HALL0                                    0x01                           //               rw-- HALL0滤波后输出电平

SFR(CMP_SR, 0xd7);                                                              // RV:  00H  比较器状态寄存器
#define CMP3OUT                                  0x08                           //               r--- CMP3比较结果
#define CMP2OUT                                  0x04                           //               r--- CMP2比较结果
#define CMP1OUT                                  0x02                           //               r--- CMP1比较结果
#define CMP0OUT                                  0x01                           //               r--- CMP0比较结果

SFR(EVT_FILT,   0xd9);                                                          // RV:  00H  驱动保护功能
#define OFF_MOEH_EN                              0x40                           //               rw-- MOE_OFF使能
#define OFF_MOEL_EN                              0x20                           //               rw-- MOE_OFF使能
#define MOEMD1                                   0x10                           //               rw-- MOE自动控制配置1
#define MOEMD0                                   0x08                           //               rw-- MOE自动控制配置0
#define INT0_MOE_EN                              0x04                           //               rw-- 外部中断MOE关断使能位
#define EFDIV1                                   0x02                           //               rw-- 母线电流保护滤波配置1
#define EFDIV0                                   0x01                           //               rw-- 母线电流保护滤波配置0

#define CMP_SAMON                      *(_IO  uint8 xdata *)0x40ac              // RV:  00H  比较器采样配置
#define CMP_SAMOFF                     *(_IO  uint8 xdata *)0x40ad              // RV:  00H  比较器采样配置
#define UPDTRG                                   0x80          					// w1-- CMP0/CMP1/CMP2采样屏蔽配置更新触发位
    /******************************************************************************///FLASH
    /**
     * @defgroup Flash
     * @brief Flash 驱动模块
     * @{
     * @defgroup Register
     * @{
     */

    /**
     * @defgroup FLA_KEY
     * @brief 解锁寄存器, 编程前必须通过该寄存器解除编程锁
     * - 解锁步骤为:
     *     1. 写入0x5a,   此时读该寄存器应该读出0x01
     *     2. 写入0x1f,   此时读该寄存器应该读出0x03,    此时Flash已经处于解锁状态, 可以进行编程操作
     * - 更多说明可参阅datasheet
     * @{
     */
SFR(FLA_KEY, 0x84);                                                             /**< 默认值: 00H<br> FLASH解锁寄存器<br>  */
    /**
     * @}
     */

    /**
     * @defgroup FLA_CR
     * @{
     */
SFR(FLA_CR, 0x85);                                                              /**< 默认值: 00H<br>  FLASH控制寄存器<br> */
#define CHIP_EN                                  0x40                                         /**< 可读写<br> FLASH全片操作使能 */
#define PAGE_EN                                  0x20                                         /**< 可读写<br> FLASH页操作使能 */
#define FLAERR                                   0x10                                         /**< 只读<br>   FLASH错误标志位 */
#define FLAPRE                                   0x04                                         /**< 可读写<br> FLASH预编程使能 */
#define FLAERS                                   0x02                                         /**< 可读写<br> FLASH清除使能 */
#define FLAEN                                    0x01                                         /**< 可读写<br> FLASH编程使能 */
    /**
     * @}
     */

    /**
     * @}
     * @}
     */
    /******************************************************************************///CRC
#define CRC_DIN                        *(_O   uint8 xdata *)0x4021              // RV:  00H  CRC数据输入寄存器

#define CRC_CR                         *(_IO  uint8 xdata *)0x4022              // RV:  10H  CRC控制寄存器
#define CRCDONE                                  0x10                           //               r--- 自动CRC完成标志
#define CRCDINI                                  0x08                           //               w1-- CRC初始化
#define CRCVAL                                   0x04                           //               rw-- CRC初始化的值
#define AUTOINT                                  0x02                           //               rw-- CRC自动计算使能
#define CRCPNT                                   0x01                           //               rw-- CRC结果访问位置

#define CRC_DR                         *(_IO  uint8 xdata *)0x4023              // RV:  00H  CRC结果输出寄存器
#define CRC_BEG                        *(_IO  uint8 xdata *)0x4024              // RV:  00H  CRC自动计算的起始位置
#define CRC_CNT                        *(_IO  uint8 xdata *)0x4025              // RV:  00H  CRC块数计数器
    /******************************************************************************///TSD
#define TSD_CR                         *(_IO  uint8 xdata *)0x402f              // RV:  06H  TSD控制寄存器
#define TSDEN                                    0x80                           //               rw-- TSD使能
#define TSDADJ3                                  0x08                           //               rw-- TSD温度调节
#define TSDADJ2                                  0x04                           //               rw-- TSD温度调节
#define TSDADJ1                                  0x02                           //               rw-- TSD温度调节
#define TSDADJ0                                  0x01                           //               rw-- TSD温度调节

    /******************************************************************************///POWER
SFR(PCON,   0x87);                                                              // RV:  00H  MCU功率控制寄存器
#define GF3                                      0x20                           //               rw-- 通用标志3
#define GF2                                      0x10                           //               rw-- 通用标志2
#define GF1                                      0x08                           //               rw-- 通用标志1
#define STOP                                     0x02                           //               rw-- MCU睡眠使能
#define IDLE                                     0x01                           //               rw-- MCU待机使能

SFR(LVSR, 0xdb);                                                                // RV:  00H  状态寄存器
#define EXT0CFG2                                 0x20                           //               rw-- 外部中断0端口配置2
#define EXT0CFG1                                 0x10                           //               rw-- 外部中断0端口配置1
#define EXT0CFG0                                 0x08                           //               rw-- 外部中断0端口配置0
#define TSDF                                     0x04                           //               rw0- 过温标志位
#define LVWF                                     0x02                           //               r--- VCC低电压预警标志

#define CCFG1                          *(_O   uint8 xdata *)0x401E              // RV:  00H
#define LVW_EN_B                                 0x80                           //               rw-- VCC低电压预警使能，复位使能
#define LVWIE                                    0x40                           //               rw-- VCC低电压预警中断使能
#define WDT_EN                                   0x20                           //               rw-- WDT使能



#ifdef __cplusplus
}
#endif

#endif  //__FU6511_MCU_H__


/*** (C) Copyright 2022 Fortior Technology Co., Ltd. ***/
