/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      Key_Driver.c
 * @author    Fortiortech  Appliction Team
 * @since     Create:2021-05-14
 * @date      Last modify:2022-07-14
 * @brief     This file contains PID function used for Motor Control
 */
/* Includes -------------------------------------------------------------------------------------*/
#include <Myproject.h>

KeyScanParam_TypeDef KS;
/**
 * @brief    按键调速
 */
void KeyScan(void)
{
    if (Key0 == 0)
    {
        KS.Key0InitCnt = 0;
        
        if (KS.Key0PressCnt < 50)
        {
            KS.Key0PressCnt++;
        }
        else
        {
            KS.Key0PressCnt = 50;
            KS.KeyDownFlag = 1;
        }
    }
    else
    {    
        if (KS.Key0PressCnt > 0)
        {
            KS.Key0PressCnt--;
        }
        else
        {
            KS.Key0PressCnt = 0;
            if(KS.KeyDownFlag == 1)
            {
                KS.KeyDownFlag = 0;
                KS.FlagKey0LastPress = KS.FlagKey0Press;
                KS.FlagKey0Press += 1;
                if(KS.FlagKey0Press >= 5)
                {
                    KS.FlagKey0Press = 0;
                }
            }
        }
    }
    
    if (ONOFFTest.ONOFF_Flag == 1)
    {
        
        switch (KS.FlagKey0Press)
        {
            case 0:
                #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = SPD_GEAR1;
                }
                #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = POW_GEAR1;
                    if(KS.FlagKey0LastPress == 4 && MotorSpeed.mcSpeed_Cnt > 0)
                    {
                        MotorSpeed.mcSpeed_Cnt --;
                        MotorSpeed.mcSpeed_Acc = 1;
                    }
                    else
                    {
                        MotorSpeed.mcSpeed_Cnt = 0;
                        MotorSpeed.mcSpeed_Acc = 0;
                    }
                }
                #endif
            break;
            case 1:
                #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = SPD_GEAR2;
                }
                #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = POW_GEAR2;
                }
                #endif
            break;
            case 2:
                #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = SPD_GEAR3;
                }
                #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = POW_GEAR3;
                }
                #endif
            break;
            case 3:
                #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = SPD_GEAR4;
                }
                #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = POW_GEAR4;
                }
                #endif
            break;
            case 4:
                #if (MOTOR_CTRL_MODE == SPEED_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = SPD_GEAR5;
                }
                #elif (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
                {
                    McCtrl.TargetValue = POW_GEAR5;
                    MotorSpeed.mcSpeed_Cnt = 100;
                }
                #endif
            break;
        }
    }
}
