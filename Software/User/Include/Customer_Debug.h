/**
 * @file     
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2024-7-15
 * @brief     使用说明
 * @brief     1.本模块通过SPI接口与"SPI_Monitor"硬件模块相连，"SPI_Monitor"将会把数据转换成模拟信号。
 * @brief     2.本模块有两种工作模式：硬件DBG模式和软件DBG模式。
 * @brief     3.硬件DBG模式：填写要发送的数据的首地址，单片机将会发送地址连续的8个字节数据。
 *            在本模式下，客户仅需要修改的以下宏定义的参数(必须是地址值)：HARD_SPIDATA
 * @brief     4.软件方式：一般选择在载波中断中填写数据 
 * 
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
    Header Definition
********************************************************************************/



#ifndef __CUSTOMER_DEBUG_H__
#define __CUSTOMER_DEBUG_H__

/**************************************************************************************************///Including Header Files
#include <FU6511.h>

/* 使用SPI调试时,SPI模式选择 */
#define DBG_SPI_3               (0)    // 三线模式，MCU的 SCLK NSS MOSI 接SPI小板的 SCLK NSS MOSI，    SPI拨码开关：SW2 = 0;SW3 = 0;    
#define DBG_SPI_2               (1)    // 双线模式，MCU的 SCLK MOSI     接SPI小板的 SCLK  MOSI，       SPI拨码开关：SW2 = 1;SW3 = 0;   
#define DBG_SPI_1               (2)    // 单线模式，MCU的 SCLK          接SPI小板的 MOSI，             SPI拨码开关：SW2 = 1;SW3 = 1;   

#define SPI_DBG_MODE                    (DBG_SPI_1)

 // 硬件DBG的参数首地址
#define HARD_SPIDATA                   TIM1__UCOP
extern uint16 xdata spidebug[4];
/*GPIO DBG模块配置--------------------------------------------------------------*/

// GP15 DBG信号配置
#define GP13_DISABLE0                   0x00                                   ///< 禁能GP13的DBG信号
#define GP13_BEMFZero                  DBGSEL0                                 ///< GP13输出方波屏蔽续流结束和检测到过零点信号
#define GP13_ADCTrigger                DBGSEL1                                 ///< GP13ADC trigger信号
#define GP13_CMPSample                 DBGSEL1 | DBGSEL0                       ///< GP13比较器采样区间信号

#define GP13_DBG_Conf0                  (GP13_DISABLE0)                          ///< GP13信号选择

 // GP03 DBG信号配置
#define GP03_DISABLE1                   0x00                                   ///<
#define GP03_CMP0                      CMPSEL0                                 ///< 输出CMP0
#define GP03_CMP1                      CMPSEL1                                 ///< 输出CMP1
#define GP03_CMP2                      CMPSEL1 | CMPSEL0                       ///< 输出CMP2
#define GP03_CMP3                      CMPSEL2                                 ////</ 输出CMP3
#define GP03_CMP4                      CMPSEL2 | CMPSEL0                       ///< 输出CMP4
#define GP03_CMP5 										 CMPSEL2 | CMPSEL1
#define GP03_CMPOX                     CMPSEL2 | CMPSEL1 | CMPSEL0             ///< 输出ADC结果比较信号(BLDC)Omega启动状态(FOC)

#define GP03_DBG_Conf1                  (GP03_CMP1)                            ///< GP07信号选择



/*DBG模块检查--------------------------------------------------------------*/
 #if (defined (SPI_DBG_HW) && defined (SPI_DBG_SW)) || (defined (UART_DBG) && (defined (SPI_DBG_SW) || defined (SPI_DBG_HW)))
     #error Only one DBG mode can be selected
 #else
     #if defined (SPI_DBG_SW)
         #pragma message("Software mode using the SPI DEBUG module")
     #elif defined (SPI_DBG_HW)
         #pragma message("Hardware mode using the SPI DEBUG module")
     #elif defined (UART_DBG)
         #pragma message("Using the UART DEBUG (ANTO protocol)")
     #endif
 #endif

#endif
