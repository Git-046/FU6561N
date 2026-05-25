/**
 * @file
 * @version  V1.0.0
 * @author   Leo.Li
 * @date     2024-7-15
 * @brief    ??????...
 *
 * @copyright Copyright (C) 2011-2022 Fortior Technology Co., Ltd. All rights reserved.

/********************************************************************************
	Header Definition
********************************************************************************/
#include <MyProject.h>

void AdaLeadAngleSamp(void)
{
	/*自动超前角，首先判断能够采多少个载波的反电势*/
	SquStart.Isample = THETA;
	switch (SquStart.Sector)
	{
	case 0:
		if (SquStart.Isample < -5000)
		{
			SquStart.Sector = 1;
			SquStart.IinterCount1 = 0;
		}
		break;
	case 1:

		if (SquStart.IinterCount1 == 0)
		{
			SquStart.IsampleSum2 = THETA;
			SquStart.IinterCount1 = 1;
		}
		else if (SquStart.IinterCount1 == 1)
		{
			SquStart.IsampleSum1 = THETA;
			SquStart.IsampleSum1 = (SquStart.IsampleSum1 - SquStart.IsampleSum2) << 2;
			SquStart.Sector = 2;
			SquStart.IinterCount1 = 0;
		}

		break;
	case 2:
		// if (SquStart.Isample > (WIN_THETA - (SquStart.IsampleSum1 << 3))) 
		if(MotorSpeed.FR == CW)
		{
			if (SquStart.Isample > (WIN_THETA_CW - 5000))  /*提前至少一个周期切换通道，可以考虑放到开窗中断里面*/
			{
				ADC_MASK = 0;
				SetBit(ADC_MASK, CH5EN);
				McCtrl.SampFlag = 0;
				SquStart.Sector = 3;
			}
		}
		else
		{
			if ((SquStart.Isample < (WIN_THETA_CCW + 5000)) && (SquStart.Isample > 0)) 
			{
				ADC_MASK = 0;
				SetBit(ADC_MASK, CH5EN);
				McCtrl.SampFlag = 0;
				SquStart.Sector = 3;
			}			
		}

		break;
	case 3:
		if(MotorSpeed.FR == CW)
		{
			if ((SquStart.Isample > (WIN_THETA_CW))) /*开窗位置角*/
			{
				// GP03 = 1;
				SetBit(ADC_CR, ADCBSY);
				while (ReadBit(ADC_CR, ADCBSY))
					;
				// GP03 = 0;
				SquStart.IsampleRecord = ADC5_DR;
				SquStart.IinterCount2 = 0;
				SquStart.Sector = 4;
			}
		}
		else
		{
			if ((SquStart.Isample < WIN_THETA_CCW) && (SquStart.Isample > 0))  /*开窗位置角*/
			{
				SetBit(ADC_CR, ADCBSY);
				while (ReadBit(ADC_CR, ADCBSY))
					;
				SquStart.IsampleRecord = ADC5_DR;
				SquStart.IinterCount2 = 0;
				SquStart.Sector = 4;
			}			
		}

		break;
	case 4:
		if (McCtrl.WindowFlag) /*判断能否采到第2/3个反电势*/
		{
			// GP03 = 1;
			SetBit(ADC_CR, ADCBSY);
			while (ReadBit(ADC_CR, ADCBSY))
				;
			// GP03 = 0;
			if (SquStart.IinterCount2 == 0)
			{
				SquStart.IinterCount2 = 1;
				SquStart.IsampleSumMin = ADC5_DR;
			}
			else if (SquStart.IinterCount2 == 1)
			{
				SquStart.IinterCount2 = 2;
				SquStart.IsampleSumMax = ADC5_DR;
				McCtrl.WindowFlag = 0;
			}
		}
		/*当窗口关闭后进行处理*/
		else
		{
			SquStart.Sector = 0;
		}

		break;
	default:
		break;
	}
}

/*自动提前角快速调节*/
void AdaLeadAngleFast(void)
{
	/*重点-当此处采样到续流，值很小时说明相位超前过小*/
	if (McCtrl.AdaMode) /*其他相干扰较大，此处需要根据on/off采样切换阈值*/
	{
		if (SquStart.IsampleRecord < BEMF_Samp_THR_L) /*这个值需要实测看多少*/
		{
			McCtrl.AdaLowFlag = 0;
			McCtrl.AdaHighFlag++;
			if (McCtrl.AdaHighFlag >= 1)
			{
				McCtrl.AdaHighFlag = 1;
				if ((MotorSpeed.mcSpeed < AdaAngle_THR_Speed_UH)) /*堵孔时需要把增益调低，否则容易抖震*/
				{
					McCtrl.mcLeadAngleActComp += Ada_S_Gain;
				}
				else
				{
					McCtrl.mcLeadAngleActComp += AdaStuck_Gain;
				}
				// McCtrl.mcAdapLeadAngleEnable = 1;
			}
		}
		else if (SquStart.IsampleRecord > McCtrl.BEMF_THR_H) /*这个值需要实测看多少*/
		{
			McCtrl.AdaLowFlag++;
			if (McCtrl.AdaLowFlag >= 1)
			{
				McCtrl.AdaHighFlag = 0;
				McCtrl.AdaLowFlag = 1;
				if ((MotorSpeed.mcSpeed < AdaAngle_THR_Speed_UH)) /*堵孔时需要把增益调低，否则容易抖震*/
				{
					McCtrl.mcLeadAngleActComp -= Ada_S_Gain;
				}
				else
				{
					McCtrl.mcLeadAngleActComp -= AdaStuck_Gain;
				}
				// McCtrl.mcAdapLeadAngleEnable = 1;
			}

			/*当此处采样到斩波，值很大时说明相位超前太大*/
		}
		else
		{
			McCtrl.AdaHighFlag = 0;
			McCtrl.AdaLowFlag = 0;
		}
	}
	else
	{
		if (SquStart.IsampleRecord < BEMF_Samp_THR_LL) /*这个值需要实测看多少*/
		{
			McCtrl.AdaLowFlag = 0;
			McCtrl.AdaHighFlag++;
			if (McCtrl.AdaHighFlag >= 2)
			{
				McCtrl.AdaHighFlag = 2;
				McCtrl.mcLeadAngleActComp += Ada_S_Gain;
				// McCtrl.mcAdapLeadAngleEnable = 1;
			}
		}

		else if (SquStart.IsampleRecord > McCtrl.BEMF_THR_H) /*这个值需要实测看多少*/
		{
			McCtrl.AdaLowFlag++;
			if (McCtrl.AdaLowFlag >= 2)
			{
				McCtrl.AdaHighFlag = 0;
				McCtrl.AdaLowFlag = 2;
				McCtrl.mcLeadAngleActComp -= Ada_S_Gain;
				// McCtrl.mcAdapLeadAngleEnable = 1;
			}

			/*当此处采样到斩波，值很大时说明相位超前太大*/
		}
		else
		{
			McCtrl.AdaHighFlag = 0;
			McCtrl.AdaLowFlag = 0;
		}
	}
}

/*自动提前角慢速调节*/
void AdaLeadAngleSlow(void)
{
	if (SquStart.IinterCount2 == 0) /*采不到第2个点*/
	{
		McCtrl.LowPWMCnt++;
		if (McCtrl.LowPWMCnt > 10) /*连续采样到10个正常载波，启动低载波模式*/
		{
			McCtrl.LowPWMCnt = 10;
			McCtrl.LowPWMMode = 1;
		}
	}
	else if (SquStart.IinterCount2 != 0 && (1 == McCtrl.AdaMode)) /*采不到第3个点-ON模式*/
	{
		McCtrl.LowPWMCnt--;
		if (McCtrl.LowPWMCnt < 0)
		{
			McCtrl.LowPWMCnt = 0;
			McCtrl.LowPWMMode = 0;
		}
		if (McCtrl.mcAdapLeadAngleEnable == 1)
		{
			SquStart.StartCurrentValue = SquStart.IsampleRecord - SquStart.IsampleSumMin;
			if ((SquStart.StartCurrentValue < 5000) && (SquStart.StartCurrentValue > 0)) /*否则一个采到on一个采样到off*/
			{
				McCtrl.sq_speedU32.s32 = (((int32)MotorSpeed.mcSpeed * SquStart.IsampleSum1) >> 14) * McCtrl.K_Speed_BEMF;
				SquStart.StartCurrentHoldValue = McCtrl.sq_speedU32.s16[0];

				if (SquStart.StartCurrentValue < (SquStart.StartCurrentHoldValue - Ada_H_DeadZone)) /*修改这个死区可能需要同时修改增益*/
				{
					McCtrl.mcLeadAngleActComp += Ada_H_Gain;
				}
				else if (SquStart.StartCurrentValue > (SquStart.StartCurrentHoldValue + Ada_H_DeadZone))
				{
					McCtrl.mcLeadAngleActComp -= Ada_H_Gain;
				}
			}
		}
#if (Special_Detected)
		if ((SquStart.StartCurrentValue < 0) && (SquStart.StartCurrentValue > -3000)) /*特殊情况-第一个波不完整的时候*/
		{
			McCtrl.mcLeadAngleActComp += Ada_H_Gain;
		}
#endif
	}
	else if (SquStart.IinterCount2 == 2 && (0 == McCtrl.AdaMode)) /*采样到3个点*/
	{
		McCtrl.LowPWMCnt--;
		/*当转速过高时，认为第三个点容易采样到错误位置，忽略*/
		SquStart.StartCurrentValue = SquStart.IsampleRecord - SquStart.IsampleSumMin;
		SquStart.StartCurrentHoldValue = SquStart.IsampleSumMin - SquStart.IsampleSumMax;
		if (SquStart.StartCurrentValue > 0 && SquStart.StartCurrentValue < 3000 && SquStart.StartCurrentHoldValue > 0 && SquStart.StartCurrentHoldValue < 3000)
		{
			if (McCtrl.mcAdapLeadAngleEnable == 1)
			{
				/*下面这个增益需要修改，与电机的参数和转速有关&右移1*/
				McCtrl.sq_speedU32.s32 = (int32)(SquStart.StartCurrentHoldValue << 1) * AdaAngle_K_L;
				SquStart.StartCurrentHoldValue = McCtrl.sq_speedU32.s16[0];
				if (SquStart.StartCurrentValue < SquStart.StartCurrentHoldValue - Ada_L_DeadZone)
				{
					McCtrl.mcLeadAngleActComp += Ada_L_Gain;
				}
				else if (SquStart.StartCurrentValue > SquStart.StartCurrentHoldValue + Ada_L_DeadZone)
				{
					McCtrl.mcLeadAngleActComp -= Ada_L_Gain;
				}
			}
		}
	}
	else
	{
		/*无操作*/
	}
	if (McCtrl.mcLeadAngleActComp > LEAD_ANGLE_MAX)
	{
		McCtrl.mcLeadAngleActComp = LEAD_ANGLE_MAX;
	}
	else if (McCtrl.mcLeadAngleActComp < LEAD_ANGLE_MIN)
	{
		McCtrl.mcLeadAngleActComp = LEAD_ANGLE_MIN;
	}
}


void AdaLeadAngleApply(void)
{
	uint16 LeadAngleActual = 0;
	/*自动超前角*/
		LeadAngleActual = McCtrl.mcLeadAngle;
		if(McCtrl.mcLeadAngleActComp > 10)
		{
			McCtrl.mcLeadAngleIntComp += 2;
			McCtrl.mcLeadAngleActComp = 0;
		}
		else if(McCtrl.mcLeadAngleActComp < -10)
		{
			McCtrl.mcLeadAngleIntComp -= 2;
			McCtrl.mcLeadAngleActComp = 0;
		}
		/*给补偿的超前角限幅*/
		if(McCtrl.mcLeadAngleIntComp > LEAD_ANGLE_MAX_H)
		{
			McCtrl.mcLeadAngleIntComp = LEAD_ANGLE_MAX_H;
		}
		else if(McCtrl.mcLeadAngleIntComp < LEAD_ANGLE_MIN_H)
		{
			McCtrl.mcLeadAngleIntComp = LEAD_ANGLE_MIN_H;
		}
		if(McCtrl.mcLeadAngleCount > STARTUP_ANGLE_DIS_TIME)
		{
			LEAD_ANGLE = LeadAngleActual + McCtrl.mcLeadAngleIntComp;
		}
		else
		{
			McCtrl.mcLeadAngleIntComp = 0;
			LEAD_ANGLE = LeadAngleActual;
		}
		LeadAngleActual = LEAD_ANGLE;
		McCtrl.RealLeadAngle = LeadAngleActual << 6; /*测试信号*/
}

/*低载波比模式，极大影响三相对称性，不建议实际运行时使用，仅作为超前角扫描*/
/*要求U相开2个窗口，开窗角度分别为0°和180°*/
void AdaLeadAngleLowPWM(void)
{
	/*自动超前角，首先判断能够采多少个载波的反电势*/
	SetReg(TIM1_CR6, WINMD1 | WINMD0, WINMD1); //设置双窗口
	/*低载波比（假设为12）每个载波大概为30° -- 2730*/
	SquStart.Isample = THETA;
	switch (SquStart.Sector)
	{
		case 0:
		{
			if (SquStart.Isample < 0 && SquStart.Isample > _Q15(-40.0 / 180.0))
			{
					GP01 = 1;
					SetBit(ADC_CR, ADCBSY);
					while (ReadBit(ADC_CR, ADCBSY))
						;
					GP01 = 0;
				SquStart.IsampleSumMin = ADC5_DR;
				ADC_Channel_Recover();
				SquStart.Sector = 1;
			}
			break;
		}
		case 1:
		{
			if ((SquStart.Isample > (WIN_THETA_CW))) /*开窗位置角*/
			{
				GP01 = 1;
				SetBit(ADC_CR, ADCBSY);
				while (ReadBit(ADC_CR, ADCBSY))
					;
				GP01 = 0;
				SquStart.IsampleRecord = ADC5_DR;
				SquStart.IinterCount2 = 0;
				SquStart.Sector = 4;
			}
			break;			
		}
		case 4:
		{
			if (McCtrl.WindowFlag) /*判断能否采到第2个反电势*/
			{
				GP01 = 1;
				SetBit(ADC_CR, ADCBSY);
				while (ReadBit(ADC_CR, ADCBSY))
					;
				GP01 = 0;
				SquStart.IsampleSumMax = ADC5_DR;
				McCtrl.WindowFlag = 0; /*结束开窗*/
				McCtrl.LowPWMCnt --;
				if(McCtrl.LowPWMCnt < 1)
				{
					McCtrl.LowPWMCnt = 1;
					McCtrl.LowPWMMode = 0;
				}
			}		
			/*当窗口关闭后进行处理*/
			else
			{
				/*有时候TIM1中断打断上面的第二个采样*/
				if(SquStart.IsampleSumMax < BEMF_Samp_THR_L)
				{
					McCtrl.LowPWMCnt ++;
					if(McCtrl.LowPWMCnt > 10)
						McCtrl.LowPWMCnt = 10;
				}
				SquStart.StartCurrentValue = SquStart.IsampleSumMin; //LEFT
				SquStart.StartCurrentHoldValue = SquStart.IsampleRecord; //RIGHT
				if(SquStart.IsampleSumMin < BEMF_Samp_THR_L)
				{
					McCtrl.mcLeadAngleActComp -= Ada_S_Gain;
				}
				else
				{
					if (SquStart.StartCurrentValue < SquStart.StartCurrentHoldValue - Ada_H_DeadZone)
					{
						McCtrl.mcLeadAngleActComp += Ada_H_Gain;
					}
					else if (SquStart.StartCurrentValue > SquStart.StartCurrentHoldValue + Ada_H_DeadZone)
					{
						McCtrl.mcLeadAngleActComp -= Ada_H_Gain;
					}
					if (McCtrl.mcLeadAngleActComp > LEAD_ANGLE_MAX)
					{
						McCtrl.mcLeadAngleActComp = LEAD_ANGLE_MAX;
					}
					else if (McCtrl.mcLeadAngleActComp < LEAD_ANGLE_MIN)
					{
						McCtrl.mcLeadAngleActComp = LEAD_ANGLE_MIN;
					}
				}
				ADC_Channel_Recover();
				McCtrl.WindowFlag = 0;
				SquStart.Sector = 0;				
			}	
			break;
		}
	}
}