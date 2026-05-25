/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      xxx.c
 * @author    Fortiortech  Appliction Team
 * @since     Create:2022-08-05
 * @date      Last modify:2022-08-05
 * @note      Last modify author is Leo.li
 * @brief
 */
 
#include <MyProject.h>

MCUART xdata Uart;

/** 
 * @brief        Uart初始化函数
 * @param        None
 * @return       none
 * @author       Marcel
 * @date         2022-07-13
 * @version      1.0   
 * @property     Public
*/


void UART_Init(void)
{
		SetBit(CK_CR, UARTCKEN); //使能串口时钟
    SetBit(PH_SEL , UARTEN);   //P0[1]as UART_RXD; P0[0]as UART_TXD
		SetBit(PH_SEL1 , UARTCH);  //功能转移

    ClrBit(P0_OE , P01);        //输入使能
    SetBit(P0_PU , P01);        //上拉电阻
    SetBit(P0_OE , P00);        //输出使能            
    SetBit(P0_PU , P00);        //上拉电阻 	

    UT_MOD1 = 0;	//00-->单线制8bit		01-->8bit uart(波特率可设置)
    UT_MOD0 = 1;	//10-->单线制9bit	    11-->9bit uart(波特率可设置)
    SM2 = 0;        //0-->单机通讯			1-->多机通讯；
    REN = 1;        //0-->不允许串行输入	1-->允许串行输入，软件清0;
    TB8 = 0;	    //模式2/3下数据发送第9位，在多机通信中，可用于判断当前数据帧的数据是地址还是数据，TB8=0为数据，TB8=1为地址
    RB8 = 0;	    //模式2/3下数据接收第9位，若SM2=0,作为停止位

    ClrBit(IP3 , PUART11);	//中断优先级时最低
    ClrBit(IP3 , PUART10);	//中断优先级时最低    

		UT_BAUD = 0x009B;//波特率可设置 = 24000000/(16/(1+ UT_BAUD[BAUD_SEL]))/(UT_BAUD+1)
                     //9B-->9600 0x000c-->115200
    ClrBit(UT_BAUD , BAUD_SEL);  //倍频使能0-->Disable  1-->Enable
    ES0 = 1;                        //UART中断使能0-->Disable  1-->Enable
    
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : UartRxClean
    Description    : 通信接收重新初始化
    Date           : 2022-03-12
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void UartRxClean(void)
{
    memset(Uart.R_DATA, 0, UART_RX_BUF_SIZE);   //通讯数组清零
    Uart.RTXDataCnt = 0;
    Uart.RecevingFlag = 0;
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : UART_SendData
    Description    : UART发送处理
    Date           : 2022-03-12
    Parameter      : T_Data: [输入] 
------------------------------------------------------------------------------------------------- */
void UART_SendData(uint8 T_Data)
{
    UT_DR = T_Data;
}


/* -------------------------------------------------------------------------------------------------
    Function Name  : CommByteChecksum
    Description    : 计算单字节校验和
    Date           : 2022-03-12
    Parameter      : Buffer: [输入] 
**			 ucNum: [输入] 
------------------------------------------------------------------------------------------------- */
uint8 CommByteChecksum(uint8 * Buffer, uint8 ucNum)
{
    uint8 ucCheckSum;
    uint8 i;
    ucCheckSum = 0;
    
    for (i = 0; i < ucNum; i++)
    {
        ucCheckSum += Buffer[i];
    }
    return (ucCheckSum);
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : UartTxResponse
    Description    : 处理串口数据反馈
    Date           : 2022-03-12
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void UartTxResponse(void)
{
    memset(Uart.T_DATA, 0, UART_TX_BUF_SIZE);                           //通讯数组清零
    Uart.UATxDelayFlag = 0;
    switch(Uart.R_DATA[1])
    {
        case 0xB2:
        {
            Uart.T_DATA[0] = 0XAA;                                      //帧头
            Uart.T_DATA[1] = 0XB2;                                      //命令类型
            Uart.T_DATA[2] = 8;                                         //数据长度
            
            Uart.T_DATA[3] = MotorSpeed.mcSpeed;                //转速低8位
            Uart.T_DATA[4]   = (MotorSpeed.mcSpeed >> 8);       //转速高8位
						Uart.T_DATA[5] = VERSIONNUM;                                //通讯协议版本号

            Uart.T_DATA[6] = (uint8)(mcFaultSource);                    //报警故障代码
            Uart.T_DATA[7] = CommByteChecksum(&Uart.T_DATA[0], 8);			//校验位
            Uart.UsaTxlen = 8;                                          // 发送总字节数
					/*
					  Uart.R_DATA[0] = 0XAA;                                      //帧头
            Uart.R_DATA[1] = 0XB2;                                      //命令类型
            Uart.R_DATA[2] = 8;                                         //数据长度
            Uart.R_DATA[3] =  //获取目标转速低8位

            Uart.R_DATA[4] = //获取目标转速高8位
            Uart.R_DATA[5] = 0;       //保留
            Uart.R_DATA[6] = 0;       //保留
            Uart.R_DATA[7] = CommByteChecksum(&Uart.R_DATA[0], 8);			//校验位
            Uart.UsaRxlen = 8;                                          // 发送总字节数
						*/
        }
        break;
        default:
        break;
    }
    TI = 0;
    ES0 = 1;
    Uart.RTXDataCnt = 0;
    UART_SendData(Uart.T_DATA[0]);
}


/* -------------------------------------------------------------------------------------------------
    Function Name  : UartDealComm
    Description    : 串口处理
    Date           : 2022-03-12
    Parameter      : None
------------------------------------------------------------------------------------------------- */
/*
主控发送示例：
开机（20000RPM）          aa b2 08 20 4E 00 00 D2
回复                      aa b2 08 00 00 00 00 64
*/
void UartDealComm(void)
{
	static int16 Target;
	int16 Target_pu;
    if (Uart.RecevingFlag)
    {
        if (Uart.RTXDataCnt)
        {
            if (Uart.RXTimeOutCnt >= 30)         //UART接收一帧数据过程中持续30ms未进接收中断则重新初始化UART
            {
                Uart.RXTimeOutCnt = 0;
                UART_Init();
            }
        }
        else
        {
            Uart.RXTimeOutCnt = 0;
        }
    }
    else
    {
        Uart.RXTimeOutCnt = 0;
    }
    
    if (Uart.ResponceFlag == 1)
    {
        if (Uart.R_DATA[Uart.UsaRxlen - 1] == CommByteChecksum(&Uart.R_DATA[0], Uart.UsaRxlen - 1))
        {
            Uart.UARTErrCnt = 0;
            switch(Uart.R_DATA[1])                                      //根据指令处理
            {
                case 0xB2:
                {
                    Target = (int16)(Uart.R_DATA[4] << 8) & 0xFF00; //获取目标转速高8位
                    Target |= Uart.R_DATA[3];                       //获取目标转速低8位
                }
                break;
                
                default:
                    break;
            }
#if (MOTOR_CTRL_MODE == POWER_LOOP_CONTROL)
						Target_pu = P_Value(Target);
						if(Target_pu > MC_POWER_MIN)
						{
						    isCtrlPowOn  = true;
                Uart.UartTarget = Target_pu;
								if(Uart.UartTarget > MC_POWER_MAX)
								{
									Uart.UartTarget = MC_POWER_MAX;
								}
            }
#else
						Target_pu = S_Value(Target * 10);
						if(Uart.UartTarget > MC_SPEED_MIN_RPM)
						{
								isCtrlPowOn  = true;
                Uart.UartTarget = Target_pu;
								if(Uart.UartTarget > MC_SPEED_MAX_RPM)
								{
									Uart.UartTarget = MC_SPEED_MAX_RPM;
								}
            }						
#endif
						else
						{
							Uart.UartTarget = 0;
							isCtrlPowOn = false;
						}
        }
        else
        {
            UART_Init();
        }
        
        Uart.UATxDelayFlag = 1;
        Uart.UATxDelayCnt = UARTDelayTimeMS;
        Uart.ResponceFlag = 0;
        Uart.RecevingFlag = 0;
    }   
    
    if ((Uart.UATxDelayFlag) && (!Uart.UATxDelayCnt))                   //接收一帧数据后延时UARTDelayTimeMS反馈
    {
        UartTxResponse();
    }
}
