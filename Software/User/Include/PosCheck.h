#ifndef __MOTORPOSCHECK_H_
#define __MOTORPOSCHECK_H_

typedef enum
{
    IPD_0		  = 0,
    IPD_1		  = 1,
    IPD_2         = 2,
    IPD_3		  = 3,
    IPD_4	      = 4,
    IPD_5		  = 5,
    IPD_6		  = 6,
	IPD_7		  = 7,
}IPD_TypeDef;

typedef struct
{
	uint16 DetectCount [8];// RPD检测时间
    uint16 InsetIdStep1[6];// RPD检测处理
    
    uint16 InsetIdMax;
	uint8  injectmin;      // 检测注入的最小值
	uint8  injectmid;      // 检测注入的中间值
    
    int16  Angle;
    int16  ThetaGet;   // RPD的角度获取
    uint8  injecttimes;// RPD注入次数

    uint8  injectstep;
    uint8  Section;

    uint8  injectCnt;
    
    uint16 Theta;

    uint8 NextShotFlag;
    
    uint8  injectstartflag;
    uint8  injectcntstartflag;
    
} IPD_Param_TypeDef;
extern void IPD (void);
extern void IPD_Detect(void);
extern void IPD_Init(void);
extern void Time2_IPD_Init(void);
extern void IPD_0_VUWinit(void);//VW
extern void IPD_1_WVinit(void);//WV
extern void IPD_2_UVWinit(void);//UV
extern void IPD_3_VUinit(void);//VU
extern void IPD_4_WUVinit(void);//WU
extern void IPD_5_UWinit(void);//UW

extern void IPDFunctions(void);
extern void IPD_Cur_TimeInject(void);

extern IPD_Param_TypeDef IPDPara;
extern IPD_TypeDef IPD_Status;
#endif