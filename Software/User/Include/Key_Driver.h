#ifndef __KEY_DRIVER__H
#define __KEY_DRIVER__H


#define Key0 	          GP01



/* Exported types -------------------------------------------------------------------------------*/
typedef struct
{
	uint8 Key0Value;
	uint8 Key0TempValue;
	uint8 FlagKey0Press;
	uint8 FlagKey0LastPress;
	uint8 KeyDownFlag;
	
	int16 Key0PressCnt;
	int16 Key0InitCnt;

} KeyScanParam_TypeDef;

/* Exported variables ---------------------------------------------------------------------------*/
extern KeyScanParam_TypeDef KS;

/* Exported functions ---------------------------------------------------------------------------*/
extern void KeyScan(void);




#endif