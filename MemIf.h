#ifndef __MEMIF_H__
#define __MEMIF_H__
typedef enum{
	MEMIF_JOB_OK=0x00,
	MEMIF_JOB_FAILED,
	MEMIF_JOB_PENDING,
	MEMIF_JOB_CANCELLED,
	MEMIF_BLOCK_INCONSISTENT,
	MEMIF_BLOCK_INVALID,
}MemIf_JobResultType;
MemIf_JobResultType MemIf_JobResult;
typedef enum{
	MEMIF_MODE_SLOW=0x00,
	MEMIF_MODE_FAST,
}MemIf_ModeType;

typedef enum{
	MEMIF_UNINIT=0x00,
	MEMIF_IDLE,
	MEMIF_BUSY,
	MEMIF_BUSY_INTERNAL,
}MemIf_StatusType;
MemIf_StatusType MemIf_Status;

#endif