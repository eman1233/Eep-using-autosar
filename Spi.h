#ifndef __SPI_H__
#define __SPI_H__
#include "Std_Types.h"
/********************************************************************************/
/********************************ENUM*******************************************/
/********************************************************************************/
//Services        :SWS_Spi_00373
//Name            :Spi_StatusType
//type            :Enumeration
//Description     :This type defines a range of specific status for SPI Handler/Driver
/********************************************************************************/
typedef enum{
	//The SPI Handler/Driver is not initialized or not usable
	SPI_UNINIT=0,
	//The SPI Handler/Driver is not currently transmitting any Job
	SPI_IDLE,
	// The SPI Handler/Driver is performing a SPI Job (transmit)
	SPI_BUSY,
}Spi_StatusType;

/********************************************************************************/
//Services        :SWS_Spi_00374
//Name            :Spi_JobResultType
//type            :Enumeration
//Description     :This type defines a range of specific Jobs status for SPI Handler/Driver
/********************************************************************************/
typedef enum{
	// The last transmission of the Job has failed
	SPI_JOB_FAILED=0,
	//The last transmission of the Job has been finished successfully
	SPI_JOB_OK ,
	// The SPI Handler/Driver is performing a SPI Job. The meaning of this status is equal to SPI_BUSY.
	SPI_JOB_PENDING,
	//An asynchronous transmit Job has been accepted, while
 //actual transmission for this Job has not started yet.
	SPI_JOB_QUEUED,
}Spi_JobResultType;
/********************************************************************************/
//Services        :SWS_Spi_00375
//Name            :Spi_SeqResultType
//type            :Enumeration
//Description     :This type defines a range of specific Sequences status for SPI Handler/Driver.
/********************************************************************************/
typedef enum{
	//The last transmission of the Sequence has failed.
 SPI_SEQ_FAILED=0,
//The last transmission of the Sequence has been finished successfully.
 SPI_SEQ_OK,
//The SPI Handler/Driver is performing a SPI Sequence. The
//meaning of this status is equal to SPI_BUSY.
 SPI_SEQ_PENDING,
//The last transmission of the Sequence has been canceled by user.
SPI_SEQ_CANCELED
}Spi_SeqResultType;
/********************************************************************************/
//Services        :SWS_Spi_00382
//Name            :Spi_AsyncModeType
//type            :Enumeration
//Description     :Specifies the asynchronous mechanism mode for SPI busses handled asynchronously in LEVEL 2.
/********************************************************************************/
typedef enum{
	//The asynchronous mechanism is ensured by polling, so
  //interrupts related to SPI busses handled asynchronously
 //are disabled.
	SPI_POLLING_MODE=0,
	//The asynchronous mechanism is ensured by interrupt, so
  //interrupts related to SPI busses handled asynchronously
  //are enabled.
	SPI_INTERRUPT_MODE
}Spi_AsyncModeType;
typedef enum{
	EB1=0,
	IB1=1
}SpiChannelType;
typedef enum{
	LSB=0,
	MSB
}SpiTransferStart;
typedef enum{
	ASYNCHRONOUS=0,
	SYNCHRONOUS
}SpiHwUnitSynchronous;
typedef enum{
	HIGH=1,
	LOW=0
}SpiCsPolarity;
typedef enum{
	CS_VIA_GPIO=0,
	CS_VIA_PERIPHERAL_ENGINE,
}SpiCsSelection;
typedef enum{
	LEADING=0,
	TRAILING
}SpiDataShiftEdge;
typedef enum{
	CSIB0=0,
	CSIB1,
	CSIB2,
	CSIB3,
}SpiHwUnit;
typedef enum{
	high=1,
	low=0
}SpiShiftClockIdleLevel;
/********************************************************************************/
//Services        :SWS_Spi_00376
//Name            :Spi_DataBufferType
//type            :uint8
//Description     :Type of application data buffer elements
/********************************************************************************/
typedef uint8_t  Spi_DataBufferType;

/********************************************************************************/
//Services        :SWS_Spi_00377
//Name            :Spi_NumberOfDataType
//type            :uint16
//Description     :Type for defining the number of data elements of the type Spi_DataBufferType to
//send and / or receive by Channel
/********************************************************************************/
typedef uint16_t Spi_NumberOfDataType;

/********************************************************************************/
//Services        :SWS_Spi_00378
//Name            :Spi_ChannelType
//type            :uint8
//Description     :Specifies the identification (ID) for a Channel
/********************************************************************************/
typedef uint8_t  Spi_ChannelType;
/********************************************************************************/
//Services        :SWS_Spi_00379
//Name            :Spi_JobType
//type            :uint16
//Description     :Specifies the identification (ID) for a Job.
/********************************************************************************/
typedef uint16_t Spi_JobType;

/********************************************************************************/
//Services        :SWS_Spi_00380
//Name            :Spi_SequenceType
//type            :uint16
//Description     :Specifies the identification (ID) for a Job.
/********************************************************************************/
typedef uint16_t Spi_SequenceType;

/********************************************************************************/
//Services        :SWS_Spi_00381
//Name            :Spi_HWUnitType
//type            :uint8
//Description     :Specifies the identification (ID) for a SPI Hardware microcontroller peripheral (unit).
/********************************************************************************/
typedef uint8_t  Spi_HWUnitType;

/*******************************************************************************/
#endif
