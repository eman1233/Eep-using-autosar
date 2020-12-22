#ifndef __EEP_H__
#define __EEP_H__
#include "Eep_Cfg.h"
#include "EcuM.h"
#include "Std_Types.h"
#include "Platform_Types.h"
#include "MemIf.h"
typedef uint32 Eep_AddressType;
typedef uint32 Eep_LengthType;
typedef struct{
	Eep_AddressType EEP_BASE_ADDRESS;
	MemIf_ModeType EEP_DEFAULT_MODE;
	Eep_LengthType EEP_FAST_READ_BLOCK_SIZE;
	Eep_LengthType EEP_FAST_WRITE_BLOCK_SIZE;
	int EEP_JOB_CALL_CYCLE;
	void (*EEP_JOB_END_NOTIFICATION)(void);
	void (*EEP_JOB_ERROR_NOTIFICATION)(void);
	Eep_LengthType EEP_NORMAL_READ_BLOCK_SIZE;
	Eep_LengthType EEP_NORMAL_WRITE_BLOCK_SIZE;
	Eep_LengthType EEP_SIZE;
}EepInitConfiguration;
typedef struct{
	uint8 EepDriverIndex;
	EepInitConfiguration *EepInitConfigurationRef;
}Eep_ConfigType;
 
typedef struct{
	
}EepExternalDriver;
#define moduleId  0x90
#define instanceId 0x00
                                         //API
#define  Eep_Init_API               0X00
#define  Eep_SetMode_API            0X01
#define  Eep_Read_API               0X02
#define  Eep_Write_API              0X03
#define  Eep_Erase_API              0X04
#define  Eep_Compare_API            0X05
#define  Eep_Cancel_API             0X06
#define  Eep_GetStatus_API          0X07
#define  Eep_GetJobResult_API       0X08
#define  Eep_GetVersionInfo_API     0X0A
#define  Eep_MainFunction_API       0X09

                                      //Development error
#define EEP_E_PARAM_CONFIG          0x10
#define EEP_E_PARAM_ADDRESS         0x11
#define EEP_E_PARAM_DATA            0x12
#define EEP_E_PARAM_LENGTH          0x13
#define EEP_E_UNINIT                0x20
#define EEP_E_BUSY                  0x21
void delay(int time);
void Eep_Init(const Eep_ConfigType* ConfigPtr);
void Eep_SetMode(MemIf_ModeType Mode);
Std_ReturnType Eep_Read(Eep_AddressType EepromAddress,uint8* DataBufferPtr,Eep_LengthType Length);
Std_ReturnType Eep_Wrire(Eep_AddressType EepromAddress,uint8* DataBufferPtr,Eep_LengthType Length);
Std_ReturnType Eep_Compare(Eep_AddressType EepromAddress,const uint8* DataBufferPtr,Eep_LengthType Length);
void Eep_Cancel(void);
MemIf_StatusType Eep_GetStatus(void);
MemIf_JobResultType Eep_GetJobResult(void);
void Eep_MainFunction(void);
t
#endif
