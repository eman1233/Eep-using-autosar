#include "MemMap.h"
#include "Eep.h"
#include "Eep_Cbk.h"
#include "Det.h"
#include "Dem.h"
#include "SchM_Eep.h"
#include "Hw_eep.h"
#include "Hw_types.h"
#define EepSize 2048
#define NULL ((void*)0)
void delay(int time){
	while(time!=0){
		time--;
	}
}
/*********************************************************************************************************/
//Service Name          :Eep_Init                                                                        
//Service ID[hex]       :0x00
//Sync/Async            :Synchronous
//Reentrancy            :Non Reentrant
//Parameters (in)       :ConfigPtr         "Pointer to configuration set."
//Parameters (inout)    :None
//Parameters (out)      :None
//Return value          :None
//Description           :Service for EEPROM initialization.
/*********************************************************************************************************/
void Eep_Init(const Eep_ConfigType* ConfigPtr){
	volatile uint32_t Status=0;
	// The function Eep_Init shall initialize all EEPROM relevant registers with the
  //values of the structure referenced by the parameter ConfigPtr
	ConfigPtr->EepInitConfigurationRef->EEP_BASE_ADDRESS=EEPROM_EEOFFSET_OFFSET_M;
	ConfigPtr->EepInitConfigurationRef->EEP_DEFAULT_MODE=MEMIF_MODE_SLOW;
	ConfigPtr->EepInitConfigurationRef->EEP_JOB_END_NOTIFICATION=Eep_JobEndNotification;
	ConfigPtr->EepInitConfigurationRef->EEP_JOB_ERROR_NOTIFICATION=Eep_JobErrorNotification;
	ConfigPtr->EepInitConfigurationRef->EEP_SIZE=EEPROM_PP_SIZE_M;
	// If development error detection for the module Eep is enabled; if the function
  //Eep_Init is called with a NULL configPtr and if a variant containing postbuild multiple
  //selectable configuration parameters is used (VariantPB), the function Eep_Init shall
  //raise the development error EEP_E_PARAM_CONFIG andreturn without any action.
	#if (EEP_DEV_ERROR_DETECT==E_OK)
	   if(ConfigPtr==NULL)
			 Det_ReportError(moduleId,instanceId,Eep_Init_API,EEP_E_PARAM_CONFIG);		 
	#endif
	//insert delay for optmization
	delay(2);
	//make sure eeprom finshing process
	while(HWREG(EEPROM_EEDONE_WORKING&EEPROM_EEDONE )){
		//spin while busy
	}
	//read if any errors
	Status=HWREG(EEPROM_EESUPP);
	//check status for error
	#if (EEP_DEV_ERROR_DETECT==E_OK)
	   //error occur
	   if(Status&(EEPROM_EESUPP_PRETRY |EEPROM_EESUPP_ERETRY))
			   Det_ReportError(moduleId,instanceId,Eep_Init_API,EEP_E_PARAM_CONFIG);		 
	#endif
	
	// After having finished the module initialization, the function Eep_Init shall set
  //the EEPROM state to MEMIF_IDLE and shall set the job result to MEMIF_JOB_OK.
	MemIf_Status=MEMIF_IDLE;
	MemIf_JobResult=MEMIF_JOB_OK;
}

/*********************************************************************************************************/
//Service Name          :Eep_SetMode                                                                        
//Service ID[hex]       :0x01
//Sync/Async            :Synchronous
//Reentrancy            :Non Reentrant
//Parameters (in)       :Mode         
//               "MEMIF_MODE_NORMAL: Normal read access / normal SPI access."
//               "MEMIF_MODE_FAST: Fast read access / SPI burst access."
//Parameters (inout)    :None
//Parameters (out)      :None
//Return value          :None
//Description           :Service for EEPROM initialization.
/*********************************************************************************************************/
void Eep_SetMode(MemIf_ModeType Mode){
	//The function Eep_SetMode shall set the EEPROM operation mode to the given mode parameter.
	Mode=MEMIF_MODE_SLOW;
	//The function Eep_SetMode checks the EEPROM state according to requirement EEP033
	#if (EEP_DEV_ERROR_DETECT==E_OK)
	   if(MemIf_Status!=MEMIF_IDLE)
			 Det_ReportError(moduleId,instanceId,Eep_SetMode_API, EEP_E_UNINIT);		 
	#endif
}

/*********************************************************************************************************/
//Service Name          :Eep_Read                                                                        
//Service ID[hex]       :0x02
//Sync/Async            :Asynchronous
//Reentrancy            :Non Reentrant
//Parameters (in)       :EepromAddress         
//               "Address offset in EEPROM (will be added to the EEPROM base address).Min.: 0 Max.: EEP_SIZE - 1"
//                      :Length 
//               "Number of bytes to read Min.: 1 Max.: EEP_SIZE - EepromAddress"           
//                      :DataBufferPtr
//               "Pointer to destination data buffer in RAM"
//Parameters (out)      :None
//Parameters (inout)    :None
//Return value          :Std_ReturnType
//              "E_OK: read command has been accepted"
//              "E_NOT_OK: read command has not been accepted"
//Description           : Reads from EEPROM.
/*********************************************************************************************************/
Std_ReturnType Eep_Read(Eep_AddressType EepromAddress,uint8* DataBufferPtr,Eep_LengthType Length){
	//The Eep module shall execute the read job asynchronously within the Eep
  //module’s job processing function. During job processing the Eep module shall read a
	//data block of size Length from EepromAddress + EEPROM base address to
  //*DataBufferPtr.
	Length=EepromAddress+*DataBufferPtr;
	//If development error detection for the module Eep is enabled: 
	#if (EEP_DEV_ERROR_DETECT==E_OK)
  //that DataBufferPtr is not NULL. If DataBufferPtr is NULL, they shall raise
  //development error EEP_E_PARAM_DATA and return with E_NOT_OK
	   if(DataBufferPtr==NULL){
			  Det_ReportError(moduleId,instanceId,Eep_Read_API,EEP_E_PARAM_CONFIG);	
			 return E_NOT_OK;
		 }
  //shall check that the parameter Length is within the specified minimum and maximum values:
  //? Min.: 1? Max.: EepSize – EepromAddress
  //If the parameter Length is not within the specified minimum and maximum values,
  //they shall raise development error EEP_E_PARAM_LENGTH and return with E_NOT_OK		 
		 if(( Length<1)||( Length>(EepSize - EepromAddress))){
			  Det_ReportError(moduleId,instanceId,Eep_Read_API,EEP_E_PARAM_LENGTH);	
			 return E_NOT_OK;
		 }
		 //shall check the EEPROM state for being MEMIF_IDLE. If the
     //EEPROM state is not MEMIF_IDLE , the called function shall
     //? raise development error EEP_E_BUSY or EEP_E_UNINIT according to the
     //EEPROM state ? reject the service with E_NOT_OK 
		 if(MemIf_Status!=MEMIF_IDLE){
			 Det_ReportError(moduleId,instanceId,Eep_Read_API, EEP_E_UNINIT);
			 return E_NOT_OK;
		 }			 	
	#endif
			//EEP009: The function Eep_Read shall copy the given parameters, initiate a read job,
  //set the EEPROM status to MEMIF_BUSY, set the job result to
  //MEMIF_JOB_PENDING and return.
		 
	//The Eep’s user shall only call Eep_Read after the Eep module has been been initialized.
	MemIf_Status=MEMIF_BUSY;
	MemIf_JobResult=MEMIF_JOB_PENDING;
}

/*********************************************************************************************************/
//Service Name          :Eep_Write                                                                        
//Service ID[hex]       :0x03
//Sync/Async            :Asynchronous
//Reentrancy            :Non Reentrant
//Parameters (in)       :EepromAddress         
//               "Address offset in EEPROM (will be added to the EEPROM base address).Min.: 0 Max.: EEP_SIZE - 1"
//                      :Length 
//               "Number of bytes to write Min.: 1 Max.: EEP_SIZE - EepromAddress"           
//                      :DataBufferPtr
//               "Pointer to destination data buffer in RAM"
//Parameters (out)      :None
//Parameters (inout)    :None
//Return value          :Std_ReturnType
//              "E_OK: write command has been accepted"
//              "E_NOT_OK: write command has not been accepted"
//Description           : Writes from EEPROM.
/*********************************************************************************************************/
Std_ReturnType Eep_Wrire(Eep_AddressType EepromAddress,uint8* DataBufferPtr,Eep_LengthType Length){
	//The Eep module shall execute the read job asynchronously within the Eep
  //module’s job processing function. During job processing the Eep module shall read a
	//data block of size Length from EepromAddress + EEPROM base address to
  //*DataBufferPtr.
	Length=EepromAddress+*DataBufferPtr;
	//If development error detection for the module Eep is enabled: 
	#if (EEP_DEV_ERROR_DETECT==E_OK)
  //that DataBufferPtr is not NULL. If DataBufferPtr is NULL, they shall raise
  //development error EEP_E_PARAM_DATA and return with E_NOT_OK
	   if(DataBufferPtr==NULL){
			  Det_ReportError(moduleId,instanceId,Eep_Write_API,EEP_E_PARAM_CONFIG);	
			 return E_NOT_OK;
		 }
  //shall check that the parameter Length is within the specified minimum and maximum values:
  //? Min.: 1? Max.: EepSize – EepromAddress
  //If the parameter Length is not within the specified minimum and maximum values,
  //they shall raise development error EEP_E_PARAM_LENGTH and return with E_NOT_OK		 
		 if(( Length<1)||( Length>(EepSize - EepromAddress))){
			  Det_ReportError(moduleId,instanceId,Eep_Write_API,EEP_E_PARAM_LENGTH);	
			 return E_NOT_OK;
		 }
		 //shall check the EEPROM state for being MEMIF_IDLE. If the
     //EEPROM state is not MEMIF_IDLE , the called function shall
     //? raise development error EEP_E_BUSY or EEP_E_UNINIT according to the
     //EEPROM state ? reject the service with E_NOT_OK 
		 if(MemIf_Status!=MEMIF_IDLE){
			 Det_ReportError(moduleId,instanceId,Eep_Write_API, EEP_E_UNINIT);
			 return E_NOT_OK;
		 }			 	
	#endif
			//EEP009: The function Eep_Write shall copy the given parameters, initiate a read job,
  //set the EEPROM status to MEMIF_BUSY, set the job result to
  //MEMIF_JOB_PENDING and return.
		 
	//The Eep’s user shall only call Eep_Write after the Eep module has been been initialized.
	MemIf_Status=MEMIF_BUSY;
	MemIf_JobResult=MEMIF_JOB_PENDING;
}

/*********************************************************************************************************/
//Service Name          :Eep_Erase                                                                       
//Service ID[hex]       :0x04
//Sync/Async            :Asynchronous
//Reentrancy            :Non Reentrant
//Parameters (in)       :EepromAddress         
//               "Address offset in EEPROM (will be added to the EEPROM base address).Min.: 0 Max.: EEP_SIZE - 1"
//                      :Length 
//               "Number of bytes to earse Min.: 1 Max.: EEP_SIZE - EepromAddress"           
//Parameters (out)      :None
//Parameters (inout)    :None
//Return value          :Std_ReturnType
//              "E_OK: erase command has been accepted"
//              "E_NOT_OK: erase command has not been accepted"
//Description           : earse from EEPROM.
/*********************************************************************************************************/
Std_ReturnType Eep_Erase(Eep_AddressType EepromAddress,Eep_LengthType Length){
  //If development error detection for the module Eep is enabled: 
	#if (EEP_DEV_ERROR_DETECT==E_OK)
  //shall check that the parameter Length is within the specified minimum and maximum values:
  //? Min.: 1? Max.: EepSize – EepromAddress
  //If the parameter Length is not within the specified minimum and maximum values,
  //they shall raise development error EEP_E_PARAM_LENGTH and return with E_NOT_OK		 
		 if(( Length<1)||( Length>(EepSize - EepromAddress))){
			  Det_ReportError(moduleId,instanceId,Eep_Erase_API,EEP_E_PARAM_LENGTH);	
			 return E_NOT_OK;
		 }
		 //shall check the EEPROM state for being MEMIF_IDLE. If the
     //EEPROM state is not MEMIF_IDLE , the called function shall
     //? raise development error EEP_E_BUSY or EEP_E_UNINIT according to the
     //EEPROM state ? reject the service with E_NOT_OK 
		 if(MemIf_Status!=MEMIF_IDLE){
			 Det_ReportError(moduleId,instanceId,Eep_Erase_API, EEP_E_UNINIT);
			 return E_NOT_OK;
		 }			 	
	#endif
	//The Eep module’s user shall only call the function Eep_Erase after the Eep
  //module has been initialized.
		//The function Eep_Erase shall copy the given parameters, initiate an erase
  //job, set the EEPROM status to MEMIF_BUSY, set the job result to
  //MEMIF_JOB_PENDING and return
	MemIf_Status=MEMIF_BUSY;
	MemIf_JobResult=MEMIF_JOB_PENDING;
}
/*********************************************************************************************************/
//Service Name          :Eep_Compare                                                                        
//Service ID[hex]       :0x05
//Sync/Async            :Asynchronous
//Reentrancy            :Non Reentrant
//Parameters (in)       :EepromAddress         
//               "Address offset in EEPROM (will be added to the EEPROM base address).Min.: 0 Max.: EEP_SIZE - 1"
//                      :Length 
//               "Number of bytes to compare Min.: 1 Max.: EEP_SIZE - EepromAddress"           
//                      :DataBufferPtr
//               "Pointer to data buffer (compare data)"
//Parameters (out)      :None
//Parameters (inout)    :None
//Return value          :Std_ReturnType
//              "E_OK: compare command has been accepted"
//              "E_NOT_OK: compare command has not been accepted"
//Description           : Compares a data block in EEPROM with an EEPROM block in the memory.
/*********************************************************************************************************/
Std_ReturnType Eep_Compare(Eep_AddressType EepromAddress,const uint8* DataBufferPtr,Eep_LengthType Length){
	//EEP026: The Eep module shall execute the compare job asynchronously within the
  //Eep module’s job processing function. During job processing the Eep module shall
  //compare the EEPROM data block at EepromAddress + EEPROM base address of
  //size Length with the data block at *DataBufferPtr of the same length
	Length=EepromAddress+*DataBufferPtr;
	//If development error detection for the module Eep is enabled: 
	#if (EEP_DEV_ERROR_DETECT==E_OK)
	//that DataBufferPtr is not NULL. If DataBufferPtr is NULL, they shall raise
  //development error EEP_E_PARAM_DATA and return with E_NOT_OK
	   if(DataBufferPtr==NULL){
			  Det_ReportError(moduleId,instanceId,Eep_Compare_API,EEP_E_PARAM_CONFIG);	
			 return E_NOT_OK;
		 }
  //shall check that the parameter Length is within the specified minimum and maximum values:
  //? Min.: 1? Max.: EepSize – EepromAddress
  //If the parameter Length is not within the specified minimum and maximum values,
  //they shall raise development error EEP_E_PARAM_LENGTH and return with E_NOT_OK		 
		 if(( Length<1)||( Length>(EepSize - EepromAddress))){
			  Det_ReportError(moduleId,instanceId,Eep_Compare_API,EEP_E_PARAM_LENGTH);	
			 return E_NOT_OK;
		 }
		 //shall check the EEPROM state for being MEMIF_IDLE. If the
     //EEPROM state is not MEMIF_IDLE , the called function shall
     //? raise development error EEP_E_BUSY or EEP_E_UNINIT according to the
     //EEPROM state ? reject the service with E_NOT_OK 
		 if(MemIf_Status!=MEMIF_IDLE){
			 Det_ReportError(moduleId,instanceId,Eep_Compare_API, EEP_E_UNINIT);
			 return E_NOT_OK;
		 }			 	
	#endif
	//The function Eep_Compare shall copy the given parameters, initiate a
  //compare job, set the EEPROM status to MEMIF_BUSY, set the job result to
  //MEMIF_JOB_PENDING and return
	//The Eep module’s user shall only call the function Eep_compare after the
  //Eep module has been initialized.
	MemIf_Status=MEMIF_BUSY;
	MemIf_JobResult=MEMIF_JOB_PENDING;
}	

/*********************************************************************************************************/
//Service Name          :Eep_Cancel                                                                        
//Service ID[hex]       :0x06
//Sync/Async            :Asynchronous
//Reentrancy            :Non Reentrant
//Parameters (in)       :None
//Parameters (out)      :None
//Parameters (inout)    :None
//Return value          :None
//Description           :Cancels a running job 
/*********************************************************************************************************/
void Eep_Cancel(void){
	//The function Eep_Cancel shall reset the module state to MEMIF_IDLE, and, if configured
	MemIf_Status=MEMIF_IDLE;
	//The function Eep_Cancel shall set the job result to
  //MEMIF_JOB_CANCELLED, if the job result has the value MEMIF_JOB_PENDING.
  //Otherwise it shall leave the job result unchanged
	if(MemIf_JobResult==MEMIF_JOB_CANCELLED)
		  MemIf_JobResult=MEMIF_JOB_PENDING;	
}
/*********************************************************************************************************/
//Service Name          :Eep_GetStatus                                                                        
//Service ID[hex]       :0x07
//Sync/Async            :Synchronous
//Reentrancy            :Reentrant
//Parameters (in)       :None
//Parameters (out)      :None
//Parameters (inout)    :None
//Return value          :MemIf_StatusType
//Description           :Returns the EEPROM status 
/*********************************************************************************************************/
MemIf_StatusType Eep_GetStatus(void){
	//The function Eep_GetStatus shall return the EEPROM status synchronously.
	return MemIf_Status;
}
/*********************************************************************************************************/
//Service Name          :Eep_GetJobResult                                                                        
//Service ID[hex]       :0x08
//Sync/Async            :Synchronous
//Reentrancy            :Reentrant
//Parameters (in)       :None
//Parameters (out)      :None
//Parameters (inout)    :None
//Return value          :MemIf_StatusType
//Description           :This service returns the result of the last job.
/*********************************************************************************************************/
MemIf_JobResultType Eep_GetJobResult(void){
	//The function Eep_GetJobResult shall synchronously return the result of the
	//last job that has been accepted by the Eep module.
	return MemIf_JobResult;
}


