/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      FU6511_Flash.h
 * @author    Any Lin
 * @since     2022-09-30 14:05:00
 * @date      2024-07-09 09:00:06
 * @note      Last modify author is Brad Yang, R&D
 * @version
 * <table>
 * <tr><th>Date <th>Version <th>Author <th>Description </tr>
 * </table>
 * @brief
 */

#ifndef __FU6511_FLASH_H__
#define __FU6511_FLASH_H__

/******************************************************************************/// Including Header Files
#include "FU6511_MCU.h"
/******************************************************************************/// Define Macro
/******************************************************************************/// Define Type
/**
 * @addtogroup FU6881_StdPeriphDriver_C51
 * @{
 * @defgroup Flash
 * @{
 * @defgroup TypeEnum
 * @{
 */

/**
 * @enum 编程命令
 * @brief 列举出Flash的可用模式及命令构成
 */
typedef enum
{
    FLASH_PROGARM  = FLAEN,                    /**< 编程 */
    FLASH_SEC_ERS  = FLAEN | FLAERS,           /**< 扇区擦除 */
    FLASH_SEC_PP   = FLAEN | FLAPRE,           /**< 扇区预擦除 */
    FLASH_PAGE_ERS = FLAEN | FLAERS | PAGE_EN, /**< 页擦除 */
    FLASH_PAGE_PP  = FLAEN | FLAPRE | PAGE_EN, /**< 页预擦除 */
} ETypeFlashCmd;

/**
 * @enum 编程结果
 * @brief 列举出Flash操作过程中会出现的各种错误
 */
typedef enum
{
    FLASH_ERR_UNKNOW, /**< 初始化 */
    FLASH_SUCCESS,    /**< 编程成功 */
    FLASH_ERR_ADDR,   /**< 由于尝试编程非法地址导致的编程错误 */
    FLASH_ERR_CMD,    /**< 由于编程命令错误导致的编程错误 */
    FLASH_ERR_UNLOCK, /**< 由于编程解锁过程中出现的错误 */
    FLASH_FORZEN,     /**< 编程锁被冻结 */
    FLASH_ERROR       /**< 编程失败 */
} ETypeFlashStatu;

/**
 * @}
 * @}
 * @}
 */
/******************************************************************************/// External Symbols
/******************************************************************************/// External Function
extern ETypeFlashStatu Flash_Ctrl(ETypeFlashCmd eCmd, unsigned short wAddr, unsigned char ucDat);
extern ETypeFlashStatu Flash_PageErase(unsigned short wAddr);
extern ETypeFlashStatu Flash_SectorErase(unsigned short wAddr);


#endif
