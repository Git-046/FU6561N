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
#ifndef __SANITYCHECK_H_
#define __SANITYCHECK_H_

    /* 驱动电平配置检查 */
    #if (PWM_LEVEL_MODE != HIGH_LEVEL && PWM_LEVEL_MODE != LOW_LEVEL && PWM_LEVEL_MODE != UP_H_DOWN_L && PWM_LEVEL_MODE != UP_L_DOWN_H)
        #error " PWM_LEVEL_MODE err.[Set: HIGH_LEVEL,LOW_LEVEL,UP_H_DOWN_L,UP_L_DOWN_H]"
    #endif

    /* VREF参考电压配置检查 */
    #if (HW_ADC_VREF == VREF3_0 || HW_ADC_VREF == VREF4_0 || HW_ADC_VREF == VREF4_5)
        #if (VREF_OUT_EN == 0)
          #error " VREF err.[Enable P3.5 AN function]"
        #endif
    #elif (HW_ADC_VREF == VREF5_0)

    #else
        #error " VREF error.[VREF can only be VREF3_0  VREF4_0  VREF4_5  VREF5_0] "  
    #endif

    /*  运放配置检查 */
    #if (HW_AMP_MODE == AMP_PGA_DUAL)
        #if (HW_AMPGAIN != AMP2x && HW_AMPGAIN != AMP4x && HW_AMPGAIN != AMP8x && HW_AMPGAIN != AMP16x)
            #error " Amp gain error.[Internal PGA gain can only be AMP2x  AMP4x  AMP8x  AMP16x] "  
        #endif
     #elif (HW_AMP_MODE == AMP_NOMAL || HW_AMP_MODE==AMP_PGA_SINGEL)
    #else
        #error "Amp mode error.[Set: AMP_NOMAL or AMP_PGA_DUAL]"
    #endif
    
 
    /*  预定位测试模式设置检查 */
    #if (ALIGN_MODE != ALIGN_DSIABLE && ALIGN_MODE != ALIGN_NOMAL && ALIGN_MODE !=ALIGN_TEST)
        #error " AlignTestMode error.[Set: Disable / Enable ] "  
    #endif
 
#endif