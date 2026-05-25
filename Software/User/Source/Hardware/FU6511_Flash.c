/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      FU6511_Flash.c
 * @author    Any Lin
 * @since     2022-10-01 22:24:50
 * @date      2024-07-09 09:00:15
 * @note      Last modify author is Brad Yang, R&D
 * @version
 * <table>
 * <tr><th>Date <th>Version <th>Author <th>Description </tr>
 * </table>
 * @brief
 */


/******************************************************************************/// Including Header Files
#include <stdbool.h>
#include <FU6511_Flash.h>
/******************************************************************************/// Define Macro
#define SEC_LEN_LOG  (8)
#define PAGE_LEN_LOG (3)
#define FLASH_LEN    (16384U)

#define __SEC_LAST   (unsigned char)((FLASH_LEN >> SEC_LEN_LOG) - 1)
#define __PAGE_LAST  (unsigned char)((FLASH_LEN >> (PAGE_LEN_LOG + SEC_LEN_LOG)) - 1)
/******************************************************************************/// Define Global Symbols
/******************************************************************************/// Function Subject
/**
 * @addtogroup FU6881_StdPeriphDriver_C51
 * @{
 * @defgroup Flash
 * @{
 * @defgroup Function
 * @{
 */

/**
 * @brief      Flash操作
 * 该函数为Flash操作的硬件抽象函数, 用户仅需向该函数传入编程操作命令, 编程地址, 编程内容即可, Flash编程的传入参数检查和完整操作过程都由该函数完成
 *
 * @param[in]  eCmd   操作命令
 *     @arg FLASH_PROGARM  编程
 *     @arg FLASH_SEC_ERS  扇区擦除
 *     @arg FLASH_SEC_PP   扇区预擦除
 *     @arg FLASH_PAGE_ERS 页擦除
 *     @arg FLASH_PAGE_PP  页预擦除
 * @param[in]  wAddr  编程地址
 * @param[in]  ucDat  编程内容
 *
 * @return     Flash操作结果
 *     @arg FLASH_ERR_UNKNOW 当收到该结果时, 意味着函数运行出现意外状况
 *     @arg FLASH_SUCCESS    编程成功
 *     @arg FLASH_ERR_ADDR   编程地址非法
 *     @arg FLASH_ERR_CMD    编程命令非法
 *     @arg FLASH_ERR_UNLOCK 编程解锁过程非法
 *     @arg FLASH_FORZEN     编程被冻结
 *     @arg FLASH_ERROR      编程失败, 意味着芯片出现问题
 */
ETypeFlashStatu Flash_Ctrl(ETypeFlashCmd eCmd, unsigned short wAddr, unsigned char ucDat)
{
    ETypeFlashCmd data   eCmdTmp  = eCmd;
    unsigned short data  wAddrTmp = wAddr;
    unsigned char data   ucDatTmp = ucDat;
    unsigned char data   ucKey1   = 0x5a;
    unsigned char data   ucKey2   = 0x1f;
    ETypeFlashStatu data eStatu   = FLASH_ERR_UNKNOW;
    unsigned char data   ucCfg;
    bool                 bEA;
    unsigned char data   ucIndex;

    // 编程命令检查
    if (eCmdTmp != FLASH_PROGARM && eCmdTmp != FLASH_SEC_ERS && eCmdTmp != FLASH_SEC_PP && eCmdTmp != FLASH_PAGE_ERS && eCmdTmp != FLASH_PAGE_PP) return FLASH_ERR_CMD;

    // 编程地址检查
    ucIndex = wAddrTmp >> SEC_LEN_LOG;
    if (ucIndex >= __SEC_LAST) return FLASH_ERR_ADDR;

    ucIndex >>= PAGE_LEN_LOG;
    if (ucIndex >= __PAGE_LAST) return FLASH_ERR_ADDR;

    // 编程锁冻结检查
    if (FLA_KEY == 0x06) return FLASH_FORZEN;

    // 编程前准备, 保存并关闭总中断, 看门狗
    bEA = EA;
    EA  = 0;

    ucCfg = CCFG1 & (WDT_EN | 0x08);
    CCFG1 &= ~(WDT_EN | 0x08);

    // 编程命令写入
    FLA_CR = eCmdTmp;

    // 编程锁解除
    FLA_KEY = ucKey1;
    FLA_KEY = ucKey2;

    // 编程锁解除检查
    if (FLA_KEY != 0x03)
    {
        eStatu = FLASH_ERR_UNLOCK;
        goto exit;
    }

    // 触发编程执行
    *(unsigned char xdata *)wAddrTmp = ucDatTmp;

    FLA_CR = 0x08;

    // 编程结果检查
    if ((FLA_CR & FLAERR) != 0)
    {
        eStatu = FLASH_ERROR;
        goto exit;
    }

    eStatu = FLASH_SUCCESS;

exit:
    // 退出前准备, 恢复总中断, 看门狗
    EA    = bEA;
    CCFG1 |= ucCfg;

    return eStatu;
}


/**
 * @brief      Flash页擦除
 * @note       页擦除大约需要120~150ms, 在此期间CPU将不进行任何动作
 * @param[in]  wAddr  编程地址
 *
 * @return     Flash操作结果
 *     @arg FLASH_ERR_UNKNOW 当收到该结果时, 意味着函数运行出现意外状况
 *     @arg FLASH_SUCCESS    编程成功
 *     @arg FLASH_ERR_ADDR   编程地址非法
 *     @arg FLASH_ERR_CMD    编程命令非法
 *     @arg FLASH_ERR_UNLOCK 编程解锁过程非法
 *     @arg FLASH_FORZEN     编程被冻结
 *     @arg FLASH_ERROR      编程失败, 意味着芯片出现问题
 */
ETypeFlashStatu Flash_PageErase(unsigned short wAddr)
{
    ETypeFlashStatu eStatu;

    // 进行预擦除, 错误则退出
    eStatu = Flash_Ctrl(FLASH_PAGE_PP, wAddr, 0xff);
    if (eStatu != FLASH_SUCCESS) goto exit;

    // 进行擦除
    eStatu = Flash_Ctrl(FLASH_PAGE_ERS, wAddr, 0xff);

exit:
    return eStatu;
}

/**
 * @brief      Flash扇区擦除
 * @note       扇区擦除大约需要120~150ms, 在此期间CPU将不进行任何动作
 * @param[in]  wAddr  编程地址
 *
 * @return     Flash操作结果
 *     @arg FLASH_ERR_UNKNOW 当收到该结果时, 意味着函数运行出现意外状况
 *     @arg FLASH_SUCCESS    编程成功
 *     @arg FLASH_ERR_ADDR   编程地址非法
 *     @arg FLASH_ERR_CMD    编程命令非法
 *     @arg FLASH_ERR_UNLOCK 编程解锁过程非法
 *     @arg FLASH_FORZEN     编程被冻结
 *     @arg FLASH_ERROR      编程失败, 意味着芯片出现问题
 */
ETypeFlashStatu Flash_SectorErase(unsigned short wAddr)
{
    ETypeFlashStatu eStatu;

    // 进行预擦除, 错误则退出
    eStatu = Flash_Ctrl(FLASH_SEC_PP, wAddr, 0xff);
    if (eStatu != FLASH_SUCCESS) goto exit;

    // 进行擦除
    eStatu = Flash_Ctrl(FLASH_SEC_ERS, wAddr, 0xff);

exit:
    return eStatu;
}

/**
 * @brief      Flash字节编程
 *
 * @param[in]  wAddr  编程地址
 * @param[in]  ucDat  编程内容
 *
 * @return     Flash操作结果
 *     @arg FLASH_ERR_UNKNOW 当收到该结果时, 意味着函数运行出现意外状况
 *     @arg FLASH_SUCCESS    编程成功
 *     @arg FLASH_ERR_ADDR   编程地址非法
 *     @arg FLASH_ERR_CMD    编程命令非法
 *     @arg FLASH_ERR_UNLOCK 编程解锁过程非法
 *     @arg FLASH_FORZEN     编程被冻结
 *     @arg FLASH_ERROR      编程失败, 意味着芯片出现问题
 */
ETypeFlashStatu Flash_Progarm(unsigned short wAddr, unsigned char ucDat)
{
    return Flash_Ctrl(FLASH_PROGARM, wAddr, ucDat);
}

/**
 * @}
 * @}
 * @}
 */
