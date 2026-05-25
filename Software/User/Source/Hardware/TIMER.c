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
#include <MyProject.h>

void Timer1_Init(void)
{
    /*  -------------------------------------------------------------------------------------------------
    TIM1_CR0
    [7]    T1RWEN    对TIM1_CR0写0x81使能TIM1_CR0[T1RCEN]，写0x80不使能TIM1_CR0[T1RCEN]
    [6:5]  T1CFLT    60度基准时间滤波选择  00-1个BCCR  01-2个BCCR 10-4个BCCR  11-8个BCCR
    [4]    T1FORC    60度强制换相使能      0-->Disable  1-->Enable
    [3:2]  T1OPS     换相触发信号选择      00-软件换相  01-重载定时器上溢触发 10-位置检测中断触发  11-RSV
    [1]    T1BCEN    基本计数器的计数器使能
    [0]    T1RCEN    对TIM1_CR0写0x81使能TIM1_CR0[T1RCEN]，写0x80不使能TIM1_CR0[T1RCEN]
    -------------------------------------------------------------------------------------------------*/
    ClrBit(TIM1_CR0, T1CFLT1);
    ClrBit(TIM1_CR0, T1CFLT0);
    ClrBit(TIM1_CR0, T1FORC);
    ClrBit(TIM1_CR0, T1OPS1);
    SetBit(TIM1_CR0, T1OPS0);
    /*  -------------------------------------------------------------------------------------------------
    TIM1_CR1
    [7]    T1BAPE    TIM1__BARR自动装载使能
    [6:5]  BSEL      屏蔽续流角度选择
    -------------------------------------------------------------------------------------------------*/
    TIM1_CR1 = 0x08;
    /*  -------------------------------------------------------------------------------------------------
    TIM1_CR2
    [7]    T1BRS     基本计数器复位源选择
    [6:5]  CSEL      换相角度选择
    -------------------------------------------------------------------------------------------------*/
    TIM1_CR2 = 0x01;
    /*-------------------------------------------------------------------------------------------------
    Tim1_CR3
    [7]    BLDC150EN 0:120°方波 1：150°方波
    [6:4]  T1PSC    定时器时钟分频选择
    [3:2]  T1TIS    输入源（TI0/TI1/TI2）选择
                     00：GPIO作为输入，根据CMP_CR1[7]选择（P1.4/P1.6/P2.1）还是（P0.2/P3.7/P3.6），CMP_SR的结果通过GPIO产生
                     01：比较器（CMP0/CMP1/CMP2）的输出作为输入，CMP_SR的结果通过CMP产生
                     10：ADC
                     11：比较器（CMP0/CMP1/CMP2）&ADC 比较器优先级高
    [1:0]  T1INM    输入TI0/TI1/TI2噪声脉宽选择
                     CMP_CR4[FAEN] = 0  00: 不滤波           01：8个时钟周期滤波   10：32个时钟周期滤波  11：64个时钟周期滤波
                     CMP_CR4[FAEN] = 1  00:32个时钟周期滤波  01：64个时钟周期滤波  10：96个时钟周期滤波  11：128个时钟周期滤波
    -------------------------------------------------------------------------------------------------*/
    SetBit(TIM1_CR3, T1PSC2); //  000-->24M     001-->12M       010-->6M    011-->3M
    SetBit(TIM1_CR3, T1PSC1); //  100-->1.5M   101-->750K      110-->375K    111-->187.5K
    ClrBit(TIM1_CR3, T1PSC0);
    // 输入来源为CMP 和 ADC
    ClrBit(TIM1_CR3, T1TIS1);
    SetBit(TIM1_CR3, T1TIS0);							//选择比较器信号作为位置检测信号
    // TI2噪声脉宽选择
    ClrBit(TIM1_CR3, T1INM1);
    SetBit(TIM1_CR3, T1INM0);
    ClrBit(TIM1_CR3, BLDC150EN);
    /*-------------------------------------------------------------------------------------------------
    TIM1_CR5
    [7]      T1POP       Driver计数器上溢事件触发数据传输使能  0-->Disable  1-->Enable
    [6]      T1WTS       PWM OFF时候换相使能，消除窄脉冲       0-->Disable  1-->Enable
    [5:4]    RSV
    [3]      ITRIP_DIS   方波忽略ITRIP电流采样使能             0-->Disable  1-->Enable
    [2]      UCOP_DIS    方波忽略导通相电压采样使能            0-->Disable  1-->Enable
    [1:0]    T1AFL       ADC 采样电压计算结果滤波次数          00-1  01-2 10-4  11-8
    
    -------------------------------------------------------------------------------------------------*/
    TIM1_CR5 = 0x00;
    /*-------------------------------------------------------------------------------------------------
    TIM1_IER            TIMER1中断控制
    [7]      T1UPD （当OPS=00时，软件对UPD写一触发数据传输）
    [6]      保留
    [5]      保留
    [4]      T1BOIE    0：禁止基本定时器上溢中断  1：使能基本定时器上溢中断
    [3]      T1ROIE    0: 禁止重载定时器上溢中断  1: 使能重载定时器上溢中断
    [2]      T1WTIE    0: 禁止写入时序中断        1: 使能写入时序中断
    [1]      T1PDIE    0: 禁止位置检测中断        1: 使能位置检测中断          (过零点检查)
    [0]      T1BDIE    0: 禁止屏蔽续流结束中断    1: 使能屏蔽续流结束中断
    -------------------------------------------------------------------------------------------------*/
    TIM1_IER = T1PDIE;
    SetReg(IP1, PTIM11 | PTIM10, PTIM11 | PTIM10);
    /*-------------------------------------------------------------------------------------------------
    TIM1_SR            TIMER1中断标志位
    [7]      保留
    [6]      保留
    [5]      T1ADIF    0: ADC中断标志位
    [4]      T1BOIE    0：基本定时器上溢中断
    [3]      T1ROIE    0: 重载定时器上溢中断标记
    [2]      T1WTIF    0: 写入时序中断标记
    [1]      T1PDIF    0: 位置检测中断标记          (过零点检查)
    [0]      T1BDIF    0: 屏蔽续流结束中断标记
    -------------------------------------------------------------------------------------------------*/
    INT_SR0 = 0x00;
    TIM1_DBR1 = ((     0 |      0 |      0 |      0 ) << 4) | (     0 | T1ULP1 |      0 |      0 );
    TIM1_DBR2 = ((     0 | T1ULP1 | T1UHE1 | T1ULE1 ) << 4) | (     0 | T1ULP1 | T1UHE1 | T1ULE1 );
    TIM1_DBR3 = ((     0 | T1ULP1 | T1UHE1 | T1ULE1 ) << 4) | (     0 | T1ULP1 | T1UHE1 | T1ULE1 );
    TIM1_DBR4 = ((     0 |      0 |      0 |      0 ) << 4) | (     0 | T1ULP1 | T1UHE1 | T1ULE1 );
    TIM1_DBR5 = ((     0 | T1ULP1 |      0 |      0 ) << 4) | (     0 | T1ULP1 |      0 |      0 );
    TIM1_DBR6 = ((     0 | T1ULP1 |      0 |      0 ) << 4) | (     0 | T1ULP1 |      0 |      0 );
    SetBit(TIM1_CR5, T1ABDIE);
    TIM1__BCOR = 10000;
    TIM1__BCNTR = 0;
    TIM1__BCCR = 10000;
    TIM1__BARR = 10000;
    TIM1__RCNTR = 0;
    TIM1_IER = T1PDIE;
}

/* 开窗正弦Timer初始化 */
void Timer1_Sv_Init(void)
{
    ClrBit(TIM1_CR3, T1PSC2); //  000-->24M     001-->12M       010-->6M    011-->3M
    SetBit(TIM1_CR3, T1PSC1); //  100-->1.5M   101-->750K      110-->375K    111-->187.5K
    SetBit(TIM1_CR3, T1PSC0);
	
	ClrBit(TIM1_CR0, T1CFLT1);                                                                               // 60度基准时间滤波选择  00-1个BCCR  01-2个BCCR 10-4个BCCR  11-8个BCCR
    ClrBit(TIM1_CR0, T1CFLT0);
	
    TIM1_CR1 = (TIM1_CR1 & (0x80)) | (SV_BSEL_ANGLE);                 // 续流屏蔽设置   TIM1_CR1  =(TIM1_CR1 &(0x80)) | (ANGLE_M*127/60)
    //输入TI0/TI1/TI2噪声脉宽选择
    SetBit(TIM1_CR3, T1INM1);                                                                                   //CMP_CR4[FAEN]=0时  00: 不滤波  01：8个时钟周期   10：32个时钟周期  11：64个时钟周期
    ClrBit(TIM1_CR3, T1INM0);                                                                                   //CMP_CR4[FAEN]=1时  00: 32个时钟周期  01：64个时钟周期   10：96个时钟周期  11：128个时钟周期
    
	ClrBit(TIM1_CR0, T1OPS1);                                         // 00-软件换相  01-重载定时器上溢触发 10-位置检测中断触发  11-RSV
    SetBit(TIM1_CR0, T1OPS0);
	
    ClrBit(TIM1_CR3, T1TIS1);                                         // 00-GPIO  01-CMP  10-ADC  11-CMP+ADC
    SetBit(TIM1_CR3, T1TIS0);                                         // 位置检测信号选择 CMP+ADC
	
	ClrBit(TIM1_CR7, ABDMD);                            			  // 禁止快速自适应续流屏蔽模式使能
	
    ClrBit(SV_CR0, WINDIS);                                            // 开窗
    SetReg(TIM1_CR6, PSEL1 | PSEL0, 0);                                // 检测相选择 00:U相 01:V相 10:W相
}

/* 电机切换到硬件运行 */
void Timer1_STH_Init(void)
{
    ClrBit(TIM1_CR0, T1OPS1);                                               // 00-软件换相  01-重载定时器上溢触发 10-位置检测中断触发  11-RSV
    SetBit(TIM1_CR0, T1OPS0);
    TIM1_CR1 = (TIM1_CR1 & (0x80)) | (15 * 127 / 60);   // 续流屏蔽设置   TIM1_CR1  =(TIM1_CR1 &(0x80)) | (ANGLE_M*127/60)
    ClrBit(TIM1_CR3, T1TIS1);                        // 00-GPIO  01-CMP  10-ADC  11-CMP+ADC
    SetBit(TIM1_CR3, T1TIS0);                         // 位置检测信号选择 CMP+ADC
}


/* -------------------------------------------------------------------------------------------------
    Function Name  : Timer3_Init
    Description    : 定时器3初始化
    Date           : 2021-11-08
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Timer3_Init(void)
{
	
	SetBit(CK_CR , T3CKEN);
    /*  -------------------------------------------------------------------------------------------------
        先停止计数，配置完寄存器后，最后启动计数
        -------------------------------------------------------------------------------------------------*/
    ClrBit(TIM3_CR1, T3EN); // 0，停止计数；1,使能计数
	/*Timer3功能转移T3CT  
00: P1.7为Timer3输入 
01: P1.3为Timer3输入输出 
10: P0.3为Timer3输入输出 
*/
	SetBit(PH_SEL, T3SEL);  // Timer3端口使能
	ClrBit(PH_SEL1, T3CT1);  
	SetBit(PH_SEL1, T3CT0);  // Timer3端口使能
    ClrBit(P1_AN, HBMOD); //若使用P1.3作为输入捕获，需要清零这一个位
    ClrBit(TIM3_CR0, T3PSC2); // 计数器时钟分频选择
    ClrBit(TIM3_CR0, T3PSC1); // 000-->24M      001-->12M       010-->6M    011-->3M
    SetBit(TIM3_CR0, T3PSC0); // 100-->1.5M    101-->750K      110-->375K    111-->48M  注: Timer3 的 输入捕获模式 下，111 对应的是 48MHz
	
    ClrBit(TIM3_CR0, T3OCM);
    ClrBit(TIM3_CR0, T3IRE); // 比较匹配中断/脉宽检测中断0-->Disable  1-->Enable
    ClrBit(TIM3_CR0, T3OPM); // 0-->计数器不停止      1-->单次模式
	ClrBit(INT_SR1, T3IP);
	ClrBit(INT_SR1, T3IF);
    SetBit(TIM3_CR1, T3IPE); // 输入Timer PWM周期检测中断使能 0-->Disable  1-->Enable
    SetBit(TIM3_CR1, T3IFE); // 计数器上溢中断使能 0-->Disable  1-->Enable
    SetBit(TIM3_CR1, T3INM1); // 输入噪声脉宽选择
    SetBit(TIM3_CR1, T3INM0); // 00-->不滤波  01-->4cycles    10-->8cycles  11-->16cycles
    ClrBit(TIM3_CR0, T3MOD); // 0-->Timer模式       1-->输出模式
	
	ClrBit(IP2, PTIM31);                                                                 ///< TIM3 設置中斷優先級
    ClrBit(IP2, PTIM30);                                                              ///< TIM3 設置中斷優先級
    
	TIM3__ARR  = 0;                                                               ///< TIM3 清除週期值
	TIM3__DR   = 0;                                                               ///< TIM3 清除比較值
	TIM3__CNTR = 0;                                                               ///< TIM3 清除計數值
  
    SetBit(TIM3_CR1, T3EN);  // TIM3使能    0-->Disable  1-->Enable
	
}

void Timer4_Init(void)
{
    SetBit(CK_CR, T4CKEN);
    ClrBit(TIM4_CR1, T4EN);
    SetBit(PH_SEL1, T4CT);                                             ///< 功能转移0-P12 1-P13
    SetBit(PH_SEL, T4SEL);                                             ///< 端口复用为TIM4输出

    SetReg(TIM4_CR0, (T4PSC2 | T4PSC1 | T4PSC0), FG_CLOCKBASE_SELECT); ///< 設置預除頻
    SetReg(TIM4_CR0, (T4OPM), 0);                                      ///< 啟動自動重載
    SetReg(TIM4_CR0, (T4OCM), 0);                                      ///< 設置預設互補輸出低電位
    SetReg(TIM4_CR0, (T4MOD), T4MOD);                                  ///< 設置 PWM 輸出模式
    SetReg(TIM4_CR1, (T4IPE | T4IFE), 0);                              ///< 啟動 IF 中斷
    SetReg(TIM4_CR0, (T4IRE), 0);                                      ///< 關閉 IR 中斷
    SetReg(TIM4_CR1, (T4INM1 | T4INM0), 0);                            ///< 不使用濾波器 (PWM 輸出模式無作用)
    ClrBit(INT_SR1, T4IR | T4IF | T4IP);                               // 清除中断标志位
    TIM4__CNTR = 0;
    TIM4__ARR = 10;  ///< 預載入 ARR
    TIM4__DR = 8000; ///< 預載入 DR

    ClrBit(IP2, PTIM41); ///< TIM4 設置中斷優先級
    ClrBit(IP2, PTIM40); ///< TIM4 設置中斷優先級

    SetBit(TIM4_CR1, T4EN); ///< 啟動 TIM4 功能
}


/* WatchDog Config-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
    Function Name ：WatchDogConfig(Value ,Status)
    Description   ：看门狗定时函数初始化，看门狗使能和复位使能，定时配置
    Input         ：Value--定时时间，单位ms，最小定时时间8ms，最大定时1800ms
                  Statue--使能控制，Disable或Enable
  Output                ：None
-------------------------------------------------------------------------------------------------*/
void WatchDogConfig(uint16 Value, bool Status)
{
    SetReg(CCFG1, WDT_EN, (Status ? WDT_EN : 0x00));
    WDT_ARR = ((uint16)(65532 - (uint32)Value * 32768 / 1000) >> 8);
    RST_SR = 0x80;									//清除看门狗及软复位标志
    SetBit(WDT_CR, WDTRF);
}


/*-------------------------------------------------------------------------------------------------
    Function Name ：WatchDogRefresh(Value ,Status)
    Description   ：刷新看门狗计数器
    Input         ：None
  Output                ：None
-------------------------------------------------------------------------------------------------*/
void WatchDogRefresh(void)
{
    SetBit(WDT_CR, WDTRF);
}

