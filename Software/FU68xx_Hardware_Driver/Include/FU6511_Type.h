/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      FU6511_Type.h
 * @author    Bruce, R&D
 * @since     2021-11-24 00:00:00
 * @date      2024-07-09 10:04:43
 * @note      Last modify author is Brad Yang, R&D
 * @brief
 */

#ifndef __FU6511_TYPE_H__
#define __FU6511_TYPE_H__

// Include external header file.
#include <ctype.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif


#define _I                              volatile const //< Defines 'read only' permissions
#define _O                              volatile       //< Defines 'write only' permissions
#define _IO                             volatile       //< Defines 'read&write' permissions

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef long int32;
typedef short int16;
typedef char int8;

typedef enum { DISABLE = 0, ENABLE } ebool;

#ifdef __cplusplus
}
#endif

#endif  //__FU6881_TYPE_H__


/*** (C) Copyright 2022 Fortior Technology Co., Ltd. ***/
