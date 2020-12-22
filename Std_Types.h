#ifndef __EEP_CFG_H__
#define __EEP_CFG_H__
#include "Compiler.h"
#include "Platform_Types.h"
typedef enum{
	E_NOT_OK=0x00,
	E_OK,
}Std_Return;
Std_Return Std_ReturnType;
typedef struct{
 uint16 vendorID;
 uint16 moduleID;
 uint8 instanceID;
 uint8 sw_major_version;
 uint8 sw_minor_version;
 uint8 sw_patch_version;
} Std_VersionInfoType;
#endif
